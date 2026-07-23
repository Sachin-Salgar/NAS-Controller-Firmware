# NAS Controller Firmware

A modular, embedded firmware system for controlling external NAS hardware via ESP32. This project provides deterministic hardware control, real-time monitoring, and a structured binary protocol for host communication.

**Status: FROZEN v1.0.0** — The firmware is the authoritative implementation for daemon and web-interface development.

## Quick Reference

| Item | Value |
|------|-------|
| **Firmware Version** | 1.0.0 |
| **Protocol Version** | 0x0100 |
| **Microcontroller** | ESP32 (ESP32-WROOM-32) |
| **Status** | FROZEN |
| **Build** | ✅ PASSED (zero warnings) |

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

## Frozen Protocol & Implementation

The firmware implementation is the source of truth for daemon and web-interface development. The frozen public contract is documented in:

- **[PROTOCOL_SPEC.md](docs/PROTOCOL_SPEC.md)** – Binary packet format, CRC algorithm, limits
- **[PROTOCOL_REGISTRY.md](docs/PROTOCOL_REGISTRY.md)** – Command IDs, payloads, response formats
- **[ARCHITECTURE.md](docs/ARCHITECTURE.md)** – Layered architecture, service initialization order
- **[CHANGE_POLICY.md](docs/CHANGE_POLICY.md)** – Immutable items in v1.0.0, protocol evolution rules

**Note**: Older ASCII protocol drafts are obsolete. The firmware only implements the binary protocol.

## Technology Stack

| Component | Details |
|-----------|---------|
| **Framework** | Arduino |
| **Build System** | PlatformIO |
| **Language** | C++17 |
| **Dependencies** | paulstoffregen/OneWire, adafruit/Adafruit NeoPixel |

## Architecture Overview

The firmware follows a **strict layered architecture** with dependencies always pointing downward (frozen for v1.x):

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

## Documentation Index

**Authoritative & Frozen:**
- **[PROTOCOL_SPEC.md](docs/PROTOCOL_SPEC.md)** – Packet format, CRC, limits
- **[PROTOCOL_REGISTRY.md](docs/PROTOCOL_REGISTRY.md)** – All 28 command IDs with payloads
- **[ARCHITECTURE.md](docs/ARCHITECTURE.md)** – Layered design, initialization order
- **[CHANGE_POLICY.md](docs/CHANGE_POLICY.md)** – Frozen items, change rules
- **[EXTENSION_POINTS.md](docs/EXTENSION_POINTS.md)** – Future extensibility guidance
- **[NOT_PLANNED.md](docs/NOT_PLANNED.md)** – Explicitly unsupported features

**Reference & Context:**
- **[Hardware.md](docs/Hardware.md)** – Hardware specifications & constraints
- **[LED_Subsystem.md](docs/LED_Subsystem.md)** – LED hardware and firmware capabilities
- **[TERMINOLOGY.md](docs/TERMINOLOGY.md)** – Glossary & terminology
- **[CodingStandard.md](docs/CodingStandard.md)** – Code style & conventions
- **[Repository-Structure.md](docs/Repository-Structure.md)** – Source code layout
- **[Decisions/](docs/Decisions/)** – Architecture Decision Records (ADRs)

**Historical Archive:**
- **[archive/firmware-freeze/](archive/firmware-freeze/)** – Freeze reports, audit reports, superseded specs

## Project Structure

```
├── docs/                          # Authoritative documentation
│   ├── PROTOCOL_SPEC.md           # Frozen binary protocol spec
│   ├── PROTOCOL_REGISTRY.md       # Frozen command registry
│   ├── ARCHITECTURE.md            # Frozen architecture design
│   ├── CHANGE_POLICY.md           # Frozen change policy
│   ├── Hardware.md                # Hardware specifications
│   ├── Decisions/                 # Architecture decision records
│   └── ...                        # Reference documentation
│
├── src/                           # Firmware source code (FROZEN)
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
├── archive/firmware-freeze/       # Historical freeze reports
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

## Getting Started

### Prerequisites

- PlatformIO Core or IDE
- ESP32 development board
- Connected hardware (relays, fans, sensors, LEDs)

### Building

```bash
# Build for ESP32 Dev Module
platformio run -e esp32dev

# Monitor serial output
platformio run -e esp32dev --monitor

# Upload to board
platformio run -e esp32dev -t upload
```

### Development

The firmware follows a strict coding standard documented in `docs/CodingStandard.md`. All code must:
- Follow the dependency direction rules
- Maintain layer separation
- Include appropriate error handling via `Result` type
- Use the Event Bus for inter-layer communication where appropriate

## Binary Protocol

The firmware communicates via USB CDC serial at **115200 baud** using a binary packet protocol.

**Packet Format:**
- Header: `0x55AA` (fixed)
- Sequence: 1 byte (echoed in response)
- Command: 1 byte (command ID)
- Payload Length: 2 bytes
- Payload: 0–500 bytes
- CRC16: 2 bytes (polynomial 0xA001, initial 0xFFFF)

**Command Categories:** System, Relay, Fan, Temperature, LED, Drive, Configuration, Statistics, Event

See **[PROTOCOL_SPEC.md](docs/PROTOCOL_SPEC.md)** for complete format details and **[PROTOCOL_REGISTRY.md](docs/PROTOCOL_REGISTRY.md)** for all 28 command IDs and payloads.

## Firmware Freeze Status

✅ **OFFICIALLY FROZEN** – v1.0.0

The firmware is feature-complete and the authoritative implementation for daemon and web-interface development.

| Item | Status |
|------|--------|
| Firmware Status | FROZEN |
| Protocol Status | FROZEN |
| Architecture Status | FROZEN |
| Build Verification | ✅ PASSED |
| Compiler Warnings | 0 |
| Compiler Errors | 0 |
| Ready for Daemon Dev | ✅ YES |

**Future Protocol Changes:** Any breaking changes require a protocol version increment (e.g., 0x0101 for v1.1.0, 0x0200 for v2.0.0). See **[CHANGE_POLICY.md](docs/CHANGE_POLICY.md)** for change rules.

## For Daemon & Web Interface Developers

The firmware behavior, protocol, and architecture are the **source of truth**. You must consume this firmware exactly as implemented. Key references:

1. **[PROTOCOL_SPEC.md](docs/PROTOCOL_SPEC.md)** – Wire protocol, packet format, CRC
2. **[PROTOCOL_REGISTRY.md](docs/PROTOCOL_REGISTRY.md)** – Command IDs, payloads, responses
3. **[ARCHITECTURE.md](docs/ARCHITECTURE.md)** – Service interfaces, initialization order
4. **[CHANGE_POLICY.md](docs/CHANGE_POLICY.md)** – What is immutable, how to request changes

The firmware is **fully documented and will not change** for v1.0.x. Bug fixes never change the public protocol.

## License

See LICENSE file for details.

## Development Notes

- The codebase uses C++17 with modern features
- All GPIO/hardware access is abstracted through the Platform and Driver layers
- Services are initialized in a specific order (see ServiceManager)
- The scheduler manages periodic task execution at configurable intervals
- Event Bus enables loosely-coupled inter-component communication

For more details on development patterns and conventions, see `docs/CodingStandard.md` and source code inline documentation.
