Document Version: 1.0  
Project Version: 1.0  
Last Updated: 2026-07-20  
Owner: NAS Controller  
Status: Frozen

# Protocol Command Registry

This is the single source of truth for all firmware commands. Each command appears once here with full specification. Use this registry to avoid duplication and maintain consistency across protocol documentation.

---

## System Commands

### PING

| Property | Value |
|----------|-------|
| **Command ID** | 0x0001 |
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

### GET_VERSION

| Property | Value |
|----------|-------|
| **Command ID** | 0x0002 |
| **Purpose** | Get firmware version |
| **Request Payload** | (none) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### GET_BUILD_INFO

| Property | Value |
|----------|-------|
| **Command ID** | 0x0003 |
| **Purpose** | Get build information |
| **Request Payload** | (none) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### GET_SYSTEM_STATUS

| Property | Value |
|----------|-------|
| **Command ID** | 0x0004 |
| **Purpose** | Get system status |
| **Request Payload** | (none) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### RESET

| Property | Value |
|----------|-------|
| **Command ID** | 0x0005 |
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

## Relay Commands

### RELAY_GET

| Property | Value |
|----------|-------|
| **Command ID** | 0x1001 |
| **Purpose** | Get state of a single relay |
| **Request Payload** | relay_id (1 byte) |
| **Response Payload** | relay_id (1 byte), state (1 byte: 0x00=OFF, 0x01=ON) |
| **Error Codes** | 0x01: Invalid relay ID |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Read current relay state (polling or on-demand) |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Payload structure not verifiable from Commands.h |

---

### RELAY_SET

| Property | Value |
|----------|-------|
| **Command ID** | 0x1002 |
| **Purpose** | Set relay state (ON or OFF) |
| **Request Payload** | relay_id (1 byte), state (1 byte: 0x00=OFF, 0x01=ON) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid relay ID, 0x02: Invalid state value |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | User clicks relay button in UI |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Payload structure not verifiable from Commands.h |

---

### RELAY_TOGGLE

| Property | Value |
|----------|-------|
| **Command ID** | 0x1003 |
| **Purpose** | Toggle relay state |
| **Request Payload** | relay_id (1 byte) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | Implementation-defined (not verifiable from Commands.h) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Command exists in firmware; detailed behavior not verifiable from Commands.h |

---

## Fan Commands

### FAN_GET

| Property | Value |
|----------|-------|
| **Command ID** | 0x1101 |
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
| **Notes** | Payload structure not verifiable from Commands.h |

---

### FAN_SET_SPEED

| Property | Value |
|----------|-------|
| **Command ID** | 0x1102 |
| **Purpose** | Set fan speed (PWM) |
| **Request Payload** | fan_id (1 byte), speed (1 byte: 0-100) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid fan ID, 0x02: Out of range (speed > 100) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | User adjusts fan speed slider |
| **Status** | ✅ Implemented in firmware |
| **Notes** | 0 = OFF, 100 = Full speed, PWM updates immediately. Payload structure not verifiable from Commands.h |

---

### FAN_SET_MODE

| Property | Value |
|----------|-------|
| **Command ID** | 0x1103 |
| **Purpose** | Set fan operating mode |
| **Request Payload** | fan_id (1 byte), mode (1 byte: 0x00=Manual, 0x01=Auto, 0x02=Off) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid fan ID, 0x02: Unsupported mode |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Switch between manual/auto/off modes |
| **Status** | ✅ Implemented in firmware |
| **Notes** | In Auto mode, firmware controls speed based on temperature thresholds. Payload structure not verifiable from Commands.h |

---

## Temperature Commands

### TEMPERATURE_GET

| Property | Value |
|----------|-------|
| **Command ID** | 0x1201 |
| **Purpose** | Read single temperature sensor |
| **Request Payload** | sensor_id (1 byte) |
| **Response Payload** | sensor_id (1 byte), temperature (2 bytes: int16, in 1/100°C, -4000 to 12700 = -40°C to 127°C) |
| **Error Codes** | 0x01: Invalid sensor ID, 0x02: Sensor error |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Read current temperature |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Payload structure not verifiable from Commands.h |

---

### TEMPERATURE_GET_ALL

| Property | Value |
|----------|-------|
| **Command ID** | 0x1202 |
| **Purpose** | Read all temperature sensors |
| **Request Payload** | (none) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## LED Commands

### LED_GET

| Property | Value |
|----------|-------|
| **Command ID** | 0x1301 |
| **Purpose** | Get LED state |
| **Request Payload** | (none) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### LED_SET_COLOR

| Property | Value |
|----------|-------|
| **Command ID** | 0x1302 |
| **Purpose** | Set LED color (RGB) |
| **Request Payload** | red (1 byte), green (1 byte), blue (1 byte) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | User selects color in UI color picker |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Values 0-255 per channel. Payload structure not verifiable from Commands.h |

---

### LED_SET_MODE

| Property | Value |
|----------|-------|
| **Command ID** | 0x1303 |
| **Purpose** | Select LED animation sequence |
| **Request Payload** | animation_id (1 byte: 0x00=Boot, 0x01=Idle, 0x02=Error, 0x03=Shutdown, 0x04=Custom) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | 0x01: Invalid animation ID |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | User selects animation in UI dropdown |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Payload structure not verifiable from Commands.h |

---

### LED_OFF

| Property | Value |
|----------|-------|
| **Command ID** | 0x1304 |
| **Purpose** | Turn LED off |
| **Request Payload** | (none) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## Drive Commands

### DRIVE_GET

| Property | Value |
|----------|-------|
| **Command ID** | 0x1401 |
| **Purpose** | Get drive information |
| **Request Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | Implementation-defined (not verifiable from Commands.h) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### DRIVE_GET_ALL

| Property | Value |
|----------|-------|
| **Command ID** | 0x1402 |
| **Purpose** | Get state of all drives |
| **Request Payload** | (none) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Bulk state read, periodic polling, after reconnection |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Large response; consider rate-limiting polling to once per second |

---

### DRIVE_POWER_ON

| Property | Value |
|----------|-------|
| **Command ID** | 0x1403 |
| **Purpose** | Power on drive |
| **Request Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | Implementation-defined (not verifiable from Commands.h) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### DRIVE_POWER_OFF

| Property | Value |
|----------|-------|
| **Command ID** | 0x1404 |
| **Purpose** | Power off drive |
| **Request Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | Implementation-defined (not verifiable from Commands.h) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## Configuration Commands

### CONFIG_LOAD

| Property | Value |
|----------|-------|
| **Command ID** | 0x1501 |
| **Purpose** | Read all configuration from firmware |
| **Request Payload** | (none) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Load configuration into editor |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Large response; single command for all config (atomic read) |

---

### CONFIG_SAVE

| Property | Value |
|----------|-------|
| **Command ID** | 0x1502 |
| **Purpose** | Write configuration to firmware |
| **Request Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Response Payload** | (none) - ACK only |
| **Error Codes** | Implementation-defined (not verifiable from Commands.h) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Save configuration from editor |
| **Status** | ✅ Implemented in firmware |
| **Notes** | Original registry documented transaction-based behavior (BEGIN/SET/COMMIT); firmware command structure differs |

---

### CONFIG_RESET

| Property | Value |
|----------|-------|
| **Command ID** | 0x1503 |
| **Purpose** | Reset configuration to defaults |
| **Request Payload** | (none) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | Implementation-defined (not verifiable from Commands.h) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## Statistics Commands

### STATISTICS_GET

| Property | Value |
|----------|-------|
| **Command ID** | 0x1601 |
| **Purpose** | Get statistics |
| **Request Payload** | (none) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | None |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### STATISTICS_RESET

| Property | Value |
|----------|-------|
| **Command ID** | 0x1602 |
| **Purpose** | Reset statistics |
| **Request Payload** | (none) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | Implementation-defined (not verifiable from Commands.h) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## Event Commands

### EVENT_READ

| Property | Value |
|----------|-------|
| **Command ID** | 0x1701 |
| **Purpose** | Read event log entries |
| **Request Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | Implementation-defined (not verifiable from Commands.h) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Usage** | Download logs to UI for display/export |
| **Status** | ✅ Implemented in firmware |

---

### EVENT_CLEAR

| Property | Value |
|----------|-------|
| **Command ID** | 0x1702 |
| **Purpose** | Clear event log |
| **Request Payload** | (none) |
| **Response Payload** | Implementation-defined (not verifiable from Commands.h) |
| **Error Codes** | Implementation-defined (not verifiable from Commands.h) |
| **Timeout** | 1000ms |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## Commands Present in Previous Registry but Not Found in Commands.h

The following commands appeared in the original registry but are not listed in `firmware/src/Protocol/Commands.h`:

- CMD_GET_ALL_STATUS (previously 0x20)
- CMD_TEMP_READ (previously 0x40)
- CMD_LED_SET_BRIGHTNESS (previously 0x51)
- CMD_CONFIG_BEGIN (previously 0x62)
- CMD_CONFIG_COMMIT (previously 0x63)
- CMD_GET_EVENT_LOG (previously 0x70)

**Note:** Firmware Commands.h does not provide sufficient evidence to determine whether these commands are deprecated, internal, obsolete, or implemented elsewhere. Only the commands explicitly listed in Commands.h are confirmed as implemented.

---

## Command Summary Table

| Command | ID | Purpose | Status |
|---------|-----|---------|--------|
| PING | 0x0001 | Connection verify | ✅ |
| GET_VERSION | 0x0002 | Firmware version | ✅ |
| GET_BUILD_INFO | 0x0003 | Build info | ✅ |
| GET_SYSTEM_STATUS | 0x0004 | System status | ✅ |
| RESET | 0x0005 | Soft restart | ✅ |
| RELAY_GET | 0x1001 | Read relay state | ✅ |
| RELAY_SET | 0x1002 | Set relay state | ✅ |
| RELAY_TOGGLE | 0x1003 | Toggle relay | ✅ |
| FAN_GET | 0x1101 | Read fan speed | ✅ |
| FAN_SET_SPEED | 0x1102 | Set fan speed | ✅ |
| FAN_SET_MODE | 0x1103 | Set fan mode | ✅ |
| TEMPERATURE_GET | 0x1201 | Read temperature | ✅ |
| TEMPERATURE_GET_ALL | 0x1202 | Read all temperatures | ✅ |
| LED_GET | 0x1301 | Read LED state | ✅ |
| LED_SET_COLOR | 0x1302 | Set LED color | ✅ |
| LED_SET_MODE | 0x1303 | Set LED mode | ✅ |
| LED_OFF | 0x1304 | Turn LED off | ✅ |
| DRIVE_GET | 0x1401 | Read drive info | ✅ |
| DRIVE_GET_ALL | 0x1402 | Read all drives | ✅ |
| DRIVE_POWER_ON | 0x1403 | Power on drive | ✅ |
| DRIVE_POWER_OFF | 0x1404 | Power off drive | ✅ |
| CONFIG_LOAD | 0x1501 | Load config | ✅ |
| CONFIG_SAVE | 0x1502 | Save config | ✅ |
| CONFIG_RESET | 0x1503 | Reset config | ✅ |
| STATISTICS_GET | 0x1601 | Read statistics | ✅ |
| STATISTICS_RESET | 0x1602 | Reset statistics | ✅ |
| EVENT_READ | 0x1701 | Read events | ✅ |
| EVENT_CLEAR | 0x1702 | Clear events | ✅ |

---

**Purpose:** Single source of truth for all protocol commands  
**Status:** Frozen v1.0 (additions require protocol versioning)  
**Maintenance:** Update command entries as firmware evolves  
**Source:** `firmware/src/Protocol/Commands.h`
