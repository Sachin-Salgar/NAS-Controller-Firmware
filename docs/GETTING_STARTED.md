# Getting Started with NAS Controller Development

Welcome to the NAS Controller project! This guide will help you understand the system and set up your development environment.

---

## Quick Start (3 minutes)

1. **Understand the system:** Read `docs/PROJECT_OVERVIEW.md`
2. **Review architecture decisions:** Check the ADRs in relevant component docs
3. **Set up environment:** Run `scripts/setup.sh`
4. **Pick a component:** firmware, daemon, or frontend?

---

## The NAS Controller System

The NAS Controller consists of three main components that work together:

```
Web Browser
    ↓ WebSocket (JSON)
Host Daemon (Node.js)
    ├── NASController.Core (pure library)
    └── Express/Socket.io API
    ↓ Serial (USB CDC, binary protocol)
ESP32 Firmware
    ├── Relay Control
    ├── Fan Management
    ├── LED Animation
    ├── Temperature Sensors
    └── Drive Monitoring
```

**Key Insight:** These layers are independent. You can develop the daemon without ESP32 hardware (use MockTransport). You can develop the frontend without the daemon running (mock Socket.io).

---

## Which Component Are You Working On?

### Firmware (ESP32 Embedded C++)

**Getting started:**
1. Read `firmware/docs/ARCHITECTURE.md`
2. Review `shared/docs/PROTOCOL_SPEC.md` (understand the command protocol)
3. Check `firmware/docs/COMMAND_SET.md`
4. Set up PlatformIO and compile

**Key principle:** Firmware owns all hardware. No UI logic in firmware.

**Testing:** Requires real hardware. Use the test console from the web UI to verify commands.

---

### Daemon (Node.js + TypeScript)

**Getting started:**
1. Read `daemon/docs/adr/0002-library-first-daemon.md` (understand the design)
2. Read `daemon/docs/ARCHITECTURE.md`
3. Review `daemon/docs/WEBSOCKET_API.md`
4. Install dependencies: `cd daemon && npm install`
5. Run tests: `npm test` (no hardware required)

**Key principle:** Daemon is a pure library (NASController.Core) with optional Express API.

**Advantages:**
- Test entire daemon logic without running server
- Use MockTransport for unit tests (no USB required)
- Reuse Core library in other contexts (CLI, Electron, etc.)

**Testing:** 
```bash
npm test                    # Unit tests (MockTransport)
npm test -- --watch       # Watch mode
npm run test:hardware     # Integration tests (requires real ESP32)
```

---

### Frontend (React + Vite)

**Getting started:**
1. Read `frontend/docs/WEB_INTERFACE_PLAN.md` (feature roadmap)
2. Read `daemon/docs/WEBSOCKET_API.md` (understand daemon communication)
3. Install dependencies: `cd frontend && npm install`
4. Start dev server: `npm run dev`
5. Open `http://localhost:5173`

**Key principle:** UI owns only presentation. All state comes from daemon via WebSocket.

**Dependencies:**
- React (components)
- Socket.io client (real-time updates)
- Tailwind CSS (styling)
- Vite (fast bundling)

**Testing:** Manual in browser (dev server is live-reloading)

---

## Shared Types (Both Daemon & Frontend)

The `shared/` package contains TypeScript types used by both daemon and frontend:

- `protocol.ts` – Packet structure and types
- `commands.ts` – Command definitions
- `events.ts` – Event types
- `capabilities.ts` – Firmware capability schema
- `config.ts` – Configuration schema

**When to update:** If you add a new command to the protocol, update both:
1. `shared/src/commands.ts`
2. `shared/docs/PROTOCOL_SPEC.md`

---

## Development Workflow

### For Any Component

1. **Write code** in the component (firmware/src/, daemon/src/, frontend/src/)
2. **Write tests** (if applicable)
3. **Run tests** (Jest for daemon, manual for frontend)
4. **Update docs** (docs colocated with code)
5. **Commit** with conventional commit messages
6. **Create PR** with description

### Testing Strategy

**Daemon:** Use MockTransport (no hardware required)
```bash
cd daemon
npm test
```

**Frontend:** Manual testing in browser
```bash
cd frontend
npm run dev  # Dev server at localhost:5173
```

**Integration:** Test daemon + mock device
```bash
cd daemon
npm test -- integration.test.ts
```

**Hardware:** Test against real ESP32
```bash
cd daemon
npm run test:hardware
```

---

## Common Tasks

### Add a New Relay Command

1. **Define in protocol:** Update `shared/docs/PROTOCOL_SPEC.md` (new command code)
2. **Add type:** Update `shared/src/commands.ts` (new command type)
3. **Implement in daemon:** Add handler in `daemon/src/core/controllers/relay.ts`
4. **Test in daemon:** Add unit test in `daemon/tests/relay.test.ts`
5. **Add to API:** Connect to Socket.io in `daemon/src/api/websocket.ts`
6. **Implement in frontend:** Create component in `frontend/src/components/`
7. **Test in frontend:** Manual testing via dev server
8. **Document:** Update `daemon/docs/ARCHITECTURE.md` if design changes

### Add a New WebSocket Event

1. **Define event:** Update `shared/src/events.ts` (new event type)
2. **Emit from daemon:** Update `daemon/src/core/events/` to broadcast new event
3. **Subscribe in frontend:** Add listener in `frontend/src/services/websocket.ts`
4. **Display in UI:** Create component to show the event

### Create a New Feature

1. **Design:** Write ADR explaining the approach (`daemon/docs/adr/0XXX-feature-name.md`)
2. **Protocol:** If firmware changes needed, update `shared/docs/PROTOCOL_SPEC.md`
3. **Types:** Add types to `shared/src/`
4. **Daemon:** Implement in daemon core (library-first)
5. **API:** Connect daemon core to Socket.io (thin bridge)
6. **Frontend:** Create UI components
7. **Test:** Unit tests for daemon, manual tests for frontend
8. **Document:** Update component docs and ADR

---

## Documentation Organization

```
nas-controller/
├── firmware/docs/         → Firmware-specific docs
├── daemon/docs/           → Daemon-specific docs
│   └── adr/              → Daemon architecture decisions
├── frontend/docs/         → Frontend-specific docs
├── shared/docs/           → Protocol specs and shared types
│   └── adr/              → System-wide architecture decisions
└── docs/                  → Project overview, roadmap, contributing
    ├── PROJECT_OVERVIEW.md
    ├── ROADMAP.md
    ├── CONTRIBUTING.md
    └── GETTING_STARTED.md (this file)
```

**Rule:** Find a document by looking in the component that it describes.

---

## Guiding Principles

Keep these in mind throughout development:

1. **Daemon owns all state** – Everything about the current system is stored in daemon
2. **Firmware owns all hardware** – Hardware control is only firmware's job
3. **UI owns only presentation** – Frontend is stateless, driven by daemon
4. **Test independently** – Each layer is tested without the next layer
5. **Document decisions** – ADRs explain why, not just what
6. **Colocate docs** – Documentation lives with the code it describes

---

## Getting Help

### Read the docs in this order

1. **System overview:** `docs/PROJECT_OVERVIEW.md`
2. **Your component:** `[component]/docs/ARCHITECTURE.md`
3. **How things work together:** `daemon/docs/WEBSOCKET_API.md` (for frontend devs)
4. **Why decisions were made:** ADRs in relevant component docs
5. **Detailed specifications:** `shared/docs/PROTOCOL_SPEC.md`

### Still have questions?

- Check `docs/CONTRIBUTING.md` for development guidelines
- Review existing code and tests
- Open a GitHub Issue or Discussion

---

## Next Steps

1. **Clone the repo** and run `scripts/setup.sh`
2. **Choose a component** (firmware, daemon, or frontend)
3. **Read the component docs** (ARCHITECTURE.md in that component)
4. **Check out Phase 1 features** in `frontend/docs/WEB_INTERFACE_PLAN.md`
5. **Start coding** and remember the guiding principles

**Welcome aboard!** We're building something good here. Let's make it great.
