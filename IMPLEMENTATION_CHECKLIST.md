# Phase 1 Implementation Checklist

**Status:** Planning Complete → Ready for Development  
**Target:** MVP (Dashboard + Controls + Test Console)  
**Estimated Duration:** 2-3 weeks

---

## Architecture & Planning ✅ COMPLETE

- [x] Overall system architecture defined (3-layer: UI → Daemon → Firmware)
- [x] Technology stack finalized (TypeScript + React + Express + Socket.io)
- [x] Build order established (Protocol → Transport → Core → API → UI)
- [x] Repository structure planned (firmware, daemon, frontend, shared, docs)
- [x] Documentation organization (colocate docs with code)
- [x] ADRs written (WebSocket-first, library-first daemon)
- [x] Protocol specification created (PROTOCOL_SPEC.md)
- [x] Guiding principles documented

---

## Pre-Development Setup (Next)

### Repository Structure
- [ ] Create `daemon/` directory
- [ ] Create `frontend/` directory
- [ ] Create `shared/` directory
- [ ] Organize existing firmware docs
- [ ] Move/create docs in component directories

### TypeScript Configuration
- [ ] Create root `tsconfig.json`
- [ ] Create `daemon/tsconfig.json` (strict mode)
- [ ] Create `frontend/tsconfig.json` (React mode)
- [ ] Create `shared/tsconfig.json` (library mode)

### Package Setup
- [ ] Create `daemon/package.json` (Express, Socket.io, serialport, Jest)
- [ ] Create `frontend/package.json` (React, Vite, TailwindCSS, Socket.io client)
- [ ] Create `shared/package.json` (TypeScript types only)

### Shared Types
- [ ] Create `shared/src/protocol.ts` (packet types from PROTOCOL_SPEC)
- [ ] Create `shared/src/commands.ts` (command definitions)
- [ ] Create `shared/src/events.ts` (event types)
- [ ] Create `shared/src/capabilities.ts` (firmware capability schema)
- [ ] Create `shared/src/config.ts` (configuration schema)

### Development Tools
- [ ] Set up linting (ESLint)
- [ ] Set up formatting (Prettier)
- [ ] Set up pre-commit hooks
- [ ] Create GitHub Actions workflow (CI/CD)
- [ ] Set up test coverage tracking

---

## Phase 1: Protocol Library (Week 1)

### Protocol Encoder/Decoder
- [ ] Packet structure (header, seq, cmd, len, payload, crc, footer)
- [ ] Encode command → binary packet
- [ ] Decode binary packet → command
- [ ] Unit tests (100% coverage)

### CRC16 Implementation
- [ ] CRC-16-CCITT algorithm
- [ ] Validation on incoming packets
- [ ] Unit tests (test vectors from PROTOCOL_SPEC)

### Sequence Number Manager
- [ ] Track outgoing sequence numbers (0x00-0xFF, rollover)
- [ ] Match response to request by sequence
- [ ] Unit tests

### ACK/NAK Handling
- [ ] ACK response detection (Cmd bit pattern 0x80)
- [ ] NAK response detection (Cmd bit pattern 0x40)
- [ ] Error code extraction
- [ ] Unit tests

### Retry Logic
- [ ] Timeout: 1000 ms per command
- [ ] Retry: Up to 3 times with backoff
- [ ] Mark command as FAILED after 3 retries
- [ ] Unit tests

---

## Phase 1: Transport Layer (Week 1)

### ITransport Interface
- [ ] Abstract base interface (connect, send, receive, disconnect)
- [ ] Event emitters (onConnect, onDisconnect, onData)
- [ ] TypeScript types

### SerialTransport Implementation
- [ ] USB CDC connection via serialport npm
- [ ] Baud rate: 115200
- [ ] Auto-detect COM port (Windows: COM*, Linux: /dev/ttyUSB*, Mac: /dev/tty.*)
- [ ] Handle USB disconnection gracefully
- [ ] Unit tests (mock serialport)

### MockTransport Implementation
- [ ] Simulated firmware responses
- [ ] Configurable delays (simulate real device latency)
- [ ] Fake hardware state (relay states, temperature, etc.)
- [ ] Unit tests

### Auto-Reconnect
- [ ] Detect USB disconnection
- [ ] Retry connection every 1 second
- [ ] Success notification
- [ ] Unit tests

### Error Classification
- [ ] Transport errors (USB unavailable, timeout)
- [ ] Protocol errors (CRC failed, invalid packet)
- [ ] Firmware errors (command not supported, invalid parameter)
- [ ] Classification functions
- [ ] Unit tests

---

## Phase 1: Daemon Core Library (Week 1-2)

### Command Queue & State Machine
- [ ] Queue data structure (FIFO)
- [ ] States: Queued → Sending → Waiting ACK → Completed/Timeout/Failed
- [ ] State transitions
- [ ] Timeout handling
- [ ] Unit tests

### Central State Store
- [ ] FirmwareState (capabilities, version, uptime)
- [ ] HardwareState (relay states, fan speeds, temperatures, drive status)
- [ ] ConfigState (current configuration)
- [ ] StatisticsState (uptime, boot count, event count)
- [ ] Subscription system (publish-subscribe)
- [ ] Unit tests

### Event Bus & Packet Inspector
- [ ] Event log (circular buffer, last 1000 events)
- [ ] Log entry: timestamp, type, data, raw packet (if applicable)
- [ ] Packet inspector: capture TX/RX packets
- [ ] Decode packet meaning
- [ ] Unit tests

### Protocol Translation
- [ ] JSON command → binary packet (uses Protocol Library)
- [ ] Binary response → JSON event
- [ ] Examples: { relay_id: 1, state: "on" } → packet
- [ ] Unit tests

### Capability Discovery
- [ ] Send GET_CAPABILITIES command on startup
- [ ] Parse response (drive count, relay count, LED count, etc.)
- [ ] Update FirmwareState.capabilities
- [ ] Use capabilities to configure UI dynamically
- [ ] Unit tests with MockTransport

### Atomic Configuration Transactions
- [ ] CONFIG_BEGIN_TRANSACTION
- [ ] Send config changes
- [ ] CONFIG_COMMIT_TRANSACTION (commit or rollback)
- [ ] Timeout: auto-rollback after 10 seconds
- [ ] Unit tests

### Reconnection Sequence
- [ ] PING
- [ ] GET_CAPABILITIES
- [ ] GET_ALL_CONFIG
- [ ] GET_ALL_STATUS
- [ ] Notify subscribers (UI) when ready
- [ ] Unit tests

### Diagnostics Mode
- [ ] Enable/disable flag (env var or config)
- [ ] Log every packet sent/received
- [ ] Record timing information
- [ ] Track queue state transitions
- [ ] Calculate performance metrics (latency, throughput)
- [ ] Unit tests

### Performance Metrics
- [ ] Average command latency
- [ ] Max command latency
- [ ] Commands per second
- [ ] Queue depth (current, peak)
- [ ] Packet loss count
- [ ] Reconnect count
- [ ] Uptime
- [ ] Expose via function call or event

### Controllers (Plugin Architecture)
- [ ] Base controller interface
- [ ] Relay controller (send relay commands, track state)
- [ ] Fan controller (set/get speed)
- [ ] LED controller (set animation mode, color)
- [ ] Temperature controller (read temps, log history)
- [ ] Drive controller (track drive status)
- [ ] System controller (ping, reset, capabilities)
- [ ] Auto-registration (no manual wiring needed)
- [ ] Unit tests for each

### Integration Tests
- [ ] Full daemon core + MockTransport (no USB)
- [ ] Test complete workflows:
  - Connect → Discover capabilities → Get status → Send relay command → Verify state update
  - USB disconnect → Auto-reconnect → State resync
  - Configuration transaction (begin → change → commit)
  - Packet loss/timeout → Retry logic
- [ ] Performance tests (measure latency, throughput)

---

## Phase 1: WebSocket API Layer (Week 2)

### Express Server Setup
- [ ] Server initialization
- [ ] CORS configuration
- [ ] Static file serving (React build)
- [ ] Health check endpoint

### Socket.io Connection Manager
- [ ] Connection handling
- [ ] Connection tracking
- [ ] Graceful disconnection
- [ ] Error handling

### WebSocket Command Router
- [ ] Route incoming commands to daemon core
- [ ] Map Socket.io message → NASController.Core method
- [ ] Example: socket.on('relay_on', id) → controller.relayOn(id)
- [ ] Error handling (invalid command, out of range, etc.)
- [ ] Response back to client

### WebSocket Broadcaster
- [ ] Subscribe to daemon core state store
- [ ] Broadcast state changes to all connected clients
- [ ] Throttle updates if necessary (optional, likely not needed)
- [ ] Error handling

### REST Endpoints (Limited)
- [ ] GET `/api/logs` → Download event log (JSON or CSV)
- [ ] GET `/api/diagnostics` → Performance metrics
- [ ] POST `/api/config/backup` → Export configuration
- [ ] POST `/api/config/restore` → Import configuration
- [ ] GET `/api/health` → Server health check

### Packet Inspector Endpoint
- [ ] GET `/api/packet-history` → Last 1000 packets
- [ ] GET `/api/packet-history?filter=RELAY_SET` → Filtered packets
- [ ] Expose via Socket.io or REST

### Documentation
- [ ] Document all Socket.io message types
- [ ] Document all REST endpoints
- [ ] Example requests/responses
- [ ] Error codes and meanings

---

## Phase 1: React Frontend (Week 2-3)

### Project Setup
- [ ] Vite configuration
- [ ] React 18 setup
- [ ] TailwindCSS setup
- [ ] Socket.io client setup
- [ ] TypeScript strict mode

### Service Layer
- [ ] WebSocket connection manager (`services/websocket.ts`)
- [ ] Command sender (wraps Socket.io emit)
- [ ] Event listener (subscribes to state updates)
- [ ] Automatic reconnection on disconnect
- [ ] Type-safe wrappers

### State Management (Hooks)
- [ ] useWebSocket hook (connection status)
- [ ] useHardwareState hook (relay/fan/LED/temp states)
- [ ] useEventLog hook (event history)
- [ ] useMetrics hook (performance data)
- [ ] Custom hooks for each major state slice

### Pages

#### Dashboard Page
- [ ] Real-time status display
  - [ ] Power status (ON/OFF)
  - [ ] Temperature reading (with trend)
  - [ ] Fan speed (visual gauge)
  - [ ] USB connection status (indicator)
  - [ ] Drive presence indicators (grid of 4-8 drives)
- [ ] System health badge (Healthy/Warning/Critical)
- [ ] Last update timestamp
- [ ] Auto-refresh (configurable, default 1s)
- [ ] Unit tests (snapshot tests)

#### Control Panel Page
- [ ] Relay controls
  - [ ] Individual relay ON/OFF buttons (grid)
  - [ ] All ON / All OFF buttons
  - [ ] Real-time state indicators (LED indicators)
- [ ] Fan control
  - [ ] Speed slider (0-100%)
  - [ ] Mode selector (Manual/Auto/Off)
  - [ ] Current speed readout
  - [ ] Visual gauge
- [ ] LED controls
  - [ ] Brightness slider
  - [ ] Animation selector (dropdown with options)
  - [ ] Animation preview (optional)
- [ ] Confirm dialogs for dangerous operations (optional for MVP)
- [ ] Unit tests

#### Test Console Page
- [ ] Command input field (textarea)
- [ ] Hex dump output
- [ ] Command history (last 50 commands)
- [ ] Quick test buttons
  - [ ] "Test all relays" (turn each on/off)
  - [ ] "Test all fans" (ramp speed 0→100)
  - [ ] "Test LED" (run animation)
  - [ ] "Read all sensors" (temperature, etc.)
- [ ] Clear history button
- [ ] Export history button
- [ ] Unit tests

#### Packet Inspector Page
- [ ] Real-time TX/RX packet display (live table)
- [ ] Columns: Timestamp, Direction (TX/RX), Hex Dump, Decoded Meaning
- [ ] Filtering by packet type (optional for MVP)
- [ ] Pause/resume live updates
- [ ] Clear history
- [ ] Export as CSV
- [ ] Tooltip on hover showing detailed packet info
- [ ] Unit tests

### Components

#### RelayControl
- [ ] Individual relay button with state indicator
- [ ] On/off toggle
- [ ] Loading state (while command pending)
- [ ] Error state (if command failed)

#### FanControl
- [ ] Slider (0-100%)
- [ ] Mode selector
- [ ] Current speed display
- [ ] Visual gauge (animated)

#### LEDControl
- [ ] Animation selector dropdown
- [ ] Brightness slider
- [ ] Preview (optional)

#### StatusIndicator
- [ ] Connection status (Connected/Disconnected/Connecting)
- [ ] Color coded (green/red/yellow)

#### TemperatureChart
- [ ] Simple line chart (optional, can use recharts or react-chartjs)
- [ ] Last 60 readings
- [ ] Min/max/average labels

#### EventLog
- [ ] Table of recent events
- [ ] Sortable by timestamp/type
- [ ] Filter by event type (optional)

### Layout & Navigation
- [ ] Header (app title, connection status)
- [ ] Sidebar (page navigation)
- [ ] Main content area
- [ ] Responsive design (mobile-friendly but not required for MVP)
- [ ] Dark/light mode (optional)

### Error Handling
- [ ] Display connection errors to user
- [ ] Display command failures
- [ ] Graceful degradation if features unavailable
- [ ] Toast notifications for important events

### Testing
- [ ] Unit tests for components (jest + React Testing Library)
- [ ] Integration tests (mock Socket.io)
- [ ] Manual testing in browser (dev server)

---

## Phase 1: Testing & Validation (Week 3)

### Unit Tests
- [ ] Protocol library: 100% coverage
- [ ] Transport layer: 100% coverage
- [ ] Daemon core: >90% coverage
- [ ] Frontend components: >80% coverage

### Integration Tests
- [ ] Daemon + MockTransport: Full workflows
- [ ] Frontend + mock daemon: Socket.io integration
- [ ] End-to-end: Real ESP32 (manual)

### Manual Testing
- [ ] All pages load and display correctly
- [ ] All controls respond to user input
- [ ] Real-time updates work
- [ ] Packet inspector shows correct packets
- [ ] Error handling graceful
- [ ] Cross-browser testing (Chrome, Firefox, Safari)
- [ ] Performance acceptable

### Hardware Testing
- [ ] Connect real ESP32 via USB
- [ ] Run test console to verify all commands
- [ ] Dashboard displays correct hardware state
- [ ] Controls actually turn relays on/off, adjust fan, etc.
- [ ] Packet inspector shows actual communication

---

## Deployment (Optional for MVP)

- [ ] Package daemon (bundled Node binary)
- [ ] Create installer script (Windows, Mac, Linux)
- [ ] Documentation for installation
- [ ] Daemon auto-starts on system boot (optional)

---

## Documentation (Throughout)

- [ ] Daemon ARCHITECTURE.md (updated after implementation)
- [ ] Frontend WEB_INTERFACE_PLAN.md (updated after implementation)
- [ ] API documentation (WebSocket messages)
- [ ] Component documentation (frontend components)
- [ ] CHANGELOG.md entry
- [ ] Contributing guide (if not already done)

---

## Success Criteria for Phase 1 MVP

- [ ] Dashboard shows real-time hardware status
- [ ] All controls (relay, fan, LED) work via web UI
- [ ] Test console sends/receives commands
- [ ] Packet inspector displays all communication
- [ ] No crashes or unhandled errors
- [ ] Latency acceptable (<500ms for user commands)
- [ ] Works on Windows, Mac, Linux
- [ ] Can be started with single command
- [ ] Clear error messages for user
- [ ] Code is tested and documented

---

## Notes

- **Build bottom-up:** Complete protocol & transport before daemon core
- **Test independently:** Each layer testable without next layer
- **Use MockTransport:** Develop without USB connected
- **Commit regularly:** Small, logical commits
- **Update docs:** As you implement, update component docs
- **Pair programming:** Complex features benefit from discussion
- **Code review:** Team reviews before merge

---

**Target:** All Phase 1 boxes checked by end of Week 3  
**Status:** Ready for development team  
**Next:** Execute implementation
