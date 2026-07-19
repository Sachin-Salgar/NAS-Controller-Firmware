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

### Control Command (User → Firmware)
```
User clicks "Turn On Relay 1"
  ↓
Web UI sends: { type: "relay_on", id: 1 }
  ↓
Daemon translates to binary packet:
  Header(0x55AA) | Seq(1) | Cmd(0x10) | Len(1) | Payload(1) | CRC16
  ↓
Sends via USB Serial
  ↓
Firmware processes command
  ↓
Daemon reads response packet
  ↓
Daemon sends status update to browser via WebSocket
  ↓
Web UI updates in real-time
```

### Status Update (Firmware → User)
```
Firmware publishes event (e.g., temperature reading)
  ↓
Daemon reads from serial (polling or interrupt)
  ↓
Parses response packet
  ↓
Updates internal state
  ↓
Broadcasts to all connected WebSocket clients
  ↓
Web UI updates dashboard in real-time
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

### Option A: Node.js + Express (RECOMMENDED)

**Pros:**
- ✅ Easiest to implement
- ✅ Large ecosystem (serialport, socket.io, etc.)
- ✅ JavaScript throughout (frontend + backend)
- ✅ Cross-platform (Windows, Mac, Linux)
- ✅ Good performance for this use case

**Stack:**
```
Framework:        Express.js
Real-time:        Socket.io
USB:              serialport npm
Config:           dotenv + JSON files
Logging:          winston or pino
Testing:          Jest
```

**Daemon Structure:**
```
src/
├── index.js              (entry point)
├── config/
│  └── index.js          (configuration)
├── controllers/
│  ├── relay.js          (relay commands)
│  ├── fan.js            (fan commands)
│  ├── led.js            (LED commands)
│  ├── temperature.js    (temp sensor)
│  ├── drive.js          (drive monitoring)
│  └── system.js         (system commands)
├── protocol/
│  ├── translator.js     (web command → binary)
│  ├── parser.js         (binary → objects)
│  └── crc16.js          (checksum validation)
├── serial/
│  ├── connection.js     (USB management)
│  └── queue.js          (command queueing)
├── state/
│  └── store.js          (in-memory state)
├── api/
│  └── routes.js         (REST endpoints)
├── websocket/
│  └── events.js         (real-time updates)
└── tests/
   └── *.test.js         (unit tests)
```

### Option B: Python + Flask

**Pros:**
- Good for data processing
- Excellent serial library (pyserial)
- Easy to integrate with data analysis

**Cons:**
- Extra Python dependency
- Slightly slower than Node

### Option C: C# + .NET

**Pros:**
- Enterprise-grade
- Excellent async/await
- Good for Windows deployment

**Cons:**
- Windows-only (unless .NET Core)
- Heavier resources

**Recommendation:** Start with **Node.js + Express**

---

## Frontend (Web UI)

### Framework Selection

**Option A: React** (RECOMMENDED)
- Large ecosystem
- Component libraries ready (Material-UI, Chakra)
- Large community
- Ideal for dashboards

**Option B: Vue 3**
- Faster to learn
- Smaller bundle
- Good performance

**Option C: Svelte**
- Most efficient
- Smallest bundle size
- Modern approach

**Recommendation:** React with Vite (fast build) + TailwindCSS

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

### 2.3 Statistics & Logging
- [ ] Uptime tracking
- [ ] Boot count
- [ ] Temperature history (graph)
- [ ] Event log
  - [ ] Timestamps
  - [ ] Event type (error, warning, info)
  - [ ] Details
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

## Architecture

- [ ] Confirm Host Daemon + Web UI approach
- [ ] Decide daemon location (same PC as NAS, separate server, cloud)
- [ ] Decide on multi-user support (yes/no)
- [ ] Decide on offline mode (queue commands while disconnected?)

## Technology

- [ ] Confirm Node.js + Express (or choose alternative)
- [ ] Confirm React + Vite (or choose alternative)
- [ ] Confirm TailwindCSS for styling
- [ ] Confirm Socket.io for real-time updates

## Features (Priority Order)

**MUST HAVE (Tier 1):**
- [ ] Dashboard
- [ ] Relay controls
- [ ] Fan controls
- [ ] LED controls
- [ ] Test console

**NICE TO HAVE (Tier 2):**
- [ ] Config editor
- [ ] LED animation builder
- [ ] Statistics & logging
- [ ] Config profiles

**FUTURE (Tier 3):**
- [ ] Remote access
- [ ] Mobile app
- [ ] Automation
- [ ] Batch testing

## Deployment

- [ ] Windows installer (.exe)
- [ ] Mac installer (.dmg)
- [ ] Linux package (.deb / .rpm)
- [ ] Docker container (optional)
- [ ] Source code distribution (optional)

## Security

- [ ] Local-only access (localhost only)
- [ ] Authentication required (yes/no)
- [ ] Rate limiting on commands
- [ ] Audit logging of actions

## Testing

- [ ] Unit tests (backend)
- [ ] Integration tests (daemon + firmware)
- [ ] Manual testing on real hardware
- [ ] Cross-platform testing (Windows, Mac, Linux)

---

# NEXT STEPS

1. **Review this plan** with stakeholders
2. **Make decisions** on all checkboxes above
3. **Adjust features** based on priorities
4. **Freeze scope** for Phase 1
5. **Create repo structure** (daemon + frontend)
6. **Start Phase 1 development**

---

# DOCUMENT STATUS

**Current Status:** PLANNING (awaiting decisions)

**To Freeze This Plan:**
- [ ] Stakeholder approval on architecture
- [ ] Feature priorities finalized
- [ ] Technology choices confirmed
- [ ] Success criteria validated
- [ ] Timeline agreed upon

**Once Frozen:**
- Will create architecture diagram
- Will create detailed API specification
- Will create UI mockups
- Will begin Phase 1 development

---

**Document Created:** July 19, 2026  
**Last Updated:** July 19, 2026  
**Awaiting:** Stakeholder review and decisions
