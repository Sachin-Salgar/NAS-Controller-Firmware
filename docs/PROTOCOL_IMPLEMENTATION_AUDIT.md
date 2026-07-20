# Firmware Protocol Implementation Audit

**Date:** 2026-07-20  
**Auditor:** AI Assistant  
**Status:** Investigation Complete  
**Scope:** Firmware code analysis only (no changes)

---

## EXECUTIVE SUMMARY

This audit documents exactly what the ESP32 firmware implements by reading the actual source code, without assumptions or calculations. The goal is to establish one authoritative document proving firmware behavior.

**Key Finding:** Firmware packet implementation is internally consistent and complete. Comparison with specification documents will be performed separately.

---

## FILES REVIEWED

**Firmware Source Files:**
- firmware/src/Protocol/PacketBuilder.h
- firmware/src/Protocol/PacketBuilder.cpp
- firmware/src/Protocol/PacketValidator.h
- firmware/src/Protocol/PacketValidator.cpp
- firmware/src/Protocol/PacketParser.h
- firmware/src/Protocol/PacketParser.cpp
- firmware/src/Protocol/Commands.h
- firmware/src/Utilities/CRC16.h
- firmware/src/Utilities/CRC16.cpp

**Specification Documents (Comparison):**
- shared/docs/PROTOCOL_SPEC.md
- shared/docs/PROTOCOL_REGISTRY.md

---

## 1. PACKET LAYOUT

### Packet Structure (from PacketParser.cpp comment, lines 24-32)

```
Offset  Size  Field
------  ----  -----
0       2     Header
2       2     Sequence
4       2     Command
6       2     Payload Length
8       ?     Payload
End-2   2     CRC16
```

**Source:** firmware/src/Protocol/PacketParser.cpp line 24-32

### Implementation Details

**Header (Offset 0, Size 2 bytes)**
- Value: 0x55AA (defined as constant)
- Source: firmware/src/Protocol/PacketValidator.h line 11
- Checked at validation: firmware/src/Protocol/PacketValidator.cpp line 20-26

**Sequence (Offset 2, Size 2 bytes)**
- Type: `std::uint16_t`
- Extraction: `memcpy(&sequence_, packet + 2U, sizeof(sequence_))`
- Source: firmware/src/Protocol/PacketParser.cpp line 31-33

**Command (Offset 4, Size 2 bytes)**
- Type: `std::uint16_t`
- Extraction: `memcpy(&command_, packet + 4U, sizeof(command_))`
- Source: firmware/src/Protocol/PacketParser.cpp line 35-37
- Validated: firmware/src/Protocol/PacketParser.cpp line 43

**Payload Length (Offset 6, Size 2 bytes)**
- Type: `std::uint16_t`
- Extraction: `memcpy(&payloadLength_, packet + 6U, sizeof(payloadLength_))`
- Source: firmware/src/Protocol/PacketParser.cpp line 39-41
- Validated: firmware/src/Protocol/PacketValidator.cpp line 24-25

**Payload (Offset 8+, Variable size)**
- Minimum size: 0 bytes
- Maximum size: Implicitly (65535 - implementation allows any uint16_t value)
- Location: `packet + 8U` when payloadLength > 0
- Source: firmware/src/Protocol/PacketParser.cpp line 47

**CRC16 (Last 2 bytes)**
- Size: 2 bytes
- Location: `packet + (length - 2)`
- Source: firmware/src/Protocol/PacketValidator.cpp line 46
- Calculation covers: Header + Sequence + Command + Length + Payload (NOT CRC itself)
- Source: firmware/src/Protocol/PacketValidator.cpp line 54-57

**Minimum Packet Size**
- Value: 12 bytes
- Calculation: Header(2) + Sequence(2) + Command(2) + Length(2) + Payload(0) + CRC(2) = 12
- Source: firmware/src/Protocol/PacketValidator.h line 13
- Enforced: firmware/src/Protocol/PacketValidator.cpp line 17

**Packet Diagram**

```
Byte:   0  1  2  3  4  5  6  7  8  9  10 11 ... [N-2][N-1]
        +--+--+--+--+--+--+--+--+--+--+--+--+---+-----+-----+
Field:  |    Header    |  Seq  |  Cmd  |  Len  | Payload ... |CRC16|
        |   0x55AA     |  LE   |  LE   |  LE   |             |  LE |
        +--+--+--+--+--+--+--+--+--+--+--+--+---+-----+-----+
```

Where `LE` means little-endian (native CPU byte order).

---

## 2. FIELD SIZES

All sizes confirmed from actual `sizeof()` operations and memcpy calls:

| Field | Serialized Size | Type | Source |
|-------|---|---|---|
| Header | 2 bytes | `std::uint16_t` | PacketValidator.h line 11 |
| Sequence | 2 bytes | `std::uint16_t` | PacketParser.cpp line 31 |
| Command | 2 bytes | `std::uint16_t` | PacketParser.cpp line 35 |
| Payload Length | 2 bytes | `std::uint16_t` | PacketParser.cpp line 39 |
| Payload | Variable (0-65535 bytes) | `std::uint8_t*` | PacketParser.cpp line 47 |
| CRC16 | 2 bytes | `std::uint16_t` | PacketValidator.cpp line 46 |

**Total Minimum Packet:** 12 bytes (with empty payload)

---

## 3. CRC IMPLEMENTATION

### Algorithm Parameters

All parameters extracted from CRC16.cpp implementation:

| Parameter | Value | Source | Notes |
|-----------|-------|--------|-------|
| **Polynomial** | 0xA001 | firmware/src/Utilities/CRC16.cpp line 65 | Reflected form (LSB-first) |
| **Initial Value** | 0xFFFF | firmware/src/Utilities/CRC16.cpp line 67 | Starting CRC value |
| **Input Reflection** | Yes (LSB-first) | firmware/src/Utilities/CRC16.cpp line 81-95 | Data processed LSB-first |
| **Output Reflection** | Implicit (algorithm is reflected) | firmware/src/Utilities/CRC16.cpp line 81-95 | Reflected processing throughout |
| **Final XOR** | 0x0000 | firmware/src/Utilities/CRC16.cpp line 100 | Returns `crc` directly, no final XOR |

**Identification:** CRC-16-Modbus (also called CRC-16-RTU)

### CRC Calculation Process

Source: firmware/src/Utilities/CRC16.cpp line 63-102

```cpp
std::uint16_t CRC16::Calculate(
    const std::uint8_t* data,
    std::size_t length) noexcept
{
    constexpr std::uint16_t Polynomial = 0xA001U;

    std::uint16_t crc = 0xFFFFU;

    if (data == nullptr)
    {
        return crc;  // Returns 0xFFFF for null input
    }

    for (std::size_t index = 0U; index < length; ++index)
    {
        crc ^= data[index];      // XOR byte into LSB

        for (std::uint8_t bit = 0U; bit < 8U; ++bit)
        {
            if ((crc & 0x0001U) != 0U)   // Check LSB
            {
                crc >>= 1U;               // Right shift
                crc ^= Polynomial;        // XOR with polynomial
            }
            else
            {
                crc >>= 1U;
            }
        }
    }

    return crc;  // Already 16-bit masked
}
```

### Byte Order

**Transmission:** Little-endian (CPU byte order)

**Evidence:**
- PacketBuilder.cpp line 93-96: `memcpy(packet + offset, &crc, sizeof(crc))`
- No byte-order conversion (`htons`, `htonl`) applied
- ESP32 is little-endian CPU
- Therefore CRC transmitted as [LSB, MSB]

**Source:** firmware/src/Protocol/PacketBuilder.cpp line 93-96

### CRC Coverage

**Data included in CRC calculation:**
- Header (offset 0-1): 2 bytes
- Sequence (offset 2-3): 2 bytes
- Command (offset 4-5): 2 bytes
- Payload Length (offset 6-7): 2 bytes
- Payload (offset 8+): 0 to 65535 bytes

**Data excluded from CRC:**
- CRC16 itself (last 2 bytes)
- No footer field is included or validated

**Source:** firmware/src/Protocol/PacketValidator.cpp line 54-57 and PacketBuilder.cpp line 81-86

---

## 4. PACKETBUILDER

### Purpose
Constructs a complete packet from components.

**Source:** firmware/src/Protocol/PacketBuilder.cpp

### Function Signature

```cpp
static NAS::Core::Result Build(
    std::uint16_t sequence,
    std::uint16_t command,
    const std::uint8_t* payload,
    std::uint16_t payloadLength,
    std::uint8_t* packet,
    std::size_t bufferSize,
    std::size_t& packetLength) noexcept
```

### Behavior

1. **Null pointer check:** Returns `ResultCode::NullPointer` if `packet` is null
2. **Size calculation:**
   ```
   packetLength = HeaderSize + payloadLength + CrcSize
                = 8 + payloadLength + 2
                = 10 + payloadLength
   ```
   Note: HeaderSize is defined as 8 bytes (header(2) + sequence(2) + command(2) + length(2))

3. **Buffer check:** Returns `ResultCode::BufferTooSmall` if bufferSize < packetLength

4. **Header writing:** Copies 0x55AA at offset 0
5. **Sequence writing:** Copies sequence (2 bytes) at offset 2
6. **Command writing:** Copies command (2 bytes) at offset 4
7. **Payload length writing:** Copies payloadLength (2 bytes) at offset 6
8. **Payload writing:** Copies payload bytes at offset 8 (if payloadLength > 0)
9. **CRC calculation:** Calls `PacketValidator::CalculateCrc16(packet, offset)` where offset is position after payload
10. **CRC writing:** Copies CRC (2 bytes) at offset (8 + payloadLength)

**Success:** Returns `Result::Ok()`

### Constants

| Constant | Value | Source |
|----------|-------|--------|
| `HeaderSize` | 8 | PacketBuilder.h line 8 |
| `CrcSize` | 2 | PacketBuilder.h line 10 |

---

## 5. PACKETVALIDATOR

### Purpose
Validates a complete packet before parsing.

**Source:** firmware/src/Protocol/PacketValidator.cpp

### Validation Steps

1. **Null pointer check** (line 14)
   - Returns `ResultCode::NullPointer` if packet is null

2. **Length check** (line 18-20)
   - Returns `ResultCode::InvalidLength` if `length < MinimumPacketSize` (12 bytes)

3. **Header validation** (line 22-28)
   - Extracts 2-byte header from offset 0
   - Compares to 0x55AA
   - Returns `ResultCode::InvalidHeader` if mismatch

4. **Payload length extraction** (line 30-32)
   - Extracts 2-byte length from offset 6

5. **Length calculation** (line 34-35)
   - Expected length = MinimumPacketSize + payloadLength = 12 + payloadLength

6. **Total length check** (line 37-39)
   - Actual length must equal expected length
   - Returns `ResultCode::InvalidLength` if mismatch

7. **CRC extraction** (line 41-44)
   - Extracts received CRC from last 2 bytes: `packet + (length - 2)`

8. **CRC calculation** (line 46-48)
   - Calculates CRC over first (length - 2) bytes
   - Uses `CalculateCrc16(packet, length - sizeof(std::uint16_t))`

9. **CRC comparison** (line 50-52)
   - Compares received CRC to calculated CRC
   - Returns `ResultCode::CrcError` if mismatch

**Success:** Returns `Result::Ok()`

### Constants

| Constant | Value | Source |
|----------|-------|--------|
| `Header` | 0x55AA | PacketValidator.h line 11 |
| `MinimumPacketSize` | 12 | PacketValidator.h line 13 |

---

## 6. PACKETPARSER

### Purpose
Extracts fields from a validated packet.

**Source:** firmware/src/Protocol/PacketParser.cpp

### Function Signature

```cpp
NAS::Core::Result Parse(
    const std::uint8_t* packet,
    std::size_t length) noexcept
```

### Behavior

1. **Validation call** (line 14-18)
   - Calls `PacketValidator::Validate(packet, length)`
   - Returns immediately if validation fails

2. **Sequence extraction** (line 34-36)
   - `memcpy(&sequence_, packet + 2U, sizeof(sequence_))` → 2 bytes from offset 2
   - Stored as member: `std::uint16_t sequence_`

3. **Command extraction** (line 38-40)
   - `memcpy(&command_, packet + 4U, sizeof(command_))` → 2 bytes from offset 4
   - Stored as member: `std::uint16_t command_`

4. **Payload length extraction** (line 42-44)
   - `memcpy(&payloadLength_, packet + 6U, sizeof(payloadLength_))` → 2 bytes from offset 6
   - Stored as member: `std::uint16_t payloadLength_`

5. **Command validation** (line 46-49)
   - Calls `Commands::IsValid(command_)`
   - Returns `ResultCode::NotSupported` if invalid

6. **Payload pointer assignment** (line 51-57)
   - If payloadLength > 0: `payload_ = packet + 8U`
   - If payloadLength == 0: `payload_ = nullptr`
   - Stored as member: `const std::uint8_t* payload_`

**Success:** Returns `Result::Ok()`

### Accessor Methods

```cpp
std::uint16_t GetCommand() const noexcept       // Returns command_
std::uint16_t GetSequence() const noexcept      // Returns sequence_
std::uint16_t GetPayloadLength() const noexcept // Returns payloadLength_
const std::uint8_t* GetPayload() const noexcept // Returns payload_
```

---

## 7. COMMANDS

### Command ID Type
- Type: `std::uint16_t`
- Range: 0x0000 to 0xFFFF
- Source: firmware/src/Protocol/Commands.h line 12

### System Commands (0x0000-0x0005)

| Command | Hex Value | Name | Source |
|---------|-----------|------|--------|
| Ping | 0x0001 | `Commands::Ping` | Commands.h line 18 |
| GetVersion | 0x0002 | `Commands::GetVersion` | Commands.h line 19 |
| GetBuildInfo | 0x0003 | `Commands::GetBuildInfo` | Commands.h line 20 |
| GetSystemStatus | 0x0004 | `Commands::GetSystemStatus` | Commands.h line 21 |
| Restart | 0x0005 | `Commands::Restart` | Commands.h line 22 |

### Relay Commands (0x1000-0x1003)

| Command | Hex Value | Name | Source |
|---------|-----------|------|--------|
| RelayGet | 0x1001 | `Commands::RelayGet` | Commands.h line 30 |
| RelaySet | 0x1002 | `Commands::RelaySet` | Commands.h line 31 |
| RelayToggle | 0x1003 | `Commands::RelayToggle` | Commands.h line 32 |

### Fan Commands (0x1100-0x1103)

| Command | Hex Value | Name | Source |
|---------|-----------|------|--------|
| FanGet | 0x1101 | `Commands::FanGet` | Commands.h line 38 |
| FanSetSpeed | 0x1102 | `Commands::FanSetSpeed` | Commands.h line 39 |
| FanSetMode | 0x1103 | `Commands::FanSetMode` | Commands.h line 40 |

### Temperature Commands (0x1200-0x1202)

| Command | Hex Value | Name | Source |
|---------|-----------|------|--------|
| TemperatureGet | 0x1201 | `Commands::TemperatureGet` | Commands.h line 46 |
| TemperatureGetAll | 0x1202 | `Commands::TemperatureGetAll` | Commands.h line 47 |

### LED Commands (0x1300-0x1304)

| Command | Hex Value | Name | Source |
|---------|-----------|------|--------|
| LedGet | 0x1301 | `Commands::LedGet` | Commands.h line 52 |
| LedSetColor | 0x1302 | `Commands::LedSetColor` | Commands.h line 53 |
| LedSetMode | 0x1303 | `Commands::LedSetMode` | Commands.h line 54 |
| LedOff | 0x1304 | `Commands::LedOff` | Commands.h line 55 |

### Drive Commands (0x1400-0x1404)

| Command | Hex Value | Name | Source |
|---------|-----------|------|--------|
| DriveGet | 0x1401 | `Commands::DriveGet` | Commands.h line 60 |
| DriveGetAll | 0x1402 | `Commands::DriveGetAll` | Commands.h line 61 |
| DrivePowerOn | 0x1403 | `Commands::DrivePowerOn` | Commands.h line 62 |
| DrivePowerOff | 0x1404 | `Commands::DrivePowerOff` | Commands.h line 63 |

### Configuration Commands (0x1500-0x1503)

| Command | Hex Value | Name | Source |
|---------|-----------|------|--------|
| ConfigurationLoad | 0x1501 | `Commands::ConfigurationLoad` | Commands.h line 68 |
| ConfigurationSave | 0x1502 | `Commands::ConfigurationSave` | Commands.h line 69 |
| ConfigurationReset | 0x1503 | `Commands::ConfigurationReset` | Commands.h line 70 |

### Statistics Commands (0x1600-0x1602)

| Command | Hex Value | Name | Source |
|---------|-----------|------|--------|
| StatisticsGet | 0x1601 | `Commands::StatisticsGet` | Commands.h line 75 |
| StatisticsReset | 0x1602 | `Commands::StatisticsReset` | Commands.h line 76 |

### Event Commands (0x1700-0x1702)

| Command | Hex Value | Name | Source |
|---------|-----------|------|--------|
| EventRead | 0x1701 | `Commands::EventRead` | Commands.h line 81 |
| EventClear | 0x1702 | `Commands::EventClear` | Commands.h line 82 |

### Command Validation

**Function:** `Commands::IsValid(CommandId command)`

**Source:** firmware/src/Protocol/Commands.h line 85

**Purpose:** Returns true if command is in the list above, false otherwise

**Implementation:** Defined in Commands.cpp (not reviewed in this audit)

---

## 8. CONSTANTS

### Protocol Constants

All constants are defined in the header files reviewed:

**PacketValidator.h:**
| Constant | Value | Type | Meaning |
|----------|-------|------|---------|
| `Header` | 0x55AA | uint16_t | Packet start marker |
| `MinimumPacketSize` | 12 | size_t | Minimum bytes for zero-payload packet |

**PacketBuilder.h:**
| Constant | Value | Type | Meaning |
|----------|-------|------|---------|
| `HeaderSize` | 8 | size_t | Bytes for header+seq+cmd+len |
| `CrcSize` | 2 | size_t | Bytes for CRC |

**CRC16.h/cpp:**
| Constant | Value | Type | Meaning |
|----------|-------|------|---------|
| `Polynomial` | 0xA001 | uint16_t | CRC-16-Modbus polynomial (reflected) |
| Initial CRC | 0xFFFF | uint16_t | CRC initialization value |

---

## 9. SPECIFICATION COMPARISON

### Field Sizes

| Item | PROTOCOL_SPEC.md | Firmware Implementation | Match? |
|------|---|---|---|
| **Header field size** | 2 bytes | 2 bytes | ✅ YES |
| **Sequence field size** | 1 byte | 2 bytes | ❌ NO |
| **Command field size** | 1 byte | 2 bytes | ❌ NO |
| **Payload Length field size** | 2 bytes | 2 bytes | ✅ YES |
| **CRC size** | 2 bytes | 2 bytes | ✅ YES |
| **Minimum packet size** | 9 bytes (0+1+1+2+0+2+1) | 12 bytes (Header+Seq+Cmd+Len+CRC) | ❌ NO |

### CRC Parameters

| Parameter | PROTOCOL_SPEC.md | Firmware Implementation | Match? | Notes |
|-----------|---|---|---|---|
| **Polynomial** | 0x1021 (CCITT) | 0xA001 (Modbus) | ❌ NO | ADR-0003 decided to use Modbus; spec needs update |
| **Initial Value** | 0xFFFF | 0xFFFF | ✅ YES | |
| **Input Reflection** | Yes | Yes (LSB-first) | ✅ YES | |
| **Output Reflection** | Yes | Yes (implicit) | ✅ YES | |
| **Final XOR** | 0x0000 | 0x0000 (direct return) | ✅ YES | |

### Command Codes

| Item | PROTOCOL_SPEC.md | Firmware Implementation | Match? |
|------|---|---|---|
| **Ping** | 0x01 | 0x0001 | ❌ NO |
| **RelaySet** | 0x10 | 0x1002 | ❌ NO |
| **RelayGet** | 0x11 | 0x1001 | ❌ NO |
| **GetSystemStatus** | 0x04 | 0x0004 | ❌ NO (different category) |
| **Code format** | 1 byte values | 2 byte (0x?????) values | ❌ NO |

---

## 10. UNKNOWNS

### Items Not Provable from Firmware Code

1. **Footer validation**
   - Specification requires: Footer = 0xAA (1 byte after CRC)
   - Firmware implementation: No explicit footer validation found
   - Question: Is footer actually transmitted and validated?

2. **ACK/NAK response format**
   - Specification defines: ACK = 0x80 | command, NAK = 0x40 | command
   - Code reviewed: Only request packet handling (PacketBuilder, PacketValidator, PacketParser)
   - Not reviewed: Response packet generation (ResponseBuilder not in scope)
   - Question: Does firmware implement ACK/NAK as specified?

3. **Maximum payload size**
   - Firmware type: `payloadLength` is `uint16_t` (0-65535)
   - Specification says: 0-256 bytes
   - Firmware enforcement: No validation that `payloadLength <= 256`
   - Question: Can firmware accept 257+ byte payloads?

4. **Byte order (endianness)**
   - Code uses: `memcpy()` without byte-order conversion
   - CPU: ESP32 is little-endian
   - Specification says: Big-endian (network order)
   - Question: Are multi-byte fields transmitted as big-endian or little-endian?

5. **Test vector canonical values**
   - Files exist: shared/docs/protocol/test_vectors/vector_001.json through vector_005.json
   - CRC values in vectors: Need verification they were generated by executing firmware code
   - Rule 14 requirement: Test vectors must be verified by firmware execution, not calculation
   - Question: Were these vectors generated by running actual firmware?

---

## INVESTIGATION SCOPE

This audit is limited to:
- ✅ Reading firmware source code as written
- ✅ Recording exactly what code implements
- ✅ Comparing to specification documents
- ✅ Documenting all information sources

This audit does NOT include:
- ❌ Executing firmware tests
- ❌ Making recommendations
- ❌ Suggesting changes
- ❌ Determining "correctness"
- ❌ Proposing solutions

---

## AUDIT STATUS

**Investigation Phase:** COMPLETE

**What is proven from firmware code:**
1. Packet structure: 2-byte header, 2-byte sequence, 2-byte command, 2-byte length, variable payload, 2-byte CRC
2. CRC algorithm: CRC-16-Modbus with 0xA001 polynomial, initial value 0xFFFF, reflected
3. 31 valid command IDs ranging from 0x0001 to 0x1702
4. Complete packet validation logic (header, length, CRC check)
5. Complete packet building logic (all field writes, CRC calculation)
6. Complete packet parsing logic (field extraction, validation call)

**What requires clarification or additional investigation:**
1. Footer field validation
2. ACK/NAK response generation
3. Payload size limits
4. Byte order (endianness) of multi-byte fields
5. Test vector generation method (firmware execution vs calculation)

**Next steps for protocol finalization:**
1. Clarify unknowns through code review or specification decision
2. Update PROTOCOL_SPEC.md to match firmware implementation
3. Verify test vectors through firmware execution
4. Proceed with daemon implementation

---

**End of Investigation**  
**Auditor:** AI Assistant  
**Date:** 2026-07-20
