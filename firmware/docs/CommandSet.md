# NAS Controller Firmware
# Command Set Specification

Document ID: NCF-CMD-001

Document Version: 1.0

Status: Released

Applies To: Firmware Version 1.x

---

# Revision History

| Version | Description |
|----------|-------------|
| 1.0 | Initial Release |

---

# Table of Contents

1. Purpose
2. Command Categories
3. Command Status
4. System Commands
5. Relay Commands
6. Fan Commands
7. Temperature Commands
8. LED Commands
9. Drive Commands
10. Configuration Commands
11. Diagnostic Commands
12. Event Messages
13. Error Responses
14. Future Commands
15. Command Lifecycle
16. Command Freeze

---

# 1. Purpose

This document defines every command supported by the NAS Controller Firmware.

This document describes:

- Command purpose
- Parameters
- Responses
- Implementation status

Commands shall be implemented incrementally as their owning firmware modules are completed. Command implementation status shall be updated throughout development.

Command syntax is defined in **Protocol.md**.

---

# 2. Command Categories

Commands are grouped into the following categories.

- System
- Relay
- Fan
- Temperature
- LED
- Drive
- Configuration
- Diagnostics

---

# 3. Command Status

Each command has one of the following states.

| Status | Description |
|---------|-------------|
| Planned | Defined but not yet implemented |
| Implemented | Fully supported |
| Reserved | Reserved for future firmware |
| Deprecated | Supported for compatibility only |

## Command Ownership

Each command has exactly one owning module responsible for implementation and maintenance. Cross-module implementations are prohibited.

---

# 4. System Commands

## HELLO

Purpose

Establish communication.

Direction

Host → Controller

Response

Firmware information.

Status

Planned

Implementation Module

Protocol/USBService

---

## PING

Purpose

Verify controller responsiveness.

Response

PONG

Status

Planned

Implementation Module

Protocol/USBService

---

## RESET

Purpose

Software restart.

Status

Planned

Implementation Module

System/SystemManager

---

## GET VERSION

Purpose

Read firmware version.

Status

Planned

Implementation Module

Core/Version

---

## GET STATUS

Purpose

Retrieve complete controller status.

Includes

- Relay State
- Fan Speed
- Temperatures
- Drive Status
- LED Status

Status

Planned

Implementation Module

Services/SystemService

---

## GET UPTIME

Purpose

Read controller uptime.

Status

Planned

Implementation Module

System/SystemManager

---

## GET BUILD

Purpose

Read firmware build information.

Status

Planned

Implementation Module

Core/BuildInfo

---

# 5. Relay Commands

## GET RELAY

Purpose

Read relay state.

Parameters

ID

Response

ON / OFF

Status

Planned

Implementation Module

Services/RelayService

---

## SET RELAY

Purpose

Switch relay output.

Parameters

ID

STATE

Status

Planned

Implementation Module

Services/RelayService

---

# 6. Fan Commands

## GET FAN

Purpose

Read PWM value.

Status

Planned

Implementation Module

Services/FanService

---

## SET FAN

Purpose

Set fan speed.

Parameters

ID

VALUE

Range

0–100%

Status

Planned

Implementation Module

Services/FanService

---

## GET FAN MODE

Purpose

Read control mode.

Modes

Manual

Automatic

Status

Planned

Implementation Module

Services/FanService

---

## SET FAN MODE

Purpose

Select automatic or manual control.

Status

Planned

Implementation Module

Services/FanService

---

# 7. Temperature Commands

## GET TEMP

Purpose

Read sensor temperature.

Parameters

ID

Response

Temperature

Unit

°C

Status

Planned

Implementation Module

Services/TemperatureService

---

## GET TEMP ALL

Purpose

Read all temperature sensors.

Status

Planned

Implementation Module

Services/TemperatureService

---

# 8. LED Commands

## GET LED

Purpose

Read LED configuration.

Status

Planned

Implementation Module

Services/LEDService

---

## SET LED

Purpose

Change LED colour.

Parameters

ID

COLOR

BRIGHTNESS

Status

Planned

Implementation Module

Services/LEDService

---

## SET LED EFFECT

Purpose

Apply animation.

Examples

Solid

Blink

Pulse

Rainbow

Status

Planned

Implementation Module

Services/LEDService

---

# 9. Drive Commands

Drive information is reported by the Host.

The controller SHALL NOT directly access storage devices.

---

## GET DRIVE

Purpose

Read cached drive state.

Status

Planned

Implementation Module

Services/DriveService

---

## GET DRIVE ALL

Purpose

Read all cached drive information.

Status

Planned

Implementation Module

Services/DriveService

---

Drive States

ONLINE

OFFLINE

READING

WRITING

IDLE

STANDBY

ERROR

---

# 10. Configuration Commands

## GET CONFIG

Purpose

Read configuration.

Status

Planned

Implementation Module

Services/ConfigService

---

## SET CONFIG

Purpose

Modify configuration.

Status

Planned

Implementation Module

Services/ConfigService

---

## SAVE CONFIG

Purpose

Store configuration in flash.

Status

Planned

Implementation Module

Services/ConfigService

---

## LOAD DEFAULTS

Purpose

Restore factory defaults.

Status

Planned

Implementation Module

Services/ConfigService

---

# 11. Diagnostic Commands

## GET LOG

Purpose

Retrieve recent log entries.

Status

Reserved

Implementation Module

Services/LoggerService

---

## GET MEMORY

Purpose

Read RAM usage.

Status

Reserved

Implementation Module

System/SystemManager

---

## GET CPU

Purpose

Read CPU utilization.

Status

Reserved

Implementation Module

System/SystemManager

---

## GET TASKS

Purpose

Read scheduler information.

Status

Reserved

Implementation Module

System/Scheduler

---

## SELF TEST

Purpose

Execute hardware diagnostics.

Status

Reserved

Implementation Module

Services/DiagnosticsService

---

# 12. Event Messages

Events are generated by the controller or the host.

Events are informational.

They do not require acknowledgement.

Examples

SYSTEM READY

RELAY CHANGED

FAN SPEED

TEMPERATURE UPDATE

DRIVE STATE

USB CONNECTED

USB DISCONNECTED

WARNING

ERROR

STATUS UPDATE

All event syntax is defined in Protocol.md.

---

# 13. Error Responses

Typical error responses include

UNKNOWN COMMAND

INVALID PARAMETER

INVALID ID

OUT OF RANGE

NOT SUPPORTED

BUSY

TIMEOUT

HARDWARE ERROR

SYSTEM ERROR

Response format is defined in Protocol.md.
Every failed command SHALL return exactly one error response.

---

# 14. Future Commands

Reserved for future firmware versions.

Network

Wi-Fi

Ethernet

MQTT

Home Assistant

RTC

UPS

Display

OTA Update

Power Management

Cloud Integration

These commands SHALL NOT affect compatibility with Firmware Version 1.x.
Future commands SHALL preserve backward compatibility with Protocol Version 1.x whenever practical.

---

# 15. Command Lifecycle

Every command progresses through the following lifecycle states:

- Planned
- Implemented
- Deprecated
- Reserved

Once a command has been implemented, its behavior is frozen unless an Architecture Decision Record (ADR) or a major firmware version requires modification.

# 16. Command Freeze

This command set is frozen for Firmware Version 1.x.

New commands may be added in future minor versions.

Existing command behaviour SHALL remain backward compatible whenever practical.

Removal or incompatible modification of commands requires:

- Architecture Decision Record (ADR)

or

- Major firmware version increment.