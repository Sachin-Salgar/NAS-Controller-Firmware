# ADR-0006: Services Layer

- Status: Accepted
- Date: 2026-07-18

---

## Context

The firmware requires a dedicated business logic layer that coordinates hardware, firmware state, scheduling, and communication.

Embedding business logic inside Drivers would tightly couple application behavior to hardware.

Embedding business logic inside Protocol would couple firmware behavior to the communication interface.

A separate Services layer provides a clean separation of responsibilities.

---

## Decision

The Services layer shall implement all firmware business logic.

Implemented services include:

- ServiceManager
- RelayService
- FanService
- TemperatureService
- LedService
- DriveService
- ConfigurationService
- StatisticsService
- SystemService
- USBService
- ProtocolService
- SchedulerService
- HealthService
- EventService

Services coordinate Drivers, Objects, and Protocol while remaining independent of hardware implementation.

Every service returns standardized `Core::Result` values.

---

## Responsibilities

Services are responsible for:

- Implementing firmware business logic.
- Coordinating Drivers.
- Updating Objects.
- Executing scheduled tasks.
- Publishing events.
- Managing firmware state transitions.
- Validating requests.
- Providing APIs for the Protocol layer.

Services are **not** responsible for:

- Direct hardware access.
- Packet parsing.
- Packet serialization.
- GPIO operations.
- Device register manipulation.
- Configuration value definitions (use Config layer instead).

### Configuration Integration

Services use compile-time configuration from the Config layer:

- Hardware counts and parameters
- Task intervals and timing
- Protocol configuration
- Feature flags

All hardware-specific values are obtained from `NAS::Config::Hardware::*` to ensure centralized configuration management.

---

## Consequences

### Advantages

- Centralized firmware logic.
- Hardware independence.
- Protocol independence.
- Easier testing.
- Better maintainability.
- Clear separation of responsibilities.

### Limitations

Services must never:

- Call Arduino APIs.
- Call ESP-IDF APIs.
- Access Platform directly.
- Manipulate hardware registers.
- Parse protocol packets.

---

## Dependency Rules

Allowed dependencies:

```
Services
│
├── Core
├── Drivers
└── Objects
```

Services must never depend on:

- Platform
- System

Protocol communicates only with Services.

---

## Architecture

Firmware execution follows this flow:

```
Protocol
      │
Services
      │
Drivers
      │
Platform
      │
Hardware
```

Objects provide shared firmware state used by Services.

```
Services
      │
Objects
```

---

## Status

Frozen for Firmware Version 1.x.
