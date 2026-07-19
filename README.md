# NAS Controller Firmware

A modular, embedded firmware system for controlling external NAS hardware via ESP32. This project provides deterministic hardware control, real-time monitoring, and a structured binary protocol for host communication.

## Overview

The NAS Controller Firmware is an embedded control plane that manages:

- **Power Relays** – Control external power outputs
- **Fan Control** – PWM-based fan speed management
- **Temperature Monitoring** – DS18B20 sensor integration
- **Addressable LEDs** – WS2812B LED strip control
- **Drive Monitoring** – Presence, state, and health reporting
- **USB Communication** – Binary packet protocol with CRC16 validation
- **Configuration** – Persistent settings storage
- **Statistics** – Runtime metrics and health tracking
- **Watchdog** – System health recovery mechanism

## Technology Stack

| Component | Details |
|-----------|---------|
| **Microcontroller** | ESP32 (ESP32-WROOM-32) |
| **Framework** | Arduino |
| **Build System** | PlatformIO |
| **Language** | C++17 |
| **Dependencies** | paulstoffregen/OneWire |

## Architecture

The firmware follows a **strict layered architecture** with dependencies always pointing downward:

```
Application
    ↓
System (Startup & Lifecycle)
    ↓
Protocol (Binary packet handling)
    ↓
Services (Business logic & orchestration)
    ↓
Objects (Runtime state models)
    ↓
Drivers (Hardware-specific control)
    ↓
Platform (ESP32/Arduino HAL)
    ↓
Core (Utilities & types)

Shared: Config & Utilities
```

### Layer Responsibilities

- **Core** – Fundamental utilities (Result, Logger, Version, EventBus)
- **Platform** – Hardware abstraction (GPIO, PWM, USB, Flash, OneWire, Watchdog)
- **Drivers** – Device-specific control (Relay, Fan, LED, Temperature, Storage, Watchdog)
- **Objects** – Runtime state (Relay, Fan, LED, Drive, TemperatureSensor, Configuration, Statistics)
- **Services** – Business logic (RelayService, FanService, ProtocolService, HealthService, etc.)
- **Protocol** – Command dispatch, packet validation, and parsing
- **System** – Firmware lifecycle and application entry points
- **Config & Utilities** – Compile-time configuration and shared helpers

## Project Structure

```
├── docs/                          # Comprehensive documentation
│   ├── Architecture.md            # Layered architecture design
│   ├── Hardware.md                # Hardware specifications
│   ├── Protocol.md                # Binary protocol format
│   ├── CommandSet.md              # Available commands
│   ├── USB-Host-Protocol.md       # Host communication spec
│   ├── CodingStandard.md          # Code conventions
│   ├── Roadmap.md                 # Development roadmap
│   ├── Decisions/                 # Architecture decision records
│   └── ...                        # Additional documentation
│
├── src/                           # Firmware source code
│   ├── main.cpp                   # Arduino entry point
│   ├── Core/                      # Foundation types & utilities
│   ├── Platform/                  # Hardware abstraction layer
│   ├── Drivers/                   # Hardware drivers
│   ├── Objects/                   # Domain models & state
│   ├── Services/                  # Business logic & orchestration
│   ├── Protocol/                  # Binary protocol handling
│   ├── System/                    # Lifecycle management
│   ├── Config/                    # Build configuration
│   └── Utilities/                 # Shared helper functions
│
├── platformio.ini                 # PlatformIO build configuration
├── LICENSE                        # License
└── README.md                      # This file
```

## Key Features

### Modular Design
- Strict separation of concerns with well-defined layer boundaries
- Hardware abstraction separates platform-specific code from application logic
- Event-driven architecture for decoupled component communication

### Deterministic Execution
- Scheduled task-based execution model
- Configurable polling intervals (USB/Protocol: 5ms, Temperature/Health: 1000ms)
- No interrupt-driven business logic

### Binary Protocol
- Custom packet format with header validation and CRC16 checksums
- Transport-independent protocol design (currently USB CDC)
- Extensible command set with multiple categories (System, Relay, Fan, Temperature, LED, Drive, Configuration, Statistics, Event)

### Hardware Features
- GPIO-based relay control
- PWM fan speed management
- OneWire temperature sensor discovery and readout
- NeoPixel/WS2812B LED control
- USB serial communication (115200 baud)
- Persistent flash storage for configuration
- Watchdog-based system recovery

### Monitoring & Diagnostics
- Runtime statistics tracking
- System health monitoring
- Boot counter and uptime tracking
- Event logging and history
- Debug logging support

## Current Status

**✅ Architecture Verified** – 100% compliant with frozen standards (ADR-0001)
**✅ Firmware Fixed** – main.cpp properly initializes SystemManager
**✅ LED System Complete** – All 60 LEDs designed with animations
⚠️ **Not Compiled Yet** – Build verification needed
⚠️ **Not Tested** – No hardware validation yet

See `STATUS_REPORT.md` for detailed layer-by-layer status.

## Getting Started

### Prerequisites

- PlatformIO Core or IDE
- ESP32 development board (ESP32-WROOM-32)
- Connected hardware: relays, fans, temperature sensors, addressable LEDs

### Build & Test

```bash
# Compile firmware
platformio run -e esp32dev

# Upload to board
platformio run -e esp32dev -t upload

# View serial output (115200 baud)
platformio run -e esp32dev --monitor
```

### Development Standards

All code must follow `docs/CodingStandard.md`:
- **Naming:** PascalCase classes, camelCase variables, k-prefix constants
- **Error Handling:** Use `Result<>` type, never boolean flags
- **Layering:** Dependencies always point downward (frozen in Architecture.md)
- **Memory:** Static allocation only, no dynamic memory after init
- **Hardware:** Platform layer only, never in Services/Objects/Drivers

## Communication

### Host Protocol

The firmware communicates with the host computer via USB CDC serial at **115200 baud**.

Packets use a binary format:
- Header: `0x55AA` (2 bytes)
- Sequence: (1 byte)
- Command: (1 byte)
- Payload Length: (2 bytes)
- Payload: (variable)
- CRC16: (2 bytes)

For detailed protocol documentation, see `docs/Protocol.md` and `docs/USB-Host-Protocol.md`.

### Supported Commands

Commands are grouped by category:
- **System** – Ping, GetVersion, GetStatus, Reset
- **Relay** – Get, Set, Enable, Disable
- **Fan** – Get, SetSpeed, SetMode
- **Temperature** – Get, Calibrate, SetThreshold
- **LED** – Get, SetColor, SetMode, SetBrightness
- **Drive** – Get, Scan, GetHealth
- **Configuration** – Get, Set, Save, Load
- **Statistics** – Get, Reset
- **Event** – Read, Clear

See `docs/CommandSet.md` for complete command specifications.

## Status & Roadmap

### Current Phase

✅ **Phase 1 & 2 Complete** – Architecture foundation and core firmware implementation
- ✅ Documented architecture (frozen for 1.x)
- ✅ All layers implemented
- ✅ Service infrastructure in place
- ✅ Protocol scaffolding complete
- ✅ **Architecture alignment verified July 19, 2026** – 100% compliant with frozen standards (ADR-0001)

### Next Steps

- ⬜ Firmware compilation and build verification
- ⬜ Hardware integration testing
- ⬜ Command implementation completion
- ⬜ Unit and integration test suite
- ⬜ Stability and performance validation

See `docs/Roadmap.md` for detailed development phases.

## Documentation

**Essential Reading:**
- `docs/Architecture.md` – Frozen system design (8 layers, dependency rules)
- `docs/CodingStandard.md` – Mandatory coding conventions (naming, error handling, memory)
- `STATUS_REPORT.md` – What's working, what's not, next steps

**Reference Docs:**
- `docs/Hardware.md` – ESP32 GPIO, relays, fans, sensors, LED wiring
- `docs/Protocol.md` – Binary packet format and validation
- `docs/CommandSet.md` – Supported commands and payloads
- `docs/Decisions/` – Architecture decision records (ADR-0001 through ADR-0008)
- `docs/Repository-Structure.md` – Source code organization
- `docs/Roadmap.md` – Development phases and milestones

## Architecture Freeze

The firmware architecture is **frozen for Version 1.x**. Architectural changes require documentation and ADR updates. This ensures stability and maintainability for the current release cycle.

See `docs/Decisions/` for architectural decisions and constraints.

## License

See LICENSE file for details.

## Development Notes

- The codebase uses C++17 with modern features
- All GPIO/hardware access is abstracted through the Platform and Driver layers
- Services are initialized in a specific order (see ServiceManager)
- The scheduler manages periodic task execution at configurable intervals
- Event Bus enables loosely-coupled inter-component communication

For more details on development patterns and conventions, see `docs/CodingStandard.md` and source code inline documentation.
