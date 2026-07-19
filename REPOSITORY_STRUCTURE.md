# NAS Controller Repository Structure

**Status:** Repository reorganization in progress

## Target Structure (Post-Reorganization)

```
nas-controller/
│
├── firmware/                          # ESP32 firmware (PlatformIO)
│   ├── src/                           # Firmware source files
│   ├── include/                       # Firmware headers
│   ├── lib/                           # Firmware libraries
│   ├── test/                          # Firmware tests
│   ├── docs/                          # Firmware documentation
│   │   ├── ARCHITECTURE.md
│   │   ├── COMMAND_SET.md
│   │   ├── CODING_STANDARD.md
│   │   ├── LED_SYSTEM.md
│   │   ├── TASKS.md
│   │   ├── HAL.md
│   │   ├── ADR/
│   │   │   ├── 0001-platform-hal.md
│   │   │   ├── 0002-drivers-layer.md
│   │   │   ├── 0003-protocol-layer.md
│   │   │   └── ...
│   │   ├── LED_AUDIT_REPORT.md
│   │   ├── LED_REDESIGN_COMPLETE.md
│   │   └── IMPLEMENTATION_CHECKLIST.md
│   └── platformio.ini
│
├── daemon/                            # Node.js + TypeScript daemon
│   ├── src/
│   │   ├── core/                      # NASController.Core library
│   │   ├── api/                       # Express + Socket.io
│   │   └── index.ts
│   ├── tests/
│   ├── docs/
│   │   ├── ARCHITECTURE.md
│   │   ├── WEBSOCKET_API.md
│   │   ├── STATE_STORE.md
│   │   ├── TRANSPORT_LAYER.md
│   │   └── adr/
│   │       ├── 0001-library-first-daemon.md
│   │       └── ...
│   └── package.json
│
├── frontend/                          # React + Vite web UI
│   ├── src/
│   │   ├── pages/
│   │   ├── components/
│   │   ├── services/
│   │   └── hooks/
│   ├── public/
│   ├── docs/
│   │   ├── WEB_INTERFACE_PLAN.md
│   │   ├── UI_GUIDELINES.md
│   │   ├── COMPONENTS.md
│   │   └── WEBSOCKET_CLIENT.md
│   └── package.json
│
├── shared/                            # TypeScript types (daemon + frontend)
│   ├── src/
│   │   ├── protocol.ts
│   │   ├── commands.ts
│   │   ├── events.ts
│   │   ├── capabilities.ts
│   │   └── config.ts
│   ├── docs/
│   │   ├── PROTOCOL_SPEC.md
│   │   ├── COMMAND_SET.md
│   │   ├── DATA_TYPES.md
│   │   └── adr/
│   │       ├── 0001-websocket-first.md
│   │       └── ...
│   └── package.json
│
├── docs/                              # Project-level documentation
│   ├── README.md                      # Documentation index & navigation
│   ├── DEVELOPER_GUIDE.md             # Entry point for contributors
│   ├── PROJECT_OVERVIEW.md
│   ├── PROJECT_RULES.md
│   ├── ARCHITECTURE_OVERVIEW.md
│   ├── ARCHITECTURE_FREEZE.md
│   ├── CODING_STANDARDS.md
│   ├── MODULE_BOUNDARIES.md
│   ├── DEPENDENCY_DIAGRAM.md
│   ├── BOOT_SHUTDOWN_SEQUENCE.md
│   ├── ERROR_RECOVERY_MATRIX.md
│   ├── SECURITY_POLICY.md
│   ├── TESTING_STRATEGY.md
│   ├── BUILD_RELEASE_PROCESS.md
│   ├── GETTING_STARTED.md
│   ├── CONTRIBUTING.md
│   ├── CHANGELOG.md
│   │
│   ├── web/                           # Web interface documentation
│   │   └── WEB_INTERFACE_PLAN.md      # (linked from frontend/docs/)
│   │
│   ├── shared/                        # Shared documentation
│   │   ├── PROTOCOL_SPEC.md           # (linked from shared/docs/)
│   │   └── ...
│   │
│   └── firmware/                      # Firmware documentation
│       └── (linked from firmware/docs/)
│
├── tools/                             # Build scripts, utilities
├── scripts/                           # Deployment, CI/CD scripts
│
├── .gitignore
├── .vscode/                           # VSCode settings
├── LICENSE
├── README.md                          # Project root entry point
├── PLANNING_SUMMARY.md                # Project history (archive)
├── STATUS_REPORT.md                   # Project status (archive)
├── CHANGELOG.md                       # Project changelog (will move to docs/)
└── .gitattributes
```

## File Migration Plan

### Root Level (Move to docs/)
- `PLANNING_SUMMARY.md` → `docs/PLANNING_SUMMARY.md` (archive)
- `STATUS_REPORT.md` → `docs/STATUS_REPORT.md` (archive)
- `CHANGELOG.md` → `docs/CHANGELOG.md`

### Firmware Documentation (Move to firmware/docs/)
- `firmware/docs/ADR/` - Already created ✓
- `LED_AUDIT_REPORT.md` → `firmware/docs/LED_AUDIT_REPORT.md`
- `LED_REDESIGN_COMPLETE.md` → `firmware/docs/LED_REDESIGN_COMPLETE.md`
- `IMPLEMENTATION_CHECKLIST.md` → `firmware/docs/IMPLEMENTATION_CHECKLIST.md`

### Firmware Source Files (Move to firmware/)
- `src/*` → `firmware/src/`
- `include/*` → `firmware/include/`
- `lib/*` → `firmware/lib/`
- `test/*` → `firmware/test/`
- `platformio.ini` → `firmware/platformio.ini`

### Shared Documentation (Link from docs/shared/)
- `PROTOCOL_SPEC.md` → `shared/docs/PROTOCOL_SPEC.md`

### Web Documentation (Link from docs/web/)
- `WEB_INTERFACE_PLAN.md` → `frontend/docs/WEB_INTERFACE_PLAN.md`

### New Project-Level Docs (Create in docs/)
These are the 12 foundation documents before coding:
- `docs/DEVELOPER_GUIDE.md` - Entry point for all contributors
- `docs/PROJECT_RULES.md` - 10 core project rules
- `docs/ARCHITECTURE_FREEZE.md` - Freeze policy
- `docs/CODING_STANDARDS.md` - Naming, formatting, standards
- `docs/MODULE_BOUNDARIES.md` - Module responsibilities
- `docs/DEPENDENCY_DIAGRAM.md` - Dependency direction enforcement
- `docs/BOOT_SHUTDOWN_SEQUENCE.md` - System startup/shutdown
- `docs/ERROR_RECOVERY_MATRIX.md` - Error handling rules
- `docs/SECURITY_POLICY.md` - Security guidelines
- `docs/TESTING_STRATEGY.md` - Testing pyramid
- `docs/BUILD_RELEASE_PROCESS.md` - Release procedure
- `docs/README.md` - Documentation index (navigation hub)

## Directory-Specific Rules

### firmware/
- **Owns:** PlatformIO firmware, ESP32 code, HAL
- **Documents:** ARCHITECTURE.md, COMMAND_SET.md, ADRs
- **Does NOT contain:** Daemon code, React code, protocol definitions (those are in shared/)

### daemon/
- **Owns:** Node.js/TypeScript backend, Express server, WebSocket
- **Documents:** ARCHITECTURE.md, WEBSOCKET_API.md, TRANSPORT_LAYER.md, ADRs
- **Does NOT contain:** Firmware code, frontend code

### frontend/
- **Owns:** React components, UI pages, browser code
- **Documents:** WEB_INTERFACE_PLAN.md, UI_GUIDELINES.md, COMPONENTS.md
- **Does NOT contain:** Firmware code, daemon code

### shared/
- **Owns:** TypeScript type definitions, protocol specs, command definitions
- **Documents:** PROTOCOL_SPEC.md, COMMAND_SET.md, DATA_TYPES.md
- **Used by:** Both daemon and frontend (no duplication)

### docs/
- **Owns:** Project-wide guidelines, standards, architecture
- **Contains:** Developer guide, coding standards, testing strategy, security policy
- **Does NOT contain:** Component-specific docs (those are in component folders)

## Navigation

**Entry Points:**
1. **For developers:** `docs/DEVELOPER_GUIDE.md` → Start here
2. **For contributors:** `docs/CONTRIBUTING.md`
3. **For project overview:** `docs/PROJECT_OVERVIEW.md`
4. **For all documentation:** `docs/README.md` (index)

**For Each Component:**
1. `firmware/docs/README.md` - Firmware-specific docs
2. `daemon/docs/README.md` - Daemon-specific docs
3. `frontend/docs/README.md` - Frontend-specific docs
4. `shared/docs/README.md` - Shared protocol docs

## Implementation Status

- [ ] Move firmware source files (src/, include/, lib/, test/)
- [ ] Move firmware config (platformio.ini)
- [ ] Create firmware/docs/README.md
- [ ] Move firmware-specific documents to firmware/docs/
- [ ] Create daemon/docs/README.md
- [ ] Create frontend/docs/README.md
- [ ] Create shared/docs/README.md
- [ ] Create docs/README.md (project index)
- [ ] Create 12 foundation documents in docs/
- [ ] Update root README.md with new structure
- [ ] Remove empty directories (src/, include/, lib/, test/ at root)

---

**Next Step:** Execute migrations following the plan above.
