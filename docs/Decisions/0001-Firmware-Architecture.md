# ADR-0001: Firmware Architecture Baseline

Status: Accepted

Date: 2026-07-17

Decision ID: ADR-0001

---

# Context

The NAS Controller Firmware project has completed its architecture and design phase.

The following foundation documents have been reviewed and frozen for Firmware Version 1.x:

- Architecture.md
- Protocol.md
- Hardware.md
- CodingStandard.md
- CommandSet.md

Beginning firmware implementation without freezing the architecture would increase the risk of inconsistent module design, changing interfaces, circular dependencies, and unnecessary refactoring.

A stable architectural baseline is therefore required before implementation begins.

---

# Decision

The project architecture is hereby declared frozen for Firmware Version 1.x.

The following principles are mandatory:

- The layered architecture defined in Architecture.md SHALL be followed.
- Protocol.md defines the external communication protocol.
- Hardware.md defines the supported hardware platform.
- CodingStandard.md defines mandatory coding practices.
- CommandSet.md defines the supported firmware command set.
- All new source code SHALL comply with these documents.

Future architectural changes SHALL NOT be made directly by modifying the documentation.

Instead, any architectural change requires a new Architecture Decision Record (ADR).

---

# Consequences

Positive:

- Stable foundation for firmware development.
- Clear separation of responsibilities.
- Predictable module dependencies.
- Reduced design churn.
- Easier code reviews.
- Better long-term maintainability.

Trade-offs:

- Architectural changes require additional review.
- Major structural changes become more deliberate.

These trade-offs are accepted to improve long-term project quality.

---

# Scope

This ADR applies to:

- Firmware Version 1.x
- All source code
- All future modules
- All contributors

---

# Related Documents

- Architecture.md
- Protocol.md
- Hardware.md
- CodingStandard.md
- CommandSet.md

---

# Future Changes

If an architectural change becomes necessary, create a new ADR.

Examples:

- ADR-0002: EventBus Redesign
- ADR-0003: Scheduler Improvements
- ADR-0004: Network Service Introduction
- ADR-0005: Custom PCB Support

Existing ADRs SHALL NOT be modified after acceptance.

New decisions supersede previous decisions while preserving project history.

---

# Approval

Status: Accepted

This Architecture Decision Record establishes the official architectural baseline for NAS Controller Firmware Version 1.x.