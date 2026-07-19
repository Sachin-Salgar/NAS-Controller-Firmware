Document Version: 1.0  
Project Version: 1.0  
Last Updated: 2026-07-19  
Owner: NAS Controller  
Status: Frozen

# Requirements Traceability Matrix

This document maps all user requirements to their implementations across firmware, daemon, and UI. Use this matrix to verify that no requirement is forgotten and to understand which components implement each feature.

---

## Phase 1: MVP Requirements

### Core Features

#### 1. Dashboard - Real-Time Status Display

| Requirement | Firmware | Daemon | Frontend | Test | Status |
|-------------|----------|--------|----------|------|--------|
| Display power status (ON/OFF) | CMD_SYSTEM_PING | State store / Broadcaster | Dashboard page | ✔ Manual | Planned |
| Display temperature reading | CMD_TEMP_READ | State store / Event log | Dashboard widget | ✔ Manual | Planned |
| Display fan speed (RPM/%) | CMD_FAN_GET | State store | Dashboard widget | ✔ Manual | Planned |
| Display USB connection status | Auto-detect disconnect | Transport / Reconnect logic | Status indicator | ✔ Integration | Planned |
| Display drive presence indicators | CMD_DRIVE_GET | State store | Drive list component | ✔ Manual | Planned |
| System health badge (Healthy/Warning/Critical) | N/A | State aggregation | Dashboard badge | ✔ Unit | Planned |
| Last update timestamp | N/A | Timestamp on state changes | Dashboard widget | ✔ Unit | Planned |
| Auto-refresh (configurable interval) | N/A | Periodic PING | Polling logic | ✔ Unit | Planned |

**Coverage:** 8/8 requirements  
**Implementation Effort:** 3-4 days  
**Dependencies:** Protocol library, Transport layer, State store

---

#### 2. Relay Controls

| Requirement | Firmware | Daemon | Frontend | Test | Status |
|-------------|----------|--------|----------|------|--------|
| Individual relay ON/OFF buttons | CMD_RELAY_SET | Command queue / Controller | Relay buttons | ✔ Unit | Planned |
| Relay state display (ON/OFF) | CMD_RELAY_GET | State store | Status indicator | ✔ Unit | Planned |
| All ON button | N/A | Batch command via queue | Button component | ✔ Unit | Planned |
| All OFF button | N/A | Batch command via queue | Button component | ✔ Unit | Planned |
| Relay status feedback | ACK/NAK response | State update broadcast | UI update | ✔ Integration | Planned |
| Error handling (relay failure) | Error code in NAK | Error classification | Error toast | ✔ Manual | Planned |
| Latency < 500ms | Serial optimization | Queue management | Perceived responsiveness | ✔ Manual | Planned |

**Coverage:** 7/7 requirements  
**Implementation Effort:** 2-3 days  
**Dependencies:** Protocol library, Command queue, State store, WebSocket API

---

#### 3. Fan Controls

| Requirement | Firmware | Daemon | Frontend | Test | Status |
|-------------|----------|--------|----------|------|--------|
| Fan speed slider (0-100%) | CMD_FAN_SET_SPEED | Command queue / Validation | Slider component | ✔ Unit | Planned |
| Current speed display | CMD_FAN_GET | State store | Speed readout | ✔ Unit | Planned |
| Fan mode selector (Manual/Auto/Off) | CMD_FAN_SET_MODE | Command queue / Controller | Dropdown component | ✔ Unit | Planned |
| Real-time feedback | ACK response | State broadcast | UI update | ✔ Integration | Planned |
| Mode persistence (EEPROM) | EEPROM storage | Read at startup | Display in UI | ✔ Manual | Planned |
| Auto mode thermostat | Temperature threshold logic | Read config / Enforce limits | (Phase 2) | ✔ Manual | Phase 2 |
| Error handling (fan failure) | Error code | Error classification | Error message | ✔ Manual | Planned |

**Coverage:** 7/7 requirements (Phase 1)  
**Implementation Effort:** 2-3 days  
**Dependencies:** Protocol library, Command queue, State store

---

#### 4. LED Controls

| Requirement | Firmware | Daemon | Frontend | Test | Status |
|-------------|----------|--------|----------|------|--------|
| Brightness slider | CMD_LED_BRIGHTNESS | Command queue | Slider component | ✔ Unit | Planned |
| Animation selector (Boot/Idle/Error/Shutdown) | CMD_LED_SET_ANIMATION | Command queue | Dropdown component | ✔ Unit | Planned |
| Color picker (RGB) | CMD_LED_COLOR | Command queue | Color picker component | ✔ Unit | Planned |
| Real-time preview on hardware | ACK → hardware executes | Command queue feedback | Sync with hardware | ✔ Manual | Planned |
| Animation persistence | EEPROM storage | Read at startup | Display in UI | ✔ Manual | Planned |
| LED animation builder | N/A | Compiler / storage | Frame editor | ✔ Manual | Phase 2 |

**Coverage:** 6/6 requirements (Phase 1)  
**Implementation Effort:** 2-3 days  
**Dependencies:** Protocol library, Command queue, LED firmware support

---

#### 5. Test Console

| Requirement | Firmware | Daemon | Frontend | Test | Status |
|-------------|----------|--------|----------|------|--------|
| Send arbitrary commands (hex) | Any CMD | Direct command dispatch | Hex input field | ✔ Unit | Planned |
| View response (hex dump) | Response packet | Parse and format | Hex display | ✔ Unit | Planned |
| Command history (last 50) | N/A | Event log | History list | ✔ Unit | Planned |
| Quick test buttons (relays/fans/LEDs) | Multiple CMDs | Batch command execution | Button group | ✔ Unit | Planned |
| Error response display | NAK codes | Error decoding | Error details | ✔ Unit | Planned |
| Timestamp for each command | Packet sent/received | Timing / Event log | History timeline | ✔ Unit | Planned |

**Coverage:** 6/6 requirements  
**Implementation Effort:** 2-3 days  
**Dependencies:** Protocol library, Command queue, Event log

---

#### 6. Live Packet Inspector

| Requirement | Firmware | Daemon | Frontend | Test | Status |
|-------------|----------|--------|----------|------|--------|
| Real-time TX packet display | N/A | Packet capture / Logger | Live stream | ✔ Unit | Planned |
| Real-time RX packet display | N/A | Packet capture / Logger | Live stream | ✔ Unit | Planned |
| Hex dump with timestamps | N/A | Formatter | Hex table | ✔ Unit | Planned |
| Decoded packet meaning (command name, parameters) | N/A | Decoder | Decoded view | ✔ Unit | Planned |
| Filtering by packet type | N/A | Filter logic | Filter dropdown | ✔ Unit | Planned |
| Export packet trace (CSV/JSON) | N/A | Formatter / File writer | Export button | ✔ Manual | Planned |
| Auto-scroll latest packets | N/A | Stream | Scroll component | ✔ Unit | Planned |

**Coverage:** 7/7 requirements  
**Implementation Effort:** 2 days  
**Dependencies:** Protocol library, Packet inspector module

---

## Architecture Requirements

### Daemon Requirements

| Requirement | Component | Implementation | Test | Status |
|-------------|-----------|-----------------|------|--------|
| USB CDC serial communication | SerialTransport | serialport npm | ✔ Integration | Planned |
| 115200 baud rate | SerialTransport | Port config | ✔ Manual | Planned |
| Auto-detect COM port (Windows/Linux/Mac) | SerialTransport | Port enumeration | ✔ Manual | Planned |
| WebSocket server (Socket.io) | API layer | Express + Socket.io | ✔ Integration | Planned |
| REST API endpoints | API layer | Express routes | ✔ Unit | Planned |
| Real-time state broadcast | Broadcaster | Subscription pattern | ✔ Unit | Planned |
| Command queue with state machine | Command queue | Queue management | ✔ Unit | Planned |
| Retry logic (3x with backoff) | Protocol | Retry manager | ✔ Unit | Planned |
| CRC16 validation | Protocol | CRC algorithm | ✔ Unit | Planned |
| Auto-reconnect on disconnect | Transport | Reconnect handler | ✔ Integration | Planned |
| Atomic config transactions | Config manager | Transaction logic | ✔ Unit | Planned |
| Error classification (6 types) | Error handler | Error mapper | ✔ Unit | Planned |
| Diagnostic mode (metrics, logging) | Diagnostics | Metrics collector | ✔ Manual | Planned |
| Mock device for testing | MockTransport | Firmware simulator | ✔ Integration | Planned |

**Coverage:** 14/14 requirements  
**Implementation Effort:** 5-7 days (protocol layer + transport layer + core daemon)

---

### Frontend Requirements

| Requirement | Component | Implementation | Test | Status |
|-------------|-----------|-----------------|------|--------|
| React + Vite setup | Build system | Vite config | ✔ Manual | Planned |
| TailwindCSS styling | CSS framework | Tailwind config | ✔ Manual | Planned |
| Socket.io WebSocket client | WebSocket layer | Socket.io client lib | ✔ Unit | Planned |
| Real-time state subscriptions | Hooks | useWebSocket hook | ✔ Unit | Planned |
| Command sending | API | sendCommand function | ✔ Unit | Planned |
| Error toast notifications | UI | Toast component | ✔ Unit | Planned |
| Responsive design (mobile/tablet/desktop) | CSS | Media queries | ✔ Manual | Planned |
| Dark mode (future) | CSS | Theme switcher | ✔ Unit | Phase 3 |

**Coverage:** 8/8 requirements  
**Implementation Effort:** 1-2 days (setup) + feature-specific

---

## Protocol Requirements

| Requirement | Implementation | Test | Status |
|-----------|-----------------|------|--------|
| Binary packet format (Header/Seq/Cmd/Len/Payload/CRC) | Protocol encoder/decoder | ✔ Unit | Planned |
| 0x55AA header | Protocol constants | ✔ Unit | Planned |
| CRC16 validation | CRC algorithm | ✔ Unit | Planned |
| Command byte with ACK/NAK bits | Protocol constants | ✔ Unit | Planned |
| Sequence number matching | Sequence manager | ✔ Unit | Planned |
| Timeout: 1000ms per command | Timer logic | ✔ Unit | Planned |
| Retry up to 3 times | Retry manager | ✔ Unit | Planned |
| Capability discovery at startup | Startup sequence | ✔ Integration | Planned |
| Error codes in NAK response | Error classification | ✔ Unit | Planned |
| Version-aware protocol | Version enum | ✔ Unit | Planned |

**Coverage:** 10/10 requirements  
**Implementation Effort:** 3-4 days (protocol library)

---

## Phase 2+ Features (Future)

| Feature | Firmware | Daemon | Frontend | Status |
|---------|----------|--------|----------|--------|
| Configuration editor | CMD_CONFIG_GET/SET | Config manager | Config page | Phase 2 |
| LED animation builder | Animation compiler | Storage / Builder | Animation editor | Phase 2 |
| Statistics & logging | Event recording | Event log / Storage | Charts page | Phase 2 |
| Configuration profiles | Profile storage | Profile manager | Profile UI | Phase 2 |
| Temperature history graph | Event log | Data aggregation | Chart component | Phase 2 |
| Fan auto mode | PWM control + thermostat | Mode manager | Mode selector | Phase 2 |
| Remote network access | N/A | Network daemon | Same UI | Phase 4 |
| Mobile app | N/A | Same daemon | React Native app | Phase 4 |
| Automation rules | Scheduler | Rules engine | Rule editor | Phase 4+ |
| Firmware update | Bootloader support | OTA manager | Update dialog | Phase 4+ |

---

## Test Coverage by Component

| Component | Unit Tests | Integration Tests | Manual Tests |
|-----------|-----------|-----------------|--------------|
| Protocol (encoder/decoder) | 100% | ✔ | ✔ |
| Transport (Serial, Mock) | 100% | ✔ | ✔ |
| Command Queue | 100% | ✔ | ✔ |
| State Store | 100% | ✔ | N/A |
| Error Handling | 100% | ✔ | ✔ |
| WebSocket API | 80%+ | ✔ | ✔ |
| Frontend Components | 80%+ | ✔ | ✔ |
| End-to-End Flow | N/A | ✔ | ✔ |

**Target:** 80%+ coverage for MVP (100% for core libraries)

---

## How to Use This Matrix

1. **Verification:** Before starting a feature, check this matrix for requirements
2. **Tracking:** Mark requirements as completed as you finish implementation
3. **Testing:** Ensure test column has ✔ mark before marking complete
4. **Coverage:** All requirements should have at least one ✔ mark
5. **Dependencies:** Check "Dependencies" column before starting implementation

---

## Adding New Requirements

When adding a new feature:

1. Create new section in appropriate phase
2. Break down into sub-requirements
3. Identify which components implement each
4. Estimate effort
5. Identify dependencies
6. Add to IMPLEMENTATION_CHECKLIST.md
7. Update this matrix as implementation progresses

---

**Purpose:** Ensure no requirement is forgotten and all features are properly tested  
**Status:** Active (updated as features are implemented)  
**Maintenance:** Update as Phase 1 implementation progresses
