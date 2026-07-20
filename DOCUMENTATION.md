# Documentation Index

**Document Version:** 1.0
**Purpose:** Navigation guide for all project documentation
**Status:** Current
**Last Updated:** 2026-07-20
**Project Status:** Architecture reviewed, shared package frozen, Phase 1 Task 2 (CRC16) next

---

## 📋 Current Project Status

- ✅ **Architecture:** Complete and reviewed (frozen v1.0)
- ✅ **Shared Package:** Frozen with protocol contracts
- ✅ **Documentation:** Complete
- 🔄 **Phase 1 MVP:** Protocol → Transport → Core → API → UI
- ⏭️ **Next:** CRC16 implementation in daemon/src/core/protocol/

**For new Builder AI sessions:** Read [docs/PROJECT_STATUS.md](./docs/PROJECT_STATUS.md) first (current status and next steps), then [README.md](./README.md), then other documents below as needed.

---

## 🎯 Start Here

| Role | Document |
|------|----------|
| **New AI session** | [docs/PROJECT_STATUS.md](./docs/PROJECT_STATUS.md) |
| **New to project** | [README.md](./README.md) |
| **Want overview** | [docs/PROJECT_OVERVIEW.md](./docs/PROJECT_OVERVIEW.md) |
| **Need setup help** | [docs/GETTING_STARTED.md](./docs/GETTING_STARTED.md) |
| **Ready to code** | [docs/IMPLEMENTATION_CHECKLIST.md](./docs/IMPLEMENTATION_CHECKLIST.md) |

---

## 📚 All Documents

### Project-Level (docs/)
| Document | Purpose |
|----------|---------|
| [PROJECT_STATUS.md](./docs/PROJECT_STATUS.md) | **Current project status, next steps, and session quick-start** |
| [PROJECT_OVERVIEW.md](./docs/PROJECT_OVERVIEW.md) | System architecture & component overview |
| [GETTING_STARTED.md](./docs/GETTING_STARTED.md) | Development environment setup |
| [IMPLEMENTATION_CHECKLIST.md](./docs/IMPLEMENTATION_CHECKLIST.md) | Phase 1-3 implementation tasks |
| [IMPLEMENTATION_ORDER.md](./docs/IMPLEMENTATION_ORDER.md) | Build order to prevent task jumping |
| [DEVELOPER_GUIDE.md](./docs/DEVELOPER_GUIDE.md) | Workflow, git, PR process |
| [CODING_STANDARDS.md](./docs/CODING_STANDARDS.md) | Naming, formatting, conventions |
| [PROJECT_RULES.md](./docs/PROJECT_RULES.md) | 10 core project principles |
| [ARCHITECTURE_FREEZE.md](./docs/ARCHITECTURE_FREEZE.md) | Frozen v1.0 design |
| [MODULE_BOUNDARIES.md](./docs/MODULE_BOUNDARIES.md) | Module responsibilities |
| [DEPENDENCY_DIAGRAM.md](./docs/DEPENDENCY_DIAGRAM.md) | Component dependencies |
| [BOOT_SHUTDOWN_SEQUENCE.md](./docs/BOOT_SHUTDOWN_SEQUENCE.md) | System startup/shutdown lifecycle |
| [ERROR_RECOVERY_MATRIX.md](./docs/ERROR_RECOVERY_MATRIX.md) | Error handling strategies |
| [SECURITY_POLICY.md](./docs/SECURITY_POLICY.md) | Security guidelines |
| [CONTRIBUTING.md](./docs/CONTRIBUTING.md) | How to contribute |
| [TERMINOLOGY.md](./docs/TERMINOLOGY.md) | Glossary of key terms |
| [DECISIONS.md](./docs/DECISIONS.md) | ADR summary table |
| [NOT_PLANNED.md](./docs/NOT_PLANNED.md) | What's intentionally out of scope |
| [EXTENSION_POINTS.md](./docs/EXTENSION_POINTS.md) | Where future features plug in |

### Architecture Decision Records (adr/)
| ADR | Decision |
|-----|----------|
| [0001-websocket-first.md](./docs/adr/0001-websocket-first.md) | Use WebSocket for real-time commands |
| [0002-library-first-daemon.md](./docs/adr/0002-library-first-daemon.md) | Daemon as pure library first |

### Firmware (firmware/docs/)
- [README.md](./firmware/README.md) - Firmware quick start
- [CHANGELOG.md](./firmware/CHANGELOG.md) - Version history
- Architecture, Hardware, Protocol, CommandSet, LED System, etc.

### Daemon (daemon/docs/)
- [README.md](./daemon/README.md) - Daemon quick start
- (TBD) Detailed daemon documentation

### Frontend (frontend/docs/)
- [WEB_INTERFACE_PLAN.md](./frontend/docs/WEB_INTERFACE_PLAN.md) - Feature roadmap
- [README.md](./frontend/docs/README.md) - Frontend quick start

### Shared (shared/docs/)
- [PROTOCOL_SPEC.md](./shared/docs/PROTOCOL_SPEC.md) - Binary protocol definition
- [PROTOCOL_REGISTRY.md](./shared/docs/PROTOCOL_REGISTRY.md) - Command registry
- [README.md](./shared/docs/README.md) - Shared types index

### Requirements Traceability
- [REQUIREMENTS_MATRIX.md](./docs/REQUIREMENTS_MATRIX.md) - Feature-to-component mapping

### Sequence Diagrams
- [SEQUENCE_DIAGRAMS.md](./docs/SEQUENCE_DIAGRAMS.md) - Key interaction flows

---

## 🔗 Quick Navigation

**Understanding the System:**
- [README.md](./README.md) - Quick overview
- [docs/PROJECT_OVERVIEW.md](./docs/PROJECT_OVERVIEW.md) - Detailed overview
- [DOCUMENTATION.md](./DOCUMENTATION.md) - This file

**Getting Started:**
- [docs/GETTING_STARTED.md](./docs/GETTING_STARTED.md) - Setup help
- [docs/IMPLEMENTATION_CHECKLIST.md](./docs/IMPLEMENTATION_CHECKLIST.md) - What to build

**Development:**
- [docs/DEVELOPER_GUIDE.md](./docs/DEVELOPER_GUIDE.md) - Workflow
- [docs/CODING_STANDARDS.md](./docs/CODING_STANDARDS.md) - Standards
- [docs/CONTRIBUTING.md](./docs/CONTRIBUTING.md) - How to contribute

**Component Guides:**
- [firmware/README.md](./firmware/README.md) - Firmware
- [daemon/README.md](./daemon/README.md) - Daemon
- [frontend/README.md](./frontend/README.md) - Frontend

**Protocol & Specs:**
- [shared/docs/PROTOCOL_SPEC.md](./shared/docs/PROTOCOL_SPEC.md) - Binary protocol
- [shared/docs/PROTOCOL_REGISTRY.md](./shared/docs/PROTOCOL_REGISTRY.md) - Command list
- [docs/SEQUENCE_DIAGRAMS.md](./docs/SEQUENCE_DIAGRAMS.md) - Interaction flows

**Decisions & Scope:**
- [docs/DECISIONS.md](./docs/DECISIONS.md) - Architectural decisions
- [docs/NOT_PLANNED.md](./docs/NOT_PLANNED.md) - Out of scope
- [docs/EXTENSION_POINTS.md](./docs/EXTENSION_POINTS.md) - Future expansions

---

**Last Updated:** 2026-07-20
**Status:** Complete
**Purpose:** Single source of truth for all project documentation
**Session Info:** Updated with new rules and frozen status - July 20, 2026
