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
  2B      2B    2B    2B    0-256B    2B      1B
```

| Field | Bytes | Value | Purpose |
|-------|-------|-------|---------|
| **Header** | 2 | 0x55AA | Frame delimiter (big-endian) |
| **Seq** | 2 | 0x0000-0xFFFF | Sequence number (rolls over) |
| **Cmd** | 2 | See below | Command ID (2-byte) |
| **Len** | 2 | 0-256 | Payload length (big-endian) |
| **Payload** | 0-256 | Varies | Command-specific data |
| **CRC16** | 2 | See CRC section | CRC-16-Modbus (covers Header through Payload) |
| **Footer** | 1 | 0xAA | Frame terminator |

### Example: Turn On Relay 1

```
Header: 0x55 0xAA     ← Frame start
Seq:    0x00 0x01     ← Sequence 1 (2-byte big-endian)
Cmd:    0x10 0x02     ← RELAY_SET command (0x1002 in 2-byte big-endian)
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

## System Commands (0x0000-0x0FFF)

| Cmd | Name | Purpose | Payload (Request) | Payload (Response) |
|-----|------|---------|-------------------|-------------------|
| 0x0001 | PING | Connectivity check | (empty) | (empty) |
| 0x0002 | GET_VERSION | Firmware version | (empty) | u32 version |
| 0x0003 | GET_BUILD_INFO | Build information | (empty) | String |
| 0x0004 | GET_SYSTEM_STATUS | Query hardware status | (empty) | See State Sync |
| 0x0005 | RESTART | Soft reset | (empty) | (empty) |

## Relay Commands (0x1000-0x1FFF)

| Cmd | Name | Purpose |
|-----|------|---------|
| 0x1001 | RELAY_GET | Get relay state |
| 0x1002 | RELAY_SET | Set relay state |
| 0x1003 | RELAY_TOGGLE | Toggle relay |

**RELAY_SET Payload:**
```
[u8 relay_id, u8 state (0=OFF, 1=ON)]
```

## Fan Commands (0x1100-0x1FFF)

| Cmd | Name | Purpose |
|-----|------|---------|
| 0x1101 | FAN_GET | Get fan speed and status |
| 0x1102 | FAN_SET_SPEED | Set fan speed (0-100%) |
| 0x1103 | FAN_SET_MODE | Set fan operating mode |

## Temperature Commands (0x1200-0x1FFF)

| Cmd | Name | Purpose |
|-----|------|---------|
| 0x1201 | TEMPERATURE_GET | Get single temperature sensor |
| 0x1202 | TEMPERATURE_GET_ALL | Get all temperature readings |

## LED Commands (0x1300-0x1FFF)

| Cmd | Name | Purpose |
|-----|------|---------|
| 0x1301 | LED_GET | Get LED state |
| 0x1302 | LED_SET_COLOR | Set LED color (RGB) |
| 0x1303 | LED_SET_MODE | Set animation mode |
| 0x1304 | LED_OFF | Turn off LED |

## Drive Commands (0x1400-0x1FFF)

| Cmd | Name | Purpose |
|-----|------|---------|
| 0x1401 | DRIVE_GET | Get single drive status |
| 0x1402 | DRIVE_GET_ALL | Get all drive status |
| 0x1403 | DRIVE_POWER_ON | Power on a drive |
| 0x1404 | DRIVE_POWER_OFF | Power off a drive |

## Configuration Commands (0x1500-0x1FFF)

| Cmd | Name | Purpose |
|-----|------|---------|
| 0x1501 | CONFIGURATION_LOAD | Get all config |
| 0x1502 | CONFIGURATION_SAVE | Write config (transaction) |
| 0x1503 | CONFIGURATION_RESET | Reset to defaults |

## Statistics Commands (0x1600-0x1FFF)

| Cmd | Name | Purpose |
|-----|------|---------|
| 0x1601 | STATISTICS_GET | Get runtime statistics |
| 0x1602 | STATISTICS_RESET | Reset statistics counters |

## Event Commands (0x1700-0x1FFF)

| Cmd | Name | Purpose |
|-----|------|---------|
| 0x1701 | EVENT_READ | Read event log entries |
| 0x1702 | EVENT_CLEAR | Clear event log |

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

- **Range:** 0x0000 to 0xFFFF (65536 values)
- **Increment:** Every command sent
- **Rollover:** 0xFFFF → 0x0000
- **Matching:** Response must have same Seq as request
- **Byte Order:** Big-endian (MSB first)

## Example Sequence

```
Cmd 1: Seq=0x0001
Cmd 2: Seq=0x0002
Cmd 3: Seq=0x0003
...
Cmd 255: Seq=0x00FF
Cmd 256: Seq=0x0100
Cmd 65535: Seq=0xFFFF
Cmd 65536: Seq=0x0000 (rolls over)
```

---

# CRC VALIDATION

## Algorithm

- **Scheme:** CRC-16-Modbus (reflected variant)
- **Polynomial:** 0xA001 (reflected form of standard 0x8005)
- **Initial Value:** 0xFFFF
- **Input Reflection:** Yes (LSB-first, data bits reflect into LSB of CRC)
- **Output Reflection:** Yes (algorithm produces reflected result)
- **Final XOR:** 0x0000 (no final XOR applied)

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

The CRC16-Modbus algorithm specified here is verified against the firmware reference implementation in `firmware/src/Utilities/CRC16.cpp::CRC16::Calculate()`.

### Official Protocol Test Vector

**Purpose:** Verify CRC16 implementation correctness across all components (firmware, daemon, frontend, diagnostics)

**Status:** ⏳ **PENDING FIRMWARE EXECUTION VERIFICATION** (as of 2026-07-20)

#### Test Vector #1: Relay Set Command Header

**Input Bytes (hex):**
```
55 AA 01 10 00 02
```

**Input Description:**
RELAY_SET command packet header (without payload)
- Header: 0x55AA
- Sequence: 0x01
- Command: 0x10 (RELAY_SET)
- Payload Length: 0x0002 (big-endian)

**CRC Algorithm Parameters:**
- Scheme: CRC-16-Modbus (reflected variant)
- Polynomial: 0xA001 (reflected form of 0x8005)
- Initial Value: 0xFFFF
- Input Reflection: Yes (LSB-first processing)
- Output Reflection: Yes (reflected algorithm)
- Final XOR: 0x0000
- **Byte Order: [PENDING VERIFICATION]**

**Expected CRC Output:**
```
[PENDING FIRMWARE EXECUTION - awaiting actual output from firmware]
```

**Required Verification Steps (Next Session):**
1. Execute `firmware/src/Utilities/CRC16.cpp::Calculate(0x55, 0xAA, 0x01, 0x10, 0x00, 0x02)`
2. Record actual CRC output from firmware
3. Confirm byte order: Is transmitted as `[MSB, LSB]` or `[LSB, MSB]`?
4. Verify against PacketBuilder.cpp transmission (lines 93-96)

**Verification Details:**
- Firmware Version: 1.0
- Protocol Version: 1.0
- Implementation: firmware/src/Utilities/CRC16.cpp::CRC16::Calculate()
- Verification Method: **Firmware execution (not calculated)** - awaiting next session
- Verified Against: Will cross-check with PacketValidator.cpp::CalculateCrc16()

**Daemon Implementation Requirement:**
When implementing CRC16 in the daemon:
1. Reimplement fresh against this specification (do not port firmware code)
2. After official test vector is verified via firmware execution, test daemon against it
3. Your daemon CRC output must match firmware output for this input
4. Do not proceed beyond Task 2 until daemon CRC matches official verified vector

**Rule 14 Compliance Note:** Per Rule 14 (Implementation Verification), this vector must be generated by executing the actual firmware code, not by manual calculation. Do not mark as VERIFIED until firmware execution confirms the output.

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

# INTEROPERABILITY REQUIREMENT

## Canonical Protocol Test Vector Compliance

Any independent implementation of the NAS Controller protocol **SHALL produce identical packet bytes** for all canonical protocol test vectors.

This requirement applies to:
- ✅ Firmware reference implementation (ESP32)
- ✅ Daemon (Node.js + TypeScript)
- ✅ Python tools and utilities
- ✅ Test harnesses and diagnostic tools
- ✅ Android/iOS mobile applications
- ✅ WebAssembly implementations
- ✅ Any future implementation or client

### What "Identical Packet Bytes" Means

**NOT just identical CRC values.** Complete packet bytes must match byte-for-byte, including:
- Header (0x55AA)
- Sequence number
- Command byte
- Length field (big-endian)
- Payload (exact byte sequence)
- CRC16 (2 bytes, big-endian)
- Footer (0xAA)

### Canonical Test Vectors

Official test vectors are stored in `shared/docs/protocol/test_vectors/`:
- `vector_001.json` - Minimum valid packet
- `vector_002.json` - Typical relay command
- `vector_003.json` - Maximum payload packet
- `vector_004.json` - Complex/random payload
- `vector_005.json` - CRC failure detection

All implementations must pass all five vectors.

### Verification Process

Before any implementation is considered complete:

1. **Firmware executes** vector inputs and records outputs
2. **Test vectors frozen** with verified expected values
3. **Daemon implementation tested** against frozen vectors
4. **Integration test** validates round-trip communication
5. **All other implementations** must match firmware output exactly

### Why This Matters

This requirement ensures:
- **Protocol correctness** is verified at the system integration level
- **Data integrity** is preserved across all implementations
- **Debugging** is easier (identical bytes = identical behavior)
- **Future compatibility** is guaranteed (all implementations agree)
- **Regression detection** is automatic (byte mismatch = bug)

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
- Interoperability requirements

Any deviation from this spec is a bug.
