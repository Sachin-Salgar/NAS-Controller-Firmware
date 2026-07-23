# NAS Controller Firmware

A modular, embedded firmware system for controlling external NAS hardware via ESP32. This repository is the authoritative implementation of the NAS Controller firmware.

## Status

| Item | Value |
|------|-------|
| **Firmware Version** | 1.0.0 |
| **Protocol Version** | 0x0100 |
| **Status** | FROZEN |
| **Microcontroller** | ESP32 (ESP32-WROOM-32) |

The firmware is frozen. Future development occurs in the daemon and web interface repositories.

Breaking protocol changes require a protocol version increment.

## Documentation

- **[PROJECT_STATUS.md](PROJECT_STATUS.md)** – Project status, verified features, known limitations
- **[docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)** – Layered architecture and service initialization order
- **[docs/PROTOCOL_SPEC.md](docs/PROTOCOL_SPEC.md)** – Binary packet format, CRC algorithm, limits
- **[docs/PROTOCOL_REGISTRY.md](docs/PROTOCOL_REGISTRY.md)** – All 28 command IDs with payloads and responses
- **[docs/CHANGE_POLICY.md](docs/CHANGE_POLICY.md)** – What is immutable in v1.0.0 and how to request changes

## Building

```bash
# Build for ESP32
platformio run -e esp32dev

# Upload to board
platformio run -e esp32dev -t upload

# Monitor serial
platformio run -e esp32dev --monitor
```

## Getting Started

1. Read **[PROJECT_STATUS.md](PROJECT_STATUS.md)** for project overview
2. Read **[docs/PROTOCOL_SPEC.md](docs/PROTOCOL_SPEC.md)** to understand the protocol
3. Read **[docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)** to understand the codebase
4. Read **[docs/PROTOCOL_REGISTRY.md](docs/PROTOCOL_REGISTRY.md)** for command details

See **[docs/](docs/)** for complete documentation.
