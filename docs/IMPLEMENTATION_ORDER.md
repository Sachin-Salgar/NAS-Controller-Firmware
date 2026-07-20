Document Version: 1.0
Project Version: 1.0
Last Updated: 2026-07-20
Owner: NAS Controller
Status: Frozen
Session Updated: 2026-07-20 - Architecture review complete, shared frozen, CRC16 is Phase 1 Task 2

# Strict Implementation Order - Phase 1 MVP

**Purpose:** Prevent jumping between unrelated tasks. Each layer must be completed and tested before the next layer is built.

**Key Principle:** Each layer is the foundation for the next. Do not skip steps.

---

## 🎯 CURRENT SESSION STATUS (Updated 2026-07-20)

### What's Done ✅
- Architecture reviewed and frozen (v1.0)
- Documentation finalized and synchronized
- Shared package frozen with protocol contracts
- PROJECT_STATUS.md created as permanent project dashboard
- New rules added: Protocol First Rule (11), Documentation Hierarchy (12), Architecture Freeze Rule (13)
- ProtocolLimits deprecated (kept for backward compatibility)
- ProtocolFeatureFlags merged to single enum with bitmask values
- ProtocolTiming confirmed as daemon responsibility (not protocol contract)

### What's Next ⏭️
**Phase 1 Task 2: CRC16 Implementation**
- Location: `daemon/src/core/protocol/crc16.ts`
- Tests: `daemon/src/core/protocol/__tests__/crc16.test.ts`
- Verify CRC16-CCITT matches firmware calculations
- Use test vectors from PROTOCOL_SPEC.md
- **Do NOT start this task in this session**

### For New Sessions 📖
Start by reading:
1. docs/PROJECT_STATUS.md (current status and next steps) ← **START HERE**
2. README.md (what is this)
3. docs/PROJECT_OVERVIEW.md (architecture)
4. docs/IMPLEMENTATION_ORDER.md (this file - build order)
5. shared/docs/PROTOCOL_SPEC.md (protocol details)
6. docs/PROJECT_RULES.md (governance - all 13 rules)

---

---

## Build Order: Protocol → Transport → Daemon Core → API → UI

```
┌─────────────────────────────────────────────────────────────┐
│  LAYER 5: React Dashboard (UI)                              │
│  Depends on: WebSocket API (Layer 4)                        │
└─────────────────────────────────────────────────────────────┘
                              ↑
┌─────────────────────────────────────────────────────────────┐
│  LAYER 4: WebSocket API (Express + Socket.io)               │
│  Depends on: Daemon Core (Layer 3)                          │
└─────────────────────────────────────────────────────────────┘
                              ↑
┌─────────────────────────────────────────────────────────────┐
│  LAYER 3: Daemon Core (NASController.Core Library)          │
│  Depends on: Protocol (Layer 1) + Transport (Layer 2)       │
└─────────────────────────────────────────────────────────────┘
                  ↑                           ↑
┌───────────────────────┐  ┌──────────────────────────┐
│  LAYER 2: Transport   │  │  LAYER 1: Protocol       │
│  (Serial, Mock, TCP)  │  │  (Encoder/Decoder/CRC)   │
│  Depends on: Protocol │  │  Depends on: (nothing)   │
└───────────────────────┘  └──────────────────────────┘
```

---

## Phase 1: Week 1 (Protocol & Transport)

### ✔ Week 1.1: Shared Types & Protocol Library

**Deliverable:** All protocol types and encoding logic tested

**Tasks (in order):**

1. ✔ **Create shared type definitions** (shared/src/)
   - `protocol.ts` - Packet types, commands, responses
   - `errors.ts` - Error classifications
   - `types.ts` - Common types (relay, fan, LED, etc.)
   - **Effort:** 1 day
   - **Test:** Unit tests for type structures
   - **Dependency:** None
   - **Acceptance:** All types are exported and importable

2. ✔ **Implement CRC16 algorithm** (daemon/src/core/protocol/)
   - CRC-16-CCITT implementation
   - Test vectors from PROTOCOL_SPEC.md
   - **Effort:** 1 day
   - **Test:** Unit tests (100% coverage, test vectors)
   - **Dependency:** shared types
   - **Acceptance:** CRC16 matches firmware calculations exactly

3. ✔ **Implement packet encoder** (daemon/src/core/protocol/)
   - Command → binary packet conversion
   - Sequence number insertion
   - CRC calculation
   - **Effort:** 1 day
   - **Test:** Unit tests (sample commands)
   - **Dependency:** CRC16, types
   - **Acceptance:** Encoded packets match wire format

4. ✔ **Implement packet decoder** (daemon/src/core/protocol/)
   - Binary packet → command/response conversion
   - Header validation (0x55AA)
   - CRC validation
   - **Effort:** 1 day
   - **Test:** Unit tests, round-trip (encode then decode)
   - **Dependency:** CRC16, encoder
   - **Acceptance:** Decoded packets match original commands

5. ✔ **Implement ACK/NAK handling** (daemon/src/core/protocol/)
   - Detect ACK (Cmd bit 0x80)
   - Detect NAK (Cmd bit 0x40)
   - Extract error codes
   - **Effort:** 1 day
   - **Test:** Unit tests for different ACK/NAK patterns
   - **Dependency:** Decoder
   - **Acceptance:** Correctly identifies all response types

**Subtotal Protocol: 5 days**

---

### ✔ Week 1.2: Transport Layer Abstraction

**Deliverable:** ITransport interface with Serial and Mock implementations

**Tasks (in order):**

6. ✔ **Define ITransport interface** (daemon/src/core/transport/)
   - `connect()`
   - `send(data: Buffer)`
   - `receive(): Promise<Buffer>`
   - `disconnect()`
   - Events: onConnect, onDisconnect, onData, onError
   - **Effort:** 1 day
   - **Test:** TypeScript compilation
   - **Dependency:** shared types
   - **Acceptance:** Interface is clean and testable

7. ✔ **Implement SerialTransport** (daemon/src/core/transport/)
   - USB CDC connection via serialport npm
   - Auto-detect COM port
   - Baud rate: 115200
   - Error handling (disconnect, timeout)
   - **Effort:** 2 days
   - **Test:** Integration test with MockTransport first, then real hardware
   - **Dependency:** ITransport interface
   - **Acceptance:** Connects to ESP32 via USB, sends/receives packets

8. ✔ **Implement MockTransport** (daemon/src/core/transport/)
   - In-process firmware simulator
   - Implements all firmware commands
   - Deterministic for testing
   - **Effort:** 2 days
   - **Test:** Unit tests (command responses)
   - **Dependency:** ITransport interface, protocol encoder/decoder
   - **Acceptance:** All firmware commands work without hardware

9. ✔ **Implement error classification** (daemon/src/core/protocol/)
   - 6 error types: Transport, Protocol, Firmware, Validation, User, Internal
   - Map errors to appropriate type
   - **Effort:** 1 day
   - **Test:** Unit tests
   - **Dependency:** types, ACK/NAK handling
   - **Acceptance:** All errors are classified correctly

10. ✔ **Implement retry logic** (daemon/src/core/protocol/)
    - Timeout: 1000ms per command
    - Retry: up to 3 times
    - Backoff: exponential (1s, 2s, 3s)
    - **Effort:** 1 day
    - **Test:** Unit tests with timer mocks
    - **Dependency:** Transport, error classification
    - **Acceptance:** Timeouts trigger retries, 3rd attempt = FAILED

**Subtotal Transport: 7 days**

**Week 1 Total: 12 days** (2+ weeks solo) → Does week 1, some of week 2

---

## Phase 1: Week 2 (Daemon Core)

### ✔ Week 2.1: Command Queue & State Store

**Deliverable:** Command queue with state machine, working with MockTransport

**Tasks (in order):**

11. ✔ **Implement command queue** (daemon/src/core/queue/)
    - FIFO queue
    - State machine: Queued → Sending → Waiting ACK → Completed/Timeout/Failed
    - Sequence number tracking
    - **Effort:** 2 days
    - **Test:** Unit tests (queue operations, state transitions)
    - **Dependency:** Transport, protocol, error classification
    - **Acceptance:** Commands execute serially, states are accurate

12. ✔ **Implement central state store** (daemon/src/core/state/)
    - FirmwareState (capabilities, version)
    - HardwareState (relay, fan, LED, temp, drive status)
    - ConfigState (persistent settings)
    - StatisticsState (uptime, command count, errors)
    - Subscription pattern (publish-subscribe)
    - **Effort:** 2 days
    - **Test:** Unit tests (state updates, subscriptions)
    - **Dependency:** shared types
    - **Acceptance:** State updates trigger subscriber notifications

13. ✔ **Implement capability discovery** (daemon/src/core/startup/)
    - Startup sequence: PING → GET_CAPABILITIES → GET_ALL_CONFIG → GET_ALL_STATUS
    - Parse capability response
    - Populate state store
    - Handle errors gracefully
    - **Effort:** 1 day
    - **Test:** Integration test with MockTransport
    - **Dependency:** Command queue, state store, protocol
    - **Acceptance:** Startup completes, state store is populated

14. ✔ **Implement event bus & logging** (daemon/src/core/events/)
    - Event log with timestamps
    - Event types: relay, fan, LED, temp, error, connection
    - Subscriber pattern
    - **Effort:** 1 day
    - **Test:** Unit tests
    - **Dependency:** State store
    - **Acceptance:** All state changes logged with timestamps

15. ✔ **Implement packet inspector** (daemon/src/core/diagnostics/)
    - Capture TX/RX packets
    - Format as hex dump with timestamps
    - Decode packet meaning
    - Filtering and export
    - **Effort:** 1 day
    - **Test:** Unit tests
    - **Dependency:** Protocol decoder, event bus
    - **Acceptance:** Packets are captured and decoded correctly

**Subtotal Daemon Core: 7 days**

---

### ✔ Week 2.2: Controllers & Diagnostics

**Deliverable:** Plugin-style controllers, working with state store

**Tasks (in order):**

16. ✔ **Implement base controller pattern** (daemon/src/core/controllers/)
    - BaseController abstract class
    - Command registration
    - State subscription
    - Error handling
    - **Effort:** 1 day
    - **Test:** Unit tests
    - **Dependency:** State store, command queue, event bus
    - **Acceptance:** Controllers can be self-registering

17. ✔ **Implement relay controller** (daemon/src/core/controllers/)
    - CMD_RELAY_SET handler
    - CMD_RELAY_GET handler
    - Update state on success
    - Handle errors
    - **Effort:** 1 day
    - **Test:** Integration tests with MockTransport
    - **Dependency:** Base controller
    - **Acceptance:** Relay commands work end-to-end

18. ✔ **Implement fan controller** (daemon/src/core/controllers/)
    - CMD_FAN_SET_SPEED handler
    - CMD_FAN_GET handler
    - CMD_FAN_SET_MODE handler
    - Validation (0-100 range)
    - **Effort:** 1 day
    - **Test:** Integration tests
    - **Dependency:** Base controller
    - **Acceptance:** Fan commands work end-to-end

19. ✔ **Implement LED controller** (daemon/src/core/controllers/)
    - CMD_LED_SET_COLOR handler
    - CMD_LED_SET_BRIGHTNESS handler
    - CMD_LED_SET_ANIMATION handler
    - **Effort:** 1 day
    - **Test:** Integration tests
    - **Dependency:** Base controller
    - **Acceptance:** LED commands work end-to-end

20. ✔ **Implement diagnostics & metrics** (daemon/src/core/diagnostics/)
    - Command latency tracking
    - Queue depth monitoring
    - Error rate calculation
    - Uptime tracking
    - **Effort:** 1 day
    - **Test:** Unit tests
    - **Dependency:** Event bus, state store
    - **Acceptance:** Metrics are accurate

**Subtotal Controllers: 5 days**

**Week 2 Total: 12 days** (full week 2 + part of week 3)

---

## Phase 1: Week 3 (API & Frontend)

### ✔ Week 3.1: WebSocket API Layer

**Deliverable:** Express + Socket.io server wrapping NASController.Core

**Tasks (in order):**

21. ✔ **Set up Express server** (daemon/src/api/)
    - HTTP server listening on localhost:3000
    - CORS configuration
    - Error handling middleware
    - Logging middleware
    - **Effort:** 1 day
    - **Test:** Manual (curl requests)
    - **Dependency:** None
    - **Acceptance:** Server starts and responds to requests

22. ✔ **Set up Socket.io** (daemon/src/api/)
    - WebSocket server on same port
    - Connection/disconnect handling
    - Message routing
    - Error handling
    - **Effort:** 1 day
    - **Test:** Manual (browser console)
    - **Dependency:** Express
    - **Acceptance:** WebSocket connects from browser

23. ✔ **Implement WebSocket command router** (daemon/src/api/websocket.ts)
    - Message format: `{ type: "relay_on", id: 1, msgId: "abc123" }`
    - Dispatch to appropriate controller
    - Send response back to client
    - Handle errors
    - **Effort:** 2 days
    - **Test:** Integration tests (send command, verify state change)
    - **Dependency:** Controllers, state store
    - **Acceptance:** Commands sent via WebSocket execute on firmware

24. ✔ **Implement state broadcaster** (daemon/src/api/websocket.ts)
    - Subscribe to state store changes
    - Broadcast to all connected clients
    - Format: `{ type: "state_update", data: {...} }`
    - Throttle to prevent flooding
    - **Effort:** 1 day
    - **Test:** Integration tests (multiple clients)
    - **Dependency:** WebSocket router, state store
    - **Acceptance:** All clients receive state updates

25. ✔ **Implement REST endpoints** (daemon/src/api/rest.ts)
    - GET /api/state → Current state
    - GET /api/logs → Event log
    - GET /api/diagnostics → Metrics
    - GET /api/config/backup → Config JSON
    - **Effort:** 1 day
    - **Test:** Manual (curl requests)
    - **Dependency:** Express, controllers
    - **Acceptance:** All endpoints return correct data

**Subtotal API: 6 days**

---

### ✔ Week 3.2: React Dashboard

**Deliverable:** Working web UI with Dashboard, Controls, Test Console

**Tasks (in order):**

26. ✔ **Set up React + Vite + TailwindCSS** (frontend/)
    - Vite project structure
    - TailwindCSS configuration
    - React Router setup
    - Socket.io client library
    - **Effort:** 1 day
    - **Test:** Manual (yarn dev)
    - **Dependency:** None
    - **Acceptance:** React app loads in browser

27. ✔ **Implement WebSocket client hook** (frontend/src/hooks/useWebSocket.ts)
    - Connect to daemon Socket.io
    - Send command: `emit(type, data)`
    - Receive state updates
    - Handle reconnection
    - **Effort:** 1 day
    - **Test:** Manual + unit tests
    - **Dependency:** Socket.io client
    - **Acceptance:** Commands sent, state updates received

28. ✔ **Implement Dashboard page** (frontend/src/pages/Dashboard.jsx)
    - Display power status
    - Display temperature
    - Display fan speed
    - Display USB connection status
    - Display drive indicators
    - System health badge
    - **Effort:** 1 day
    - **Test:** Manual (verify updates in real-time)
    - **Dependency:** useWebSocket hook
    - **Acceptance:** Dashboard displays current hardware state

29. ✔ **Implement Control Panel page** (frontend/src/pages/Controls.jsx)
    - Relay ON/OFF buttons
    - All ON / All OFF buttons
    - Fan speed slider
    - Fan mode selector
    - LED brightness slider
    - LED animation selector
    - LED color picker
    - **Effort:** 2 days
    - **Test:** Manual (verify commands execute on hardware)
    - **Dependency:** Dashboard components
    - **Acceptance:** All controls work, hardware responds

30. ✔ **Implement Test Console page** (frontend/src/pages/TestConsole.jsx)
    - Command input (hex)
    - Send button
    - Response display (hex)
    - Command history (last 50)
    - Quick test buttons (relays, fans, LEDs)
    - **Effort:** 1 day
    - **Test:** Manual (send various commands)
    - **Dependency:** useWebSocket hook
    - **Acceptance:** Raw commands work, history displays

31. ✔ **Implement Packet Inspector page** (frontend/src/pages/PacketInspector.jsx)
    - Live TX/RX packet display
    - Hex dump with timestamps
    - Decoded packet meaning
    - Filter by type
    - Export button
    - **Effort:** 1 day
    - **Test:** Manual (watch packets during commands)
    - **Dependency:** Daemon packet capture
    - **Acceptance:** Packets displayed in real-time

32. ✔ **Implement error handling & toasts** (frontend/src/components/Toast.jsx)
    - Error toast on command failure
    - Success toast on command success
    - Connection status indicator
    - Offline warning
    - **Effort:** 1 day
    - **Test:** Manual (disconnect USB, trigger errors)
    - **Dependency:** useWebSocket hook
    - **Acceptance:** Errors displayed to user

**Subtotal Frontend: 8 days**

**Week 3 Total: 14 days** (full week 3 + part of week 4)

---

## Phase 1: Week 4 (Testing & Validation)

### ✔ Week 4.1: Testing & Cross-Platform Validation

**Deliverable:** MVP works on Windows, Mac, Linux

**Tasks (in order):**

33. ✔ **Run unit test suite** (jest)
    - Protocol tests (encoder, decoder, CRC)
    - Transport tests (serial, mock)
    - Command queue tests
    - State store tests
    - **Effort:** 1 day
    - **Test:** jest --coverage
    - **Target:** 100% for protocol & transport, 80%+ for core
    - **Acceptance:** All tests pass, coverage above targets

34. ✔ **Run integration tests** (jest)
    - MockTransport + command queue
    - Full startup sequence
    - Relay, fan, LED commands
    - Error recovery
    - **Effort:** 1 day
    - **Test:** jest --testPathPattern=integration
    - **Acceptance:** All integration tests pass

35. ✔ **Manual testing on Windows** (with real hardware)
    - USB connection
    - Relay on/off
    - Fan speed adjustment
    - LED color/brightness
    - Test console
    - Packet inspector
    - **Effort:** 2 days
    - **Test:** Live hardware testing
    - **Acceptance:** All features work smoothly

36. ✔ **Manual testing on Mac** (with real hardware)
    - Same as Windows
    - Verify COM port detection
    - **Effort:** 1 day
    - **Test:** Live hardware testing
    - **Acceptance:** Works on Mac without modifications

37. ✔ **Manual testing on Linux** (with real hardware)
    - Same as Windows
    - Verify /dev/ttyUSB* detection
    - **Effort:** 1 day
    - **Test:** Live hardware testing
    - **Acceptance:** Works on Linux without modifications

38. ✔ **Performance testing**
    - Command latency (target: <500ms)
    - Queue throughput (commands/sec)
    - Memory usage
    - CPU usage under load
    - **Effort:** 1 day
    - **Test:** Load tests, latency measurements
    - **Acceptance:** Performance meets targets

39. ✔ **Edge case testing**
    - USB disconnect during command
    - Multiple commands queued
    - Rapid slider changes
    - Invalid commands
    - Firmware errors
    - **Effort:** 1 day
    - **Test:** Manual + automated scenarios
    - **Acceptance:** App handles errors gracefully

**Subtotal Testing: 8 days**

**Week 4 Total: 8 days**

---

## Phase 1 Summary

| Phase | Effort | Cumulative |
|-------|--------|-----------|
| Protocol & Transport | 12 days | 12 days |
| Daemon Core | 12 days | 24 days |
| WebSocket API | 6 days | 30 days |
| React Frontend | 8 days | 38 days |
| Testing & Validation | 8 days | 46 days |

**Total Phase 1 Effort:** ~46 person-days (~9-10 weeks solo, ~4-5 weeks with 2 devs)

**Estimated Timeline:**
- Backend dev (1 person): Protocol + Transport + Daemon + API = 38 days = ~7-8 weeks
- Frontend dev (1 person in parallel): React UI = 8 days = ~2 weeks
- Overlap testing: 8 days = ~2 weeks
- **With 2 devs in parallel: ~5-6 weeks total**
- **With 1 dev solo: ~9-10 weeks total**

---

## Do NOT Skip Steps

⚠️ **Common Mistakes:**

❌ Don't start frontend before API is working  
❌ Don't start daemon core before transport is working  
❌ Don't start transport before protocol is tested  
❌ Don't test on real hardware until MockTransport works  
❌ Don't optimize before implementation is complete  

✅ **Do this instead:**

✔️ Protocol tests must pass (100% coverage)  
✔️ MockTransport must work with all firmware commands  
✔️ Command queue must execute serially, state accurate  
✔️ WebSocket API must handle all commands  
✔️ Frontend must work with daemon before touching hardware  
✔️ Test on real hardware only after MockTransport validates logic  

---

## Checkpoints

After each week, verify:

**End of Week 1 (Protocol & Transport):**
- ✔ All unit tests pass
- ✔ MockTransport responds to all firmware commands
- ✔ Packet encoder/decoder round-trip works

**End of Week 2 (Daemon Core):**
- ✔ Startup sequence completes with MockTransport
- ✔ Command queue executes commands serially
- ✔ State store updates accurately
- ✔ All controllers work with MockTransport

**End of Week 3 (API & Frontend):**
- ✔ WebSocket commands reach daemon (tested with MockTransport)
- ✔ Dashboard displays state updates
- ✔ Controls send commands and receive feedback
- ✔ Packet inspector shows TX/RX

**End of Week 4 (Testing & Validation):**
- ✔ Unit and integration tests pass
- ✔ Manual testing on Windows, Mac, Linux succeeds
- ✔ All success criteria from WEB_INTERFACE_PLAN.md met
- ✔ Performance meets targets

---

## If You Get Blocked

1. **Refer to the layer diagram** - What layer is missing?
2. **Check dependencies** - What must be completed first?
3. **Review sequence diagrams** - What is the expected flow?
4. **Use MockTransport** - Test logic without hardware
5. **Write unit tests** - Isolate the problem
6. **Don't skip ahead** - Complete current layer first

---

**Purpose:** Prevent context switching, ensure quality, maintain progress visibility  
**Status:** Frozen (changes require stakeholder consensus)  
**Maintenance:** Update as actual progress deviates from estimates
