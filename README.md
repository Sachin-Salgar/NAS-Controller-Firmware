# NAS Controller

**Document Version:** 1.0
**Project Version:** 1.0
**Status:** Active Development
**Last Updated:** 2026-07-19

A complete ecosystem for controlling and monitoring external NAS hardware via an ESP32 microcontroller with a modern web interface.

## What is This?

Three components working together:
1. **ESP32 Firmware** – Embedded hardware control
2. **Host Daemon** – Protocol translation & state management
3. **Web Dashboard** – Browser-based interface

## Quick Links

- **Getting Started:** `docs/GETTING_STARTED.md`
- **System Architecture:** `docs/PROJECT_OVERVIEW.md`
- **Documentation Index:** `DOCUMENTATION.md`
- **Protocol Spec:** `shared/docs/PROTOCOL_SPEC.md`

## Repository Structure

```
├── firmware/          → ESP32 firmware (C++17)
├── daemon/            → Host daemon (Node.js + TypeScript)
├── frontend/          → Web UI (React + Vite)
├── shared/            → Shared types & protocol specs
└── docs/              → Project-level documentation
```

## For Different Roles

| Role | Start Here |
|------|------------|
| **Firmware Developer** | `firmware/README.md` + `shared/docs/PROTOCOL_SPEC.md` |
| **Daemon Developer** | `daemon/README.md` + `daemon/docs/adr/0002-library-first-daemon.md` |
| **Frontend Developer** | `frontend/docs/WEB_INTERFACE_PLAN.md` + `daemon/docs/WEBSOCKET_API.md` |
| **Project Manager** | `docs/PROJECT_OVERVIEW.md` + `docs/IMPLEMENTATION_CHECKLIST.md` |

## Technology Stack

| Component | Tech |
|-----------|------|
| **Firmware** | C++17, PlatformIO, Arduino |
| **Daemon** | Node.js, TypeScript, Express, Socket.io |
| **Frontend** | React, Vite, TailwindCSS |

## Current Status

- ✅ Architecture & planning complete
- ✅ Documentation organized by component
- 🔄 Phase 1 development (Protocol → Transport → Core → API → UI)

## License & Contributing

See `LICENSE` for license details.
See `docs/CONTRIBUTING.md` for contribution guidelines.
