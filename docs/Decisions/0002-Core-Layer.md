# ADR-0002: Core Firmware Foundation

Status: Accepted

Date: 2026-07-17

Decision ID: ADR-0002

---

# Context

The firmware required a hardware-independent foundation shared by every layer.

Application logic, hardware abstraction, and services must never duplicate common functionality.

A stable foundation is required to keep the dependency tree clear and to avoid repeated implementations of common services.

---

# Decision

The Core layer shall contain:

- Result
- ResultCode
- BuildInfo
- Version
- Event
- EventBus
- Logger

The Core layer SHALL:

- Be hardware independent.
- Contain no Arduino code.
- Contain no ESP-IDF code.
- Contain no protocol implementation.
- Contain no drivers.
- Contain no services.
- Contain no application logic.

Every higher firmware layer depends on Core.

Core depends only on standard C++.

---

# Consequences

## Benefits

- Clean dependency tree.
- Reusable components.
- Easier testing.
- Hardware independence.
- Stable project foundation.

## Trade-offs

- Slightly larger abstraction layer.
- More files.

---

# Scope

This decision applies to all firmware versions 1.x.

This decision is frozen.

---

# Approval

Status: Accepted

This decision establishes the Core firmware foundation for NAS Controller Firmware Version 1.x.
