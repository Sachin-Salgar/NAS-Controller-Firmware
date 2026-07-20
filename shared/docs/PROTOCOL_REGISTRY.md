# Protocol Registry v1.0.0

**NAS Controller Binary Protocol — Complete Enumeration**

**Status:** Verification in Progress  
**Version:** 1.0.0  
**Last Updated:** July 2026  
**Scope:** Protocol 1.0.0 elements (commands, codes, constants)

---

## Purpose

This document is the authoritative enumeration of all protocol elements for Protocol v1.0.0:

- All command codes and categories
- All result codes and error conditions
- All constant values
- All byte ranges and enumeration values

**Every protocol implementation must reference this document.**

---

## Result Codes

Response result codes returned in command responses:

| Code | Hex | Name | Description |
|------|-----|------|-------------|
| 0 | 0x0000 | Ok | Operation succeeded |
| 1 | 0x0001 | InvalidHeader | Packet header mismatch |
| 2 | 0x0002 | InvalidLength | Payload/packet length mismatch |
| 3 | 0x0003 | CrcError | CRC16 calculation mismatch |
| 4 | 0x0004 | UnknownCommand | Command not recognized |
| 5 | 0x0005 | InvalidPayload | Payload doesn't match command spec |
| 6 | 0x0006 | Busy | Device temporarily busy (retry) |
| 7 | 0x0007 | InternalError | Firmware internal error |

---

## Protocol Constants

| Constant | Value | Hex | Purpose |
|----------|-------|-----|---------|
| Packet Header | 21930 | 0x55AA | Frame delimiter |
| CRC Polynomial | 40961 | 0xA001 | CRC16-Modbus polynomial |
| CRC Initial | 65535 | 0xFFFF | CRC16-Modbus initial value |
| Minimum Packet Size | 12 | 0x000C | Header + seq + cmd + len + CRC |
| Maximum Packet Size | 512 | 0x0200 | Total packet bytes |
| Maximum Payload | 500 | 0x01F4 | Payload bytes (512 - 12) |
| USB Baud Rate | 115200 | — | Default serial speed |

---

## System Commands

### Category: System (0x0000-0x00FF)

#### PING (0x0000)

**Purpose:** Verify controller responsiveness

**Direction:** Host → Controller

**Request Payload:** Empty (0 bytes)

**Response Payload:**
```
Status: Ok (0x0000)
(No additional payload)
```

**Use Case:** Periodic keepalive, connection verification

**Status:** Planned

---

#### HELLO (0x0001)

**Purpose:** Establish communication session

**Direction:** Host → Controller

**Request Payload:** Empty (0 bytes)

**Response Payload:**
```
Offset 0: uint16  Firmware Version (little-endian)
Offset 2: uint8   Feature Flags
Offset 3: uint8   Reserved
```

**Status:** Planned

---

#### GET VERSION (0x0002)

**Purpose:** Read firmware version and build information

**Direction:** Host → Controller

**Request Payload:** Empty

**Response Payload:**
```
Offset 0: uint16  Major Version (little-endian)
Offset 2: uint16  Minor Version (little-endian)
Offset 4: uint16  Patch Version (little-endian)
Offset 6: uint32  Build Number (little-endian)
Offset 10: uint8  Build Type (0=Debug, 1=Release)
Offset 11: uint8  Reserved
```

**Status:** Planned

---

#### GET STATUS (0x0003)

**Purpose:** Retrieve complete controller status

**Direction:** Host → Controller

**Request Payload:** Empty

**Response Payload:**
```
Offset 0: uint8   Uptime (hours)
Offset 1: uint8   Temperature (°C)
Offset 2: uint8   Relay Bitmask (bits 0-7 = relay state)
Offset 3: uint8   Fan Speed (0-100%)
Offset 4: uint16  Drive Bitmask (bits 0-15 = drive present)
Offset 6: uint8   System Health (0=Healthy, 1=Warning, 2=Critical)
```

**Status:** Planned

---

#### RESET (0x0004)

**Purpose:** Software restart

**Direction:** Host → Controller

**Request Payload:** Empty

**Response Payload:**
```
Status: Ok (sent before restart)
(No additional payload)
Note: Firmware restarts immediately after response sent
```

**Status:** Planned

---

#### GET UPTIME (0x0005)

**Purpose:** Read controller uptime

**Direction:** Host → Controller

**Request Payload:** Empty

**Response Payload:**
```
Offset 0: uint32  Uptime (seconds since boot)
```

**Status:** Planned

---

### Reserved System Commands

```
0x0006-0x00FF: Reserved for future system commands
```

---

## Relay Commands

### Category: Relay (0x0100-0x01FF)

#### GET RELAY (0x0100)

**Purpose:** Read relay state

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   Relay Index (0-7)
```

**Response Payload:**
```
Offset 0: uint8   Relay Index (echoed)
Offset 1: uint8   Relay State (0=Off, 1=On)
```

**Status:** Planned

---

#### SET RELAY (0x0101)

**Purpose:** Set relay state

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   Relay Index (0-7)
Offset 1: uint8   Relay State (0=Off, 1=On)
```

**Response Payload:**
```
Offset 0: uint8   Relay Index (echoed)
Offset 1: uint8   New Relay State
```

**Status:** Planned

---

#### GET ALL RELAYS (0x0102)

**Purpose:** Read all relay states

**Direction:** Host → Controller

**Request Payload:** Empty

**Response Payload:**
```
Offset 0: uint8   Relay Bitmask (bit 0=Relay0, bit 7=Relay7)
```

**Status:** Planned

---

### Reserved Relay Commands

```
0x0103-0x01FF: Reserved for future relay commands
```

---

## Fan Commands

### Category: Fan (0x0200-0x02FF)

#### GET FAN (0x0200)

**Purpose:** Read fan PWM value

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   Fan Index (0-3)
```

**Response Payload:**
```
Offset 0: uint8   Fan Index (echoed)
Offset 1: uint8   Fan Speed (0-100%)
```

**Status:** Planned

---

#### SET FAN (0x0201)

**Purpose:** Set fan speed

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   Fan Index (0-3)
Offset 1: uint8   Fan Speed (0-100%)
```

**Response Payload:**
```
Offset 0: uint8   Fan Index (echoed)
Offset 1: uint8   New Fan Speed
```

**Status:** Planned

---

#### GET FAN MODE (0x0202)

**Purpose:** Read fan control mode

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   Fan Index (0-3)
```

**Response Payload:**
```
Offset 0: uint8   Fan Index (echoed)
Offset 1: uint8   Mode (0=Manual, 1=Automatic)
```

**Status:** Planned

---

#### SET FAN MODE (0x0203)

**Purpose:** Set fan control mode

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   Fan Index (0-3)
Offset 1: uint8   Mode (0=Manual, 1=Automatic)
```

**Response Payload:**
```
Offset 0: uint8   Fan Index (echoed)
Offset 1: uint8   New Mode
```

**Status:** Planned

---

### Reserved Fan Commands

```
0x0204-0x02FF: Reserved for future fan commands
```

---

## Temperature Commands

### Category: Temperature (0x0300-0x03FF)

#### GET TEMPERATURE (0x0300)

**Purpose:** Read temperature from specific sensor

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   Sensor Index (0-7)
```

**Response Payload:**
```
Offset 0: uint8   Sensor Index (echoed)
Offset 1: int8    Temperature (°C, -40 to +85)
Offset 2: uint8   Sensor Status (0=OK, 1=Error, 2=Disconnected)
```

**Status:** Planned

---

#### GET ALL TEMPERATURES (0x0301)

**Purpose:** Read all temperature sensors

**Direction:** Host → Controller

**Request Payload:** Empty

**Response Payload:**
```
Offset 0: uint8   Sensor Count (N)
Offset 1: int8    Temp 0 (°C)
Offset 2: int8    Temp 1 (°C)
...
Offset N: int8    Temp N-1 (°C)
```

**Status:** Planned

---

#### SET TEMPERATURE THRESHOLD (0x0302)

**Purpose:** Set alarm threshold for sensor

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   Sensor Index (0-7)
Offset 1: int8    Warning Threshold (°C)
Offset 2: int8    Critical Threshold (°C)
```

**Response Payload:**
```
Offset 0: uint8   Sensor Index (echoed)
Offset 1: uint8   Status
```

**Status:** Planned

---

### Reserved Temperature Commands

```
0x0303-0x03FF: Reserved for future temperature commands
```

---

## LED Commands

### Category: LED (0x0400-0x04FF)

#### GET LED (0x0400)

**Purpose:** Read LED state and color

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   LED Index (0 for main strip)
```

**Response Payload:**
```
Offset 0: uint8   LED Index (echoed)
Offset 1: uint8   Red (0-255)
Offset 2: uint8   Green (0-255)
Offset 3: uint8   Blue (0-255)
Offset 4: uint8   Brightness (0-100%)
```

**Status:** Planned

---

#### SET LED (0x0401)

**Purpose:** Set LED color

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   LED Index
Offset 1: uint8   Red (0-255)
Offset 2: uint8   Green (0-255)
Offset 3: uint8   Blue (0-255)
Offset 4: uint8   Brightness (0-100%)
```

**Response Payload:**
```
Offset 0: uint8   LED Index (echoed)
Offset 1: uint8   Status
```

**Status:** Planned

---

#### SET LED MODE (0x0402)

**Purpose:** Set LED animation/mode

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   LED Index
Offset 1: uint8   Mode (0=Static, 1=Blink, 2=Pulse, 3=Rainbow)
Offset 2: uint16  Mode Parameter (speed, etc.)
```

**Response Payload:**
```
Offset 0: uint8   LED Index (echoed)
Offset 1: uint8   Mode (echoed)
```

**Status:** Planned

---

### Reserved LED Commands

```
0x0403-0x04FF: Reserved for future LED commands
```

---

## Drive Commands

### Category: Drive (0x0500-0x05FF)

#### GET DRIVE STATUS (0x0500)

**Purpose:** Read drive presence and health

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   Drive Index (0-15)
```

**Response Payload:**
```
Offset 0: uint8   Drive Index (echoed)
Offset 1: uint8   Present (0=Absent, 1=Present)
Offset 2: uint8   Health (0=Healthy, 1=Warning, 2=Critical)
Offset 3: uint8   Reserved
```

**Status:** Planned

---

#### GET ALL DRIVES (0x0501)

**Purpose:** Read all drive status

**Direction:** Host → Controller

**Request Payload:** Empty

**Response Payload:**
```
Offset 0: uint16  Drive Bitmask (bit N = drive N present)
Offset 2: uint16  Health Bitmask (2-bit per drive)
```

**Status:** Planned

---

#### SCAN DRIVES (0x0502)

**Purpose:** Scan for drives

**Direction:** Host → Controller

**Request Payload:** Empty

**Response Payload:**
```
Offset 0: uint8   Scan Result (0=OK, 1=In Progress, 2=Error)
Offset 1: uint8   Drives Found
```

**Status:** Planned

---

### Reserved Drive Commands

```
0x0503-0x05FF: Reserved for future drive commands
```

---

## Configuration Commands

### Category: Configuration (0x0600-0x06FF)

#### GET CONFIG (0x0600)

**Purpose:** Read configuration parameter

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   Config ID
```

**Response Payload:**
```
Offset 0: uint8   Config ID (echoed)
Offset 1-N: Variable configuration value
```

**Status:** Planned

---

#### SET CONFIG (0x0601)

**Purpose:** Set configuration parameter

**Direction:** Host → Controller

**Request Payload:**
```
Offset 0: uint8   Config ID
Offset 1-N: Variable configuration value
```

**Response Payload:**
```
Offset 0: uint8   Config ID (echoed)
Offset 1: uint8   Status
```

**Status:** Planned

---

#### SAVE CONFIG (0x0602)

**Purpose:** Persist configuration to flash

**Direction:** Host → Controller

**Request Payload:** Empty

**Response Payload:**
```
Status: Ok or InternalError
```

**Status:** Planned

---

### Reserved Configuration Commands

```
0x0603-0x06FF: Reserved for future configuration commands
```

---

## Statistics Commands

### Category: Statistics (0x0700-0x07FF)

#### GET STATISTICS (0x0700)

**Purpose:** Read system statistics

**Direction:** Host → Controller

**Request Payload:** Empty

**Response Payload:**
```
Offset 0: uint32  Boot Count
Offset 4: uint32  Total Commands Processed
Offset 8: uint32  CRC Errors Detected
Offset 12: uint32 Uptime (seconds)
```

**Status:** Planned

---

#### RESET STATISTICS (0x0701)

**Purpose:** Reset statistics counters

**Direction:** Host → Controller

**Request Payload:** Empty

**Response Payload:**
```
Status: Ok
```

**Status:** Planned

---

### Reserved Statistics Commands

```
0x0702-0x07FF: Reserved for future statistics commands
```

---

## Event Commands

### Category: Events (0x0800-0x08FF)

#### READ EVENT (0x0800)

**Purpose:** Read next event from event log

**Direction:** Host → Controller

**Request Payload:** Empty

**Response Payload:**
```
Offset 0: uint16  Event ID
Offset 2: uint8   Event Type
Offset 3: uint8   Event Severity (0=Info, 1=Warning, 2=Error)
Offset 4: uint32  Timestamp (seconds since boot)
Offset 8-N: Event Data
```

**Status:** Planned

---

#### CLEAR EVENTS (0x0801)

**Purpose:** Clear event log

**Direction:** Host → Controller

**Request Payload:** Empty

**Response Payload:**
```
Offset 0: uint16  Events Cleared
```

**Status:** Planned

---

### Reserved Event Commands

```
0x0802-0x08FF: Reserved for future event commands
```

---

## Reserved Command Ranges

```
0x0900-0xEFFF: Reserved for future use
0xF000-0xFFFF: Manufacturer-specific commands (not part of standard protocol)
```

---

## Enumeration Values

### Relay State

| Value | Meaning |
|-------|---------|
| 0 | OFF |
| 1 | ON |

---

### Fan Mode

| Value | Meaning |
|-------|---------|
| 0 | Manual (PWM control) |
| 1 | Automatic (temperature-based) |

---

### Temperature Sensor Status

| Value | Meaning |
|-------|---------|
| 0 | OK (valid reading) |
| 1 | Error (sensor malfunction) |
| 2 | Disconnected (OneWire not responding) |

---

### System Health

| Value | Meaning |
|-------|---------|
| 0 | Healthy (all systems OK) |
| 1 | Warning (degraded operation) |
| 2 | Critical (shutdown imminent) |

---

### Drive Health

| Value (2 bits) | Meaning |
|---|---|
| 0 | Healthy |
| 1 | Warning |
| 2 | Critical |
| 3 | Offline |

---

### LED Mode

| Value | Meaning |
|-------|---------|
| 0 | Static color |
| 1 | Blinking |
| 2 | Pulsing |
| 3 | Rainbow animation |

---

### Config IDs

| ID | Parameter | Type |
|----|-----------|------|
| 1 | Relay 0 Invert | uint8 (boolean) |
| 2 | Relay 1 Invert | uint8 (boolean) |
| 3-8 | Reserved | — |
| 9 | Fan Mode | uint8 (enum) |
| 10 | Temperature Threshold | int8 (°C) |
| 11-31 | Reserved | — |

---

### Event Types

| Type | Description |
|------|-------------|
| 1 | Relay State Change |
| 2 | Temperature Threshold Exceeded |
| 3 | Drive Detected |
| 4 | Drive Disconnected |
| 5 | CRC Error Detected |
| 6 | System Reset |
| 7-255 | Reserved |

---

## Protocol Version Information

| Field | Value |
|-------|-------|
| Protocol Version | 1.0.0 |
| Command Count | ~35 (planned) |
| Result Codes | 8 |
| Reserved Ranges | 15 (for future expansion) |
| Maximum Payload | 500 bytes |

---

## Byte Order Verification

**All multi-byte fields use little-endian (LSB-first) order:**

Example: uint16 value 0x1234
- Big-endian (network):    `[0x12, 0x34]`
- Little-endian (ESP32):   `[0x34, 0x12]` ← Used in this protocol

---

## Extension Rules

When adding new commands:

1. **Reserve ranges:** Use next available command code in category
2. **Document fully:** Specify request/response payloads exactly
3. **Update registry:** Add to this document before implementation
4. **Create test vector:** Include in `shared/docs/protocol/test_vectors/`
5. **Increment version:** Protocol becomes 1.1.0, 2.0.0, etc.

---

## References

- **PROTOCOL_SPEC.md** — Complete protocol specification
- **shared/docs/protocol/test_vectors/** — Canonical test fixtures
- **docs/CommandSet.md** — Implementation status and details

---

## Document Control

| Field | Value |
|-------|-------|
| Status | Verification in Progress |
| Version | 1.0.0 |
| Created | July 2026 |
| Last Updated | July 2026 |
| Lock Status | Will be locked after freeze |

Once Protocol 1.0.0 is frozen, this registry becomes immutable for v1.x. New commands require version increment (2.0.0).
