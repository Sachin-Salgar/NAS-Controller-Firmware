# NAS Controller - Documentation Index

**This file guides you to all project documentation.**

---

## 📍 Quick Navigation

### Start Here
- **[README.md](./README.md)** - Project overview and quick start

### Development
- **[docs/IMPLEMENTATION_CHECKLIST.md](./docs/IMPLEMENTATION_CHECKLIST.md)** - Phase 1 implementation tasks
- **[firmware/CHANGELOG.md](./firmware/CHANGELOG.md)** - Firmware version history

### Component Documentation

| Component | Key Documents |
|-----------|---|
| **Firmware** | [firmware/README.md](./firmware/README.md) + [firmware/docs/](./firmware/docs/) |
| **Daemon** | [daemon/docs/](./daemon/docs/) (coming soon) |
| **Frontend** | [frontend/docs/WEB_INTERFACE_PLAN.md](./frontend/docs/WEB_INTERFACE_PLAN.md) |
| **Shared (Protocol)** | [shared/docs/PROTOCOL_SPEC.md](./shared/docs/PROTOCOL_SPEC.md) |

---

## 📁 Documentation Structure

```
project root/
├── README.md                      ← START HERE
├── DOCUMENTATION.md               ← You are here
├── LICENSE                        ← License
│
├── docs/                          ← Project-wide documentation
│   ├── DEVELOPER_GUIDE.md         ← Development workflow & standards
│   ├── PROJECT_OVERVIEW.md        ← System overview
│   ├── PROJECT_RULES.md           ← Core project rules
│   ├── GETTING_STARTED.md         ← Setup instructions
│   ├── IMPLEMENTATION_CHECKLIST.md ← Development tasks
│   ├── ARCHITECTURE_FREEZE.md     ← Frozen architecture
│   ├── DEPENDENCY_DIAGRAM.md      ← Dependency rules
│   ├── CODING_STANDARDS.md        ← Coding conventions
│   ├── MODULE_BOUNDARIES.md       ← Module definitions
│   ├── BOOT_SHUTDOWN_SEQUENCE.md  ← System lifecycle
│   ├── ERROR_RECOVERY_MATRIX.md   ← Error handling
│   ├── SECURITY_POLICY.md         ← Security guidelines
│   ├── CONTRIBUTING.md            ← Contribution guidelines
│   ├── adr/                       ← Architecture decision records
│   │   ├── 0001-websocket-first.md
│   │   └── 0002-library-first-daemon.md
│   └── archive/                   ← Archived planning docs
│
├── firmware/                      ← ESP32 Firmware (C++)
│   ├── README.md                  ← Firmware quick start
│   ├── CHANGELOG.md               ← Firmware version history
│   ├── docs/                      ← Firmware-specific documentation
│   │   ├── README.md              ← Firmware docs index
│   │   ├── Architecture.md        ← Firmware architecture
│   │   ├── Hardware.md            ← Hardware specifications
│   │   ├── Protocol.md            ← Protocol details
│   │   ├── CommandSet.md          ← Command reference
│   │   ├── CodingStandard.md      ← Firmware coding standards
│   │   ├── LED_Subsystem.md       ← LED system documentation
│   │   ├── USB-Host-Protocol.md   ← USB protocol spec
│   │   ├── Roadmap.md             ← Firmware development roadmap
│   │   ├── Repository-Structure.md ← Repository layout and layers
│   │   ├── Decisions/             ← Firmware ADRs
│   │   └── ...
│   ├── src/                       ← Source code
│   └── platformio.ini             ← Build config
│
├── daemon/                        ← Host Daemon (Node.js)
│   ├── docs/                      ← Daemon documentation
│   │   └── adr/                   ← (component-specific ADRs if any)
│   └── src/                       ← Source code
│
├── frontend/                      ← Web Interface (React)
│   ├── docs/
│   │   ├── WEB_INTERFACE_PLAN.md  ← Feature roadmap & architecture
│   │   ├── UI_GUIDELINES.md       ← Design system
│   │   └── README.md              ← Frontend docs index
│   └── src/                       ← Source code
│
└── shared/                        ← Shared Types & Protocol
    ├── docs/
    │   ├── PROTOCOL_SPEC.md       ← Binary protocol definition
    │   ├── README.md              ← Shared docs index
    │   └── adr/                   ← (component-specific ADRs if any)
    └── src/                       ← Shared TypeScript types
```

---

## 📚 Documentation by Purpose

### For New Developers
1. Read [README.md](./README.md) (project overview)
2. Choose your component: Firmware, Daemon, or Frontend
3. Read that component's README.md
4. Read the [Implementation Checklist](./docs/IMPLEMENTATION_CHECKLIST.md) to understand what needs to be built

### For Understanding Architecture
- **Overall System:** [README.md](./README.md) - Architecture Overview section
- **Protocol Design:** [shared/docs/PROTOCOL_SPEC.md](./shared/docs/PROTOCOL_SPEC.md)
- **Web Interface Plan:** [frontend/docs/WEB_INTERFACE_PLAN.md](./frontend/docs/WEB_INTERFACE_PLAN.md)
- **Firmware Design:** [firmware/docs/](./firmware/docs/) (see firmware README)

### For Contributing Code
- See [firmware/docs/](./firmware/docs/) for firmware coding standards
- Frontend and Daemon standards TBD

### For Understanding Project Decisions
- **Archived Planning Docs:** [docs/archive/](./docs/archive/) - Previous planning summaries
- **Decision Records:** Look for ADR files in component-specific docs

---

## 🗂️ Document Status

| Document | Location | Status | Purpose |
|----------|----------|--------|---------|
| README.md | Root | ✅ Current | Project overview & quick start |
| DOCUMENTATION.md | Root | ✅ Current | This navigation guide |
| WEB_INTERFACE_PLAN.md | Root | ✅ Current | Web interface development plan (reference) |
| LICENSE | Root | ✅ Current | Project license |
| DEVELOPER_GUIDE.md | docs/ | ✅ Current | Development workflow & standards |
| PROJECT_OVERVIEW.md | docs/ | ✅ Current | System overview & architecture |
| PROJECT_RULES.md | docs/ | ✅ Current | 10 core project rules |
| CODING_STANDARDS.md | docs/ | ✅ Current | Coding conventions |
| ARCHITECTURE_FREEZE.md | docs/ | ✅ Current | Frozen architecture v1.0 |
| DEPENDENCY_DIAGRAM.md | docs/ | ✅ Current | Dependency flow rules |
| MODULE_BOUNDARIES.md | docs/ | ✅ Current | Module definitions |
| BOOT_SHUTDOWN_SEQUENCE.md | docs/ | ✅ Current | System lifecycle |
| ERROR_RECOVERY_MATRIX.md | docs/ | ✅ Current | Error handling rules |
| SECURITY_POLICY.md | docs/ | ✅ Current | Security guidelines |
| GETTING_STARTED.md | docs/ | ✅ Current | Setup instructions |
| CONTRIBUTING.md | docs/ | ✅ Current | Contribution guidelines |
| IMPLEMENTATION_CHECKLIST.md | docs/ | ✅ Current | Phase 1 development tasks |
| docs/adr/0001-websocket-first.md | docs/adr/ | ✅ Current | WebSocket architecture decision |
| docs/adr/0002-library-first-daemon.md | docs/adr/ | ✅ Current | Daemon architecture decision |
| firmware/README.md | firmware/ | ✅ Current | Firmware quick start |
| firmware/CHANGELOG.md | firmware/ | ✅ Current | Firmware version history |
| firmware/docs/Architecture.md | firmware/docs/ | ✅ Current | Firmware architecture |
| firmware/docs/Hardware.md | firmware/docs/ | ✅ Current | Hardware documentation |
| firmware/docs/Protocol.md | firmware/docs/ | ✅ Current | Protocol details |
| firmware/docs/CommandSet.md | firmware/docs/ | ✅ Current | Command reference |
| firmware/docs/CodingStandard.md | firmware/docs/ | ✅ Current | Firmware coding standards |
| firmware/docs/LED_Subsystem.md | firmware/docs/ | ✅ Current | LED system documentation |
| firmware/docs/USB-Host-Protocol.md | firmware/docs/ | ✅ Current | USB protocol specification |
| firmware/docs/Roadmap.md | firmware/docs/ | ✅ Current | Firmware development roadmap |
| firmware/docs/Repository-Structure.md | firmware/docs/ | ✅ Current | Repository layout and layers |
| firmware/docs/Decisions/ | firmware/docs/ | ✅ Current | Firmware ADRs |
| PROTOCOL_SPEC.md | shared/docs/ | ✅ Current | Binary protocol definition |
| WEB_INTERFACE_PLAN.md | frontend/docs/ | ✅ Current | Web UI feature roadmap |
| UI_GUIDELINES.md | frontend/docs/ | ✅ Current | Design system |
| frontend/docs/README.md | frontend/docs/ | ✅ Current | Frontend docs index |
| shared/docs/README.md | shared/docs/ | ✅ Current | Shared types docs |
| daemon/docs/ | daemon/docs/ | ⏳ In Progress | Daemon documentation |

---

## 🗜️ Consolidated Information

### Archived (See docs/archive/)
- **PLANNING_SUMMARY.md** → Content absorbed into WEB_INTERFACE_PLAN.md
- **ORGANIZATION_SUMMARY.md** → Structure now implemented
- **REPOSITORY_SETUP_COMPLETE.md** → Status from July 19, 2026
- **REPOSITORY_STRUCTURE_COMPLETE.md** → Implementation details

These documents are kept for reference but are superseded by current implementations.

---

## 🎯 Key Documents by Role

### Firmware Developer
- [firmware/README.md](./firmware/README.md) - Setup and quick start
- [firmware/docs/](./firmware/docs/) - All firmware documentation
- [shared/docs/PROTOCOL_SPEC.md](./shared/docs/PROTOCOL_SPEC.md) - Protocol to implement

### Daemon Developer
- daemon/docs/ (coming soon)
- [shared/docs/PROTOCOL_SPEC.md](./shared/docs/PROTOCOL_SPEC.md) - Protocol to implement
- [frontend/docs/WEB_INTERFACE_PLAN.md](./frontend/docs/WEB_INTERFACE_PLAN.md) - API specification

### Frontend Developer
- [frontend/docs/WEB_INTERFACE_PLAN.md](./frontend/docs/WEB_INTERFACE_PLAN.md) - Features and architecture
- frontend/docs/README.md (TBD) - Components and patterns
- [shared/docs/PROTOCOL_SPEC.md](./shared/docs/PROTOCOL_SPEC.md) - WebSocket protocol

### Project Manager / Lead
- [README.md](./README.md) - Overall system design
- [docs/IMPLEMENTATION_CHECKLIST.md](./docs/IMPLEMENTATION_CHECKLIST.md) - Development roadmap
- [CHANGELOG.md](./CHANGELOG.md) - Version history

---

## 📖 How to Find Things

**"I want to understand the overall system"**
→ [README.md](./README.md)

**"I want to build the web interface"**
→ [frontend/docs/WEB_INTERFACE_PLAN.md](./frontend/docs/WEB_INTERFACE_PLAN.md)

**"I want to understand the communication protocol"**
→ [shared/docs/PROTOCOL_SPEC.md](./shared/docs/PROTOCOL_SPEC.md)

**"I want to develop the firmware"**
→ [firmware/README.md](./firmware/README.md) and [firmware/docs/](./firmware/docs/)

**"I want to know what to build next"**
→ [docs/IMPLEMENTATION_CHECKLIST.md](./docs/IMPLEMENTATION_CHECKLIST.md)

**"I want to see what's changed"**
→ [firmware/CHANGELOG.md](./firmware/CHANGELOG.md)

---

## ✅ Documentation Status Summary

### Project-Level (docs/)
- ✅ Complete documentation structure established
- ✅ Developer guide, standards, and rules
- ✅ Architecture decisions documented (ADRs)
- ✅ Error recovery and security policies
- ✅ Contribution guidelines

### Firmware (firmware/docs/)
- ✅ Complete firmware documentation
- ✅ Architecture, hardware, and protocol specs
- ✅ Firmware ADRs in Decisions/

### Frontend (frontend/docs/)
- ✅ WEB_INTERFACE_PLAN.md (comprehensive feature roadmap)
- ✅ UI_GUIDELINES.md (design system)
- ✅ README.md (documentation index)

### Shared (shared/docs/)
- ✅ PROTOCOL_SPEC.md (complete protocol definition)
- ✅ README.md (shared types index)

### Daemon (daemon/docs/)
- ⏳ In Progress - Daemon architecture documentation

### Organization
- ✅ ADRs consolidated in docs/adr/ (single source of truth)
- ✅ Firmware decisions moved to firmware/docs/Decisions/
- ✅ No duplicate files across component docs

---

## 🔗 Quick Links

- **Project Root:** [./](../)
- **Firmware:** [./firmware/](./firmware/)
- **Daemon:** [./daemon/](./daemon/)
- **Frontend:** [./frontend/](./frontend/)
- **Shared:** [./shared/](./shared/)
- **Documentation:** [./docs/](./docs/)
- **Archived Docs:** [./docs/archive/](./docs/archive/)

---

**Last Updated:** July 19, 2026 (reorganized documentation structure)
**Status:** ✅ Documentation fully organized and deduplicated
**Purpose:** Consolidate and organize all project documentation by component
