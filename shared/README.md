# Shared Components

This directory contains specifications, contracts, and utilities shared across the entire NAS Controller project (firmware, daemon, and frontend).

## Contents

- **`docs/`** - Shared documentation and specifications
  - **`PROTOCOL_SPEC.md`** - Binary communication protocol specification (contract between firmware and daemon)
  - **`adr/`** - Architecture Decision Records for shared design decisions
  - **`README.md`** - Documentation index

## Key Shared Contracts

### Binary Protocol (`PROTOCOL_SPEC.md`)

The protocol defines the communication contract between:
- **Firmware** (ESP32 - listens)
- **Daemon** (Host - initiates)

The protocol is **implementation-agnostic** and applies to any language or platform. Both the firmware and daemon must strictly adhere to this specification.

**Key Points:**
- Binary packet format with header validation
- CRC16 checksums for data integrity
- Command-response model
- Multiple command categories (System, Relay, Fan, Temperature, LED, Drive, Configuration, Statistics, Event)

See `docs/PROTOCOL_SPEC.md` for complete specification.

## Shared Utilities

(TBD) - Utility code shared between multiple components (e.g., CRC calculation, common data structures) will be added here as the project develops.

## For Component Developers

- **Firmware Developers**: See `docs/PROTOCOL_SPEC.md` for command implementation requirements
- **Daemon Developers**: See `docs/PROTOCOL_SPEC.md` for command dispatch and packet validation
- **Frontend Developers**: See `WEB_INTERFACE_PLAN.md` at the project root for API contracts with the daemon

## Contributing

Changes to shared contracts require careful coordination to maintain compatibility across all components. Please refer to `/docs/CONTRIBUTING.md`.
