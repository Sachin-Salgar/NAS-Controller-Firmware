# NAS Controller Project Documentation

Welcome! This is the master index for all project documentation. Start here to navigate the entire documentation system.

---

## 🚀 Quick Start for Contributors

**New to the project?** Read these in order:

1. **[DEVELOPER_GUIDE.md](DEVELOPER_GUIDE.md)** ← **START HERE**
   - Project rules and guiding principles
   - Coding standards and conventions
   - Development workflow
   - How to contribute

2. **[PROJECT_OVERVIEW.md](PROJECT_OVERVIEW.md)**
   - What is NAS Controller?
   - System architecture at a glance
   - Technology stack
   - Project timeline

3. **[GETTING_STARTED.md](GETTING_STARTED.md)**
   - Setup development environment
   - Build and run firmware
   - Build and run daemon
   - Build and run frontend
   - Run tests

---

## 📚 Architecture & Design

### Overall System

- **[ARCHITECTURE_OVERVIEW.md](ARCHITECTURE_OVERVIEW.md)** - High-level system design
  - Three-tier architecture (Firmware → Daemon → UI)
  - Data flow diagrams
  - Communication protocols
  - Dependency model

- **[ARCHITECTURE_FREEZE.md](ARCHITECTURE_FREEZE.md)** - Frozen architecture v1.0
  - What can change (bug fixes, clarifications)
  - What cannot change (features, technologies, structure)
  - Preventing architecture drift

- **[DEPENDENCY_DIAGRAM.md](DEPENDENCY_DIAGRAM.md)** - Dependency flow rules
  - UI → Daemon → Transport → Firmware
  - Allowed vs forbidden dependencies
  - Circular dependency prevention

### Project Rules & Standards

- **[PROJECT_RULES.md](PROJECT_RULES.md)** - 10 core project rules
  - Firmware ≠ UI
  - Daemon owns state
  - Everything is logged
  - No global mutable state
  - Every module independently testable
  - ...and more

- **[CODING_STANDARDS.md](CODING_STANDARDS.md)** - Coding conventions
  - Naming conventions (variables, functions, classes, enums)
  - File and folder naming
  - Logging style and levels
  - Error handling patterns
  - Comments and documentation
  - Commit message format

- **[MODULE_BOUNDARIES.md](MODULE_BOUNDARIES.md)** - Module definitions
  - For each major module: responsibilities, inputs, outputs, dependencies
  - Public API vs internal API
  - Keeping modules focused and testable

---

## 🛠️ Development Guidelines

- **[TESTING_STRATEGY.md](TESTING_STRATEGY.md)** - Testing pyramid and approach
  - Unit tests (protocol, transport, core)
  - Integration tests (daemon + mock device)
  - UI component tests
  - Real hardware tests
  - Regression test suite

- **[ERROR_RECOVERY_MATRIX.md](ERROR_RECOVERY_MATRIX.md)** - Error handling rules
  - Failure scenarios (serial disconnect, CRC error, timeout, etc.)
  - Retry policies and backoff
  - User messages and recovery actions
  - Consistent error handling everywhere

- **[SECURITY_POLICY.md](SECURITY_POLICY.md)** - Security guidelines
  - Maximum packet sizes
  - Input validation rules
  - Rate limiting
  - Configuration validation
  - Path sanitization
  - Firmware verification

- **[BOOT_SHUTDOWN_SEQUENCE.md](BOOT_SHUTDOWN_SEQUENCE.md)** - System lifecycle
  - Daemon startup sequence (config → serial → ping → capabilities → status)
  - Shutdown sequence (queue drain → close serial → exit)
  - Browser connection lifecycle

---

## 📦 Release & Deployment

- **[BUILD_RELEASE_PROCESS.md](BUILD_RELEASE_PROCESS.md)** - Release workflow
  - Tagging versions
  - Building artifacts (firmware, daemon, frontend)
  - Packaging installers
  - Publishing releases
  - Generating release notes

- **[CONTRIBUTING.md](CONTRIBUTING.md)** - Contribution guidelines
  - How to submit changes
  - Pull request process
  - Code review criteria
  - Testing requirements

- **[CHANGELOG.md](CHANGELOG.md)** - Project changelog
  - Version history
  - Breaking changes
  - New features and fixes

---

## 🎯 Component Documentation

Each component has its own documentation folder with detailed guides:

### Firmware (ESP32)
**Location:** `firmware/docs/`

- **[firmware/docs/README.md](../firmware/docs/README.md)** - Firmware overview
- `firmware/docs/ARCHITECTURE.md` - Firmware architecture
- `firmware/docs/COMMAND_SET.md` - Command reference
- `firmware/docs/CODING_STANDARD.md` - Firmware coding standards
- `firmware/docs/HAL.md` - Hardware Abstraction Layer
- `firmware/docs/ADR/` - Architecture decision records

### Daemon (Node.js + TypeScript)
**Location:** `daemon/docs/`

- **[daemon/docs/README.md](../daemon/docs/README.md)** - Daemon overview
- `daemon/docs/ARCHITECTURE.md` - Daemon design
- `daemon/docs/WEBSOCKET_API.md` - WebSocket protocol
- `daemon/docs/STATE_STORE.md` - State management
- `daemon/docs/TRANSPORT_LAYER.md` - Transport abstraction
- `daemon/docs/adr/` - Architecture decision records

### Frontend (React + Vite)
**Location:** `frontend/docs/`

- **[frontend/docs/README.md](../frontend/docs/README.md)** - Frontend overview
- `frontend/docs/WEB_INTERFACE_PLAN.md` - Feature roadmap
- `frontend/docs/UI_GUIDELINES.md` - Design system
- `frontend/docs/COMPONENTS.md` - Component catalog
- `frontend/docs/WEBSOCKET_CLIENT.md` - Client integration

### Shared Types & Protocol
**Location:** `shared/docs/`

- **[shared/docs/README.md](../shared/docs/README.md)** - Shared overview
- `shared/docs/PROTOCOL_SPEC.md` - Binary protocol specification
- `shared/docs/COMMAND_SET.md` - Command reference
- `shared/docs/DATA_TYPES.md` - TypeScript type definitions
- `shared/docs/adr/` - Architecture decision records

---

## 📋 Documentation Map

```
docs/
├── README.md                          ← YOU ARE HERE
├── DEVELOPER_GUIDE.md                 ← START HERE
├── PROJECT_OVERVIEW.md
├── PROJECT_RULES.md
├── ARCHITECTURE_OVERVIEW.md
├── ARCHITECTURE_FREEZE.md
├── CODING_STANDARDS.md
├── MODULE_BOUNDARIES.md
├── DEPENDENCY_DIAGRAM.md
├── BOOT_SHUTDOWN_SEQUENCE.md
├── ERROR_RECOVERY_MATRIX.md
├── SECURITY_POLICY.md
├── TESTING_STRATEGY.md
├── BUILD_RELEASE_PROCESS.md
├── GETTING_STARTED.md
├── CONTRIBUTING.md
├── CHANGELOG.md
│
├── web/
│   └── (linked from frontend/docs/)
│
├── shared/
│   └── (linked from shared/docs/)
│
└── firmware/
    └── (linked from firmware/docs/)

firmware/docs/
├── README.md
├── ARCHITECTURE.md
├── COMMAND_SET.md
├── CODING_STANDARD.md
├── HAL.md
├── ADR/
│   ├── 0001-platform-hal.md
│   └── ...

daemon/docs/
├── README.md
├── ARCHITECTURE.md
├── WEBSOCKET_API.md
├── STATE_STORE.md
├── TRANSPORT_LAYER.md
├── adr/
│   ├── 0001-library-first-daemon.md
│   └── ...

frontend/docs/
├── README.md
├── WEB_INTERFACE_PLAN.md
├── UI_GUIDELINES.md
├── COMPONENTS.md
└── WEBSOCKET_CLIENT.md

shared/docs/
├── README.md
├── PROTOCOL_SPEC.md
├── COMMAND_SET.md
├── DATA_TYPES.md
└── adr/
    ├── 0001-websocket-first.md
    └── ...
```

---

## 🔍 Finding What You Need

**I want to understand...**
- The overall system → [ARCHITECTURE_OVERVIEW.md](ARCHITECTURE_OVERVIEW.md)
- Firmware specifically → [firmware/docs/README.md](../firmware/docs/README.md)
- How the daemon works → [daemon/docs/README.md](../daemon/docs/README.md)
- Frontend structure → [frontend/docs/README.md](../frontend/docs/README.md)
- How to write code → [DEVELOPER_GUIDE.md](DEVELOPER_GUIDE.md)
- How to test → [TESTING_STRATEGY.md](TESTING_STRATEGY.md)
- What we've decided → Project rules and ADRs in component `docs/adr/` folders

**I'm about to...**
- Write a new feature → [DEVELOPER_GUIDE.md](DEVELOPER_GUIDE.md) then [CODING_STANDARDS.md](CODING_STANDARDS.md)
- Fix a bug → [ERROR_RECOVERY_MATRIX.md](ERROR_RECOVERY_MATRIX.md)
- Add a command → [shared/docs/PROTOCOL_SPEC.md](../shared/docs/PROTOCOL_SPEC.md)
- Deploy to production → [BUILD_RELEASE_PROCESS.md](BUILD_RELEASE_PROCESS.md)
- Design a new page → [frontend/docs/UI_GUIDELINES.md](../frontend/docs/UI_GUIDELINES.md)

---

## 📞 Getting Help

- **Architecture question?** → Check relevant ADR in `component/docs/adr/`
- **Coding question?** → [CODING_STANDARDS.md](CODING_STANDARDS.md)
- **How does X work?** → [ARCHITECTURE_OVERVIEW.md](ARCHITECTURE_OVERVIEW.md)
- **Found a problem?** → Read [ERROR_RECOVERY_MATRIX.md](ERROR_RECOVERY_MATRIX.md)

---

## 📁 Repository Structure

```
nas-controller/
├── firmware/              # ESP32 firmware (PlatformIO)
├── daemon/                # Node.js backend + Express + WebSocket
├── frontend/              # React + Vite web UI
├── shared/                # TypeScript types (daemon + frontend)
├── docs/                  # Project-level documentation (this folder)
├── tools/                 # Build scripts and utilities
├── scripts/               # Deployment and CI/CD
├── LICENSE
└── README.md              # Project root entry point
```

---

## ✅ Last Updated

**Document Status:** ✅ **READY FOR PHASE 1 IMPLEMENTATION**

**Structure Created:** July 19, 2026

All project-level documentation has been organized and indexed. Each component (firmware, daemon, frontend, shared) maintains its own documentation folder.

---

**Ready to contribute?** Start with [DEVELOPER_GUIDE.md](DEVELOPER_GUIDE.md)
