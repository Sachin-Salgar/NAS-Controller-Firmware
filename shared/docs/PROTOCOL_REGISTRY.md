Document Version: 1.0  
Project Version: 1.0  
Last Updated: 2026-07-19  
Owner: NAS Controller  
Status: Frozen

# Protocol Command Registry

This is the single source of truth for all firmware commands. Each command appears once here with full specification. Use this registry to avoid duplication and maintain consistency across protocol documentation.

---

## System Commands

### CMD_SYSTEM_PING

| Property | Value |
|----------|-------|
| **Command Byte** | 0x01 |
| **ACK Byte** | 0x81 |
| **NAK Byte** | 0x41 |
| **Purpose** | Verify firmware is alive and responsive |
| **Request Payload** | (none) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | None (no NAK possible) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Periodic heartbeat, connection verification |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Earliest indicator of USB connection status |

---

### CMD_SYSTEM_RESET

| Property | Value |
|----------|-------|
| **Command Byte** | 0x02 |
| **ACK Byte** | 0x82 |
| **NAK Byte** | 0x42 |
| **Purpose** | Soft reset firmware (restart without power cycle) |
| **Request Payload** | (none) |
| **Response Payload** | (none) - ACK before reset |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Recovery from error state, apply configuration changes |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Firmware will not respond to subsequent commands for ~2-3 seconds during boot |

---

### CMD_GET_CAPABILITIES

| Property | Value |
|----------|-------|
| **Command Byte** | 0x03 |
| **ACK Byte** | 0x83 |
| **NAK Byte** | 0x43 |
| **Purpose** | Report firmware capabilities and hardware configuration |
| **Request Payload** | (none) |
| **Response Payload** | See protocol_spec.md for format |
| **Response Fields** | Protocol version, Config version, Relay count, Fan count, LED count, Temp sensor count, Drive count |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Startup capability discovery, UI adaptation |
| **Status** | ✅ Implemented in firmware |
| **Notes** | First command after PING during startup sequence |

---

## Relay Commands

### CMD_RELAY_SET

| Property | Value |
|----------|-------|
| **Command Byte** | 0x10 |
| **ACK Byte** | 0x90 |
| **NAK Byte** | 0x50 |
| **Purpose** | Set relay state (ON or OFF) |
| **Request Payload** | relay_id (1 byte), state (1 byte: 0x00=OFF, 0x01=ON) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid relay ID, 0x02: Invalid state value |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | User clicks relay button in UI |
| **Status** | ✅ Implemented in firmware |
| **Example** | `0x55 0xAA 0x01 0x10 0x02 0x00 0x01 [CRC16]` → Relay 0 ON |

---

### CMD_RELAY_GET

| Property | Value |
|----------|-------|
| **Command Byte** | 0x11 |
| **ACK Byte** | 0x91 |
| **NAK Byte** | 0x51 |
| **Purpose** | Get state of a single relay |
| **Request Payload** | relay_id (1 byte) |
| **Response Payload** | relay_id (1 byte), state (1 byte: 0x00=OFF, 0x01=ON) |
| **Error Codes** | 0x01: Invalid relay ID |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Read current relay state (polling or on-demand) |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Use CMD_GET_ALL_STATUS for bulk read of all relays |

---

### CMD_GET_ALL_STATUS

| Property | Value |
|----------|-------|
| **Command Byte** | 0x20 |
| **ACK Byte** | 0xA0 |
| **NAK Byte** | 0x60 |
| **Purpose** | Get state of all hardware in one command |
| **Request Payload** | (none) |
| **Response Payload** | See protocol_spec.md: relay states, fan speeds, temperatures, drive status |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Startup state resync, periodic polling, after reconnection |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Large response; consider rate-limiting polling to once per second |

---

## Fan Commands

### CMD_FAN_SET_SPEED

| Property | Value |
|----------|-------|
| **Command Byte** | 0x30 |
| **ACK Byte** | 0xB0 |
| **NAK Byte** | 0x70 |
| **Purpose** | Set fan speed (PWM) |
| **Request Payload** | fan_id (1 byte), speed (1 byte: 0-100) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid fan ID, 0x02: Out of range (speed > 100) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | User adjusts fan speed slider |
| **Status** | ✅ Implemented in firmware |
| **Notes** | 0 = OFF, 100 = Full speed, PWM updates immediately |

---

### CMD_FAN_GET

| Property | Value |
|----------|-------|
| **Command Byte** | 0x31 |
| **ACK Byte** | 0xB1 |
| **NAK Byte** | 0x71 |
| **Purpose** | Get current fan speed |
| **Request Payload** | fan_id (1 byte) |
| **Response Payload** | fan_id (1 byte), speed (1 byte: 0-100), status (1 byte) |
| **Status Byte** | 0x00: OK, 0x01: Stalled, 0x02: Over-temperature |
| **Error Codes** | 0x01: Invalid fan ID |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Read current fan speed |
| **Status** | ✅ Implemented in firmware |

---

### CMD_FAN_SET_MODE

| Property | Value |
|----------|-------|
| **Command Byte** | 0x32 |
| **ACK Byte** | 0xB2 |
| **NAK Byte** | 0x72 |
| **Purpose** | Set fan operating mode |
| **Request Payload** | fan_id (1 byte), mode (1 byte: 0x00=Manual, 0x01=Auto, 0x02=Off) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid fan ID, 0x02: Unsupported mode |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Switch between manual/auto/off modes |
| **Status** | ✅ Implemented in firmware |
| **Notes** | In Auto mode, firmware controls speed based on temperature thresholds |

---

## Temperature Commands

### CMD_TEMP_READ

| Property | Value |
|----------|-------|
| **Command Byte** | 0x40 |
| **ACK Byte** | 0xC0 |
| **NAK Byte** | 0x80 |
| **Purpose** | Read single temperature sensor |
| **Request Payload** | sensor_id (1 byte) |
| **Response Payload** | sensor_id (1 byte), temperature (2 bytes: int16, in 1/100°C, -4000 to 12700 = -40°C to 127°C) |
| **Error Codes** | 0x01: Invalid sensor ID, 0x02: Sensor error |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Read current temperature |
| **Status** | ✅ Implemented in firmware |
| **Example** | Read sensor 0: `0x55 0xAA 0x01 0x40 0x01 0x00 [CRC16]` → Response: 25.00°C |

---

## LED Commands

### CMD_LED_SET_COLOR

| Property | Value |
|----------|-------|
| **Command Byte** | 0x50 |
| **ACK Byte** | 0xD0 |
| **NAK Byte** | 0x90 |
| **Purpose** | Set LED color (RGB) |
| **Request Payload** | red (1 byte), green (1 byte), blue (1 byte) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | User selects color in UI color picker |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Values 0-255 per channel |

---

### CMD_LED_SET_BRIGHTNESS

| Property | Value |
|----------|-------|
| **Command Byte** | 0x51 |
| **ACK Byte** | 0xD1 |
| **NAK Byte** | 0x91 |
| **Purpose** | Set LED brightness |
| **Request Payload** | brightness (1 byte: 0-100) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Out of range (brightness > 100) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | User adjusts brightness slider |
| **Status** | ✅ Implemented in firmware |

---

### CMD_LED_SET_ANIMATION

| Property | Value |
|----------|-------|
| **Command Byte** | 0x52 |
| **ACK Byte** | 0xD2 |
| **NAK Byte** | 0x92 |
| **Purpose** | Select LED animation sequence |
| **Request Payload** | animation_id (1 byte: 0x00=Boot, 0x01=Idle, 0x02=Error, 0x03=Shutdown, 0x04=Custom) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid animation ID |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | User selects animation in UI dropdown |
| **Status** | ✅ Implemented in firmware |

---

## Configuration Commands

### CMD_CONFIG_GET

| Property | Value |
|----------|-------|
| **Command Byte** | 0x60 |
| **ACK Byte** | 0xE0 |
| **NAK Byte** | 0xA0 |
| **Purpose** | Read all configuration from firmware |
| **Request Payload** | (none) |
| **Response Payload** | Config version (1 byte), hardware counts, thresholds, curves (see protocol_spec.md) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Load configuration into editor |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Large response; single command for all config (atomic read) |

---

### CMD_CONFIG_SET

| Property | Value |
|----------|-------|
| **Command Byte** | 0x61 |
| **ACK Byte** | 0xE1 |
| **NAK Byte** | 0xA1 |
| **Purpose** | Write configuration to firmware (sent in transaction) |
| **Request Payload** | Config version, fields being updated (see protocol_spec.md) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid config version, 0x02: Validation error, 0x03: EEPROM write error |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Save configuration from editor (part of transaction) |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Must be preceded by CONFIG_BEGIN and followed by CONFIG_COMMIT |

---

### CMD_CONFIG_BEGIN

| Property | Value |
|----------|-------|
| **Command Byte** | 0x62 |
| **ACK Byte** | 0xE2 |
| **NAK Byte** | 0xA2 |
| **Purpose** | Begin configuration transaction |
| **Request Payload** | (none) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Start atomic config change |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Firmware locks configuration for exclusive access |

---

### CMD_CONFIG_COMMIT

| Property | Value |
|----------|-------|
| **Command Byte** | 0x63 |
| **ACK Byte** | 0xE3 |
| **NAK Byte** | 0xA3 |
| **Purpose** | Commit configuration transaction to EEPROM |
| **Request Payload** | (none) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: EEPROM write error, 0x02: Config invalid |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Finalize atomic config change |
| **Status** | ✅ Implemented in firmware |
| **Notes** | If fails, transaction rolls back; must retry BEGIN → SET → COMMIT |

---

## Event & Statistics Commands

### CMD_GET_EVENT_LOG

| Property | Value |
|----------|-------|
| **Command Byte** | 0x70 |
| **ACK Byte** | 0xF0 |
| **NAK Byte** | 0xB0 |
| **Purpose** | Read event log entries |
| **Request Payload** | start_index (2 bytes), count (1 byte) |
| **Response Payload** | Entries with timestamps and event data (see protocol_spec.md) |
| **Error Codes** | 0x01: Invalid index |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Download logs to UI for display/export |
| **Status** | ✅ Implemented in firmware |

---

## Command Summary Table

| Command | Byte | Purpose | Status |
|---------|------|---------|--------|
| PING | 0x01 | Connection verify | ✅ |
| RESET | 0x02 | Soft reset | ✅ |
| GET_CAPABILITIES | 0x03 | Firmware info | ✅ |
| RELAY_SET | 0x10 | Turn relay on/off | ✅ |
| RELAY_GET | 0x11 | Read relay state | ✅ |
| GET_ALL_STATUS | 0x20 | Bulk state read | ✅ |
| FAN_SET_SPEED | 0x30 | Set fan PWM | ✅ |
| FAN_GET | 0x31 | Read fan speed | ✅ |
| FAN_SET_MODE | 0x32 | Select fan mode | ✅ |
| TEMP_READ | 0x40 | Read temperature | ✅ |
| LED_SET_COLOR | 0x50 | Set LED color | ✅ |
| LED_SET_BRIGHTNESS | 0x51 | Set LED brightness | ✅ |
| LED_SET_ANIMATION | 0x52 | Select animation | ✅ |
| CONFIG_GET | 0x60 | Read config | ✅ |
| CONFIG_SET | 0x61 | Write config | ✅ |
| CONFIG_BEGIN | 0x62 | Start transaction | ✅ |
| CONFIG_COMMIT | 0x63 | Commit transaction | ✅ |
| GET_EVENT_LOG | 0x70 | Read events | ✅ |

---

**Purpose:** Single source of truth for all protocol commands  
**Status:** Frozen v1.0 (additions require protocol versioning)  
**Maintenance:** Update command entries as firmware evolves
