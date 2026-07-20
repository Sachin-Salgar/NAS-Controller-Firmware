# Protocol Specification v1.0.0

**NAS Controller Binary Protocol**

**Status:** Verification in Progress (90% Complete)  
**Version:** 1.0.0 (Draft - Pending Freeze)  
**Last Updated:** July 2026  
**Freeze Authority:** Technical Lead

---

## Overview

The NAS Controller Protocol is a binary, packet-based, request/response protocol for communication between a host computer and an ESP32-based NAS controller.

**Key Characteristics:**
- Transport-independent (USB CDC, UART, Ethernet, etc.)
- Little-endian byte order throughout
- CRC16-Modbus error detection
- Command-based architecture
- Fixed packet structure
- Request/response communication model

---

## Protocol Versioning

| Version | Status | Date | Notes |
|---------|--------|------|-------|
| 1.0.0 | Verification in Progress | Jul 2026 | Initial protocol version |

**Backward Compatibility:** Once frozen, Protocol 1.0.0 will maintain backward compatibility within the 1.x release cycle. Version 2.0.0 will permit breaking changes.

---

## Packet Format

### Structure

Every packet follows this exact structure:

```
Offset  Size   Field          Description
------  ----   -----          -----------
0       2      Header         Fixed value: 0x55AA
2       2      Sequence       Host-controlled, firmware-echoed
4       2      Command        Command identifier (enumerated)
6       2      Length         Payload length (0-500 bytes)
8       N      Payload        Command-specific data
8+N     2      CRC16          CRC16-Modbus over header through payload
```

### Total Packet Size

```
Total = 8 bytes (header + seq + cmd + len) + Payload Length + 2 bytes (CRC)
      = 10 + Payload Length
Minimum: 12 bytes (empty payload)
Maximum: 512 bytes (500 bytes payload + 12 bytes overhead)
```

### Byte Order

**All multi-byte fields use little-endian (LSB-first) byte order.**

Example: `0x55AA` is transmitted as bytes `[0xAA, 0x55]`

---

## Field Specifications

### Header

**Value:** `0x55AA` (fixed, always the same)

**Transmission:** Little-endian  
- Byte 0: `0xAA`
- Byte 1: `0x55`

**Purpose:** Packet frame delimiter. Every valid packet begins with this exact header.

**Validation:** If header ≠ 0x55AA, reject packet immediately.

---

### Sequence Number

**Type:** `uint16_t` (2 bytes, little-endian)

**Range:** 0x0000 to 0xFFFF (wraps around)

**Behavior:**
- Host increments sequence for each request (application-managed)
- Host may use any numbering scheme (sequential, random, etc.)
- Firmware copies received sequence into response unchanged
- Allows host to match responses to requests

**Example:**
```
Request 1:  Sequence = 0x0001
Request 2:  Sequence = 0x0002
Response 1: Sequence = 0x0001 (matched)
Request 3:  Sequence = 0x0003
```

---

### Command

**Type:** `uint16_t` (2 bytes, little-endian)

**Range:** Enumerated in PROTOCOL_REGISTRY.md

**Categories:**
- `0x0000-0x00FF`: System commands (Ping, Version, Status, Reset)
- `0x0100-0x01FF`: Relay commands
- `0x0200-0x02FF`: Fan commands
- `0x0300-0x03FF`: Temperature commands
- `0x0400-0x04FF`: LED commands
- `0x0500-0x05FF`: Drive commands
- `0x0600-0x06FF`: Configuration commands
- `0x0700-0x07FF`: Statistics commands
- `0x0800-0x08FF`: Event commands

**Unknown Command:** If firmware receives unrecognized command, response includes ResultCode = `0x0004` (UnknownCommand).

---

### Length

**Type:** `uint16_t` (2 bytes, little-endian)

**Range:** 0 to 500 (bytes)

**Meaning:** Number of payload bytes following this field.

**Validation Rules:**
- Must match actual payload size
- If Length ≠ actual_payload_size, reject packet
- Empty payload: Length = 0 (packet is still 12 bytes minimum)

**Example:**
```
Payload = "HELLO" (5 ASCII bytes)
Length field = 0x0005 (transmitted as [0x05, 0x00])
```

---

### Payload

**Type:** Variable length (0-500 bytes)

**Format:** Command-specific

**Structure:** Defined per command in PROTOCOL_REGISTRY.md

**Rules:**
- No padding
- No alignment requirements
- No reserved bytes
- Exact match to command specification

---

### CRC16

**Type:** `uint16_t` (2 bytes, little-endian)

**Calculation:** CRC16-Modbus over bytes 0 through (8 + Length - 1)

**Placement:** Last 2 bytes of packet

**Transmission Order:** Little-endian (LSB first)

---

## CRC16-Modbus Algorithm

### Specification

| Parameter | Value |
|-----------|-------|
| Algorithm | CRC16-Modbus (Reflected) |
| Polynomial | 0xA001 |
| Initial Value | 0xFFFF |
| Input Reflection | Yes (reflected) |
| Output Reflection | Yes (reflected) |
| Final XOR | 0x0000 (none) |

### Calculation

```pseudocode
function CRC16_Calculate(data, length):
    polynomial = 0xA001
    crc = 0xFFFF
    
    for index = 0 to length - 1:
        crc ^= data[index]
        
        for bit = 0 to 7:
            if (crc & 0x0001) != 0:
                crc >>= 1
                crc ^= polynomial
            else:
                crc >>= 1
    
    return crc
```

### Reference Implementation

See `src/Utilities/CRC16.cpp` (firmware)

### Test Vector

**Input:** bytes `[0x55, 0xAA, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00]` (empty payload packet)

**Expected CRC:** `0x5F4D`

---

## Validation Rules

### Strict Packet Validation

Every received packet must satisfy ALL of:

1. **Length Check:** Packet size ≥ 12 bytes
2. **Header Check:** First 2 bytes = 0x55AA (little-endian)
3. **Size Check:** Packet size = (12 + Length field)
4. **CRC Check:** Calculated CRC matches received CRC

If any check fails, reject packet and return error.

### Error Response Codes

| Code | Name | Description |
|------|------|-------------|
| 0x0000 | Ok | Success |
| 0x0001 | InvalidHeader | Header mismatch |
| 0x0002 | InvalidLength | Size inconsistency |
| 0x0003 | CrcError | CRC mismatch |
| 0x0004 | UnknownCommand | Unrecognized command |
| 0x0005 | InvalidPayload | Payload doesn't match command spec |
| 0x0006 | Busy | Device temporarily busy |
| 0x0007 | InternalError | Firmware internal error |

---

## Communication Model

### Request/Response Pattern

```
Host                           Firmware
│                              │
├─→ Request Packet ────────────→│
│                              │
│                      Process & Execute
│                              │
│←──── Response Packet ─────────┤
│                              │
```

**Rules:**
- Every request receives exactly one response
- Response includes original sequence number
- Response includes result code (0x0000 for success)
- No unsolicited messages from firmware
- Firmware does not queue requests (must process sequentially)

### Timeout Handling

- **Host:** Implement timeout for request/response (suggested: 1-5 seconds)
- **Firmware:** Process each command to completion before accepting next request
- **Recovery:** Close and reopen USB connection if timeout occurs

---

## Multiple Commands Example

### Scenario: Host sends 3 commands

```
Time   Host                              Firmware
────   ────────────────────────────────  ────────────────────────
T1     Send: Ping (seq=0x0001)
       [55 AA 01 00 00 00 00 00 ... CRC]
                                        Receive & validate
                                        Execute Ping
T2                                      Send: Pong (seq=0x0001)
       Receive: Pong (seq=0x0001)
       [55 AA 01 00 00 00 00 00 ... CRC]
       
T3     Send: Get Relay (seq=0x0002)
       [55 AA 02 00 01 10 00 00 ... CRC]
                                        Receive & validate
                                        Read relay state
T4                                      Send: Response (seq=0x0002)
       Receive: Response (seq=0x0002)
       [55 AA 02 00 01 10 02 00 ... CRC]
       
T5     Send: Set Relay ON (seq=0x0003)
       [55 AA 03 00 02 10 01 00 01 ... CRC]
                                        Receive & validate
                                        Set relay ON
T6                                      Send: Response (seq=0x0003)
       Receive: Response (seq=0x0003)
       [55 AA 03 00 02 10 00 00 ... CRC]
```

---

## Payload Specifications

### Command-Specific Format

Each command defines its request and response payload:

**Format:**
- Byte offset (0-indexed, relative to start of payload)
- Data type (uint8, uint16, uint32, string, etc.)
- Byte order (always little-endian)
- Value range or valid values
- Example values

**Example:** Relay Set Command

```
Request Payload:
  Offset 0: uint8    Relay Index (0-7)
  Offset 1: uint8    Relay State (0=OFF, 1=ON)

Response Payload:
  Offset 0: uint8    Result Code (0=OK, other=error)
  Offset 1: uint8    Relay Index (echoed)
  Offset 2: uint8    Relay State (current state)
```

Complete payload specifications are documented in **PROTOCOL_REGISTRY.md**.

---

## Transport Independence

The packet format remains identical across transports:

| Transport | Baud/Speed | Notes |
|-----------|-----------|-------|
| USB CDC | 115200 | Virtual COM port |
| UART | 115200 | Serial connection |
| TCP/IP | Ethernet | Future |
| UDP | Ethernet | Future |
| Wi-Fi | 802.11 | Future |
| Bluetooth | BLE | Future |

**No protocol changes for different transports.** Only the underlying transport layer changes.

---

## Interoperability Requirement

**CRITICAL REQUIREMENT:**

> Any independent implementation of Protocol v1.0.0 shall produce identical packet bytes for every canonical protocol test vector.

This includes:
- ✅ Firmware (reference implementation)
- ✅ Daemon (host service)
- ✅ Python utilities
- ✅ Mobile applications
- ✅ WebAssembly implementations
- ✅ Future implementations

**Verification Method:** All implementations must successfully validate all test vectors in `shared/docs/protocol/test_vectors/`.

**Failure to pass any vector = implementation is non-compliant.**

---

## Packet Capture Example

### Ping Command (Empty Payload)

```
Packet: [AA 55 01 00 00 00 00 00 4D 5F]

Breakdown:
  Offset 0-1:  AA 55      = 0x55AA (header, little-endian)
  Offset 2-3:  01 00      = 0x0001 (sequence)
  Offset 4-5:  00 00      = 0x0000 (command: Ping)
  Offset 6-7:  00 00      = 0x0000 (length: no payload)
  Offset 8-9:  4D 5F      = 0x5F4D (CRC16-Modbus)
  
Total Size: 10 bytes (12 minimum, but this shows without padding)
Actually Transmitted: 10 bytes (no payload padding needed)
```

---

## Future Protocol Changes

**For Protocol v1.1.0 or later:**

Any modification requires:
1. New ADR document
2. PROTOCOL_SPEC.md update
3. New command enumeration in PROTOCOL_REGISTRY.md
4. New canonical test vectors
5. Firmware implementation
6. Daemon implementation
7. Integration testing
8. Explicit protocol version freeze

See **PROJECT_RULES.md Rule 15** for complete governance process.

---

## Verification Status

### Firmware Verification ✓ (Complete)

- ✅ CRC16-Modbus algorithm verified
- ✅ Packet structure verified
- ✅ Header validation verified
- ✅ Payload handling verified
- ✅ Response generation verified

### Test Vectors 🔄 (In Progress)

- 🔄 Generate minimum 5 canonical vectors
- 🔄 Document vector metadata
- 🔄 Include firmware-generated CRC values
- 🔄 Create vector validation harness

### Daemon Implementation ⬜ (Pending)

- ⬜ CRC implementation against firmware
- ⬜ Packet assembly and validation
- ⬜ All canonical vectors passing
- ⬜ Integration testing complete

### Protocol Freeze ⬜ (Pending)

Requirements before freeze:
- ✅ Specification documented
- ✅ Firmware reference implementation complete
- 🔄 Canonical test vectors complete
- ⬜ Daemon implementation complete
- ⬜ All vectors passing in daemon
- ⬜ Integration tests passing

See **shared/docs/protocol/PROTOCOL_FREEZE_CHECKLIST.md** for complete checklist.

---

## References

- **PROTOCOL_REGISTRY.md** — All enumerated protocol elements
- **shared/docs/protocol/test_vectors/** — Canonical test fixtures
- **src/Utilities/CRC16.cpp** — Firmware CRC implementation
- **src/Protocol/PacketBuilder.cpp** — Firmware packet assembly
- **src/Protocol/PacketValidator.cpp** — Firmware packet validation
- **PROJECT_RULES.md** — Rule 14 (verification chain)
- **docs/USB-Host-Protocol.md** — Communication overview

---

## Document Control

| Field | Value |
|-------|-------|
| Status | Draft (Verification in Progress) |
| Version | 1.0.0 |
| Created | July 2026 |
| Last Updated | July 2026 |
| Lock Status | Will be locked after freeze |
| Authority | Technical Lead + Stakeholders |

---

## Next Steps

This specification will transition to **FROZEN** status once:

1. ✅ All protocol elements documented
2. ✅ CRC algorithm verified against firmware
3. 🔄 Canonical test vectors generated (5 minimum)
4. ⬜ Daemon implementation passing vectors
5. ⬜ Integration tests complete

**Target Freeze Date:** End of Phase 3 (current week)

See **IMPLEMENTATION_ORDER.md** for phase transition criteria.
