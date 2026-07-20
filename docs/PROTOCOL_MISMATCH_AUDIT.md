# Protocol Specification vs Firmware Implementation Audit

**Document Type:** Inconsistency Report  
**Date Generated:** 2026-07-20  
**Status:** Engineering decision required  
**Scope:** Packet structure field definitions, serialization, parsing, and CRC coverage

---

## EXECUTIVE SUMMARY

The NAS Controller Protocol Specification (PROTOCOL_SPEC.md) and the firmware implementation (PacketBuilder.cpp, PacketValidator.cpp) contain critical inconsistencies in the packet structure definition.

**Critical Finding:** Specification and firmware disagree on:
- Sequence field size (1 byte vs 2 bytes)
- Command field size (1 byte vs 2 bytes)
- Footer presence (included vs absent)
- Minimum packet size (9 bytes vs 12 bytes)

These differences make the specification and firmware incompatible for implementation.

---

## FIELD COMPARISON TABLE

| Field | Offset | Spec Size | Firmware Size | Endianness | Match |
|-------|--------|-----------|---------------|-----------|-------|
| Header | 0 | 2 bytes | 2 bytes | Big-endian | ✅ |
| Sequence | 2 | 1 byte | 2 bytes | N/A | ❌ MISMATCH |
| Command | 3 | 1 byte | 2 bytes | N/A | ❌ MISMATCH |
| Length | 4-5 (spec) / 6-7 (fw) | 2 bytes | 2 bytes | Big-endian | ⚠️ OFFSET MISMATCH |
| Payload | 6+ (spec) / 8+ (fw) | 0-256 | 0-256 | N/A | ⚠️ OFFSET MISMATCH |
| CRC | Variable | 2 bytes | 2 bytes | Big-endian | ✅ |
| Footer | End | 1 byte (0xAA) | ABSENT | N/A | ❌ MISMATCH |

---

## DETAILED FIELD ANALYSIS

### Field 1: Header

**Specification Definition (PROTOCOL_SPEC.md):**
```
Header: 2 bytes, value 0x55AA, big-endian
"Frame start marker for protocol synchronization"
```

**Firmware Implementation:**

*PacketBuilder.cpp (lines 49-53):*
```cpp
const std::uint16_t header =
    PacketValidator::Header;

std::memcpy(packet + offset, &header, sizeof(header));
offset += sizeof(header);
```

*PacketValidator.h (line 25):*
```cpp
static constexpr std::uint16_t Header = 0x55AA;
```

*PacketValidator.cpp (lines 26-29):*
```cpp
std::uint16_t header = 0U;
std::memcpy(&header, packet, sizeof(header));
if (header != Header)
```

**Analysis:**
- Spec: 2 bytes, 0x55AA
- Firmware: `std::uint16_t` (2 bytes), 0x55AA
- Offset: 0
- Size: 2 bytes
- Endianness: Big-endian (uint16_t in serialization context)
- **Status: ✅ MATCH**

---

### Field 2: Sequence Number

**Specification Definition (PROTOCOL_SPEC.md):**
```
Sequence: 1 byte
Range: 0x00-0xFF
Purpose: "Sequence number (rolls over)"
Increment: Every command sent
Rollover: 0xFF → 0x00
```

**Firmware Implementation:**

*PacketBuilder.h (line 36):*
```cpp
static NAS::Core::Result Build(
    std::uint16_t sequence,  // ← TYPE: uint16_t (2 bytes)
    ...
```

*PacketBuilder.cpp (lines 55-58):*
```cpp
std::memcpy(
    packet + offset,
    &sequence,
    sizeof(sequence));  // sizeof(uint16_t) = 2 bytes

offset += sizeof(sequence);  // offset += 2
```

**Offset Calculation:**
- After header (2 bytes), offset = 2
- Firmware writes sequence at offset 2, size = 2 bytes

**Analysis:**
- Spec claims: 1 byte at offset 2
- Firmware implements: 2 bytes at offset 2-3
- Declared type: `std::uint16_t`
- Bytes serialized: 2 (via `sizeof(sequence)`)
- **Status: ❌ CRITICAL MISMATCH**

**Impact:**
- Daemon encoder using 1-byte sequence will be incompatible with firmware
- Byte offsets of all subsequent fields will be incorrect
- CRC will fail (wrong data range included)

---

### Field 3: Command

**Specification Definition (PROTOCOL_SPEC.md):**
```
Command: 1 byte
Range: 0x00-0x3F for commands, 0x40 for NAK, 0x80 for ACK, 0x90 for response
Purpose: "Command byte"
```

**Firmware Implementation:**

*PacketBuilder.h (line 36):*
```cpp
std::uint16_t command,  // ← TYPE: uint16_t (2 bytes)
```

*PacketBuilder.cpp (lines 59-62):*
```cpp
std::memcpy(
    packet + offset,
    &command,
    sizeof(command));  // sizeof(uint16_t) = 2 bytes

offset += sizeof(command);  // offset += 2
```

**Offset Calculation:**
- After header (2) + sequence (2), offset = 4
- Firmware writes command at offset 4, size = 2 bytes

**Analysis:**
- Spec claims: 1 byte at offset 3
- Firmware implements: 2 bytes at offset 4-5
- Declared type: `std::uint16_t`
- Bytes serialized: 2 (via `sizeof(command)`)
- **Status: ❌ CRITICAL MISMATCH**

**Impact:**
- Daemon encoder using 1-byte command will be incompatible with firmware
- Byte offset of length field is now off by 1
- All downstream field offsets are incorrect
- CRC will fail

---

### Field 4: Payload Length

**Specification Definition (PROTOCOL_SPEC.md):**
```
Length: 2 bytes
Byte order: Big-endian
Range: 0-256
Position: After Cmd
```

**Firmware Implementation:**

*PacketBuilder.h (line 38):*
```cpp
std::uint16_t payloadLength,  // ← TYPE: uint16_t (2 bytes)
```

*PacketBuilder.cpp (lines 63-66):*
```cpp
std::memcpy(
    packet + offset,
    &payloadLength,
    sizeof(payloadLength));  // sizeof(uint16_t) = 2 bytes

offset += sizeof(payloadLength);  // offset += 2
```

*PacketValidator.cpp (lines 41-42):*
```cpp
std::memcpy(
    &payloadLength,
    packet + 6U,  // ← OFFSET IS 6, NOT 4
    sizeof(payloadLength));
```

**Offset Calculation:**
- Spec: offset 4-5 (after header 2 + seq 1 + cmd 1)
- Firmware: offset 6-7 (after header 2 + seq 2 + cmd 2)

**Analysis:**
- Spec claims: 2 bytes at offset 4-5
- Firmware implements: 2 bytes at offset 6-7
- Declared type: `std::uint16_t`
- Bytes serialized: 2
- Byte order: Big-endian (uint16_t)
- **Status: ❌ OFFSET MISMATCH** (field size is correct, but offset is wrong)

**Impact:**
- Because sequence and command are 2 bytes (not 1), length field offset shifts by 2 bytes
- Daemon encoder will write length at wrong offset
- CRC will fail

---

### Field 5: Payload

**Specification Definition (PROTOCOL_SPEC.md):**
```
Payload: 0-256 bytes
Range: Variable based on command
Position: After Length field
Purpose: Command-specific data
```

**Firmware Implementation:**

*PacketBuilder.cpp (lines 67-72):*
```cpp
if ((payloadLength > 0U) &&
    (payload != nullptr))
{
    std::memcpy(
        packet + offset,
        payload,
        payloadLength);

    offset += payloadLength;
}
```

**Offset Calculation:**
- After header (2) + seq (2) + cmd (2) + len (2) = 8 bytes
- Payload starts at offset 8

**Analysis:**
- Spec claims: starts at offset 6 (incorrect due to seq/cmd mismatch)
- Firmware implements: starts at offset 8
- Max size: 256 bytes
- **Status: ⚠️ OFFSET MISMATCH** (due to upstream field size errors)

**Impact:**
- Offset is correct in firmware but wrong in spec
- Daemon encoder will write payload at wrong offset

---

### Field 6: CRC16

**Specification Definition (PROTOCOL_SPEC.md):**
```
CRC16: 2 bytes
Algorithm: CRC-16-Modbus (reflected variant)
Polynomial: 0xA001 (reflected form of 0x8005)
Initial Value: 0xFFFF
Input Reflection: Yes
Output Reflection: Yes
Final XOR: 0x0000
Coverage: Header | Seq | Cmd | Len | Payload (NOT CRC or Footer)
Byte Order: Big-endian
```

**Firmware Implementation:**

*PacketBuilder.cpp (lines 74-77):*
```cpp
const std::uint16_t crc =
    PacketValidator::CalculateCrc16(
        packet,
        offset);  // ← CRC covers from packet[0] to packet[offset-1]

std::memcpy(packet + offset, &crc, sizeof(crc));
```

*PacketValidator.cpp (lines 51-67):*
```cpp
const std::uint16_t calculatedCrc =
    CalculateCrc16(
        packet,
        length - sizeof(std::uint16_t));  // ← Coverage excludes CRC bytes

if (receivedCrc != calculatedCrc)
{
    return Result(ResultCode::CrcError);
}
```

*PacketValidator.cpp (lines 70-89) - CRC algorithm:*
```cpp
constexpr std::uint16_t Polynomial = 0xA001U;
std::uint16_t crc = 0xFFFFU;

for (std::size_t i = 0U; i < length; ++i)
{
    crc ^= data[i];
    for (std::uint8_t bit = 0U; bit < 8U; ++bit)
    {
        if ((crc & 0x0001U) != 0U)
        {
            crc >>= 1U;
            crc ^= Polynomial;
        }
        else
        {
            crc >>= 1U;
        }
    }
}
return crc;
```

**CRC Coverage Calculation (Firmware):**
- Header (2) + Seq (2) + Cmd (2) + Len (2) + Payload (0-256) = 8 to 264 bytes
- CRC includes all bytes from offset 0 to (total_length - 2)
- CRC does NOT include the CRC bytes themselves

**Analysis:**
- Spec coverage: Header|Seq|Cmd|Len|Payload (correct, matches firmware)
- Firmware coverage: packet[0..offset-1] where offset = header + seq + cmd + len + payload
- Algorithm: CRC-16-Modbus (reflected)
- Polynomial: 0xA001
- Initial: 0xFFFF
- Final XOR: 0x0000 (implicit, not in code)
- Byte order: Big-endian (uint16_t serialization)
- **Status: ✅ MATCH**

**Note:** CRC calculation is correct and matches spec, but because sequence and command sizes differ, the CRC will cover different byte ranges between spec and firmware.

---

### Field 7: Footer

**Specification Definition (PROTOCOL_SPEC.md):**
```
Frame Format:
[Header] [Seq] [Cmd] [Len] [Payload] [CRC16] [Footer]
  2B      1B    1B    2B    0-256B    2B      1B

Footer: 1 byte, value 0xAA
Purpose: "Frame terminator"
```

**Firmware Implementation:**

*PacketBuilder.cpp (lines 74-77):*
```cpp
std::memcpy(packet + offset, &crc, sizeof(crc));

return Result::Ok();  // ← NO FOOTER APPENDED
```

*PacketValidator.h (line 28):*
```cpp
static constexpr std::size_t MinimumPacketSize = 12U;
```

Minimum packet size calculation (no payload):
- Header (2) + Seq (2) + Cmd (2) + Len (2) + CRC (2) = 12 bytes
- If footer existed, minimum would be 13 bytes

*PacketValidator.cpp (lines 45-50):*
```cpp
std::uint16_t payloadLength = 0U;
std::memcpy(&payloadLength, packet + 6U, sizeof(payloadLength));

const std::size_t expectedLength =
    MinimumPacketSize + payloadLength;  // 12 + payload, NO +1 for footer
```

*PacketValidator.cpp (lines 52-55):*
```cpp
std::uint16_t receivedCrc = 0U;
std::memcpy(
    &receivedCrc,
    packet + (length - sizeof(std::uint16_t)),  // CRC is last field
    sizeof(receivedCrc));
```

**Analysis:**
- Spec claims: 1 byte (0xAA) footer appended after CRC
- Firmware implements: NO footer; packet ends with CRC
- Minimum packet size spec: 9 bytes (with footer)
- Minimum packet size firmware: 12 bytes (no footer)
- **Status: ❌ CRITICAL MISMATCH**

**Impact:**
- Daemon encoder appending footer will create packets that firmware rejects (wrong length)
- Firmware packets will be 1 byte shorter than spec predicts
- Payload length calculation will be off

---

## COMPLETE PACKET STRUCTURE COMPARISON

### According to PROTOCOL_SPEC.md

```
Byte Offset | Field        | Size | Value/Range | Notes
------------|--------------|------|-------------|--------
0-1         | Header       | 2    | 0x55AA      | Big-endian
2           | Sequence     | 1    | 0x00-0xFF   | Rolls over
3           | Command      | 1    | 0x00-0xFF   | Command byte
4-5         | Length       | 2    | 0-256       | Big-endian
6+          | Payload      | 0-256| Variable    | Command-specific
(6+N)-(7+N) | CRC16        | 2    | Calculated  | Big-endian
(8+N)       | Footer       | 1    | 0xAA        | Frame terminator

Minimum size (no payload): 9 bytes
Maximum size (256 payload): 265 bytes
Total: Header(2) + Seq(1) + Cmd(1) + Len(2) + Payload(0-256) + CRC(2) + Footer(1)
```

### According to Firmware (PacketBuilder.cpp + PacketValidator.cpp)

```
Byte Offset | Field        | Size | Value/Range | Notes
------------|--------------|------|-------------|--------
0-1         | Header       | 2    | 0x55AA      | Big-endian
2-3         | Sequence     | 2    | 0x0000-0xFFFF | uint16_t
4-5         | Command      | 2    | 0x0000-0xFFFF | uint16_t
6-7         | Length       | 2    | 0-256       | Big-endian, uint16_t
8+          | Payload      | 0-256| Variable    | Command-specific
(8+N)-(9+N) | CRC16        | 2    | Calculated  | Big-endian
(No footer) | -            | -    | -           | ABSENT

Minimum size (no payload): 12 bytes
Maximum size (256 payload): 268 bytes
Total: Header(2) + Seq(2) + Cmd(2) + Len(2) + Payload(0-256) + CRC(2)
```

---

## MISMATCH DETAILS

### MISMATCH #1: Sequence Field Size

**Location:**
- Specification: PROTOCOL_SPEC.md, section "PACKET STRUCTURE", line table entry for "Seq"
- Firmware: firmware/src/Protocol/PacketBuilder.h, line 36 and PacketBuilder.cpp, lines 55-58

**Specification states:**
```
Seq | 1B | 0x00-0xFF | Sequence number (rolls over)
```

**Firmware implements:**
```cpp
std::memcpy(packet + offset, &sequence, sizeof(sequence));  // sizeof(uint16_t) = 2
offset += sizeof(sequence);  // += 2
```

**Actual firmware type:** `std::uint16_t sequence` (2 bytes)

**Impact:**
- Daemon encoder following spec will write 1 byte for sequence; firmware expects 2
- All downstream field offsets will be incorrect by 1 byte
- CRC will fail because it includes wrong byte range
- Packets will be rejected by firmware as having wrong length

---

### MISMATCH #2: Command Field Size

**Location:**
- Specification: PROTOCOL_SPEC.md, section "PACKET STRUCTURE", line table entry for "Cmd"
- Firmware: firmware/src/Protocol/PacketBuilder.h, line 36 and PacketBuilder.cpp, lines 59-62

**Specification states:**
```
Cmd | 1B | See below | Command byte
```

**Firmware implements:**
```cpp
std::memcpy(packet + offset, &command, sizeof(command));  // sizeof(uint16_t) = 2
offset += sizeof(command);  // += 2
```

**Actual firmware type:** `std::uint16_t command` (2 bytes)

**Impact:**
- Daemon encoder following spec will write 1 byte for command; firmware expects 2
- Length field offset will be at byte 4-5 (spec) instead of byte 6-7 (firmware)
- All downstream fields are misaligned
- CRC calculation covers wrong byte range

---

### MISMATCH #3: Payload Length Field Offset

**Location:**
- Specification: PROTOCOL_SPEC.md, section "PACKET STRUCTURE", frame format diagram implies offset 4-5
- Firmware: firmware/src/Protocol/PacketValidator.cpp, line 41 hardcodes offset 6

**Specification implies:**
```
[Header(2)] [Seq(1)] [Cmd(1)] [Len(2)] → Length at offset 4-5
```

**Firmware implements:**
```cpp
std::memcpy(&payloadLength, packet + 6U, sizeof(payloadLength));
```

**Actual offset:** packet[6-7]

**Root cause:** Sequence (2 bytes not 1) + Command (2 bytes not 1)

**Impact:**
- Daemon encoder following spec will write length at offset 4-5
- Firmware reads it from offset 6-7
- Firmware validation will fail with `InvalidLength` error

---

### MISMATCH #4: Payload Field Offset

**Location:**
- Specification: PROTOCOL_SPEC.md, section "PACKET STRUCTURE", implicit from frame format
- Firmware: firmware/src/Protocol/PacketBuilder.cpp, line 68 writes payload after 8-byte header

**Specification implies:**
```
[Header(2)] [Seq(1)] [Cmd(1)] [Len(2)] [Payload] → Payload at offset 6+
```

**Firmware implements:**
```cpp
offset = 0;
offset += 2;  // header
offset += 2;  // sequence
offset += 2;  // command
offset += 2;  // length
// Now offset = 8, payload starts here
```

**Actual offset:** packet[8] for first payload byte

**Impact:**
- Daemon encoder following spec will write payload at offset 6
- Firmware expects it at offset 8
- CRC will include wrong data
- Validation will fail

---

### MISMATCH #5: CRC Coverage Byte Range

**Location:**
- Specification: PROTOCOL_SPEC.md, section "CRC VALIDATION", states "Coverage: Header | Seq | Cmd | Len | Payload"
- Firmware: firmware/src/Protocol/PacketBuilder.cpp, line 75 and PacketValidator.cpp, line 51

**Specification states:**
```
CRC covers: Header | Seq | Cmd | Len | Payload (NOT the CRC itself or Footer)

With spec field sizes:
CRC covers bytes 0-(5+N) where N is payload length
```

**Firmware implements:**
```cpp
const std::uint16_t crc = PacketValidator::CalculateCrc16(packet, offset);
// where offset = 8 + payloadLength
// So CRC covers bytes 0-(7+N)
```

**Actual CRC coverage:** bytes 0 through (7+payloadLength)

**Impact:**
- Spec CRC covers: Header(2) + Seq(1) + Cmd(1) + Len(2) + Payload(N) = 6+N bytes
- Firmware CRC covers: Header(2) + Seq(2) + Cmd(2) + Len(2) + Payload(N) = 8+N bytes
- CRC will be different because it includes different data
- Daemon encoder will compute wrong CRC if following spec byte ranges

---

### MISMATCH #6: Footer Presence

**Location:**
- Specification: PROTOCOL_SPEC.md, section "PACKET STRUCTURE", shows `[Footer] 1B 0xAA` in frame format
- Firmware: firmware/src/Protocol/PacketBuilder.cpp, lines 74-77 (NO footer appended)

**Specification states:**
```
[Header] [Seq] [Cmd] [Len] [Payload] [CRC16] [Footer]
                                             1B        0xAA Frame terminator
```

**Firmware implements:**
```cpp
std::memcpy(packet + offset, &crc, sizeof(crc));
return Result::Ok();  // Packet ends here, NO 0xAA appended
```

**Actual behavior:** NO footer byte

**Firmware validation (PacketValidator.cpp, line 42):**
```cpp
const std::size_t expectedLength = MinimumPacketSize + payloadLength;
// = 12 + payloadLength
// If footer existed, would be 13 + payloadLength
```

**Impact:**
- Spec-compliant encoder will append 1-byte footer; firmware does not expect it
- Minimum packet size: Spec predicts 9 bytes, firmware requires 12 bytes
- Maximum packet size: Spec predicts 265 bytes, firmware requires 268 bytes
- Firmware validation will reject all packets with footer appended (wrong length)

---

## MINIMUM AND MAXIMUM PACKET SIZES

### According to Specification

**Minimum (no payload):**
```
Header(2) + Seq(1) + Cmd(1) + Len(2) + Payload(0) + CRC(2) + Footer(1) = 9 bytes
```

**Maximum (256-byte payload):**
```
Header(2) + Seq(1) + Cmd(1) + Len(2) + Payload(256) + CRC(2) + Footer(1) = 265 bytes
```

### According to Firmware

**Minimum (no payload):**
```
Header(2) + Seq(2) + Cmd(2) + Len(2) + Payload(0) + CRC(2) = 12 bytes
```

*Source: PacketValidator.h, line 28: `MinimumPacketSize = 12U`*

**Maximum (256-byte payload):**
```
Header(2) + Seq(2) + Cmd(2) + Len(2) + Payload(256) + CRC(2) = 268 bytes
```

### Discrepancy

| Metric | Specification | Firmware | Difference |
|--------|---------------|----------|------------|
| Minimum | 9 bytes | 12 bytes | +3 bytes |
| Maximum | 265 bytes | 268 bytes | +3 bytes |

The difference arises from:
- Sequence field: 1 byte (spec) vs 2 bytes (firmware) = +1 byte
- Command field: 1 byte (spec) vs 2 bytes (firmware) = +1 byte
- Footer field: 1 byte (spec) vs 0 bytes (firmware) = -1 byte
- Net offset shift for all downstream fields = +2 bytes cumulative

*Source: firmware/src/Protocol/PacketValidator.h, line 28: `MinimumPacketSize = 12U`*

---

## SUMMARY OF MISMATCHES

| # | Field | Issue | Spec | Firmware | Source | Impact |
|---|-------|-------|------|----------|--------|--------|
| 1 | Sequence | Size | 1 byte | 2 bytes (uint16_t) | PacketBuilder.h:36, cpp:55-58 | All downstream offsets shift; CRC invalid |
| 2 | Command | Size | 1 byte | 2 bytes (uint16_t) | PacketBuilder.h:36, cpp:59-62 | Length field offset incorrect; CRC invalid |
| 3 | Length | Offset | 4-5 | 6-7 | PacketValidator.cpp:41 | Packets rejected; validation fails |
| 4 | Payload | Offset | 6+ | 8+ | PacketBuilder.cpp:68 | Data corruption; wrong byte ranges |
| 5 | CRC | Coverage | 0-5+N | 0-7+N | PacketValidator.cpp:51-54 | Different data included; CRC mismatch |
| 6 | Footer | Presence | 1 byte (0xAA) | ABSENT | PacketBuilder.cpp:74-77 | Firmware rejects packets with footer |

---

## CONSEQUENCE FOR IMPLEMENTATION

**Daemon encoder implementations following PROTOCOL_SPEC.md will be incompatible with firmware.**

Specifically:
1. Packets will have wrong total length
2. Packet structure will be byte-offset incorrectly
3. CRC will be calculated over wrong byte ranges
4. Firmware validation will reject all packets as invalid

**Example:** A daemon encoder following the spec will produce:
```
[55AA] [01] [10] [0002] [0101] [????] [AA]  (9 or 11 bytes with payload)
```

But firmware expects:
```
[55AA] [0001] [0010] [0002] [0101] [????]  (12+ bytes, no footer)
```

The firmware will reject the daemon's packet immediately during validation.

---

## STATUS

**Protocol specification and firmware are inconsistent.**

**Engineering decision required before implementation continues.**

The daemon Packet Encoder implementation (Task 9) was created following PROTOCOL_SPEC.md, but will not be compatible with the actual firmware implementation due to the field size and footer mismatches documented above.

Options:
1. Update PROTOCOL_SPEC.md to match firmware implementation
2. Modify firmware implementation to match specification
3. Create a translation layer or compatibility shim

**No code changes should proceed until this inconsistency is resolved.**
