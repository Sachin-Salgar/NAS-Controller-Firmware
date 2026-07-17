# ADR-0002: Core Layer

- Status: Accepted
- Date: 2026-07-18

---

## Context

The firmware requires a foundation that is completely independent of hardware, communication protocols, operating systems, and application logic.

Every other layer depends on common functionality including:

- Result handling
- Version information
- Build information
- Scheduler
- Event handling
- Logging

To maximize portability and testability, this functionality must remain platform independent.

---

## Decision

The Core layer shall:

- Contain only platform-independent code.
- Never include Arduino headers.
- Never include ESP-IDF headers.
- Never access GPIO, timers, USB, flash, or peripherals.
- Provide common services used by every firmware layer.

The Core layer consists of:

- Result
- ResultCode
- BuildInfo
- Version
- Scheduler
- SchedulerTask
- Event
- EventBus
- Logger

---

## Consequences

### Advantages

- Platform independent
- Easily unit tested
- Reusable across projects
- Clear dependency hierarchy
- Stable foundation

### Limitations

Core cannot:

- Read hardware
- Write hardware
- Allocate platform resources
- Access operating system services

---

## Dependency Rules

Allowed dependencies:

Core
↓

(None)

Every layer may depend on Core.

Core depends on nothing.

---

## Status

Frozen for Firmware Version 1.x.