Document Version: 1.0  
Project Version: 1.0  
Last Updated: 2026-07-20  
Owner: NAS Controller  
Status: Frozen

# Protocol Command Registry

This is the single source of truth for all firmware commands. Each command appears once here with full specification. Use this registry to avoid duplication and maintain consistency across protocol documentation.

**Note:** Command IDs are 2-byte values (0x0000-0xFFFF). All IDs listed below are the authoritative firmware implementation as defined in `firmware/src/Protocol/Commands.h`.

---

## System Commands

### Ping

| Property | Value |
|----------|-------|
| **Command ID** | 0x0001 |
| **Purpose** | Verify firmware is alive and responsive |
| **Request Payload** | (none) |
| **Response Payload** | (none) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### GetVersion

| Property | Value |
|----------|-------|
| **Command ID** | 0x0002 |
| **Purpose** | Get firmware version |
| **Request Payload** | (none) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### GetBuildInfo

| Property | Value |
|----------|-------|
| **Command ID** | 0x0003 |
| **Purpose** | Get build information |
| **Request Payload** | (none) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### GetSystemStatus

| Property | Value |
|----------|-------|
| **Command ID** | 0x0004 |
| **Purpose** | Get system status |
| **Request Payload** | (none) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### Restart

| Property | Value |
|----------|-------|
| **Command ID** | 0x0005 |
| **Purpose** | Soft restart firmware |
| **Request Payload** | (none) |
| **Response Payload** | (none) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## Relay Commands

### RelayGet

| Property | Value |
|----------|-------|
| **Command ID** | 0x1001 |
| **Purpose** | Get relay state |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### RelaySet

| Property | Value |
|----------|-------|
| **Command ID** | 0x1002 |
| **Purpose** | Set relay state |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### RelayToggle

| Property | Value |
|----------|-------|
| **Command ID** | 0x1003 |
| **Purpose** | Toggle relay state |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## Fan Commands

### FanGet

| Property | Value |
|----------|-------|
| **Command ID** | 0x1101 |
| **Purpose** | Get fan state |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### FanSetSpeed

| Property | Value |
|----------|-------|
| **Command ID** | 0x1102 |
| **Purpose** | Set fan speed |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### FanSetMode

| Property | Value |
|----------|-------|
| **Command ID** | 0x1103 |
| **Purpose** | Set fan operating mode |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## Temperature Commands

### TemperatureGet

| Property | Value |
|----------|-------|
| **Command ID** | 0x1201 |
| **Purpose** | Get single temperature reading |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### TemperatureGetAll

| Property | Value |
|----------|-------|
| **Command ID** | 0x1202 |
| **Purpose** | Get all temperature readings |
| **Request Payload** | (none) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## LED Commands

### LedGet

| Property | Value |
|----------|-------|
| **Command ID** | 0x1301 |
| **Purpose** | Get LED state |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### LedSetColor

| Property | Value |
|----------|-------|
| **Command ID** | 0x1302 |
| **Purpose** | Set LED color |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### LedSetMode

| Property | Value |
|----------|-------|
| **Command ID** | 0x1303 |
| **Purpose** | Set LED mode |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### LedOff

| Property | Value |
|----------|-------|
| **Command ID** | 0x1304 |
| **Purpose** | Turn LED off |
| **Request Payload** | (none) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## Drive Commands

### DriveGet

| Property | Value |
|----------|-------|
| **Command ID** | 0x1401 |
| **Purpose** | Get drive information |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### DriveGetAll

| Property | Value |
|----------|-------|
| **Command ID** | 0x1402 |
| **Purpose** | Get all drive information |
| **Request Payload** | (none) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### DrivePowerOn

| Property | Value |
|----------|-------|
| **Command ID** | 0x1403 |
| **Purpose** | Power on drive |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### DrivePowerOff

| Property | Value |
|----------|-------|
| **Command ID** | 0x1404 |
| **Purpose** | Power off drive |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## Configuration Commands

### ConfigurationLoad

| Property | Value |
|----------|-------|
| **Command ID** | 0x1501 |
| **Purpose** | Load configuration |
| **Request Payload** | (none) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### ConfigurationSave

| Property | Value |
|----------|-------|
| **Command ID** | 0x1502 |
| **Purpose** | Save configuration |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### ConfigurationReset

| Property | Value |
|----------|-------|
| **Command ID** | 0x1503 |
| **Purpose** | Reset configuration to defaults |
| **Request Payload** | (none) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## Statistics Commands

### StatisticsGet

| Property | Value |
|----------|-------|
| **Command ID** | 0x1601 |
| **Purpose** | Get statistics |
| **Request Payload** | (none) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### StatisticsReset

| Property | Value |
|----------|-------|
| **Command ID** | 0x1602 |
| **Purpose** | Reset statistics |
| **Request Payload** | (none) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## Event Commands

### EventRead

| Property | Value |
|----------|-------|
| **Command ID** | 0x1701 |
| **Purpose** | Read events |
| **Request Payload** | (implementation-defined) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

### EventClear

| Property | Value |
|----------|-------|
| **Command ID** | 0x1702 |
| **Purpose** | Clear events |
| **Request Payload** | (none) |
| **Response Payload** | (implementation-defined) |
| **Firmware Support** | v1.0+ |
| **Protocol Version** | 1.0 |
| **Status** | ✅ Implemented in firmware |

---

## Command Summary Table

| Command | ID | Purpose | Status |
|---------|-----|---------|--------|
| Ping | 0x0001 | Connection verify | ✅ |
| GetVersion | 0x0002 | Firmware version | ✅ |
| GetBuildInfo | 0x0003 | Build info | ✅ |
| GetSystemStatus | 0x0004 | System status | ✅ |
| Restart | 0x0005 | Soft restart | ✅ |
| RelayGet | 0x1001 | Read relay state | ✅ |
| RelaySet | 0x1002 | Set relay state | ✅ |
| RelayToggle | 0x1003 | Toggle relay | ✅ |
| FanGet | 0x1101 | Read fan state | ✅ |
| FanSetSpeed | 0x1102 | Set fan speed | ✅ |
| FanSetMode | 0x1103 | Set fan mode | ✅ |
| TemperatureGet | 0x1201 | Read temperature | ✅ |
| TemperatureGetAll | 0x1202 | Read all temperatures | ✅ |
| LedGet | 0x1301 | Read LED state | ✅ |
| LedSetColor | 0x1302 | Set LED color | ✅ |
| LedSetMode | 0x1303 | Set LED mode | ✅ |
| LedOff | 0x1304 | Turn LED off | ✅ |
| DriveGet | 0x1401 | Read drive info | ✅ |
| DriveGetAll | 0x1402 | Read all drives | ✅ |
| DrivePowerOn | 0x1403 | Power on drive | ✅ |
| DrivePowerOff | 0x1404 | Power off drive | ✅ |
| ConfigurationLoad | 0x1501 | Load config | ✅ |
| ConfigurationSave | 0x1502 | Save config | ✅ |
| ConfigurationReset | 0x1503 | Reset config | ✅ |
| StatisticsGet | 0x1601 | Read statistics | ✅ |
| StatisticsReset | 0x1602 | Reset statistics | ✅ |
| EventRead | 0x1701 | Read events | ✅ |
| EventClear | 0x1702 | Clear events | ✅ |

---

**Purpose:** Single source of truth for all protocol commands  
**Status:** Frozen v1.0 (additions require protocol versioning)  
**Maintenance:** Update command entries as firmware evolves  
**Source:** `firmware/src/Protocol/Commands.h`
