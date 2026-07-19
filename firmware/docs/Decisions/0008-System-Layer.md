# ADR-0008: System Layer

- Status: Accepted
- Date: 2026-07-18

---

## Context

The firmware requires a single layer responsible for controlling the complete firmware lifecycle.

Without a dedicated System layer, initialization would become scattered across multiple modules, making startup order difficult to understand, maintain, and extend.

The System layer provides a single entry point responsible for bootstrapping the firmware and controlling runtime execution.

---

## Decision

The System layer shall own the firmware lifecycle.

Implemented modules include:

- Startup
- Application
- SystemManager
- main.cpp entry point

The System layer coordinates initialization of every firmware layer in the correct order and starts the main execution loop.

---

## Responsibilities

The System layer is responsible for:

- Firmware startup.
- Initialization sequencing.
- Runtime lifecycle management.
- Application startup.
- Main execution loop.
- Fatal error handling.
- System shutdown and restart coordination.

The System layer is **not** responsible for:

- Hardware abstraction.
- Device control.
- Packet processing.
- Business logic.
- Runtime firmware state.

---

## Startup Sequence

Firmware initialization follows the architecture defined in ADR-0001.

```
main()

↓

SystemManager::Initialize()

↓

Startup::Initialize()

↓

Platform

↓

Drivers

↓

Objects

↓

Services

↓

Application::Initialize()

↓

SystemManager::Run()

↓

Application::Run()

↓

Scheduler

↓

Services

↓

Drivers

↓

Platform

↓

Hardware
```

The startup sequence is deterministic and every layer must initialize successfully before runtime execution begins.

---

## Consequences

### Advantages

- Single firmware entry point.
- Deterministic initialization.
- Centralized lifecycle management.
- Easier debugging.
- Easier future expansion.
- Clean separation from business logic.

### Limitations

The System layer must never:

- Access hardware directly.
- Parse protocol packets.
- Execute device-specific logic.
- Store firmware state.
- Replace Service functionality.

---

## Dependency Rules

Allowed dependencies:

```
System
│
├── Core
├── Platform
├── Drivers
├── Objects
├── Services
└── Protocol
```

The System layer is the highest firmware layer.

No other layer depends on System.

---

## Architecture

Overall firmware execution follows:

```
main
 │
 ▼
System
 │
 ▼
Protocol
 │
 ▼
Services
 │
 ▼
Objects
 │
 ▼
Drivers
 │
 ▼
Platform
 │
 ▼
ESP32 Hardware
```

The System layer coordinates the firmware but contains no business logic or hardware implementation.

---

## Status

Frozen for Firmware Version 1.x.