# Repository Structure

This document describes the repository organization for the NAS Controller Firmware.

---

# Root Structure

```
NAS-Controller-Firmware/
│
├── docs/
├── include/
├── lib/
├── src/
├── test/
├── tools/
│
├── platformio.ini
├── README.md
├── LICENSE
└── .gitignore
```

---

# docs/

Contains all project documentation.

```
docs/
│
├── Architecture.md
├── Hardware.md
├── Protocol.md
├── CommandSet.md
├── CodingStandard.md
├── Repository-Structure.md
├── USB-Host-Protocol.md
├── Roadmap.md
├── CHANGELOG.md
│
└── Decisions/
    ├── 0001-Firmware-Architecture.md
    ├── 0002-Core-Layer.md
    ├── 0003-Platform-HAL.md
    ├── 0004-Drivers-Layer.md
    ├── 0005-Objects-Layer.md
    ├── 0006-Services-Layer.md
    ├── 0007-Protocol-Layer.md
    └── 0008-System-Layer.md
```

---

# src/

Contains firmware source code.

```
src/
│
├── Core/
├── Platform/
├── Drivers/
├── Objects/
├── Services/
├── Protocol/
├── System/
├── Config/
├── Utilities/
│
└── main.cpp
```

---

# Layer Responsibilities

## Core

Platform-independent framework.

Examples:

- Result
- Logger
- Scheduler
- Version
- EventBus

---

## Platform

Hardware Abstraction Layer.

Examples:

- GPIO
- PWM
- USB
- SPI
- I2C
- UART
- Flash

---

## Drivers

Hardware device drivers.

Examples:

- RelayDriver
- FanDriver
- LedDriver
- TemperatureDriver

---

## Objects

Firmware runtime objects.

Examples:

- Relay
- Fan
- Drive
- Configuration
- Statistics

---

## Services

Firmware business logic.

Examples:

- RelayService
- FanService
- DriveService
- USBService

---

## Protocol

Communication protocol.

Examples:

- PacketParser
- PacketBuilder
- CommandDispatcher

---

## System

Firmware lifecycle.

Examples:

- Startup
- Application
- SystemManager

---

## Config

Compile-time configuration.

Examples:

- Features
- HardwareConfig
- ProtocolConfig
- TaskConfig

---

## Utilities

Reusable helper classes.

Examples:

- CRC16
- ByteBuffer
- BitHelpers
- StringHelpers
- TimeHelpers

---

# Dependency Rules

```
System
    ↓
Protocol
    ↓
Services
   ↙     ↘
Objects Drivers
            ↓
        Platform
            ↓
          Core
```

Every layer may use:

- Core
- Utilities
- Config

Only Drivers communicate with Platform.

Only Services communicate with Drivers.

Only Protocol communicates with Services.

Only System controls initialization.

---

# Build System

- PlatformIO
- Arduino Framework
- ESP32 Dev Module

---

# Repository Status

Architecture frozen for Firmware Version 1.x.