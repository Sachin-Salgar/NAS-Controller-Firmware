# Firmware Documentation

This folder contains all ESP32 firmware documentation and specifications.

## Core Documentation

- **[ARCHITECTURE.md](ARCHITECTURE.md)** - Firmware architecture and design patterns
- **[COMMAND_SET.md](COMMAND_SET.md)** - Complete command reference
- **[CODING_STANDARD.md](CODING_STANDARD.md)** - Firmware coding standards and conventions
- **[HAL.md](HAL.md)** - Hardware Abstraction Layer design and usage

## System Components

- **[LED_SYSTEM.md](LED_SYSTEM.md)** - LED control system and animation framework
- **[TASKS.md](TASKS.md)** - Task scheduling and real-time monitoring
- **[PROTOCOL_LAYER.md](PROTOCOL_LAYER.md)** - Binary protocol implementation

## Planning & Reports

- **[IMPLEMENTATION_CHECKLIST.md](IMPLEMENTATION_CHECKLIST.md)** - Feature implementation status
- **[LED_AUDIT_REPORT.md](LED_AUDIT_REPORT.md)** - LED system audit findings
- **[LED_REDESIGN_COMPLETE.md](LED_REDESIGN_COMPLETE.md)** - LED system redesign summary

## Architecture Decisions (ADR)

All significant architecture decisions are documented in `ADR/`:

- `0001-platform-hal.md` - Hardware Abstraction Layer design
- `0002-drivers-layer.md` - Driver architecture
- `0003-protocol-layer.md` - Protocol layer design
- ... (additional ADRs as decisions are made)

## Quick Links

- **Source Code:** `../src/`
- **Headers:** `../include/`
- **Libraries:** `../lib/`
- **Tests:** `../test/`
- **Configuration:** `../platformio.ini`

## Contributing to Firmware

Before modifying firmware code, read:
1. [ARCHITECTURE.md](ARCHITECTURE.md)
2. [CODING_STANDARD.md](CODING_STANDARD.md)
3. Relevant ADR in `ADR/` folder
4. Project-level [DEVELOPER_GUIDE.md](../../docs/DEVELOPER_GUIDE.md)

## Project Integration

This firmware is part of the NAS Controller project. See [Project Overview](../../docs/README.md) for system-wide documentation.
