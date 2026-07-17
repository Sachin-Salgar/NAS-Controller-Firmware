# ADR-0007: Protocol Layer

- Status: Accepted
- Date: 2026-07-18

---

## Context

The firmware communicates with the host computer over USB using a custom binary protocol.

To allow future communication transports (USB, Ethernet, Wi-Fi, BLE, UART, etc.) without modifying firmware business logic, packet handling must be isolated from the Services layer.

The Protocol layer provides this abstraction by handling packet validation, parsing, dispatching, and response generation.

---

## Decision

The Protocol layer shall be responsible for all protocol-related operations.

Implemented modules include:

- Commands
- PacketValidator
- PacketParser
- PacketBuilder
- ResponseBuilder
- CommandDispatcher

The Protocol layer communicates only with the Services layer and remains independent of hardware implementation.

All protocol processing returns standardized `Core::Result` values.

---

## Responsibilities

The Protocol layer is responsible for:

- Receiving packets from communication transports.
- Validating packet integrity.
- Verifying packet CRC.
- Parsing packet headers.
- Extracting payloads.
- Dispatching commands.
- Building response packets.
- Building error responses.

The Protocol layer is **not** responsible for:

- Hardware access.
- Business logic.
- Device control.
- Configuration management.
- Scheduling.

---

## Consequences

### Advantages

- Communication transport independence.
- Centralized protocol implementation.
- Easier protocol maintenance.
- Easier protocol testing.
- Future transport reuse.
- Reduced coupling between communication and firmware logic.

### Limitations

The Protocol layer must never:

- Access Platform.
- Access Drivers.
- Modify firmware state directly.
- Execute hardware operations.
- Call Arduino APIs.
- Call ESP-IDF APIs.

---

## Dependency Rules

Allowed dependencies:

```
Protocol
│
├── Core
├── Utilities
└── Services
```

Protocol must never depend on:

- Platform
- Drivers
- Objects
- System

Services provide the firmware API used by Protocol.

---

## Architecture

Packet processing follows this sequence:

```
USB / UART / Network
          │
PacketValidator
          │
PacketParser
          │
CommandDispatcher
          │
Services
          │
Drivers
          │
Platform
          │
Hardware
```

Response generation follows:

```
Services
     │
ResponseBuilder
     │
PacketBuilder
     │
Communication Transport
```

This architecture keeps packet handling completely independent of firmware implementation details.

---

## Status

Frozen for Firmware Version 1.x.