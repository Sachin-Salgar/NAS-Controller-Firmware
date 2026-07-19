Document Version: 1.0  
Project Version: 1.0  
Last Updated: 2026-07-19  
Owner: NAS Controller  
Status: Frozen

# Extension Points - Where Future Features Plug In

This document identifies the places in the architecture where new features can be added without requiring a complete redesign. Understanding these extension points helps future developers add features in the proper locations.

---

## 1. Hardware Support

### Adding New Sensors

**Location:** `firmware/src/sensors/`  
**Pattern:** Inherit from `BaseSensor` interface

**Current:** Temperature sensors (OneWire DS18B20)  
**Future:** Humidity, pressure, motion sensors, etc.

**Extension:**
1. Create new sensor class in `firmware/src/sensors/NewSensor.h`
2. Implement `getSensorData()` and `getStatus()`
3. Register in `hardware.cpp`
4. Add command in firmware (e.g., `CMD_HUMIDITY_GET`)
5. Add protocol type in `shared/src/types/`
6. Daemon automatically supports new sensor via capability discovery
7. UI can subscribe to new sensor data

**Effort:** ~1-2 days per sensor type  
**No changes needed:** Daemon core, daemon API, transport layer

---

### Adding New Relay Boards

**Location:** `firmware/src/relays/`  
**Pattern:** Inherit from `BaseRelayBoard` interface

**Current:** GPIO-based relay control  
**Future:** I2C relay boards, CAN bus relay modules, networked relays

**Extension:**
1. Create new board class in `firmware/src/relays/NewBoard.h`
2. Implement `setRelay()`, `getRelay()`, `initialize()`
3. Register board type in `hardware.cpp`
4. Firmware capability report includes relay count
5. Daemon and UI adapt automatically

**Effort:** ~2-3 days per board type  
**No changes needed:** Daemon, UI, protocol (uses existing commands)

---

### Adding New LED Types

**Location:** `firmware/src/leds/`  
**Pattern:** Inherit from `BaseLedStrip` interface

**Current:** WS2812B addressable LEDs  
**Future:** APA102, SK6812, standard GPIO LEDs, RGB CCT lights

**Extension:**
1. Create new LED class in `firmware/src/leds/NewLed.h`
2. Implement color/brightness control methods
3. Register in `hardware.cpp`
4. Add LED type to capability report
5. Daemon and UI adapt automatically

**Effort:** ~1-2 days per LED type  
**No changes needed:** Protocol, daemon, UI layout

---

## 2. Communication Protocols

### Adding New Transport Methods

**Location:** `daemon/src/core/transport/`  
**Pattern:** Implement `ITransport` interface

**Current:** SerialTransport (USB), MockTransport (testing)  
**Future:** TCP/IP network daemon, Bluetooth, CAN bus, WiFi

**Extension:**
1. Create new transport class: `daemon/src/core/transport/[NewTransport].ts`
2. Implement: `connect()`, `send()`, `receive()`, `disconnect()`
3. Emit events: `onConnect`, `onDisconnect`, `onData`
4. Handle errors and reconnection in same way
5. Daemon core works unchanged with new transport
6. Configuration specifies which transport to use
7. UI and API unchanged

**Effort:** ~3-5 days per transport (including error handling)  
**No changes needed:** Daemon core, state store, command queue, API, UI

**Benefits:** Single codebase supports multiple communication methods (USB, Network, BLE)

---

### Adding New Protocol Versions

**Location:** `shared/src/protocol/versions/`  
**Pattern:** Extend protocol version handling

**Current:** Protocol v1.0  
**Future:** Protocol v1.1, v2.0 with additional commands

**Extension:**
1. Add new version to `ProtocolVersion` enum
2. Create version-specific encoder/decoder in `shared/src/protocol/versions/v1_1.ts`
3. Daemon detects firmware version via capability discovery
4. Router dispatches to correct encoder/decoder
5. Backwards compatible: old firmware works with new daemon
6. New firmware features available only with new daemon

**Effort:** ~2-3 days per protocol version  
**No changes needed:** Transport layer, state store, API, UI

---

## 3. Hardware Features

### Adding New Control Features

**Location:** `firmware/docs/CommandSet.md` → `firmware/src/commands/` → `shared/src/protocol.ts` → `daemon/src/core/controllers/` → `frontend/src/components/`

**Current:** Relay ON/OFF, Fan PWM, LED color/brightness  
**Future:** Temperature thresholds, fan curves, LED sequences, automated schedules

**Extension Path:**
1. Define new command in `firmware/docs/CommandSet.md`
2. Implement handler in `firmware/src/commands/new_command.cpp`
3. Add protocol packet type in `shared/src/protocol.ts`
4. Create daemon controller in `daemon/src/core/controllers/NewFeature.ts`
5. Add WebSocket handler in `daemon/src/api/websocket.ts`
6. Create UI component in `frontend/src/components/NewFeature.jsx`
7. Wire into dashboard or control panel

**Effort:** ~3-5 days per feature (spans all layers)  
**Pattern:** Follows same pattern as existing relay/fan/LED features

---

### Adding Fan Modes

**Location:** `firmware/src/fan.cpp` → `firmware/docs/CommandSet.md`  
**Current:** Manual (fixed speed), Off  
**Future:** Auto (thermostat-controlled), Curve (based on temperature), Ramp (smooth acceleration)

**Extension:**
1. Add new mode constants in `firmware/include/fan.h`
2. Implement mode logic in `firmware/src/fan.cpp`
3. Add `CMD_FAN_SET_MODE` command if not exists
4. Report supported modes in capability discovery
5. Daemon and UI adapt to available modes

**Effort:** ~2-3 days per mode  
**No changes needed:** Daemon core, protocol structure

---

## 4. Daemon Features

### Adding New Controllers (Plugin System)

**Location:** `daemon/src/core/controllers/`  
**Pattern:** Inherit from `BaseController`

**Current:** RelayController, FanController, LedController, TemperatureController  
**Future:** DriveController, PowerController, custom hardware controllers

**Extension:**
1. Create new controller: `daemon/src/core/controllers/MyController.ts`
2. Extend `BaseController` class
3. Register commands: `this.registerCommand('cmd_name', handler)`
4. Subscribe to state changes
5. Emit state updates to subscribers
6. Controller auto-registers via factory pattern

**Effort:** ~1-2 days per controller  
**No changes needed:** Core framework, state store, API

**Benefits:** New features without touching daemon core

---

### Adding New WebSocket Handlers

**Location:** `daemon/src/api/websocket.ts`  
**Pattern:** Register new message types

**Extension:**
1. Define new WebSocket message type in `shared/src/websocket.ts`
2. Add handler in `daemon/src/api/websocket.ts`
3. Handler dispatches to appropriate controller or method
4. Response sent back to client
5. UI sends and receives messages

**Effort:** ~1 day per handler  
**No changes needed:** Transport, protocol, core logic

---

### Adding REST Endpoints

**Location:** `daemon/src/api/rest.ts`  
**Current:** GET /api/logs, GET /api/diagnostics, GET /api/config/backup  
**Future:** POST /api/automation, GET /api/statistics, etc.

**Extension:**
1. Create route in `daemon/src/api/rest.ts`
2. Implement handler using daemon state store
3. Return JSON response
4. No firmware changes needed
5. WebSocket preferred for real-time, REST for bulk operations

**Effort:** ~1-2 days per endpoint  
**No changes needed:** Daemon core, state store, transport

---

## 5. Frontend Features

### Adding New Pages

**Location:** `frontend/src/pages/`  
**Current:** Dashboard, Controls, TestConsole, PacketInspector (Phase 1)  
**Future:** Configuration, Statistics, LedAnimator, etc.

**Extension:**
1. Create page component in `frontend/src/pages/NewPage.jsx`
2. Import state hooks from `hooks/`
3. Subscribe to WebSocket data
4. Render components
5. Add route in `App.jsx`
6. Add navigation link

**Effort:** ~2-3 days per page  
**No changes needed:** State management, WebSocket, daemon

---

### Adding New UI Components

**Location:** `frontend/src/components/`  
**Pattern:** React functional components with hooks

**Current:** RelayControl, FanControl, LedControl, StatusIndicator, etc.  
**Future:** TemperatureChart, ConfigEditor, AnimationBuilder, etc.

**Extension:**
1. Create component in `frontend/src/components/NewComponent.jsx`
2. Use `useWebSocket()` hook to subscribe to state
3. Use `sendCommand()` to send commands
4. Render UI
5. Import into pages

**Effort:** ~1-2 days per component  
**No changes needed:** WebSocket, daemon, API

---

### Adding Dark Mode / Theme Variants

**Location:** `frontend/src/styles/themes/`  
**Extension:**
1. Create theme CSS file: `frontend/src/styles/themes/dark.css`
2. Define color variables
3. Add theme switcher component
4. Store theme preference in localStorage
5. No daemon changes needed

**Effort:** ~2-3 days  
**No changes needed:** Daemon, state, API

---

## 6. Testing & Validation

### Adding Unit Tests

**Location:** `[component]/__tests__/` or `.test.ts` files  
**Pattern:** Jest test files

**Extension:**
1. Create test file: `firmware/test/test_new_feature.cpp` or `daemon/src/__tests__/new.test.ts`
2. Write test cases
3. Mock external dependencies
4. Run with Jest or firmware test framework
5. Integrate into CI/CD

**Effort:** Varies (same as feature)  
**Pattern:** Test-driven development (write tests first)

---

### Adding Integration Tests

**Location:** `daemon/src/__tests__/integration/`  
**Pattern:** Test multiple components together

**Extension:**
1. Use MockTransport to simulate firmware
2. Create command sequence test
3. Verify state changes and responses
4. Test error handling
5. Test recovery sequences

**Effort:** ~1-2 days per integration test  
**Benefits:** Catch cross-component issues early

---

## 7. Deployment & Operations

### Adding New Deployment Target

**Location:** `scripts/` and `docs/`  
**Current:** Windows, Mac, Linux desktop  
**Future:** Raspberry Pi, Docker container, cloud service

**Extension:**
1. Create deployment script in `scripts/deploy-[target].sh`
2. Document setup in `docs/DEPLOY_[TARGET].md`
3. Test on actual target hardware
4. No code changes usually needed (handles different platforms via abstraction)

**Effort:** ~2-3 days per platform  
**No changes needed:** Daemon, UI core logic

---

## 8. Diagnostics & Observability

### Adding Diagnostic Metrics

**Location:** `daemon/src/core/diagnostics/`  
**Current:** Command latency, queue depth, CRC validation  
**Future:** CPU usage, memory usage, error rates, uptime

**Extension:**
1. Add metric collector in `daemon/src/core/diagnostics/metrics.ts`
2. Compute aggregates (avg, max, percentiles)
3. Expose via `/api/diagnostics` endpoint
4. UI displays metrics (optional)
5. No firmware changes needed

**Effort:** ~1-2 days per metric  
**No changes needed:** Core protocol, command queue, state store

---

## 9. Configuration Management

### Adding Configuration Profiles

**Location:** `daemon/src/core/config/` and `frontend/src/components/ConfigProfiles.jsx`  
**Extension:**
1. Add profile storage in daemon (JSON files)
2. Add profile management commands
3. UI for save/load/delete profiles
4. No firmware changes needed

**Effort:** ~2-3 days  
**No changes needed:** Protocol, command execution

---

### Adding Runtime Tuning

**Location:** `daemon/src/core/` (constants) and UI  
**Extension:**
1. Expose tunable parameters: timeout, retry count, backoff strategy
2. UI allows changing at runtime
3. Changes don't require restart
4. Persist to configuration file

**Effort:** ~1-2 days  
**No changes needed:** Firmware, protocol

---

## 10. Documentation

### Adding New Guides

**Location:** `docs/` and component-specific `docs/`  
**Pattern:** Markdown documentation

**Future Guides:**
- Troubleshooting guide
- Customization guide
- Developer API documentation
- Firmware porting guide (for new microcontroller)

**Extension:**
1. Create `.md` file with clear structure
2. Include examples and links to relevant code
3. Keep consistent with existing documentation style
4. No code changes needed

**Effort:** ~1 day per guide

---

## Extension Point Design Principles

When adding features, follow these principles:

1. **Abstraction:** Hide implementation details behind interfaces (ITransport, BaseController)
2. **Dependency Injection:** Pass dependencies instead of hardcoding them
3. **Plugin Pattern:** Self-registering components (controllers, handlers)
4. **Capability Discovery:** Report capabilities to enable adaptive UI
5. **Configuration-Driven:** Parameterize behavior instead of hardcoding
6. **Separation of Concerns:** Keep layers independent
7. **Minimal Changes:** New features shouldn't require modifying existing code

---

## Forbidden Extension Points

These should NOT be extended without architectural review:

❌ **Modifying binary protocol** (except new commands following existing patterns)  
❌ **Changing daemon core state store** (add to state, don't restructure)  
❌ **Altering ITransport interface** (extend with new transports, don't change interface)  
❌ **Rewriting command queue** (logic proven, don't change)  
❌ **Moving API from WebSocket to REST** (would break real-time guarantees)

---

## Contact & Approval

To add a new feature using an extension point:

1. Identify the appropriate extension point from this document
2. Verify it doesn't violate forbidden extension points
3. Create PR with feature implementation
4. Reference this document in PR description
5. Code review verifies extension point usage
6. Merge and verify on test hardware

For new extension points (architecture changes):
1. Document in ADR (Architecture Decision Record)
2. Stakeholder review and approval required
3. Update ARCHITECTURE_FREEZE.md
4. Update this document (EXTENSION_POINTS.md)

---

**Purpose:** Enable future development without architectural redesign  
**Status:** Frozen (new extension points require ADR)  
**Maintenance:** Update this document as new extension points are designed
