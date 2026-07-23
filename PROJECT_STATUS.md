# Project Status — Firmware Frozen v1.0.0

The firmware is the **authoritative implementation** for daemon and web-interface development.

## Firmware Contract (Frozen)

| Item | Value |
|------|-------|
| **Firmware Version** | 1.0.0 |
| **Protocol Version** | 0x0100 |
| **Microcontroller** | ESP32 (ESP32-WROOM-32) |
| **Registered Commands** | 28 |
| **Implemented Success Paths** | 26 |
| **Registered Unsupported** | 2 (`LedGet`, `LedSetColor`) |

## Frozen Protocol Specification

- **Packet Format**: `0x55AA` header, 1-byte sequence, 1-byte command, 2-byte payload length, 0–500 byte payload, 2-byte CRC16
- **CRC Algorithm**: Polynomial 0xA001, initial value 0xFFFF
- **Transport**: USB CDC serial @ 115200 baud
- **Command Categories**: System, Relay, Fan, Temperature, LED, Drive, Configuration, Statistics, Event
- **Configuration Storage**: Binary record at flash address 0 with magic 0x31474643, version 1, relay/fan/temperature/LED config
- **Error Responses**: Original sequence and command echo with 2-byte ResultCode payload
- **Limits**: Max packet 512 bytes, max payload 500 bytes

See **[PROTOCOL_SPEC.md](docs/PROTOCOL_SPEC.md)** and **[PROTOCOL_REGISTRY.md](docs/PROTOCOL_REGISTRY.md)** for complete specifications.

## Architecture (Frozen)

- **Layered Design**: Core → Platform → Drivers → Objects → Services → Protocol → System
- **Service Initialization Order**: Config → Statistics → System → Relay → Fan → Temperature → LED → Drive → USB → Protocol → Health → Event → Scheduler
- **Polling Intervals**: USB/Protocol 5ms, Temperature/Health 1000ms
- **State Machine**: Application runs `ServiceManager::Execute()` loop continuously
- **Singleton Pattern**: Services own static state; Objects hold runtime data

See **[ARCHITECTURE.md](docs/ARCHITECTURE.md)** for full design documentation.

## Implemented Services

- **Configuration** – Persistent storage management
- **Statistics** – Boot count, USB counters, relay cycles, fan runtime, uptime
- **System** – Firmware lifecycle and startup/shutdown
- **Relay** – Power relay control (2 relays)
- **Fan** – PWM fan control (5 fans)
- **Temperature** – OneWire temperature sensor readout (0–4 sensors)
- **LED** – WS2812B addressable LED control (mode, brightness, off only; color unsupported)
- **Drive** – Drive presence and health reporting
- **USB** – Character and binary I/O on serial port
- **Protocol** – Binary packet validation, parsing, command dispatch
- **Health** – System monitoring and watchdog recovery
- **Event** – Event queue management (read/clear only; no async push)
- **Scheduler** – Periodic task execution

## Unsupported or Partial Features

- **LED Control**: `LedGet` and `LedSetColor` return `NotSupported` permanently for v1.0.0
- **ASCII Protocol**: Not implemented; only binary protocol is supported
- **No Async Events**: Firmware has no asynchronous event push mechanism; only command-based event read/clear
- **No Protocol Negotiation**: No version negotiation, authentication, or encryption
- **No Firmware Update**: No over-the-wire firmware upload mechanism
- **No File Transfer**: No file transfer protocol
- **No Packet Fragmentation**: All commands fit in single packets
- **No Endian Encoding**: Uses native target byte order (no cross-architecture support)

## Verification Status

| Item | Status | Evidence |
|------|--------|----------|
| **PlatformIO Build** | ✅ PASSED | Real PlatformIO environment, zero warnings |
| **Compiler Warnings** | ✅ NONE | Verified in build output |
| **Compiler Errors** | ✅ NONE | Verified in build output |
| **Protocol Audit** | ✅ PASSED | All 28 command IDs verified against source |
| **Architecture Compliance** | ✅ PASSED | All layers verified, dependencies correct |
| **Documentation Consistency** | ✅ PASSED | Firmware behavior matches frozen specs |

## Freeze Status

| Phase | Status |
|-------|--------|
| **Firmware Implementation** | ✅ COMPLETE |
| **Firmware Freeze** | ✅ OFFICIAL |
| **Protocol Freeze** | ✅ OFFICIAL |
| **Architecture Freeze** | ✅ OFFICIAL (v1.x) |
| **Documentation Freeze** | ✅ COMPLETE |
| **Ready for Daemon Development** | ✅ YES |

## Change Policy

The firmware is **immutable for v1.0.x**. See **[CHANGE_POLICY.md](docs/CHANGE_POLICY.md)** for rules on:

- What cannot change in v1.0.x (packet format, command IDs, CRC, configuration format, etc.)
- How to request protocol changes (require version increment)
- Bug fix policy (no public protocol changes)
- Future protocol evolution rules

## Documentation

All authoritative documentation is in the `docs/` directory:

- **Frozen Specs**: PROTOCOL_SPEC.md, PROTOCOL_REGISTRY.md, ARCHITECTURE.md, CHANGE_POLICY.md
- **Reference**: Hardware.md, LED_Subsystem.md, TERMINOLOGY.md, CodingStandard.md, Repository-Structure.md
- **Decisions**: Decisions/*.md (Architecture Decision Records)
- **Archive**: archive/firmware-freeze/ (historical freeze/audit reports)

## Project Phase

The NAS Controller Firmware has completed its initial development and is now in **long-term maintenance mode**. Future work focuses on:

1. **Daemon Development** – Consume this firmware via the binary protocol
2. **Web Interface** – Use daemon as intermediary to the firmware
3. **Bug Fixes** – Only non-protocol-breaking fixes allowed in v1.0.x
4. **Future Versions** – Any breaking changes require a new protocol version

The firmware will **not change** until a new version tag is created.
