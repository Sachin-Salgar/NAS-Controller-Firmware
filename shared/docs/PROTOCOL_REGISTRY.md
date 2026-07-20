Document Version: 1.0  
Project Version: 1.0  
Last Updated: 2026-07-20  
Owner: NAS Controller  
Status: Reconciled

# Protocol Command Registry

This is the single source of truth for all firmware commands. Each command appears once here with full specification. This registry is reconciled with the firmware implementation to ensure exact alignment of command codes and behaviors.

---

## System Commands (0x0000-0x000F)

### CMD_PING

| Property | Value |
|----------|-------|
| **Command Code** | 0x0001 |
| **ACK Code** | 0x8001 |
| **NAK Code** | 0x4001 |
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

### CMD_GET_VERSION

| Property | Value |
|----------|-------|
| **Command Code** | 0x0002 |
| **ACK Code** | 0x8002 |
| **NAK Code** | 0x4002 |
| **Purpose** | Get firmware version |
| **Request Payload** | (none) |
| **Response Payload** | u32 version (big-endian) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Firmware identification and version checking |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Response is 4-byte version code |

---

### CMD_GET_BUILD_INFO

| Property | Value |
|----------|-------|
| **Command Code** | 0x0003 |
| **ACK Code** | 0x8003 |
| **NAK Code** | 0x4003 |
| **Purpose** | Report firmware build information |
| **Request Payload** | (none) |
| **Response Payload** | Build date, build number, configuration (see protocol_spec.md) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Diagnostic and debugging information |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Variable-length response |

---

### CMD_GET_SYSTEM_STATUS

| Property | Value |
|----------|-------|
| **Command Code** | 0x0004 |
| **ACK Code** | 0x8004 |
| **NAK Code** | 0x4004 |
| **Purpose** | Get current system status and all hardware state |
| **Request Payload** | (none) |
| **Response Payload** | Relay states, fan speeds, temperatures, drive status (see protocol_spec.md) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Startup state resync, periodic polling, reconnection recovery |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Large response; rate-limit polling to once per second |

---

### CMD_RESTART

| Property | Value |
|----------|-------|
| **Command Code** | 0x0005 |
| **ACK Code** | 0x8005 |
| **NAK Code** | 0x4005 |
| **Purpose** | Soft restart firmware (restart without power cycle) |
| **Request Payload** | (none) |
| **Response Payload** | (none) - ACK sent before restart |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Recovery from error state, apply configuration changes |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Firmware will not respond for ~2-3 seconds during boot. Daemon must reconnect. |

---

## Relay Commands (0x1000-0x1FFF)

### CMD_RELAY_GET

| Property | Value |
|----------|-------|
| **Command Code** | 0x1001 |
| **ACK Code** | 0x9001 |
| **NAK Code** | 0x5001 |
| **Purpose** | Get state of a single relay |
| **Request Payload** | relay_id (1 byte) |
| **Response Payload** | relay_id (1 byte), state (1 byte: 0x00=OFF, 0x01=ON) |
| **Error Codes** | 0x01: Invalid relay ID |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Read current relay state (polling or on-demand) |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Use GET_SYSTEM_STATUS for bulk read of all relays |

---

### CMD_RELAY_SET

| Property | Value |
|----------|-------|
| **Command Code** | 0x1002 |
| **ACK Code** | 0x9002 |
| **NAK Code** | 0x5002 |
| **Purpose** | Set relay state (ON or OFF) |
| **Request Payload** | relay_id (1 byte), state (1 byte: 0x00=OFF, 0x01=ON) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid relay ID, 0x02: Invalid state value |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | User controls relay from UI |
| **Status** | ✅ Implemented in firmware |
| **Example** | Set Relay 0 ON: `0x55 0xAA 0x00 0x01 0x10 0x02 0x00 0x02 0x00 0x01 [CRC16] 0xAA` |

---

### CMD_RELAY_TOGGLE

| Property | Value |
|----------|-------|
| **Command Code** | 0x1003 |
| **ACK Code** | 0x9003 |
| **NAK Code** | 0x5003 |
| **Purpose** | Toggle relay state (ON ↔ OFF) |
| **Request Payload** | relay_id (1 byte) |
| **Response Payload** | relay_id (1 byte), new_state (1 byte) |
| **Error Codes** | 0x01: Invalid relay ID |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Simple toggle without state query |
| **Status** | ✅ Implemented in firmware |

---

## Fan Commands (0x1100-0x1FFF)

### CMD_FAN_GET

| Property | Value |
|----------|-------|
| **Command Code** | 0x1101 |
| **ACK Code** | 0x9101 |
| **NAK Code** | 0x5101 |
| **Purpose** | Get current fan speed and status |
| **Request Payload** | fan_id (1 byte) |
| **Response Payload** | fan_id (1 byte), speed (1 byte: 0-100), status (1 byte) |
| **Status Byte** | 0x00: OK, 0x01: Stalled, 0x02: Over-temperature |
| **Error Codes** | 0x01: Invalid fan ID |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Monitor fan speed and health |
| **Status** | ✅ Implemented in firmware |

---

### CMD_FAN_SET_SPEED

| Property | Value |
|----------|-------|
| **Command Code** | 0x1102 |
| **ACK Code** | 0x9102 |
| **NAK Code** | 0x5102 |
| **Purpose** | Set fan speed (PWM) |
| **Request Payload** | fan_id (1 byte), speed (1 byte: 0-100) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid fan ID, 0x02: Out of range (speed > 100) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | User adjusts fan speed slider in UI |
| **Status** | ✅ Implemented in firmware |
| **Notes** | 0 = OFF, 100 = Full speed, PWM updates immediately |

---

### CMD_FAN_SET_MODE

| Property | Value |
|----------|-------|
| **Command Code** | 0x1103 |
| **ACK Code** | 0x9103 |
| **NAK Code** | 0x5103 |
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

## Temperature Commands (0x1200-0x1FFF)

### CMD_TEMPERATURE_GET

| Property | Value |
|----------|-------|
| **Command Code** | 0x1201 |
| **ACK Code** | 0x9201 |
| **NAK Code** | 0x5201 |
| **Purpose** | Read single temperature sensor |
| **Request Payload** | sensor_id (1 byte) |
| **Response Payload** | sensor_id (1 byte), temperature (2 bytes: i16 in 1/100°C, range -40°C to 127°C) |
| **Error Codes** | 0x01: Invalid sensor ID, 0x02: Sensor error |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Read current temperature |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Temperature in 1/100°C: e.g., 0x0BB8 = 3000 = 30.00°C |

---

### CMD_TEMPERATURE_GET_ALL

| Property | Value |
|----------|-------|
| **Command Code** | 0x1202 |
| **ACK Code** | 0x9202 |
| **NAK Code** | 0x5202 |
| **Purpose** | Get all temperature readings in one command |
| **Request Payload** | (none) |
| **Response Payload** | All sensor readings (see protocol_spec.md) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Bulk temperature read during startup or polling |
| **Status** | ✅ Implemented in firmware |
| **Notes** | More efficient than individual sensor queries |

---

## LED Commands (0x1300-0x1FFF)

### CMD_LED_GET

| Property | Value |
|----------|-------|
| **Command Code** | 0x1301 |
| **ACK Code** | 0x9301 |
| **NAK Code** | 0x5301 |
| **Purpose** | Get current LED state |
| **Request Payload** | (none) |
| **Response Payload** | color (RGB: 3 bytes), brightness (1 byte: 0-100), mode (1 byte) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Read LED configuration for UI display |
| **Status** | ✅ Implemented in firmware |

---

### CMD_LED_SET_COLOR

| Property | Value |
|----------|-------|
| **Command Code** | 0x1302 |
| **ACK Code** | 0x9302 |
| **NAK Code** | 0x5302 |
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

### CMD_LED_SET_MODE

| Property | Value |
|----------|-------|
| **Command Code** | 0x1303 |
| **ACK Code** | 0x9303 |
| **NAK Code** | 0x5303 |
| **Purpose** | Set LED animation mode |
| **Request Payload** | mode (1 byte: 0x00=Boot, 0x01=Idle, 0x02=Error, 0x03=Shutdown, 0x04=Custom) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid mode |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | User selects animation in UI dropdown |
| **Status** | ✅ Implemented in firmware |

---

### CMD_LED_OFF

| Property | Value |
|----------|-------|
| **Command Code** | 0x1304 |
| **ACK Code** | 0x9304 |
| **NAK Code** | 0x5304 |
| **Purpose** | Turn off LED immediately |
| **Request Payload** | (none) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Quick LED off without animation |
| **Status** | ✅ Implemented in firmware |

---

## Drive Commands (0x1400-0x1FFF)

### CMD_DRIVE_GET

| Property | Value |
|----------|-------|
| **Command Code** | 0x1401 |
| **ACK Code** | 0x9401 |
| **NAK Code** | 0x5401 |
| **Purpose** | Get single drive status |
| **Request Payload** | drive_id (1 byte) |
| **Response Payload** | drive_id (1 byte), power_state (1 byte), health (1 byte), capacity (4 bytes) |
| **Error Codes** | 0x01: Invalid drive ID |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Poll single drive status |
| **Status** | ✅ Implemented in firmware |

---

### CMD_DRIVE_GET_ALL

| Property | Value |
|----------|-------|
| **Command Code** | 0x1402 |
| **ACK Code** | 0x9402 |
| **NAK Code** | 0x5402 |
| **Purpose** | Get all drive status |
| **Request Payload** | (none) |
| **Response Payload** | Array of drive entries (see protocol_spec.md) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Bulk read of all drives during startup |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Efficient single-command read of all drives |

---

### CMD_DRIVE_POWER_ON

| Property | Value |
|----------|-------|
| **Command Code** | 0x1403 |
| **ACK Code** | 0x9403 |
| **NAK Code** | 0x5403 |
| **Purpose** | Power on a drive |
| **Request Payload** | drive_id (1 byte) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid drive ID |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Power on spinup from UI |
| **Status** | ✅ Implemented in firmware |

---

### CMD_DRIVE_POWER_OFF

| Property | Value |
|----------|-------|
| **Command Code** | 0x1404 |
| **ACK Code** | 0x9404 |
| **NAK Code** | 0x5404 |
| **Purpose** | Power off a drive (safe shutdown) |
| **Request Payload** | drive_id (1 byte) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid drive ID |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Graceful drive shutdown from UI |
| **Status** | ✅ Implemented in firmware |

---

## Configuration Commands (0x1500-0x1FFF)

### CMD_CONFIGURATION_LOAD

| Property | Value |
|----------|-------|
| **Command Code** | 0x1501 |
| **ACK Code** | 0x9501 |
| **NAK Code** | 0x5501 |
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

### CMD_CONFIGURATION_SAVE

| Property | Value |
|----------|-------|
| **Command Code** | 0x1502 |
| **ACK Code** | 0x9502 |
| **NAK Code** | 0x5502 |
| **Purpose** | Write configuration to firmware |
| **Request Payload** | Config version, fields being updated (see protocol_spec.md) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid config version, 0x02: Validation error, 0x03: EEPROM write error |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Save configuration from editor |
| **Status** | ✅ Implemented in firmware |

---

### CMD_CONFIGURATION_RESET

| Property | Value |
|----------|-------|
| **Command Code** | 0x1503 |
| **ACK Code** | 0x9503 |
| **NAK Code** | 0x5503 |
| **Purpose** | Reset configuration to factory defaults |
| **Request Payload** | (none) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Factory reset from UI |
| **Status** | ✅ Implemented in firmware |

---

## Statistics Commands (0x1600-0x1FFF)

### CMD_STATISTICS_GET

| Property | Value |
|----------|-------|
| **Command Code** | 0x1601 |
| **ACK Code** | 0x9601 |
| **NAK Code** | 0x5601 |
| **Purpose** | Get runtime statistics |
| **Request Payload** | (none) |
| **Response Payload** | Uptime, command count, error count, etc. (see protocol_spec.md) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Diagnostics and performance monitoring |
| **Status** | ✅ Implemented in firmware |

---

### CMD_STATISTICS_RESET

| Property | Value |
|----------|-------|
| **Command Code** | 0x1602 |
| **ACK Code** | 0x9602 |
| **NAK Code** | 0x5602 |
| **Purpose** | Reset statistics counters |
| **Request Payload** | (none) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Clear statistics for baseline measurement |
| **Status** | ✅ Implemented in firmware |

---

## Event Commands (0x1700-0x1FFF)

### CMD_EVENT_READ

| Property | Value |
|----------|-------|
| **Command Code** | 0x1701 |
| **ACK Code** | 0x9701 |
| **NAK Code** | 0x5701 |
| **Purpose** | Read event log entries |
| **Request Payload** | start_index (2 bytes), count (1 byte) |
| **Response Payload** | Array of event entries with timestamps and event data (see protocol_spec.md) |
| **Error Codes** | 0x01: Invalid index |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Download logs to UI for display/export |
| **Status** | ✅ Implemented in firmware |

---

### CMD_EVENT_CLEAR

| Property | Value |
|----------|-------|
| **Command Code** | 0x1702 |
| **ACK Code** | 0x9702 |
| **NAK Code** | 0x5702 |
| **Purpose** | Clear event log |
| **Request Payload** | (none) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Clear old events from firmware storage |
| **Status** | ✅ Implemented in firmware |

---

## Command Summary Table

| Command | Code | Purpose | Status |
|---------|------|---------|--------|
| PING | 0x0001 | Connection verify | ✅ |
| GET_VERSION | 0x0002 | Get firmware version | ✅ |
| GET_BUILD_INFO | 0x0003 | Build information | ✅ |
| GET_SYSTEM_STATUS | 0x0004 | Bulk state read | ✅ |
| RESTART | 0x0005 | Soft restart | ✅ |
| RELAY_GET | 0x1001 | Read relay state | ✅ |
| RELAY_SET | 0x1002 | Control relay | ✅ |
| RELAY_TOGGLE | 0x1003 | Toggle relay | ✅ |
| FAN_GET | 0x1101 | Read fan speed | ✅ |
| FAN_SET_SPEED | 0x1102 | Set fan PWM | ✅ |
| FAN_SET_MODE | 0x1103 | Select fan mode | ✅ |
| TEMPERATURE_GET | 0x1201 | Read temperature | ✅ |
| TEMPERATURE_GET_ALL | 0x1202 | Read all temperatures | ✅ |
| LED_GET | 0x1301 | Read LED state | ✅ |
| LED_SET_COLOR | 0x1302 | Set LED color | ✅ |
| LED_SET_MODE | 0x1303 | Set LED animation | ✅ |
| LED_OFF | 0x1304 | Turn off LED | ✅ |
| DRIVE_GET | 0x1401 | Read drive status | ✅ |
| DRIVE_GET_ALL | 0x1402 | Read all drives | ✅ |
| DRIVE_POWER_ON | 0x1403 | Power on drive | ✅ |
| DRIVE_POWER_OFF | 0x1404 | Power off drive | ✅ |
| CONFIGURATION_LOAD | 0x1501 | Read config | ✅ |
| CONFIGURATION_SAVE | 0x1502 | Write config | ✅ |
| CONFIGURATION_RESET | 0x1503 | Reset config | ✅ |
| STATISTICS_GET | 0x1601 | Read statistics | ✅ |
| STATISTICS_RESET | 0x1602 | Clear statistics | ✅ |
| EVENT_READ | 0x1701 | Read event log | ✅ |
| EVENT_CLEAR | 0x1702 | Clear event log | ✅ |

---

**Purpose:** Single source of truth for all protocol commands  
**Status:** Reconciled with firmware v1.0 (2026-07-20)  
**Maintenance:** Update command entries as firmware evolves  
**Reconciliation:** All command codes verified against firmware/src/Protocol/Commands.h
