# NAS Controller Web Interface Plan

**Status:** PLANNING PHASE (Features TBD)  
**Date Created:** July 19, 2026  
**Architecture:** Host Daemon + Web Dashboard  
**Firmware Impact:** ZERO (no firmware changes required)

---

# TABLE OF CONTENTS

1. [Overview](#overview)
2. [Architecture](#architecture)
3. [Technology Stack](#technology-stack)
4. [Feature Categories](#feature-categories)
5. [Phase Breakdown](#phase-breakdown)
6. [Integration Points](#integration-points)
7. [Decision Checklist](#decision-checklist)

---

# OVERVIEW

## Purpose

Provide a web-based interface to control, monitor, configure, and test the NAS Controller Firmware. Enable operators to:
- View real-time hardware status
- Control relays, fans, LEDs
- Edit and save configurations
- Run test sequences
- Build LED animations visually
- Monitor statistics and logs

## Scope

- **In Scope:** Web UI, host daemon, USB communication bridge
- **Not In Scope:** Firmware modifications (use existing protocol)
- **Optional Later:** Remote access, mobile app, cloud sync

## Why This Approach?

✅ Firmware remains unchanged  
✅ Rich, responsive web interface possible  
✅ No embedded platform constraints  
✅ Multiple users can control simultaneously  
✅ Easy to extend with new features  
✅ Can run on any PC/server connected to hardware  

---

# ARCHITECTURE

## System Diagram

```
┌─────────────────────────────────────────────────────┐
│         WEB BROWSER (Chrome, Firefox, Safari)       │
│  ├─ Dashboard (status, real-time updates)          │
│  ├─ Control Panel (relays, fans, LEDs)             │
│  ├─ Configuration Editor                           │
│  ├─ LED Animation Builder                          │
│  ├─ Test Console                                   │
│  └─ Statistics/Logging                             │
└──────────────────┬────────────────────────────────┘
                   │ HTTP/WebSocket
                   │ (localhost:3000)
                   ↓
┌─────────────────────────────────────────────────────┐
│      HOST DAEMON (Node.js / Python / C#)            │
│  ├─ REST API Endpoints                             │
│  ├─ WebSocket Server (real-time updates)           │
│  ├─ USB Protocol Translator                        │
│  ├─ Command Queue Manager                          │
│  ├─ State Synchronizer                             │
│  └─ Configuration Persistence                      │
└──────────────────┬────────────────────────────────┘
                   │ USB CDC Serial
                   │ (COM3 / /dev/ttyUSB0)
                   │ 115200 baud
                   ↓
┌─────────────────────────────────────────────────────┐
│     ESP32 FIRMWARE (existing, unchanged)            │
│  ├─ Binary Protocol Handler                        │
│  ├─ Hardware Control                               │
│  ├─ Real-time Monitoring                           │
│  └─ Configuration Storage                          │
└─────────────────────────────────────────────────────┘
```

## Data Flow

### Control Command (User → Firmware) - WebSocket-First ✅

```
User clicks "Turn On Relay 1"
  ↓
Web UI sends via WebSocket: { type: "relay_on", id: 1, msgId: "abc123" }
  ↓
Daemon receives on persistent WebSocket connection
  ↓
Command Router adds to Command Queue (state: Queued)
  ↓
Serial Queue Manager translates to binary packet:
  Header(0x55AA) | Seq(1) | Cmd(0x10) | Len(1) | Payload(1) | CRC16
  ↓
Sends via USB Serial (state: Sending)
  ↓
Firmware processes command, sends ACK/response
  ↓
State: Waiting ACK → Completed/Timeout/Failed
  ↓
Daemon broadcasts status update via WebSocket to all clients
  ↓
Web UI updates in real-time
```

### Status Update (Firmware → User)
```
Firmware publishes event (e.g., temperature reading)
  ↓
Daemon reads from serial (polling or interrupt)
  ↓
Parses response packet with timestamp
  ↓
Updates internal State Store (FirmwareState, HardwareState)
  ↓
Logs to Event Log
  ↓
Broadcasts to all connected WebSocket clients
  ↓
Web UI components subscribed to State Store update
```

### REST Usage (Limited to Static/File Operations)
```
Browser needs to download logs or firmware file
  ↓
Use HTTP REST endpoint
  ↓
GET /api/logs → Returns JSON or CSV
GET /api/config/backup → Returns JSON
(Minimizes REST, reserves it for bulk/static resources)
```

## Communication Protocol

**Uses existing firmware protocol** (CommandSet.md):
- Header: 0x55AA
- Command byte: Identifies operation
- Payload: Variable length data
- CRC16 validation
- No changes needed to firmware

---

# TECHNOLOGY STACK

## Backend (Host Daemon)

### SELECTED: Node.js + TypeScript + Express ✅

**Rationale:** Easiest to implement, large ecosystem, JavaScript/TypeScript throughout (frontend + backend), cross-platform.

**Stack (Library-First Architecture):**
```
Core Library:     NASController.Core (pure TypeScript, no external deps)
  ├── Protocol (encoder/decoder, CRC, ACK/NAK, retry logic)
  ├── Transport (ITransport abstraction: Serial, Mock, TCP, BT)
  ├── Queue (command queue with state machine)
  ├── State (centralized store with subscriptions)
  ├── Events (event bus and packet inspector)
  └── Diagnostics (metrics, diagnostic mode)

API Layer:        Express.js + Socket.io (thin bridge UI ↔ Core)
Testing:          Jest + MockTransport (test core without hardware)
USB:              serialport npm
Config:           dotenv + JSON files
Logging:          winston or pino
Build:            ts-node / esbuild
```

**Why Library-First?**
- ✅ Entire daemon testable without server running
- ✅ Reusable in other applications (CLI, Electron app, etc.)
- ✅ Clear separation: business logic ≠ HTTP plumbing
- ✅ No vendor lock-in

**Daemon Structure (production-ready):**
```
src/
├── index.ts              (entry point)
├── config/
│  └── index.ts          (configuration, API versioning)
├── transport/            (ABSTRACTED LAYER - critical for testability)
│  ├── ITransport.ts     (transport interface)
│  ├── SerialTransport.ts (USB CDC implementation)
│  ├── MockTransport.ts  (testing/development without hardware)
│  ├── TcpTransport.ts   (future: network)
│  └── BluetoothTransport.ts (future)
├── protocol/
│  ├── translator.ts     (web command → binary)
│  ├── parser.ts         (binary → objects)
│  ├── crc16.ts          (checksum validation)
│  ├── capabilities.ts   (firmware capability discovery)
│  ├── errorClassifier.ts (error categorization)
│  └── sync.ts           (state synchronization on reconnect)
├── controllers/          (plugin-style, self-registering)
│  ├── baseController.ts (common interface)
│  ├── relay.ts, fan.ts, led.ts, etc.
├── queue/
│  ├── commandQueue.ts   (state machine: Queued → Sending → Waiting ACK → Completed/Timeout/Failed)
│  ├── transaction.ts    (atomic config changes: Begin → Send → Validate → Commit)
│  └── reconnect.ts      (reconnection sequence)
├── state/
│  ├── store.ts          (global state store - DAEMON OWNS ALL STATE)
│  ├── firmware.ts       (FirmwareState)
│  ├── hardware.ts       (HardwareState)
│  ├── config.ts         (ConfigState)
│  └── statistics.ts     (StatisticsState)
├── events/
│  ├── eventLog.ts       (event history with timestamps)
│  └── broadcaster.ts    (WebSocket event broadcaster)
├── diagnostics/         (NEW: production debugging)
│  ├── diagnosticMode.ts (packet logging, timing, metrics)
│  ├── performance.ts    (latency, throughput, queue depth tracking)
│  └── metrics.ts        (avg latency, max latency, cmds/sec, reconnect count, uptime)
├── api/
│  ├── websocket.ts      (WebSocket command router)
│  ├── rest.ts           (minimal REST: logs, files, static)
│  ├── packetInspector.ts (live packet sniffer/decoder)
│  └── diagnostics.ts    (expose diagnostic metrics)
└── tests/
   ├── *.test.ts         (unit tests)
   ├── mocks/            (mock device for integration testing)
   └── integration/      (end-to-end tests)
```

**Key Architectural Principles:**
- **Daemon owns all state. Firmware owns all hardware. UI owns only presentation.**
- **Abstracted transport layer:** ITransport interface allows serial, TCP, Bluetooth, mock
- **Atomic config transactions:** Begin → Send → Validate → Commit (all-or-nothing)
- **State synchronization on reconnect:** PING → GET_CAPABILITIES → GET_ALL_CONFIG → GET_ALL_STATUS
- **Error classification:** Transport, Protocol, Firmware, Validation, User, Internal
- **Diagnostic mode:** Every packet logged, timing info, queue transitions, CRC verification
- **Mock device:** Complete simulator for testing UI + daemon without hardware
- **Performance metrics:** Latency, throughput, queue depth, reconnect count, uptime


---

## Frontend (Web UI)

### SELECTED: React + TypeScript + Vite + TailwindCSS ✅

**Rationale:** Large ecosystem, component libraries, ideal for dashboards, TypeScript for type safety across the stack.

### Frontend Structure

```
frontend/
├── public/
│  ├── index.html
│  └── favicon.ico
├── src/
│  ├── index.jsx
│  ├── App.jsx
│  ├── pages/
│  │  ├── Dashboard.jsx
│  │  ├── Controls.jsx
│  │  ├── Configuration.jsx
│  │  ├── LedAnimator.jsx
│  │  ├── TestConsole.jsx
│  │  └── Statistics.jsx
│  ├── components/
│  │  ├── RelayControl.jsx
│  │  ├── FanControl.jsx
│  │  ├── LedControl.jsx
│  │  ├── TemperatureChart.jsx
│  │  ├── StatusIndicator.jsx
│  │  └── ...
│  ├── services/
│  │  ├── api.js          (HTTP calls)
│  │  └── websocket.js    (real-time updates)
│  ├── hooks/
│  │  ├── useFirmware.js
│  │  └── useWebSocket.js
│  ├── store/
│  │  ├── appStore.js     (state management)
│  │  └── firmware.js     (firmware state)
│  └── styles/
│     └── globals.css
├── package.json
├── vite.config.js
└── tailwind.config.js
```

---

# FEATURE CATEGORIES

## TIER 1: Core Features (Phase 1)

These features must work for MVP.

### 1.1 Dashboard
- [ ] Real-time status display
  - [ ] Power status (ON/OFF)
  - [ ] Temperature reading
  - [ ] Fan speed
  - [ ] USB connection status
  - [ ] Drive presence indicators
- [ ] System health badge (Healthy/Warning/Critical)
- [ ] Last update timestamp
- [ ] Auto-refresh (configurable interval)

### 1.2 Control Panel
- [ ] Relay controls
  - [ ] Individual relay ON/OFF buttons
  - [ ] All ON / All OFF buttons
  - [ ] Relay state display
- [ ] Fan control
  - [ ] Speed slider (0-100%)
  - [ ] Mode selector (Manual/Auto/Off)
  - [ ] Current speed readout
- [ ] LED controls
  - [ ] Brightness slider
  - [ ] Animation selector (Boot/Idle/Error/Shutdown)

### 1.3 Test Console
- [ ] Send arbitrary commands
- [ ] View response (hex dump)
- [ ] Command history (last 50 commands)
- [ ] Quick test buttons
  - [ ] Test all relays
  - [ ] Test all fans
  - [ ] Test LED animation
  - [ ] Read all sensors

### 1.4 Live Packet Inspector (NEW) ✅
- [ ] Real-time TX/RX packet display
- [ ] Hex dump with timestamps
- [ ] Decoded packet meaning (command name, parameters)
- [ ] Filtering by packet type
- [ ] Export packet trace (CSV/JSON)

---

## TIER 2: Advanced Features (Phase 2)

Additional features that add value.

### 2.1 Configuration Editor
- [ ] Load current config from firmware
- [ ] Edit settings in UI
  - [ ] Hardware counts
  - [ ] Task intervals
  - [ ] Temperature thresholds
  - [ ] Fan curves
- [ ] Validate changes
- [ ] Save to firmware flash
- [ ] Revert to saved version

### 2.2 LED Animation Builder
- [ ] Visual timeline editor
- [ ] Frame-by-frame LED control
  - [ ] Color picker for each region
  - [ ] Brightness per frame
  - [ ] Timing (duration, ease)
- [ ] Preview on live hardware
- [ ] Export animation (code or config)
- [ ] Import pre-built animations

### 2.3 Statistics & Logging (Enhanced) ✅
- [ ] Uptime tracking
- [ ] Boot count
- [ ] Temperature history (graph)
- [ ] Rich Event Log
  - [ ] Timestamps
  - [ ] Event type (error, warning, info)
  - [ ] Details
  - [ ] Examples: Relay 3 ON, Drive 2 Removed, Fan Failure, Temperature High, Power Lost, Configuration Saved, USB Reconnected
- [ ] Export logs (CSV, JSON)

### 2.4 Configuration Profiles
- [ ] Save multiple configurations
- [ ] Load/switch between profiles
- [ ] Compare two profiles (diff view)
- [ ] Factory reset option

---

## TIER 3: Future Features (Phase 3+)

Optional enhancements.

### 3.1 Remote Access
- [ ] Daemon accessible over network
- [ ] Secure authentication (API key / OAuth)
- [ ] Rate limiting
- [ ] Audit log of remote actions

### 3.2 Mobile Support
- [ ] Responsive design (tablet/phone)
- [ ] Native mobile app (React Native)
- [ ] Touch-optimized controls

### 3.3 Automation
- [ ] Scheduled tasks (cron)
- [ ] Conditional rules (if temp > X, then run fan)
- [ ] Alerts (email/webhook on events)
- [ ] Firmware update checker

### 3.4 Manufacturing Test Suite
- [ ] Pre-programmed test sequences
- [ ] Pass/fail validation
- [ ] Report generation
- [ ] Batch testing (multiple devices)

---

# PHASE BREAKDOWN

## Phase 1: MVP (Weeks 1-2)

**Goal:** Basic dashboard + control panel + test console

**Deliverables:**
- Host daemon (USB bridge, command translator)
- Web dashboard (status display)
- Control panel (relay/fan/LED buttons)
- Test console (raw command sender)

**Success Criteria:**
- [ ] Can turn relays ON/OFF via web UI
- [ ] Can adjust fan speed via slider
- [ ] Can change LED animation
- [ ] Dashboard updates in real-time
- [ ] Test console sends and receives commands
- [ ] Works on Windows, Mac, Linux

**Estimated Time:** 2-3 weeks

**Resources:**
- Backend: 1 developer (5 days)
- Frontend: 1 developer (5 days)
- Testing: Shared (3 days)

---

## Phase 2: Full Features (Weeks 3-4)

**Goal:** Configuration editor, animation builder, statistics

**Deliverables:**
- Configuration editor (load/edit/save)
- LED animation builder (visual editor)
- Statistics & logging UI
- Configuration profiles

**Success Criteria:**
- [ ] Can edit and persist configuration
- [ ] Can build LED animations visually
- [ ] Can view temperature history as graph
- [ ] Can load/switch between config profiles

**Estimated Time:** 2-3 weeks

**Resources:**
- Backend: 1 developer (3 days)
- Frontend: 1 developer (8 days)
- Design: 1 designer (2 days)

---

## Phase 3: Polish & Deploy (Week 5)

**Goal:** Production-ready, documented, packaged

**Deliverables:**
- Installer (Windows .exe, Mac .dmg, Linux .deb)
- User documentation
- API documentation
- Docker container (optional)

**Success Criteria:**
- [ ] Single-click install and run
- [ ] Works out-of-box with firmware
- [ ] Comprehensive user guide
- [ ] No external dependencies (bundled)

**Estimated Time:** 1 week

---

## Phase 4: Advanced (Optional, Weeks 6+)

**Goal:** Remote access, mobile, automation

**Includes:**
- Network daemon deployment
- Mobile app (iOS/Android)
- Automation rules engine
- Firmware updater

**Estimated Time:** 2-4 weeks

---

# INTEGRATION POINTS

## With Existing Firmware

### Protocol Compatibility
- ✅ Uses existing binary protocol (CommandSet.md)
- ✅ No firmware changes needed
- ✅ All commands defined in docs/CommandSet.md
- ✅ CRC16 validation (utils/CRC16.h)

### Command Mapping

Firmware commands → Web UI actions:

| Category | Command | Web UI |
|----------|---------|--------|
| Relay | CMD_RELAY_SET | Toggle button |
| Relay | CMD_RELAY_GET | Status indicator |
| Fan | CMD_FAN_SET_SPEED | Speed slider |
| Fan | CMD_FAN_GET | Current speed display |
| LED | CMD_LED_SET_MODE | Animation selector |
| LED | CMD_LED_SET_COLOR | Color picker |
| Temp | CMD_TEMP_GET | Temperature display |
| Temp | CMD_TEMP_READ_ALL | Temperature graph |
| Drive | CMD_DRIVE_GET | Drive list |
| System | CMD_SYSTEM_PING | Connection indicator |
| Config | CMD_CONFIG_GET | Load into editor |
| Config | CMD_CONFIG_SET | Save from editor |

## Firmware Modifications (None)

- ❌ No changes to firmware
- ❌ No new commands needed
- ❌ No protocol modifications
- ✅ Existing protocol sufficient

---

# DECISION CHECKLIST

Before starting Phase 1, finalize these decisions:

## Architecture ✅ PRODUCTION-READY (FROZEN)

- [x] **Host Daemon + Web UI approach** (CONFIRMED)
- [x] **Daemon location:** Separate server/PC on network
- [x] **Multi-user support:** NO (single-user only)
- [x] **Offline mode:** NO (fail immediately if USB unavailable)
- [x] **Auto-Reconnect:** YES (disconnect detected, auto-reconnect every 1s, full state resync)
- [x] **Communication:** WebSocket-first (REST only for logs/files/static)
- [x] **Capability Discovery:** YES (firmware reports capabilities on connect)
- [x] **Transport Layer:** ABSTRACTED (ITransport interface for serial/TCP/BT/mock)
- [x] **Config Changes:** ATOMIC TRANSACTIONS (Begin → Send → Validate → Commit)
- [x] **Error Handling:** CLASSIFIED (Transport, Protocol, Firmware, Validation, User, Internal)
- [x] **Diagnostic Mode:** YES (packet logging, timing, queue transitions, metrics)
- [x] **Virtual Device:** YES (complete mock device for testing)

## Technology ✅ FROZEN

- [x] **Backend:** Node.js + TypeScript (library-first, Express + Socket.io for API)
- [x] **Frontend:** React + TypeScript + Vite + TailwindCSS
- [x] **Real-time:** Socket.io (WebSocket-first)
- [x] **USB:** serialport npm library
- [x] **State Management:** Centralized store with subscriptions
- [x] **Command Queue:** State machine (Queued → Sending → Waiting ACK → Completed/Timeout/Failed)
- [x] **Plugin Architecture:** Controllers self-register via common interface
- [x] **Type Sharing:** Shared package with Protocol, Commands, Events, Capabilities, Config types
- [x] **Schema Versioning:** Protocol v1, Config v1, Events v1, Capabilities v1 (future-proof)

## Features (Priority Order) ✅ FROZEN

**MUST HAVE (Tier 1 - Phase 1):**
- [x] Dashboard
- [x] Relay controls
- [x] Fan controls
- [x] LED controls
- [x] Test console

**PHASE 2 (Starting with Configuration Editor):**
- [x] **Config editor** (PRIORITY)
- [ ] LED animation builder
- [ ] Statistics & logging
- [ ] Config profiles

**FUTURE (Tier 3):**
- [ ] Remote access (network daemon already supports this)
- [ ] Mobile app
- [ ] Automation
- [ ] Batch testing

## Deployment ✅ PLANNED

- [ ] Windows installer (.exe)
- [ ] Mac installer (.dmg)
- [ ] Linux package (.deb / .rpm)
- [ ] Docker container (optional)
- [ ] Source code distribution (optional)

## Security ✅ FROZEN

- [x] **Local-only access (localhost only)** - No authentication required
- [x] **Rate limiting on commands:** Yes (to prevent command flooding)
- [x] **Audit logging of actions:** Yes (for debugging)

## Testing ✅ PLANNED

- [ ] Unit tests (backend)
- [ ] Integration tests (daemon + firmware)
- [ ] Manual testing on real hardware
- [ ] Cross-platform testing (Windows, Mac, Linux)

---

# DECISIONS FINALIZED ✅

**Backend:** Node.js + TypeScript + Express
**Frontend:** React + TypeScript + Vite + TailwindCSS
**Deployment:** Separate server/PC on network
**Users:** Single-user (no multi-user sync)
**Offline:** Fail immediately (no command queue)
**Phase 2 Focus:** Configuration Editor
**Security:** Localhost-only, no auth required

**Tier 1 Enhancements (Stakeholder Review Round 1):**
- **WebSocket-first:** All commands via persistent WebSocket (REST only for static/logs)
- **Command Queue States:** Queued → Sending → Waiting ACK → Completed/Timeout/Failed
- **Auto-Reconnect:** USB disconnect detection + automatic reconnection + full state resync
- **Capability Discovery:** Firmware reports capabilities (drive count, relay count, LED count, etc.)
- **State Store:** Centralized state (FirmwareState, HardwareState, ConfigState, StatisticsState)
- **Event Log:** Rich event history with timestamps for all state changes
- **Plugin Controllers:** Self-registering controllers with common interface
- **Live Packet Inspector:** Real-time TX/RX packet viewer with hex dump + decoded meaning
- **API Versioning:** Protocol versioning support built in from the start

**Tier 2 Enhancements (Stakeholder Review Round 2 - Production Quality):**
- **Abstracted Transport Layer:** ITransport interface (SerialTransport, MockTransport, TcpTransport, BluetoothTransport)
  - Allows testing entire application without ESP32 hardware
  - Easy to add new communication methods
- **Protocol Specification Document:** PROTOCOL_SPEC.md (packet layout, byte order, CRC, ACK/NAK, timeouts, retry policy, error codes)
- **State Synchronization on Reconnect:** PING → GET_CAPABILITIES → GET_ALL_CONFIG → GET_ALL_STATUS
  - Guarantees UI reflects firmware's actual state
- **Atomic Config Transactions:** Begin Config → Send Changes → Validate → Commit (all-or-nothing)
  - Prevents partially applied configurations
- **Error Classification:** Transport, Protocol, Firmware, Validation, User, Internal errors
  - Simplifies logging and troubleshooting
- **Diagnostic Mode:** Packet logging, timing information, queue state transitions, CRC verification, performance metrics
- **Virtual Device (Mock Transport):** Complete firmware simulator for development and testing without hardware
- **Performance Monitoring:** Command latency, throughput, queue depth, packet loss, reconnect count, uptime
- **Guiding Principle:** Daemon owns all state. Firmware owns all hardware. UI owns only presentation.

---

# IMPLEMENTATION ROADMAP

## Pre-Development (Repository Setup)

1. ✅ **Decisions made** (July 19, 2026)
2. ✅ **Architecture finalized** (production-ready)
3. ✅ **Protocol specification created** (PROTOCOL_SPEC.md)
4. **Establish repository structure (docs colocated with code)**
   ```
   nas-controller/
   │
   ├── firmware/                      (existing ESP32 firmware)
   │   ├── src/
   │   ├── docs/
   │   │   ├── ARCHITECTURE.md
   │   │   ├── COMMAND_SET.md
   │   │   ├── LED_SYSTEM.md
   │   │   ├── TASKS.md
   │   │   └── ...
   │   └── platformio.ini
   │
   ├── daemon/                        (Node.js + TypeScript daemon)
   │   ├── src/
   │   │   ├── core/                  (NASController.Core library)
   │   │   │   ├── protocol/          (packet encoding/decoding)
   │   │   │   ├── transport/         (ITransport interface)
   │   │   │   ├── queue/             (command queue + state machine)
   │   │   │   ├── state/             (state store + subscriptions)
   │   │   │   ├── events/            (event bus + packet inspector)
   │   │   │   ├── diagnostics/       (metrics, diagnostic mode)
   │   │   │   └── controllers/       (plugin-style controllers)
   │   │   ├── api/                   (Express + Socket.io layer)
   │   │   └── index.ts               (daemon entry point)
   │   ├── tests/
   │   ├── docs/
   │   │   ├── ARCHITECTURE.md        (daemon architecture)
   │   │   ├── WEBSOCKET_API.md       (WebSocket protocol)
   │   │   ├── STATE_STORE.md         (state management)
   │   │   ├── TRANSPORT_LAYER.md     (transport abstraction)
   │   │   └── adr/                   (architecture decisions)
   │   │       ├── 0002-library-first-daemon.md
   │   │       └── ...
   │   └── package.json
   │
   ├── frontend/                      (React + Vite)
   │   ├── src/
   │   │   ├── pages/
   │   │   ├── components/
   │   │   ├── services/
   │   │   └── hooks/
   │   ├── public/
   │   ├── docs/
   │   │   ├── WEB_INTERFACE_PLAN.md  (feature list, roadmap)
   │   │   ├── UI_GUIDELINES.md       (design system)
   │   │   ├── COMPONENTS.md          (component catalog)
   │   │   └── WEBSOCKET_CLIENT.md    (Socket.io integration)
   │   └── package.json
   │
   ├── shared/                        (TypeScript types, shared by daemon + frontend)
   │   ├── src/
   │   │   ├── protocol.ts            (packet types)
   │   │   ├── commands.ts            (command definitions)
   │   │   ├── events.ts              (event types)
   │   │   ├── capabilities.ts        (firmware capability schema)
   │   │   └── config.ts              (configuration schema)
   │   ├── docs/
   │   │   ├── PROTOCOL_SPEC.md       (binary protocol definition)
   │   │   ├── COMMAND_SET.md         (command reference)
   │   │   ├── DATA_TYPES.md          (shared type definitions)
   │   │   └── adr/
   │   │       ├── 0001-websocket-first.md
   │   │       └── ...
   │   └── package.json
   │
   ├── docs/                          (project-level documentation)
   │   ├── PROJECT_OVERVIEW.md
   │   ├── GETTING_STARTED.md
   │   ├── ROADMAP.md
   │   ├── CONTRIBUTING.md
   │   ├── CHANGELOG.md
   │   └── ARCHITECTURE_OVERVIEW.md   (high-level system design)
   │
   ├── tools/                         (build scripts, utilities)
   ├── scripts/                       (setup, deployment, CI/CD)
   └── README.md                      (project entry point)
   ```

   **Documentation Organization Principle:**
   - `firmware/docs/` → Firmware-specific documentation
   - `daemon/docs/` → Daemon-specific documentation
   - `frontend/docs/` → Frontend-specific documentation
   - `shared/docs/` → Protocol specs and shared contracts (used by both firmware and daemon)
   - `docs/` (root) → Project overview, roadmap, contribution guidelines

**Document Placement Guide:**

| Document | Location | Rationale |
|----------|----------|-----------|
| WEB_INTERFACE_PLAN.md | `frontend/docs/` | Frontend feature roadmap and architecture |
| PROTOCOL_SPEC.md | `shared/docs/` | Defines contract between firmware and daemon |
| CommandSet.md | `shared/docs/` | Shared command reference (firmware + daemon) |
| Daemon Architecture | `daemon/docs/ARCHITECTURE.md` | Daemon internal design |
| Transport Layer | `daemon/docs/TRANSPORT_LAYER.md` | Daemon component design |
| ADR: WebSocket-first | `shared/docs/adr/` | Affects entire system |
| ADR: Library-first daemon | `daemon/docs/adr/` | Daemon-specific decision |
| ADR: Capability discovery | `daemon/docs/adr/` | Daemon pattern |
| Project Roadmap | `docs/ROADMAP.md` | High-level timeline |
| Contributing Guide | `docs/CONTRIBUTING.md` | Development guidelines |
| Firmware Architecture | `firmware/docs/ARCHITECTURE.md` | Firmware design (already exists) |

5. **Set up TypeScript configurations**
   - Root tsconfig.json (shared settings)
   - daemon/tsconfig.json (strict mode)
   - frontend/tsconfig.json (react mode)
   - shared/tsconfig.json (library mode)

6. **Define shared type packages**
   - Protocol types (PROTOCOL_SPEC → shared/protocol.ts)
   - Command definitions (shared/commands.ts)
   - Event types (shared/events.ts)
   - Capability schema (shared/capabilities.ts)
   - Configuration schema (shared/config.ts)
   - Versioning for all (protocol v1, config v1, events v1, etc.)

7. **Create ADRs (Architecture Decision Records)**
   - Document WHY each major decision was made
   - ADRs colocated with component docs:
     - `shared/docs/adr/0001-websocket-first.md` (affects entire system)
     - `daemon/docs/adr/0002-library-first-daemon.md` (daemon-specific)
     - `daemon/docs/adr/0003-capability-discovery.md` (daemon pattern)
     - `shared/docs/adr/0004-transport-abstraction.md` (protocol/transport layer)
   - Reference: PROTOCOL_SPEC.md, WEB_INTERFACE_PLAN.md

## Phase 1: Core Implementation (MVP) - Bottom-Up Build Order ✅

**Build Order:** Protocol → Transport → Daemon Core → WebSocket API → React UI
(Each higher layer is built on tested lower layer)

### 1. Protocol Library (Foundation)
- [x] Packet encoder/decoder
- [x] CRC16 implementation
- [x] Sequence number handling
- [x] ACK/NAK response logic
- [x] Retry policy with backoff
- [ ] Unit tests (jest)
- [ ] Integration tests with mock device

### 2. Transport Layer (Abstraction)
- [ ] ITransport interface definition
- [ ] SerialTransport (USB CDC implementation)
- [ ] MockTransport (virtual device for testing)
- [ ] Error classification system
- [ ] Auto-reconnect logic
- [ ] Unit tests for each transport

### 3. Daemon Core (NASController.Core Library)
- [ ] Command Queue with state machine
- [ ] Central State Store (subscriptions)
- [ ] Event Bus (event log with timestamps)
- [ ] Packet Inspector (packet capture/decode)
- [ ] Capability Discovery (query firmware at startup)
- [ ] Configuration Transactions (atomic changes)
- [ ] Diagnostic mode (packet logging, metrics)
- [ ] NO dependency on Express or Socket.io (pure library)
- [ ] Comprehensive unit tests

### 4. WebSocket API (Express + Socket.io)
- [ ] Express server setup
- [ ] Socket.io connection manager
- [ ] WebSocket command router (bridge UI ↔ daemon core)
- [ ] REST endpoints (logs, files, diagnostics)
- [ ] Packet inspector endpoint
- [ ] Performance metrics endpoint

### 5. React Dashboard (Web UI)
- [ ] Dashboard page (real-time status)
- [ ] Control panel (relays, fans, LEDs)
- [ ] Test console (send raw commands)
- [ ] Packet inspector page
- [ ] State subscriptions via Socket.io
- [ ] Real-time updates
- [ ] Cross-browser testing

### 6. Testing & Validation
- [ ] Unit test coverage (protocol, transport, core)
- [ ] Integration tests (daemon core + mock transport)
- [ ] Manual testing on real hardware
- [ ] Cross-platform validation (Windows, Mac, Linux)

## Phase 2: Advanced Features

14. **Configuration Editor** (priority)
15. **LED Animation Builder**
16. **Statistics & Logging**
17. **Configuration Profiles**

## Phase 3: Production Readiness

18. **Installers** (.exe, .dmg, .deb)
19. **Documentation** (user guide, API docs)
20. **Performance optimization** (if needed)
21. **Deployment** to separate server

---

# DOCUMENT STATUS

**Status:** ✅ **FROZEN** (ready for Phase 1 development)

**Decisions Completed:**
- [x] Architecture confirmed
- [x] Technology stack locked in
- [x] Feature priorities finalized
- [x] Security approach chosen
- [x] Deployment model selected

**Ready to Start:**
- Set up GitHub repos (daemon + frontend)
- Create TypeScript configurations
- Build Phase 1 features (Dashboard, Controls, Test Console)
- Test with real hardware

---

---

# ARCHITECTURAL PRINCIPLES (Guiding Philosophy)

> **The daemon owns all state. The firmware owns all hardware. The UI owns only presentation.**

This separation of responsibilities ensures:
- **Testability:** Daemon can be tested without hardware (via MockTransport)
- **Maintainability:** Clear layer boundaries make changes safer
- **Extensibility:** New hardware features don't require UI changes (capability discovery)
- **Reliability:** Single source of truth prevents inconsistent state

---

# DOCUMENT STATUS

**Document Created:** July 19, 2026
**Round 1 Review:** July 19, 2026 (architecture enhanced with WebSocket-first, auto-reconnect, capability discovery)
**Round 2 Review:** July 19, 2026 (production-quality refinements: abstracted transport, atomic transactions, diagnostics)
**Round 3 Review:** July 19, 2026 (implementation strategy finalized: bottom-up build order, library-first daemon, shared types, ADRs)
**Status:** ✅ **READY FOR PHASE 1 IMPLEMENTATION**
**Overall Assessment:** **10/10** (per stakeholder reviews)

---

# IMPLEMENTATION STRATEGY SUMMARY

## Before Writing Code

1. ✅ Establish repository structure (mono-repo with firmware, daemon, frontend, shared, docs)
2. ✅ Create TypeScript configurations (root + daemon + frontend + shared)
3. ✅ Define shared types (protocol, commands, events, capabilities, config)
4. ✅ Write ADRs documenting major decisions (WebSocket-first, library-first daemon, etc.)
5. ✅ Protocol specification complete (PROTOCOL_SPEC.md)

## Build Order (Bottom-Up)

**Step 1: Protocol Library**
- Packet encoder/decoder, CRC16, sequence numbers
- ACK/NAK logic, retry handling
- Unit tests with 100% coverage

**Step 2: Transport Layer**
- ITransport interface, SerialTransport, MockTransport
- Auto-reconnect, error classification
- Unit tests for each transport

**Step 3: Daemon Core (NASController.Core Library)**
- Command queue, state store, event bus
- Packet inspector, capability discovery
- Configuration transactions
- Diagnostic mode + metrics
- Pure TypeScript, NO external dependencies (except serialport)
- Full unit test coverage
- Integration tests with MockTransport

**Step 4: WebSocket API (Express + Socket.io)**
- Thin bridge between React UI and NASController.Core
- WebSocket command router
- REST endpoints (logs, files, diagnostics)

**Step 5: React Dashboard**
- Pages: Dashboard, Controls, Test Console, Packet Inspector
- Real-time state subscriptions
- Cross-browser testing

## Key Principles

> **The daemon owns all state. The firmware owns all hardware. The UI owns only presentation.**

> **NASController.Core is a library first, Express server second.**

> **Each layer is tested independently before the next layer is built.**

## Tools & Infrastructure

- **Testing:** Jest (protocol, transport, core tests run without hardware)
- **Types:** Shared package with protocol definitions
- **Documentation:** ADRs explain WHY decisions were made
- **Mocking:** MockTransport enables full development/testing without ESP32
- **Versioning:** Protocol v1, Config v1, Events v1, Capabilities v1 (built-in)

---

# NEXT: REPOSITORY SETUP

You're ready to begin Phase 1 implementation.

**First steps (in order):**
1. Establish the repository structure (firmware, daemon, frontend, shared, docs)
2. Colocate documentation with code (firmware/docs/, daemon/docs/, etc.)
3. Create TypeScript configurations (root + daemon + frontend + shared)
4. Define shared types (protocol, commands, events, capabilities, config)
5. Write ADRs documenting major decisions
6. Create project-level overview (docs/PROJECT_OVERVIEW.md)

**Documentation Organization:**
- Each component owns its documentation
- `shared/docs/` contains protocol specs and shared contracts
- ADRs live with the component they affect
- This makes it immediately clear which docs apply to which part of the system

**Key Takeaway:**
Architecture evolved from functional to production-grade with clear layering, extensibility, testability, and maintainability. Design risks are resolved. Implementation risks remain (as expected at this phase).

**Build strategy:** Protocol → Transport → Daemon Core → API → UI (bottom-up ensures each layer has tested foundation)

**Documentation strategy:** Docs colocated with code (firmware/docs/ for firmware, daemon/docs/ for daemon, etc.) makes the system self-documenting and easy to navigate.

The architecture is solid. The implementation path is clear. You're ready to code.
