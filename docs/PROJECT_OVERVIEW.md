# NAS Controller: Project Overview

**Status:** Phase 1 Development  
**Date:** July 19, 2026  

---

## What is the NAS Controller?

The NAS Controller is a modular hardware management system consisting of three main components:

1. **ESP32 Firmware** – Embedded hardware control (relays, fans, LEDs, sensors, drives)
2. **Host Daemon** – Orchestration layer (protocol translation, state management, command queue)
3. **Web UI** – Browser-based dashboard (real-time status, controls, diagnostics)

Together, these components provide a complete solution for controlling and monitoring NAS hardware.

---

## System Architecture

```
Web Browser (Chrome, Firefox, Safari)
    ↓ WebSocket
Host Daemon (Node.js + TypeScript)
    ├── NASController.Core (pure library)
    │   ├── Protocol Layer (CRC, ACK, retry)
    │   ├── Command Queue (state machine)
    │   ├── State Store (subscriptions)
    │   └── Event Bus (packet inspector)
    ↓ ITransport abstraction
SerialTransport (USB CDC) / MockTransport (testing)
    ↓ Binary Protocol
ESP32 Firmware
    ├── Relay Control
    ├── Fan Management
    ├── LED Animation
    ├── Temperature Monitoring
    └── Drive Status
```

---

## Repository Structure

```
nas-controller/
├── firmware/           → ESP32 firmware (embedded C++)
├── daemon/             → Host daemon (Node.js + TypeScript)
├── frontend/           → Web UI (React + Vite)
├── shared/             → Shared types and protocol specs
└── docs/               → Project-level documentation
```

**Key Principle:** Documentation is colocated with code.
- `firmware/docs/` → Firmware-specific docs
- `daemon/docs/` → Daemon-specific docs
- `frontend/docs/` → Frontend-specific docs
- `shared/docs/` → Protocol specs and shared contracts
- `docs/` → Project overview, roadmap, guidelines

---

## Key Architectural Decisions

**ADR 0001: WebSocket-First Architecture**
- All real-time commands via persistent WebSocket
- REST reserved for bulk operations (logs, files)
- Location: `shared/docs/adr/0001-websocket-first.md`

**ADR 0002: Library-First Daemon**
- Daemon is a pure TypeScript library (NASController.Core) with optional Express API
- Enables testing without server, reuse in other contexts
- Location: `daemon/docs/adr/0002-library-first-daemon.md`

**Guiding Principle:**
> **Daemon owns all state. Firmware owns all hardware. UI owns only presentation.**

---

## Development Phases

### Phase 1: MVP (In Progress)
- Dashboard (real-time status)
- Control panel (relays, fans, LEDs)
- Test console (raw commands)
- Live packet inspector

### Phase 2: Full Features
- Configuration editor
- LED animation builder
- Statistics & logging
- Configuration profiles

### Phase 3: Polish & Deploy
- Installers (.exe, .dmg, .deb)
- Documentation
- Docker container (optional)

---

## Getting Started

1. **Read the docs** (start with `docs/GETTING_STARTED.md`)
2. **Understand the protocol** (`shared/docs/PROTOCOL_SPEC.md`)
3. **Review architecture decisions** (ADRs in respective component docs)
4. **Set up development environment** (`scripts/setup.sh`)
5. **Begin Phase 1 implementation** (see `frontend/docs/WEB_INTERFACE_PLAN.md`)

---

## Documentation Index

- **Project Level**
  - `docs/PROJECT_OVERVIEW.md` (this file)
  - `docs/ROADMAP.md` (timeline and milestones)
  - `docs/CONTRIBUTING.md` (development guidelines)
  - `docs/CHANGELOG.md` (version history)

- **Firmware** (`firmware/docs/`)
  - `ARCHITECTURE.md` (firmware design)
  - `COMMAND_SET.md` (hardware commands)
  - `LED_SYSTEM.md` (LED control system)
  - `TASKS.md` (firmware tasks and scheduling)

- **Daemon** (`daemon/docs/`)
  - `ARCHITECTURE.md` (daemon design)
  - `WEBSOCKET_API.md` (WebSocket protocol)
  - `STATE_STORE.md` (state management)
  - `TRANSPORT_LAYER.md` (transport abstraction)
  - `adr/0002-library-first-daemon.md` (architectural decision)

- **Frontend** (`frontend/docs/`)
  - `WEB_INTERFACE_PLAN.md` (feature roadmap)
  - `UI_GUIDELINES.md` (design system)
  - `COMPONENTS.md` (component catalog)
  - `WEBSOCKET_CLIENT.md` (Socket.io integration)

- **Shared** (`shared/docs/`)
  - `PROTOCOL_SPEC.md` (binary protocol definition)
  - `COMMAND_SET.md` (command reference)
  - `DATA_TYPES.md` (shared type definitions)
  - `adr/0001-websocket-first.md` (architectural decision)

---

## For Different Roles

**Firmware Developer:** Start with `firmware/docs/ARCHITECTURE.md` and `shared/docs/PROTOCOL_SPEC.md`

**Daemon Developer:** Start with `daemon/docs/ARCHITECTURE.md` and `daemon/docs/adr/0002-library-first-daemon.md`

**Frontend Developer:** Start with `frontend/docs/WEB_INTERFACE_PLAN.md` and `daemon/docs/WEBSOCKET_API.md`

**Project Manager:** Start with `docs/ROADMAP.md` and `docs/PROJECT_OVERVIEW.md`

---

**Document Created:** July 19, 2026  
**Last Updated:** July 19, 2026  
**Status:** Active development
