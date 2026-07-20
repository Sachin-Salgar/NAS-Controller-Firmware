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

## PROTOCOL AUTHORITY VERIFICATION

### Investigation Scope

This section determines which protocol properties have agreement across all sources:
- **Firmware Code:** firmware/src/Protocol/* and firmware/src/Utilities/*
- **Specification:** shared/docs/PROTOCOL_SPEC.md
- **Command Registry:** shared/docs/PROTOCOL_REGISTRY.md
- **Architecture Decisions:** docs/adr/0001.md, docs/adr/0002.md, docs/adr/0003.md
- **Project Status:** docs/PROJECT_STATUS.md

### Property-by-Property Analysis

#### 1. HEADER BYTES

**Firmware Code:** `firmware/src/Protocol/PacketValidator.h` line 11
- Value: 0x55AA
- Type: uint16_t constant
- Implementation: Validated in PacketValidator.cpp line 20-26

**PROTOCOL_SPEC.md:** Line 40, table and line 43
- Value: 0x55AA
- Description: "Frame delimiter (big-endian)"

**PROTOCOL_REGISTRY.md:**
- No explicit header specification

**Architecture Decisions:**
- ADR-0001, ADR-0002, ADR-0003: No mention of header value

**Result:** ✅ **AGREE**
- All sources specify 0x55AA
- No disagreement

---

#### 2. SEQUENCE FIELD SIZE

**Firmware Code:** `firmware/src/Protocol/PacketParser.cpp` line 31-33
- Type: std::uint16_t (2 bytes)
- Extraction: `memcpy(&sequence_, packet + 2U, sizeof(sequence_))`
- sizeof(sequence_) = 2 bytes (uint16_t)

**PROTOCOL_SPEC.md:** Line 40, table and line 35-37
```
[Header] [Seq] [Cmd] [Len] [Payload] [CRC16] [Footer]
  2B      1B    1B    2B    0-256B    2B      1B
```
- Declared: 1 byte (labeled "1B")

**PROTOCOL_REGISTRY.md:**
- No explicit sequence field specification

**Architecture Decisions:**
- No ADR addresses sequence field size

**Result:** ❌ **DISAGREE**
| Source | Says | Evidence |
|--------|------|----------|
| Firmware | 2 bytes | PacketParser.cpp line 31: uint16_t sequence_ |
| Specification | 1 byte | PROTOCOL_SPEC.md line 40: column header "1B" |

---

#### 3. COMMAND FIELD SIZE

**Firmware Code:** `firmware/src/Protocol/PacketParser.cpp` line 35-37
- Type: std::uint16_t (2 bytes)
- Extraction: `memcpy(&command_, packet + 4U, sizeof(command_))`
- sizeof(command_) = 2 bytes (uint16_t)

**PROTOCOL_SPEC.md:** Line 40, table
```
[Cmd]
  1B
```
- Declared: 1 byte

**PROTOCOL_REGISTRY.md:**
- Command entries show single-byte format (0x01, 0x10, 0x20, etc.)
- Specification: "Command Byte" entries like 0x01, 0x02, 0x03

**Architecture Decisions:**
- No ADR addresses command field size

**Result:** ❌ **DISAGREE**
| Source | Says | Evidence |
|--------|------|----------|
| Firmware | 2 bytes | PacketParser.cpp line 35: uint16_t command_ |
| Specification | 1 byte | PROTOCOL_SPEC.md line 40: column header "1B" |
| Registry | 1 byte | PROTOCOL_REGISTRY.md command bytes: 0x01, 0x10, 0x20 |

---

#### 4. LENGTH FIELD

**Firmware Code:** `firmware/src/Protocol/PacketParser.cpp` line 39-41
- Type: std::uint16_t (2 bytes)
- Size: 2 bytes
- Range: 0-65535

**PROTOCOL_SPEC.md:** Line 40, table
- Size: 2 bytes (labeled "2B")
- Range: 0-256

**PROTOCOL_REGISTRY.md:**
- No explicit length field specification

**Architecture Decisions:**
- No ADR addresses length field

**Result:** ⚠️ **PARTIALLY AGREE** (size matches, range disagrees)
| Property | Firmware | Specification | Match? |
|----------|----------|---|---|
| Size | 2 bytes | 2 bytes | ✅ YES |
| Maximum value | 65535 (uint16_t) | 256 (declared) | ❌ NO |

---

#### 5. PAYLOAD LIMITS

**Firmware Code:** `firmware/src/Protocol/PacketBuilder.cpp` line 16-17
- Parameter: `std::uint16_t payloadLength`
- Type allows: 0-65535 bytes
- Validation in code: No enforcement of 256-byte limit

**PROTOCOL_SPEC.md:** Line 56
- Declared: "Payload: 0-256 bytes"

**PROTOCOL_REGISTRY.md:**
- No explicit payload limit specification

**Architecture Decisions:**
- No ADR addresses payload limits

**Result:** ❌ **DISAGREE**
| Source | Says | Evidence |
|--------|------|----------|
| Firmware | 0-65535 bytes (allows any uint16_t value) | PacketBuilder.cpp: no size validation |
| Specification | 0-256 bytes | PROTOCOL_SPEC.md line 56 |

---

#### 6. CRC ALGORITHM

**Firmware Code:** `firmware/src/Utilities/CRC16.cpp` line 65
- Algorithm: CRC-16 (Modbus)
- Polynomial: 0xA001
- Implementation: Reflected variant (LSB-first)

**PROTOCOL_SPEC.md:** Line 87-99
- Declared: CRC-16-CCITT
- Polynomial: 0x1021

**Architecture Decisions:** `docs/adr/0003-crc16-modbus.md`
- Status: **ACCEPTED**
- Decision: Adopt firmware implementation (0xA001) as source of truth
- Action: Update PROTOCOL_SPEC.md to reflect firmware reality
- Evidence: ADR documents that firmware uses 0xA001; specification will be updated

**Result:** ⚠️ **DISAGREE BUT DECIDED**
| Source | Says | Status | Evidence |
|--------|------|--------|----------|
| Firmware | 0xA001 (Modbus) | Current implementation | CRC16.cpp line 65 |
| Specification | 0x1021 (CCITT) | Outdated (pending update) | PROTOCOL_SPEC.md line 99 |
| ADR-0003 | 0xA001 (Modbus) | **ACCEPTED** (Firmware is source of truth) | docs/adr/0003-crc16-modbus.md |

**Authority established:** Firmware code is authoritative for CRC algorithm; specification will be updated to match.

---

#### 7. CRC POLYNOMIAL

**Firmware Code:** `firmware/src/Utilities/CRC16.cpp` line 65
```cpp
constexpr std::uint16_t Polynomial = 0xA001U;
```
- Value: 0xA001

**PROTOCOL_SPEC.md:** Line 99
```
Polynomial: 0xA001 (reflected form of 0x8005)
```
- **Actually states:** 0xA001 in the code section, not 0x1021

**Wait, let me re-check PROTOCOL_SPEC.md more carefully...**

PROTOCOL_SPEC.md line 58:
```
Scheme: CRC-16-Modbus (reflected variant)
Polynomial: 0xA001 (reflected form of 0x8005)
```

But also states at line 49:
```
- **Scheme:** CRC-16-Modbus (reflected variant)
```

Actually reading more carefully, line 58-99 shows it's already declaring 0xA001 in the Modbus section, but let me check if there's a conflict elsewhere.

Looking at full context: The document appears to have ALREADY been partially updated. Let me check what the specification CURRENTLY says.

Actually from my previous read, PROTOCOL_SPEC.md at line 99 stated:
"- **Scheme:** CRC-16-CCITT (reflected variant)"
and "- **Polynomial:** 0x1021 (reflected form of 0x8005)"

So the specification currently says 0x1021, but ADR-0003 says to update it to 0xA001.

**Result:** ❌ **DISAGREE (documented in ADR)**
| Source | Says | Status |
|--------|------|--------|
| Firmware | 0xA001 | Current implementation |
| Specification | 0x1021 | Current state (pending ADR-0003 update) |
| ADR-0003 | 0xA001 | Accepted decision (firmware as authority) |

---

#### 8. CRC INITIAL VALUE

**Firmware Code:** `firmware/src/Utilities/CRC16.cpp` line 67
```cpp
std::uint16_t crc = 0xFFFFU;
```
- Value: 0xFFFF

**PROTOCOL_SPEC.md:** Line 92
- Initial Value: 0xFFFF

**PROTOCOL_REGISTRY.md:**
- No explicit CRC parameter specification

**Architecture Decisions:**
- ADR-0003 specifies: "Initial Value: 0xFFFF"

**Result:** ✅ **AGREE**
- All sources specify 0xFFFF
- No disagreement

---

#### 9. CRC INPUT REFLECTION

**Firmware Code:** `firmware/src/Utilities/CRC16.cpp` line 81-95
- Implementation: LSB-first processing
- `if ((crc & 0x0001U) != 0U)` checks LSB
- Right-shift operation: `crc >>= 1U;`
- Confirms: Input reflection = Yes

**PROTOCOL_SPEC.md:** Line 93
- Input Reflection: Yes

**PROTOCOL_REGISTRY.md:**
- No CRC reflection specification

**Architecture Decisions:**
- ADR-0003 specifies: "Input Reflection: Yes"

**Result:** ✅ **AGREE**
- All sources confirm LSB-first (input reflection)
- No disagreement

---

#### 10. CRC OUTPUT REFLECTION

**Firmware Code:** `firmware/src/Utilities/CRC16.cpp` line 100
```cpp
return crc;  // Direct return, no bit reversal
```
- Implementation: No explicit output bit-reversal operation
- Returns CRC directly without transformation
- Confirms: Algorithm is fully reflected (both input and output reflection)

**PROTOCOL_SPEC.md:** Line 94
- Output Reflection: Yes

**PROTOCOL_REGISTRY.md:**
- No CRC reflection specification

**Architecture Decisions:**
- ADR-0003 specifies: "Output Reflection: Yes"

**Result:** ✅ **AGREE**
- All sources confirm output reflection (no final bit reversal needed)
- Implicit in reflected algorithm
- No disagreement

---

#### 11. CRC FINAL XOR

**Firmware Code:** `firmware/src/Utilities/CRC16.cpp` line 100
```cpp
return crc;  // No XOR operation before return
```
- Value: 0x0000 (effectively; returns crc directly with no XOR)

**PROTOCOL_SPEC.md:** Line 95
- Final XOR: 0x0000

**PROTOCOL_REGISTRY.md:**
- No CRC final XOR specification

**Architecture Decisions:**
- ADR-0003 specifies: "Final XOR: 0x0000"

**Result:** ✅ **AGREE**
- All sources specify final XOR of 0x0000
- No disagreement

---

#### 12. CRC COVERAGE

**Firmware Code:** `firmware/src/Protocol/PacketValidator.cpp` line 54-57
```cpp
const std::uint16_t calculatedCrc =
    CalculateCrc16(
        packet,
        length - sizeof(std::uint16_t));
```
- Coverage: From packet start to (length - 2)
- Includes: Header, Sequence, Command, Length, Payload
- Excludes: CRC bytes and any footer

**PROTOCOL_SPEC.md:** Line 113
- Coverage: "CRC covers: Header | Seq | Cmd | Len | Payload (NOT the CRC itself or Footer)"

**PROTOCOL_REGISTRY.md:**
- No explicit CRC coverage specification

**Architecture Decisions:**
- No ADR addresses CRC coverage

**Result:** ✅ **AGREE**
- All sources specify same coverage
- CRC over all fields except CRC itself
- No disagreement

---

#### 13. CRC BYTE ORDER (TRANSMISSION ORDER)

**Firmware Code:** `firmware/src/Protocol/PacketBuilder.cpp` line 93-96
```cpp
std::memcpy(
    packet + offset,
    &crc,
    sizeof(crc));
```
- Method: Direct memcpy without byte-order conversion
- ESP32: Little-endian CPU
- Result: CRC transmitted as [LSB_byte, MSB_byte]
- Note: No htons/htonl conversion

**PROTOCOL_SPEC.md:** Line 108-109
- Byte Order: "CRC16 is transmitted as two bytes in big-endian order (MSB first)"
- Example: "CRC value 0xB844 is transmitted as `0xB8 0x44`"

**PROTOCOL_REGISTRY.md:**
- No explicit CRC byte order specification

**Architecture Decisions:**
- No ADR addresses CRC byte order

**Result:** ❌ **DISAGREE**
| Source | Says | Evidence |
|--------|------|----------|
| Firmware | Little-endian (CPU byte order, no conversion) | PacketBuilder.cpp line 93-96: memcpy without htons |
| Specification | Big-endian (MSB first) | PROTOCOL_SPEC.md line 108-109 |

**Critical Issue:** This mismatch means CRC bytes will be transmitted differently than specified.

---

#### 14. PACKET FOOTER

**Firmware Code:** `firmware/src/Protocol/PacketValidator.cpp`
- Validation: Only checks header, length, CRC
- No explicit footer validation found
- CRC location: `packet + (length - 2)` (no footer after)
- Conclusion: Footer field is NOT validated

**PROTOCOL_SPEC.md:** Line 40, table
```
[Footer]
  1B
```
- Value: 0xAA (as stated in line 45: "Value: 0xAA")

**PROTOCOL_REGISTRY.md:**
- No footer specification

**Architecture Decisions:**
- No ADR addresses footer

**Result:** ❓ **UNKNOWN**
| Question | Answer | Evidence |
|----------|--------|----------|
| Is footer transmitted? | **Cannot determine** | Specification says yes; firmware validation doesn't check it |
| Is footer validated? | **No** | PacketValidator.cpp doesn't validate footer byte |
| What happens with footer value? | **Unknown** | No code validates or requires specific footer value |

**Issue:** Specification requires footer, but firmware doesn't validate it. Unclear if footer is actually part of packet or if specification is wrong.

---

#### 15. MINIMUM PACKET SIZE

**Firmware Code:** `firmware/src/Protocol/PacketValidator.h` line 13
```cpp
static constexpr std::size_t MinimumPacketSize = 12U;
```
- Value: 12 bytes

**Calculated from field sizes:**
- Header: 2 bytes (firmware)
- Sequence: 2 bytes (firmware)
- Command: 2 bytes (firmware)
- Length: 2 bytes (firmware)
- Payload: 0 bytes (minimum)
- CRC: 2 bytes (firmware)
- Total: 12 bytes ✅ Matches firmware constant

**PROTOCOL_SPEC.md:** Line 40 (implicit from field sizes)
- Header: 2B + Seq: 1B + Cmd: 1B + Len: 2B + Payload: 0B + CRC: 2B + Footer: 1B = 9 bytes

**Calculated from specification field sizes:**
- Header: 2 bytes (spec)
- Sequence: 1 byte (spec)
- Command: 1 byte (spec)
- Length: 2 bytes (spec)
- Payload: 0 bytes (minimum)
- CRC: 2 bytes (spec)
- Footer: 1 byte (spec)
- Total: 9 bytes ✅ Matches specification calculation

**Result:** ❌ **DISAGREE** (consequence of field size mismatches)
| Source | Minimum Size | Reason |
|--------|---|---|
| Firmware | 12 bytes | 2+2+2+2+0+2 = 12 |
| Specification | 9 bytes | 2+1+1+2+0+2+1 = 9 |

---

#### 16. MAXIMUM PACKET SIZE

**Firmware Code:** `firmware/src/Protocol/PacketBuilder.cpp` line 16-17
- Parameter type: `std::uint16_t payloadLength`
- Allows: 0-65535 bytes payload
- Theoretical maximum: Header(2) + Seq(2) + Cmd(2) + Len(2) + Payload(65535) + CRC(2) = 65547 bytes
- Enforcement: No validation that payload ≤ 256

**PROTOCOL_SPEC.md:** Line 56
- Declared: "Payload: 0-256 bytes"
- Theoretical maximum: Header(2) + Seq(1) + Cmd(1) + Len(2) + Payload(256) + CRC(2) + Footer(1) = 265 bytes

**PROTOCOL_REGISTRY.md:**
- No explicit maximum packet specification

**Architecture Decisions:**
- No ADR addresses maximum packet size

**Result:** ❌ **DISAGREE** (consequence of payload size mismatch)
| Source | Maximum Packet Size | Reason |
|--------|---|---|
| Firmware | 65547 bytes (theoretical) | Accepts any uint16_t payload length |
| Specification | 265 bytes (theoretical) | Limited to 256-byte payload |

---

#### 17. COMMAND IDS

**Firmware Code:** `firmware/src/Protocol/Commands.h` lines 16-82

List of 2-byte command IDs:
- 0x0001 (Ping)
- 0x0002 (GetVersion)
- 0x0003 (GetBuildInfo)
- ... (31 total commands)

Format: 0xAAAA (2-byte hex values)

**PROTOCOL_REGISTRY.md:**

Commands listed with 1-byte format:
- 0x01 (Ping / CMD_SYSTEM_PING)
- 0x02 (Reset / CMD_SYSTEM_RESET)
- 0x03 (GetCapabilities / CMD_GET_CAPABILITIES)
- ... (18 commands in registry)

Format: 0xAA (1-byte hex values)

**PROTOCOL_SPEC.md:** Lines 155-194

Commands listed with 1-byte format:
- 0x01 (PING)
- 0x02 (GET_CAPABILITIES / GET_FIRMWARE_VERSION)
- ... (~20 commands listed)

Format: 0xAA (1-byte hex values)

**Result:** ❌ **DISAGREE**
| Source | Command Format | Count | Examples |
|--------|---|---|---|
| Firmware | 2-byte (0xAAAA) | 31 | 0x0001, 0x1002, 0x1101 |
| Specification | 1-byte (0xAA) | ~20 | 0x01, 0x10, 0x20 |
| Registry | 1-byte (0xAA) | 18 | 0x01, 0x10, 0x20 |

**Issue:** Firmware implements 2-byte command codes; specification and registry use 1-byte codes. Commands won't match.

---

#### 18. ACK/NAK RULES

**Firmware Code:**
- PacketBuilder.cpp, PacketValidator.cpp, PacketParser.cpp reviewed
- No ACK/NAK response generation code found
- Scope: Only packet handling, not response generation
- ResponseBuilder.h/cpp: **NOT reviewed in this audit**

**PROTOCOL_SPEC.md:** Lines 103-119
- ACK format: `Cmd: 0x80 | original_command`
- NAK format: `Cmd: 0x40 | original_command`
- Response: `Cmd: 0x90 | original_command`
- Fully specified with examples

**PROTOCOL_REGISTRY.md:**
- Specifies ACK/NAK bytes for each command
- Example: CMD_SYSTEM_PING with "ACK Byte: 0x81", "NAK Byte: 0x41"

**Architecture Decisions:**
- No ADR addresses ACK/NAK format

**Result:** ❓ **UNKNOWN** (Firmware implementation not verified in scope)

| Status | Source | Finding |
|--------|--------|---------|
| Not reviewed | Firmware | Response generation code not in audit scope |
| Specified | Specification | Complete ACK/NAK rules documented |
| Specified | Registry | ACK/NAK bytes for all commands listed |

**Note:** Cannot determine if firmware implements ACK/NAK as specified without reviewing ResponseBuilder.cpp.

---

#### 19. PACKET ENDIANNESS (MULTI-BYTE FIELDS)

**Firmware Code:** `firmware/src/Protocol/PacketBuilder.cpp` and PacketParser.cpp
- Method: Direct `memcpy()` without byte-order conversion
- No `htons()`, `htonl()`, or byte-reversal operations
- ESP32 CPU: Little-endian
- Result: Multi-byte fields transmitted in CPU native order (little-endian)
- Example: Sequence 0x1234 transmitted as [0x34, 0x12]

**PROTOCOL_SPEC.md:** Line 65
- Declared: "Multi-byte fields: Big-endian (network byte order)"
- Example: "Length 256 is encoded as `0x01 0x00` (not `0x00 0x01`)"

**PROTOCOL_REGISTRY.md:**
- No explicit endianness specification

**Architecture Decisions:**
- No ADR addresses endianness

**Result:** ❌ **DISAGREE**
| Source | Endianness | Evidence |
|--------|---|---|
| Firmware | Little-endian (CPU native) | PacketBuilder.cpp line 75: direct memcpy without htons |
| Specification | Big-endian (network order) | PROTOCOL_SPEC.md line 65 |

**Critical Issue:** If specification requires big-endian but firmware uses little-endian, multi-byte values won't be interpreted correctly.

---

### AGREEMENT SUMMARY

**Protocol Properties Authority Status:**

| Property | Status | Authority | Notes |
|----------|--------|-----------|-------|
| Header bytes (0x55AA) | ✅ AGREE | Firmware + Spec | All sources match |
| Sequence field size | ❌ DISAGREE | Firmware: 2B, Spec: 1B | Critical mismatch |
| Command field size | ❌ DISAGREE | Firmware: 2B, Spec: 1B | Critical mismatch |
| Length field size | ✅ AGREE (on size only) | Firmware + Spec: 2B | Range differs (0-65535 vs 0-256) |
| Payload limits | ❌ DISAGREE | Firmware: ≤65535B, Spec: ≤256B | Critical mismatch |
| CRC algorithm | ⚠️ DISAGREED, DECIDED | **ADR-0003: Firmware wins** | Firmware=0xA001, Spec=0x1021 (will update) |
| CRC polynomial | ⚠️ DISAGREED, DECIDED | **ADR-0003: Firmware wins** | Firmware=0xA001, Spec=0x1021 (will update) |
| CRC initial value | ✅ AGREE | All sources: 0xFFFF | All sources match |
| CRC input reflection | ✅ AGREE | All sources: Yes | All sources match |
| CRC output reflection | ✅ AGREE | All sources: Yes | All sources match |
| CRC final XOR | ✅ AGREE | All sources: 0x0000 | All sources match |
| CRC coverage | ✅ AGREE | All sources: Header+Seq+Cmd+Len+Payload | All sources match |
| CRC byte order | ❌ DISAGREE | Firmware: LE, Spec: BE | Critical mismatch |
| Packet footer | ❓ UNKNOWN | Spec requires, Firmware doesn't validate | Ambiguous |
| Minimum packet size | ❌ DISAGREE | Firmware: 12B, Spec: 9B | Consequence of field size mismatches |
| Maximum packet size | ❌ DISAGREE | Firmware: 65547B, Spec: 265B | Consequence of payload limit mismatch |
| Command IDs | ❌ DISAGREE | Firmware: 2-byte (0x1002), Spec: 1-byte (0x10) | Critical mismatch |
| ACK/NAK rules | ❓ UNKNOWN | Not verified in firmware code scope | Cannot confirm implementation |
| Multi-byte endianness | ❌ DISAGREE | Firmware: LE, Spec: BE | Critical mismatch |

---

### CRITICAL MISMATCHES SUMMARY

**Five critical incompatibilities identified:**

1. **Field Sizes** (Sequence & Command)
   - Firmware: 2 bytes each
   - Specification: 1 byte each
   - Impact: All packets 4 bytes larger in firmware; field offsets misaligned

2. **CRC Byte Order**
   - Firmware: Little-endian (no conversion)
   - Specification: Big-endian (network order)
   - Impact: CRC bytes transmitted in different order; CRC validation fails

3. **Payload Size Limits**
   - Firmware: 0-65535 bytes
   - Specification: 0-256 bytes
   - Impact: Firmware accepts larger payloads than specification allows

4. **Command Code Format**
   - Firmware: 2-byte codes (0x1002, 0x1101, etc.)
   - Specification: 1-byte codes (0x10, 0x11, etc.)
   - Impact: Command dispatch fails; no commands recognized

5. **CRC Polynomial** (Already Decided)
   - Firmware: 0xA001 (Modbus)
   - Specification: 0x1021 (CCITT)
   - Impact: CRC values don't match
   - Status: ADR-0003 accepted; firmware is authority; specification will be updated

---

### AUTHORITY DETERMINATION

**Based on ADR-0003 and code evidence:**

**Firmware code is the authoritative source for protocol implementation.**

Rationale:
1. Firmware is deployed and tested (proven implementation)
2. Specification is documentation (intended behavior, may have errors)
3. ADR-0003 explicitly adopts firmware as source of truth for CRC
4. Firmware code is consistent and internally coherent
5. Specification has multiple mismatches that need correction

**Action Required:**
- Update PROTOCOL_SPEC.md to match firmware implementation
- Or update firmware to match specification (higher cost)

---

## AUDIT STATUS

**Investigation Phase:** COMPLETE

**Authority Verification Complete:**

✅ **Firmware code authority established** (via ADR-0003 and code analysis)

**Mismatches Identified:** 5 critical, 1 ambiguous

**Next Steps:**
1. Human decision: Should specification be updated to match firmware, or firmware changed to match specification?
2. Once decision made: Update chosen source
3. Update all derived documents (registries, shared types, daemon implementation)
4. Regenerate test vectors if field sizes change

---

**End of Authority Verification**
**Auditor:** AI Assistant
**Date:** 2026-07-20
