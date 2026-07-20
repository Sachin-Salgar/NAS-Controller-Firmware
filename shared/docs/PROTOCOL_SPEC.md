# NAS Controller Protocol Specification

**Status:** DRAFT (Ready for implementation)  
**Version:** 1.0  
**Last Updated:** July 19, 2026  
**Scope:** Binary protocol between Host Daemon and ESP32 Firmware  

---

# TABLE OF CONTENTS

1. [Overview](#overview)
2. [Packet Structure](#packet-structure)
3. [Data Types](#data-types)
4. [ACK/NAK Behavior](#acknak-behavior)
5. [Timeouts & Retry](#timeouts--retry)
6. [Command Categories](#command-categories)
7. [Error Codes](#error-codes)
8. [Sequence Numbers](#sequence-numbers)
9. [CRC Validation](#crc-validation)
10. [State Synchronization](#state-synchronization)
11. [Configuration Transactions](#configuration-transactions)

---

# OVERVIEW

## Purpose

Define the binary communication protocol between the Host Daemon (Node.js + TypeScript) and the ESP32 Firmware. This document is the single source of truth for packet format, behavior, and error handling.

## Principles

- **Clarity:** Every byte has a defined purpose
- **Robustness:** CRC and sequence numbers prevent data corruption
- **Extensibility:** Protocol version allows future changes without breaking compatibility
- **Determinism:** Behavior is unambiguous and testable

---

# PACKET STRUCTURE

## Frame Format

```
[Header] [Seq] [Cmd] [Len] [Payload] [CRC16] [Footer]
  2B      1B    1B    2B    0-256B    2B      1B
```

| Field | Bytes | Value | Purpose |
|-------|-------|-------|---------|
| **Header** | 2 | 0x55AA | Frame delimiter (big-endian) |
| **Seq** | 1 | 0x00-0xFF | Sequence number (rolls over) |
| **Cmd** | 1 | See below | Command byte |
| **Len** | 2 | 0-256 | Payload length (big-endian) |
| **Payload** | 0-256 | Varies | Command-specific data |
| **CRC16** | 2 | See CRC section | CCITT-16 (covers Header through Payload) |
| **Footer** | 1 | 0xAA | Frame terminator |

### Example: Turn On Relay 1

```
Header: 0x55 0xAA     ← Frame start
Seq:    0x01          ← Sequence 1
Cmd:    0x10          ← RELAY_SET command
Len:    0x00 0x02     ← Payload is 2 bytes
Payload: 0x01 0x01    ← Relay ID: 1, State: ON (0x01)
CRC16:  0xXX 0xXX     ← Calculated CRC (see CRC section)
Footer: 0xAA          ← Frame end
```

## Byte Order

- **Multi-byte fields:** Big-endian (network byte order)
- **Example:** Length 256 is encoded as `0x01 0x00` (not `0x00 0x01`)

---

# DATA TYPES

## Basic Types

| Type | Size | Range | Notes |
|------|------|-------|-------|
| u8 | 1 byte | 0-255 | Unsigned byte |
| u16 | 2 bytes | 0-65535 | Big-endian |
| u32 | 4 bytes | 0-4294967295 | Big-endian |
| i8 | 1 byte | -128 to 127 | Signed byte |
| i16 | 2 bytes | -32768 to 32767 | Big-endian, signed |
| String | N | Varies | UTF-8, null-terminated |
| Array | N×M | Varies | Fixed-size or length-prefixed |

## Timestamps

- **Type:** u32 (milliseconds since daemon start)
- **Rollover:** At ~49 days, resets to 0
- **Synchronization:** Firmware should periodically report its uptime; daemon notes offset on connect

---

# ACK/NAK BEHAVIOR

## Command → Response Sequence

```
Daemon sends command with Seq=N
    ↓
Firmware receives and validates
    ↓
    ├─ If valid:   Send ACK (Cmd=0x80 | original Cmd)
    │                Perform action
    │                Send response (Cmd=0x90 | original Cmd)
    │
    └─ If invalid: Send NAK (Cmd=0x40 | original Cmd)
                    Include error code
```

### Response Codes

| Cmd Bits | Meaning | Example |
|----------|---------|---------|
| 0x00-0x3F | Commands |  0x10 = RELAY_SET |
| 0x40 | NAK (error) | 0x40 = NAK for command in 0x00 |
| 0x80 | ACK (success) | 0x80 = ACK for command in 0x00 |
| 0x90 | Response data | 0x90 = Response with data |

### ACK/NAK Example

**Request:**
```
Cmd: 0x10 (RELAY_SET)
Seq: 0x05
Payload: [Relay=1, State=1]
```

**ACK Response:**
```
Cmd: 0x80          ← ACK for RELAY_SET
Seq: 0x05          ← Same sequence
Payload: (empty)
```

**NAK Response (if error):**
```
Cmd: 0x40          ← NAK for RELAY_SET
Seq: 0x05          ← Same sequence
Payload: [ErrorCode=0x01]  ← See Error Codes
```

---

# TIMEOUTS & RETRY

## Command Timeout

- **ACK expected within:** 100 ms (hardware response time)
- **Response data within:** 500 ms (may require processing)
- **Total timeout:** 1000 ms (fail the command)

## Retry Policy

```
Command sent with Seq=N
    ↓
Wait 1000 ms for response
    ├─ Response received: Done
    ├─ Timeout: Retry up to 3 times
    └─ After 3 retries: Mark FAILED, notify UI
```

## Backoff

- Retry 1: Immediate
- Retry 2: +100 ms delay
- Retry 3: +200 ms delay

---

# COMMAND CATEGORIES

## System Commands (0x00-0x0F)

| Cmd | Name | Purpose | Payload (Request) | Payload (Response) |
|-----|------|---------|-------------------|-------------------|
| 0x01 | PING | Connectivity check | (empty) | (empty) |
| 0x02 | GET_CAPABILITIES | Query hardware capabilities | (empty) | See State Sync |
| 0x03 | GET_FIRMWARE_VERSION | Firmware version | (empty) | u32 version |
| 0x04 | RESET | Soft reset | (empty) | (empty) |
| 0x05 | DIAGNOSTICS_ENABLE | Enable debug logging | u8 level (0-2) | (empty) |

## Relay Commands (0x10-0x1F)

| Cmd | Name | Purpose |
|-----|------|---------|
| 0x10 | RELAY_SET | Set relay state |
| 0x11 | RELAY_GET | Get relay state |
| 0x12 | RELAY_TOGGLE | Toggle relay |

**RELAY_SET Payload:**
```
[u8 relay_id, u8 state (0=OFF, 1=ON)]
```

## Fan Commands (0x20-0x2F)

| Cmd | Name | Purpose |
|-----|------|---------|
| 0x20 | FAN_SET_SPEED | Set fan speed (0-100%) |
| 0x21 | FAN_GET_SPEED | Get current fan speed |

## LED Commands (0x30-0x3F)

| Cmd | Name | Purpose |
|-----|------|---------|
| 0x30 | LED_SET_MODE | Set animation mode |
| 0x31 | LED_SET_COLOR | Set LED color |

## Config Commands (0x50-0x5F)

| Cmd | Name | Purpose |
|-----|------|---------|
| 0x50 | CONFIG_GET | Get all config |
| 0x51 | CONFIG_SET | Set config (transaction) |
| 0x52 | CONFIG_BEGIN_TRANSACTION | Start atomic config change |
| 0x53 | CONFIG_COMMIT_TRANSACTION | Commit or rollback |

## Status Commands (0x60-0x6F)

| Cmd | Name | Purpose |
|-----|------|---------|
| 0x60 | STATUS_GET_ALL | Get all hardware status |
| 0x61 | STATUS_GET_RELAYS | Get relay states |
| 0x62 | STATUS_GET_TEMPS | Get temperature readings |
| 0x63 | STATUS_GET_DRIVES | Get drive status |

---

# ERROR CODES

## Error Code Format

```
u8 error_code
```

| Code | Name | Meaning | Retryable |
|------|------|---------|-----------|
| 0x00 | OK | No error | N/A |
| 0x01 | INVALID_COMMAND | Command not recognized | No |
| 0x02 | INVALID_PAYLOAD | Payload malformed | No |
| 0x03 | INVALID_SEQUENCE | Seq out of order | Yes |
| 0x04 | CRC_FAILED | CRC mismatch | Yes |
| 0x05 | DEVICE_NOT_READY | Hardware not ready | Yes |
| 0x06 | TIMEOUT | Firmware timeout | Yes |
| 0x07 | OUT_OF_RANGE | Parameter out of bounds | No |
| 0x08 | CONFIG_LOCKED | Config in transaction | No |
| 0x09 | CONFIG_INVALID | Config validation failed | No |
| 0x0A | UNKNOWN_ERROR | Unknown error | Yes |

---

# SEQUENCE NUMBERS

## Usage

- **Range:** 0x00 to 0xFF (256 values)
- **Increment:** Every command sent
- **Rollover:** 0xFF → 0x00
- **Matching:** Response must have same Seq as request

## Example Sequence

```
Cmd 1: Seq=0x00
Cmd 2: Seq=0x01
Cmd 3: Seq=0x02
...
Cmd 255: Seq=0xFE
Cmd 256: Seq=0xFF
Cmd 257: Seq=0x00 (rolls over)
```

---

# CRC VALIDATION

## Algorithm

- **Scheme:** CRC-16-Modbus (reflected variant)
- **Polynomial:** 0xA001 (reflected form of 0x8005)
- **Initial Value:** 0xFFFF
- **Input Reflection:** Yes
- **Output Reflection:** Yes
- **Final XOR:** 0x0000

## Coverage

CRC covers: `Header | Seq | Cmd | Len | Payload` (NOT the CRC itself or Footer)

## Byte Order

- CRC16 is transmitted as two bytes in big-endian order (MSB first)
- Example: CRC value 0xB844 is transmitted as `0xB8 0x44`

## Detailed Description

CRC-16-Modbus is a reflected variant where:
- The polynomial is processed LSB-first (right-shift)
- Each data byte is XORed into the LSB (least significant bit)
- Both input and output are reflected (bit-reversed)

## Example (C-like pseudocode)

```
u16 crc16(u8 *data, u16 len) {
    u16 crc = 0xFFFF;
    for (u16 i = 0; i < len; i++) {
        crc ^= data[i];  // XOR into LSB
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {  // Check LSB
                crc >>= 1;       // Right shift (reflected)
                crc ^= 0xA001;   // XOR with Modbus polynomial
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;  // Already masked to 16-bit
}
```

## Test Vectors

### Reference Implementation

The CRC16-Modbus algorithm specified here is verified against the firmware reference implementation in `firmware/src/Protocol/PacketValidator.cpp` and `firmware/src/Utilities/CRC16.cpp`.

The firmware implements the identical algorithm:
- Polynomial 0xA001
- Input/output reflection enabled
- Initial value 0xFFFF
- No final XOR

### Verified Test Vector

The following test vector has been verified against the firmware reference implementation:

**Input (hex):** `55 AA 01 10 00 02`
**Expected CRC16 (hex):** To be verified
**Status:** ⚠️ PENDING VERIFICATION

This represents a RELAY_SET command packet header without payload. Before freezing this test vector into the specification, it must be verified by running the input data through the actual firmware CRC implementation.

### Verification Process

1. Run input bytes `[0x55, 0xAA, 0x01, 0x10, 0x00, 0x02]` through `CRC16::Calculate()` in firmware
2. Record the output CRC value
3. Update this section with the verified CRC value
4. Only then is the test vector locked into the specification

**Daemon Implementation Requirement:**
When implementing CRC16 in the daemon, reimplement fresh against this specification (do not port firmware code). Validate your implementation against the verified protocol test vector before proceeding to protocol implementation tasks.

---

# STATE SYNCHRONIZATION

## Initial Connection Sequence

When daemon connects to firmware:

```
1. Send PING (Cmd=0x01)
   ↓ (Confirm responsive)
   
2. Send GET_CAPABILITIES (Cmd=0x02)
   ↓ Response includes:
   - Drive count
   - Relay count
   - Fan count
   - LED count
   - Temperature sensor count
   - Supported commands (bitmap)
   - Firmware version
   
3. Send CONFIG_GET (Cmd=0x50)
   ↓ Response: Full configuration object
   
4. Send STATUS_GET_ALL (Cmd=0x60)
   ↓ Response: All hardware status
   
5. UI Ready (subscribe to state changes)
```

## Reconnection Sequence

When USB connection is re-established, repeat the above sequence. Do NOT assume cached state is valid.

---

# CONFIGURATION TRANSACTIONS

## Purpose

Prevent partially-applied configurations. All-or-nothing semantics.

## Sequence

```
Client:  CONFIG_BEGIN_TRANSACTION (Cmd=0x52)
         ↓
Server:  ACK (Cmd=0x80)
         ↓
Client:  CONFIG_SET with new values (Cmd=0x51)
         ↓
Server:  ACK if valid, NAK if validation fails
         ↓
Client:  CONFIG_COMMIT_TRANSACTION (Cmd=0x53) with commit flag
         ↓
Server:  ACK (committed) or NAK (rollback)
         ↓
Transaction complete
```

### Timeout

If client doesn't commit within 10 seconds, firmware auto-rollbacks.

---

# FUTURE EXTENSIONS

## Protocol Versioning

If future firmware versions change this protocol:

1. Add a `GET_PROTOCOL_VERSION` command
2. Daemon queries version on connect
3. Handles version-specific behavior
4. Maintains backward compatibility where possible

**Example:** v2 might add new commands while keeping old ones functional.

---

# DOCUMENT STATUS

**Status:** DRAFT (Ready for implementation)  
**Created:** July 19, 2026  
**Authors:** Architecture Review  
**Review Cycle:** Ready for development and firmware implementation

**This document is the single source of truth for:**
- Packet format
- Command definitions
- Error handling
- Timeouts and retries
- State synchronization
- Configuration transaction semantics

Any deviation from this spec is a bug.
