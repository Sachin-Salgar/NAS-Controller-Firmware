# Project Status

## Firmware Status

| Item | Status |
|------|--------|
| **Firmware Version** | 1.0.0 |
| **Protocol Version** | 0x0100 |
| **Build Status** | ✅ PASSED (zero warnings, zero errors) |
| **Freeze Status** | ✅ FROZEN |
| **Ready for Daemon Development** | ✅ YES |

## Firmware Specifications

**Microcontroller**: ESP32 (ESP32-WROOM-32)  
**Language**: C++17  
**Build System**: PlatformIO  

**Protocol**:
- Binary USB packet protocol at 115200 baud
- Packet format: `0x55AA` header, sequence, command, payload length, payload, CRC16
- CRC: Polynomial 0xA001, initial 0xFFFF
- Limits: Max packet 512 bytes, max payload 500 bytes

**Registered Commands**: 28 total  
- 26 with implemented success paths
- 2 registered as unsupported (`LedGet`, `LedSetColor`)

## Implemented Services

- Configuration
- Statistics  
- System
- Relay (2 channels)
- Fan (5 channels)
- Temperature (0–4 OneWire sensors)
- LED (mode, brightness, off; color unsupported)
- Drive
- USB
- Protocol
- Health
- Event (read/clear only)
- Scheduler

## Unsupported Features

- ASCII protocol (binary only)
- LED color control (`LedGet` and `LedSetColor` return `NotSupported`)
- Asynchronous event push (command-based event read/clear only)
- Protocol negotiation or authentication
- Firmware update mechanism
- File transfer protocol
- Packet fragmentation
- Endian-independent encoding (uses native target byte order)

## Immutable Items for v1.0.x

The following cannot change without a protocol version increment:

- Packet format (header, sequence, command, payload length, CRC)
- Command IDs (0x0001–0x1702)
- CRC algorithm (CRC-16/Modbus, polynomial 0xA001, initial 0xFFFF)
- Configuration persistence format
- Statistics format
- Event format
- Response format and error codes
- Protocol timing (5ms USB/protocol polling, 1000ms temperature/health polling)
- Service interfaces and initialization order

See **[docs/CHANGE_POLICY.md](docs/CHANGE_POLICY.md)** for detailed change rules.

## Documentation

All authoritative documentation is in the `docs/` directory:

- **[docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)** – Layered architecture, service initialization order
- **[docs/PROTOCOL_SPEC.md](docs/PROTOCOL_SPEC.md)** – Binary protocol format and limits
- **[docs/PROTOCOL_REGISTRY.md](docs/PROTOCOL_REGISTRY.md)** – Command IDs, payloads, responses
- **[docs/CHANGE_POLICY.md](docs/CHANGE_POLICY.md)** – Immutable items and change rules
- **[docs/DECISIONS.md](docs/DECISIONS.md)** – Architecture Decision Records (ADRs)
- **[docs/TERMINOLOGY.md](docs/TERMINOLOGY.md)** – Glossary
- **[docs/EXTENSION_POINTS.md](docs/EXTENSION_POINTS.md)** – Future extensibility guidance
- **[docs/NOT_PLANNED.md](docs/NOT_PLANNED.md)** – Explicitly unsupported features
- **[docs/Hardware.md](docs/Hardware.md)** – Hardware specifications
- **[docs/LED_Subsystem.md](docs/LED_Subsystem.md)** – LED hardware and firmware capabilities
- **[docs/CodingStandard.md](docs/CodingStandard.md)** – Code style and conventions
- **[docs/Repository-Structure.md](docs/Repository-Structure.md)** – Source code layout

## Next Steps

Future development occurs in:

1. **Daemon Repository** – Consumes this firmware via the binary protocol
2. **Web Interface Repository** – Uses daemon as intermediary to the firmware

Both must follow the frozen protocol specification exactly as implemented in this firmware.
