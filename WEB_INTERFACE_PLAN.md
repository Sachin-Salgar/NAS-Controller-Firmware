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

**Stack:**
```
Language:         TypeScript
Framework:        Express.js
Real-time:        Socket.io
USB:              serialport npm
Config:           dotenv + JSON files
Logging:          winston or pino
Testing:          Jest
Build:            ts-node / esbuild
```

**Daemon Structure (with improvements):**
```
src/
├── index.ts              (entry point)
├── config/
│  └── index.ts          (configuration, API versioning)
├── controllers/          (plugin-style, self-registering)
│  ├── baseController.ts (common interface)
│  ├── relay.ts          (relay commands)
│  ├── fan.ts            (fan commands)
│  ├── led.ts            (LED commands)
│  ├── temperature.ts    (temp sensor)
│  ├── drive.ts          (drive monitoring)
│  └── system.ts         (system commands)
├── protocol/
│  ├── translator.ts     (web command → binary)
│  ├── parser.ts         (binary → objects)
│  ├── crc16.ts          (checksum validation)
│  └── capabilities.ts   (firmware capability discovery)
├── serial/
│  ├── connection.ts     (USB management with auto-reconnect)
│  ├── queue.ts          (command queueing with state machine)
│  └── reconnect.ts      (auto-reconnect + state resync logic)
├── state/
│  ├── store.ts          (global state store)
│  ├── firmware.ts       (FirmwareState)
│  ├── hardware.ts       (HardwareState)
│  ├── config.ts         (ConfigState)
│  └── statistics.ts     (StatisticsState)
├── events/
│  ├── eventLog.ts       (event history with timestamps)
│  └── broadcaster.ts    (WebSocket event broadcaster)
├── api/
│  ├── websocket.ts      (WebSocket command router)
│  ├── rest.ts           (minimal REST: logs, files, static)
│  └── packetInspector.ts (live packet sniffer/decoder)
└── tests/
   └── *.test.ts         (unit tests)
```

**Key Architectural Improvements:**
- Plugin-style controllers with common interface
- Command Queue with state machine (Queued → Sending → Waiting ACK → Completed/Timeout/Failed)
- Auto-reconnect with USB disconnect handling
- Capability discovery from firmware
- Single state store with subscriptions
- Event log for all state changes
- Live packet inspector for debugging
- API versioning support built-in


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

## Architecture ✅ FROZEN

- [x] **Host Daemon + Web UI approach** (CONFIRMED)
- [x] **Daemon location:** Separate server/PC on network (can control from anywhere on network)
- [x] **Multi-user support:** NO (single-user only, simpler implementation)
- [x] **Offline mode:** NO (fail immediately if USB unavailable)
- [x] **Auto-Reconnect:** YES (disconnect detected, auto-reconnect every 1s, state resync)
- [x] **Communication:** WebSocket-first (REST only for logs/files/static)
- [x] **Capability Discovery:** YES (firmware reports its capabilities on connect)

## Technology ✅ FROZEN

- [x] **Backend:** Node.js + TypeScript + Express
- [x] **Frontend:** React + TypeScript + Vite + TailwindCSS
- [x] **Real-time:** Socket.io (WebSocket-first)
- [x] **USB:** serialport npm library
- [x] **State Management:** Centralized store with subscriptions
- [x] **Command Queue:** State machine (Queued → Sending → Waiting ACK → Completed/Timeout/Failed)
- [x] **Plugin Architecture:** Controllers self-register via common interface

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

**Architecture Enhancements (Per Stakeholder Review):**
- **WebSocket-first:** All commands via persistent WebSocket (REST only for static/logs)
- **Command Queue States:** Queued → Sending → Waiting ACK → Completed/Timeout/Failed
- **Auto-Reconnect:** USB disconnect detection + automatic reconnection + state resync
- **Capability Discovery:** Firmware reports capabilities (drive count, relay count, LED count, etc.)
- **State Store:** Centralized state (FirmwareState, HardwareState, ConfigState, StatisticsState)
- **Event Log:** Rich event history with timestamps for all state changes
- **Plugin Controllers:** Self-registering controllers with common interface
- **Live Packet Inspector:** Real-time TX/RX packet viewer with hex dump + decoded meaning
- **API Versioning:** Protocol versioning support built in from the start

---

# NEXT STEPS

1. ✅ **Decisions made** (July 19, 2026)
2. ✅ **Architecture enhanced** (stakeholder feedback incorporated)
3. **Create repo structure** (daemon + frontend separate)
4. **Set up TypeScript** configurations for both
5. **Define WebSocket protocol** (message types, command queue state machine)
6. **Design State Store** (FirmwareState, HardwareState, ConfigState, StatisticsState schemas)
7. **Create detailed API specification** (WebSocket messages, capability discovery response)
8. **Create UI mockups** for main pages (Dashboard, Controls, Packet Inspector)
9. **Begin Phase 1 development**
   - Start with serial connection + auto-reconnect
   - Implement capability discovery
   - Build command queue with state machine
   - Create state store + WebSocket broadcaster
   - Build UI pages
10. Deploy to separate server once ready

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

**Document Created:** July 19, 2026
**Decisions Finalized:** July 19, 2026
**Architecture Enhanced:** July 19, 2026 (stakeholder feedback incorporated)
**Status:** FROZEN - Ready to begin Phase 1 development
**Overall Assessment:** 9.5/10 (per stakeholder review)
