# NAS Controller Firmware
# Software Architecture

Document ID: NCF-ARCH-001

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
2. Design Goals
3. Architectural Principles
4. Layered Architecture
5. Project Structure
6. Layer Responsibilities
7. Dependency Rules
8. System Startup
9. Event Flow
10. Communication Flow
11. Module Overview
12. Future Expansion
13. Architecture Freeze

---

# 1. Purpose

This document defines the software architecture of the NAS Controller Firmware.

It specifies

- project structure
- software layers
- responsibilities
- dependencies
- startup sequence
- communication flow

The objective is to provide a modular, maintainable and extensible firmware architecture suitable for long-term development.

---

# 2. Design Goals

The firmware SHALL be

- Modular
- Reliable
- Deterministic
- Hardware Independent
- Maintainable
- Extensible
- Testable
- Production Ready

The firmware SHALL separate hardware control from business logic.

---

# 3. Architectural Principles

The architecture follows these principles.

## Separation of Concerns

Every layer has a single responsibility.

---

## Dependency Direction

Dependencies always point downward.

Higher layers may use lower layers.

Lower layers SHALL NEVER depend on higher layers.

---

## Hardware Abstraction

Application logic SHALL NOT directly access GPIO.

Hardware access SHALL occur only through Drivers.

---

## Protocol Independence

Internal firmware logic SHALL NOT depend on the USB protocol.

Different communication protocols may be implemented without changing business logic.

---

## Event Driven

Modules communicate through events whenever practical.

Direct coupling between unrelated modules is discouraged.

---

# 4. Layered Architecture

```
Application
        │
Protocol
        │
Services
        │
Objects
        │
Drivers
        │
Platform
        │
ESP32 Hardware
```

Each layer exposes services to the layer above.

No upward dependencies are permitted.

---

# 5. Project Structure

```
NAS-Controller-Firmware/

docs/

src/

    Core/

    Platform/

    Drivers/

    Objects/

    Services/

    Protocol/

    System/

    Utilities/

    Config/

    main.cpp

include/

lib/

test/
```

---

# 6. Layer Responsibilities

## Core

Foundation of the firmware.

Contains

- Version
- Build Information
- Result Types
- Logger
- EventBus
- Scheduler
- Common Types

No hardware dependencies.

### Result Types

Extended `ResultCode` enumeration to support comprehensive error reporting:

- **Success** - Operation succeeded
- **Failed** - Generic operation failure
- **InvalidArgument** - Invalid function argument
- **InvalidState** - Object in invalid state
- **NotInitialized** - Component not initialized
- **AlreadyInitialized** - Component already initialized
- **Timeout** - Operation timed out
- **Busy** - Resource busy
- **Unsupported/NotSupported** - Unsupported operation
- **AccessDenied** - Permission denied
- **OutOfMemory** - Memory allocation failed
- **NullPointer** - Null pointer passed to function
- **BufferTooSmall** - Output buffer insufficient
- **BufferFull** - Buffer overflow
- **CommunicationError** - Communication failure
- **ChecksumError** - Checksum validation failed
- **CrcError** - CRC validation failed
- **InvalidHeader** - Malformed packet header
- **InvalidLength** - Invalid length specification
- **NotFound** - Item not found
- **HardwareError** - Hardware fault
- **DeviceNotFound** - Device not detected
- **InvalidConfiguration** - Configuration error
- **InternalError** - Internal firmware error

### Scheduler

Time-based task scheduler for periodic task execution.

- `Initialize()` - Initialize the scheduler
- `AddTask(TaskFunction, intervalMs)` - Register periodic task
- `Execute()` - Process scheduled tasks (non-blocking)

Supports up to 16 concurrent tasks with configurable intervals. Uses `millis()` for timing.

---

## Platform

Provides low-level access to ESP32 hardware.

Examples

GPIO

PWM

Timers

USB

Flash

Watchdog

Platform SHALL hide ESP32 SDK implementation details.

---

## Drivers

Hardware-specific drivers.

Examples

Relay Driver

PWM Fan Driver

Temperature Sensor Driver

Addressable LED Driver

Drive Detection Driver

Storage Driver

Watchdog Driver

Drivers SHALL NOT implement business logic.

Drivers SHALL use only Platform abstractions for hardware access.

All hardware-specific implementation details are contained within Drivers.

---

## Objects

Represents physical hardware as software objects.

Examples

Relay

Fan

TemperatureSensor

LEDStrip

Objects combine driver functionality with state management.

---

## Services

Implements application behaviour.

Examples

RelayService

FanService

TemperatureService

LEDService

USBService

Services coordinate Objects.

---

## Protocol

Implements USB Communication Protocol.

Responsibilities

Command Parser

Response Builder

Serializer

Command Dispatcher

Protocol SHALL NOT directly manipulate hardware.

Protocol communicates only through Services.

---

## System

Responsible for firmware lifecycle.

Contains

SystemManager

Scheduler

Initialization

Shutdown

Watchdog Supervision

---

## Utilities

Generic helper functionality.

Examples

CRC

String Utilities

Math Helpers

Time Helpers

Utilities SHALL NOT depend on higher layers.

---

## Config

Compile-time configuration.

Examples

GPIO Assignments

Feature Flags

Version Numbers

Timing Constants

---

# 7. Dependency Rules

Allowed

```
Protocol
↓

Services
↓

Objects
↓

Drivers
↓

Platform
```

Not Allowed

Drivers → Services

Platform → Protocol

Objects → USB

Services → GPIO

Protocol → Drivers

Circular dependencies are prohibited.

---

# 8. System Startup

Firmware startup SHALL follow this order.

1.

Platform Initialization

↓

2.

Logger Initialization

↓

3.

Configuration Load

↓

4.

Driver Initialization

↓

5.

Object Creation

↓

6.

Service Initialization

↓

7.

Protocol Initialization

↓

8.

Scheduler Start

↓

9.

System Ready

If initialization fails,

SystemManager SHALL determine whether recovery or safe mode is required.

---

# 9. Event Flow

The firmware follows an event-driven architecture.

Example

```
Temperature Driver

↓

Temperature Object

↓

Temperature Service

↓

EventBus

↓

LED Service

↓

Fan Service

↓

USB Service
```

Modules SHOULD communicate through EventBus whenever appropriate.

---

# 10. Communication Flow

External communication

```
Host Computer

↓

USB CDC

↓

Protocol Parser

↓

Command Dispatcher

↓

Service

↓

Object

↓

Driver

↓

Hardware
```

Responses follow the reverse direction.

---

# 11. Module Overview

## Core

Provides shared infrastructure.

---

## Platform

Provides ESP32 abstraction.

---

## Drivers

Controls hardware.

---

## Objects

Represents hardware state.

---

## Services

Implements application logic.

---

## Protocol

Implements USB communication.

---

## System

Coordinates firmware lifecycle.

---

## Utilities

Provides reusable helper functions.

---

## Config

Contains compile-time configuration.

---

# 12. Future Expansion

Architecture supports future implementation of

Ethernet

Wi-Fi

MQTT

Home Assistant

OLED Display

Additional Relays

Additional PWM Outputs

RTC

UPS Monitoring

Additional Sensors

These features SHALL be implemented by extending Services and Protocol without modifying lower layers.

---

# 13. Architecture Freeze

This architecture is frozen for Firmware Version 1.x.

Architectural changes require

- Architecture Decision Record (ADR)

or

- Major Firmware Version increment.

Layer responsibilities SHALL remain stable throughout Firmware Version 1.x.

All firmware modules SHALL comply with this architecture.
