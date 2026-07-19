# ADR-0005: Objects Layer

- Status: Accepted
- Date: 2026-07-18

---

## Context

The firmware requires a central location to store runtime state independently of hardware access and business logic.

Without a dedicated Objects layer, Services would own state, causing duplication, tighter coupling, and more difficult testing.

A separate Objects layer provides a shared data model for the firmware.

---

## Decision

The Objects layer shall contain only firmware state and data models.

Implemented objects include:

- ObjectManager
- Relay
- Fan
- TemperatureSensor
- Led
- Drive
- Configuration
- Statistics
- SystemStatus

Objects expose data through well-defined interfaces and never communicate directly with hardware.

---

## Responsibilities

Objects are responsible for:

- Holding runtime state.
- Holding configuration values.
- Holding statistics.
- Holding device status.
- Providing getters and setters.
- Providing validation where appropriate.

Objects are **not** responsible for:

- Hardware access.
- Business logic.
- Communication protocols.
- Scheduling.
- Event handling.

---

## Consequences

### Advantages

- Single source of truth.
- Shared firmware data model.
- Easy serialization.
- Easy persistence.
- Easier unit testing.
- Reduced coupling.

### Limitations

Objects must never:

- Include Arduino APIs.
- Include ESP-IDF APIs.
- Access Platform.
- Access Drivers.
- Execute business logic.

---

## Dependency Rules

Allowed dependencies:

```
Objects
│
└── Core
```

Objects must never depend on:

- Platform
- Drivers
- Services
- Protocol
- System

Services are responsible for reading and updating Objects.

---

## Architecture

The Objects layer acts as the firmware's runtime database.

```
Services
      │
Objects
      │
Firmware State
```

Objects remain passive containers that represent the current firmware state without owning any execution logic.

---

## Status

Frozen for Firmware Version 1.x.