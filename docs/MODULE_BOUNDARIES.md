# Module Boundaries

For each major module: define responsibilities, inputs, outputs, dependencies, public API, and internal API.

---

## Protocol Layer

### Responsibilities
- Encode web commands to binary packets
- Decode binary packets from firmware
- CRC16 calculation and validation
- Sequence number management
- ACK/NAK handling
- Timeout and retry logic

### Inputs
- Web command objects (from command queue)
- Binary packets (from serial)

### Outputs
- Binary packets (to serial)
- Decoded command objects (to state store)
- ACK/NAK responses

### Dependencies
- **ONLY:** Standard library (TypeScript/C++ std)
- **NOT:** Firmware code, UI code, Express, Socket.io
- **NOT:** State store or any external state

### Public API
```typescript
encodePacket(command: Command): Uint8Array;
decodePacket(data: Uint8Array): Packet;
calculateCRC16(data: Uint8Array): number;
isValidPacket(packet: Packet): boolean;
```

### Internal API
```typescript
_encodeHeader(): Uint8Array;
_encodePayload(cmd: Command): Uint8Array;
_validateSequence(seq: number): boolean;
_buildAckPacket(ackId: number): Packet;
```

### Testing
✅ Unit testable without hardware
✅ 100% of paths covered
✅ Test encoder/decoder independently
✅ Mock transport for integration tests

---

## Transport Layer

### Responsibilities
- Abstract communication method (serial, TCP, mock, Bluetooth)
- Connect/disconnect
- Send packets
- Receive packets
- Error handling (connection lost, timeout)
- Automatic reconnection

### Inputs
- Configuration (port, baudrate, timeout)
- Packets to send
- Transport-specific errors

### Outputs
- Received packets
- Connection status
- Transport errors

### Dependencies
- **ONLY:** Protocol layer, standard library
- **NOT:** State store, command queue, daemon specifics
- **NOT:** Express, Socket.io, UI code

### Public API (ITransport Interface)
```typescript
connect(): Promise<void>;
disconnect(): Promise<void>;
send(packet: Uint8Array): Promise<void>;
receive(callback: (packet: Uint8Array) => void): void;
isConnected(): boolean;
```

### Implementations
```
SerialTransport     - USB serial (real hardware)
MockTransport       - Virtual device for testing
TcpTransport        - Network socket (future)
BluetoothTransport  - Bluetooth (future)
```

### Testing
✅ Unit testable (mock serial)
✅ Each transport independently tested
✅ No real hardware needed
✅ Timeout behavior testable

---

## Command Queue

### Responsibilities
- Accept commands from UI
- Maintain queue order
- Track command states (Queued → Sending → Waiting ACK → Completed/Failed)
- Retry failed commands
- Timeout management
- Rate limiting

### Inputs
- Commands from UI (via daemon)
- Responses from firmware (via transport)
- Acknowledgments/timeouts

### Outputs
- Packets to send (to transport)
- Status updates (to state store)
- Completion notifications

### Dependencies
- **ONLY:** Protocol layer, timer library
- **NOT:** Transport (depends on queue)
- **NOT:** State store directly (but reports results)
- **NOT:** UI code

### Public API
```typescript
add(command: Command): Promise<CommandResult>;
pause(): void;
resume(): void;
clear(): void;
drain(): Promise<void>;
```

### Internal API
```typescript
_getNextCommand(): Command;
_markSending(id: string): void;
_markWaitingAck(id: string): void;
_handleTimeout(id: string): void;
_handleAck(id: string): void;
_retryCommand(id: string): void;
```

### State Machine
```
Queued
  ↓ (next in line)
Sending
  ↓ (packet sent)
WaitingAck (timeout set)
  ├→ Completed (ACK received)
  ├→ TimedOut (timeout → retry)
  └→ Failed (3 retries exceeded)
```

### Testing
✅ Unit testable (no hardware)
✅ Mock responses for state transitions
✅ Timeout behavior testable
✅ Retry logic fully covered

---

## State Store

### Responsibilities
- **Single source of truth** for application state
- Maintain FirmwareState (what device knows)
- Maintain ConfigState (settings)
- Maintain StatisticsState (uptime, boot count)
- Event logging (all state changes)
- Subscriber notifications (when state changes)

### Inputs
- Firmware status messages
- Command responses
- User configuration changes
- Events from all modules

### Outputs
- State snapshots (to WebSocket clients)
- Event notifications (to subscribers)
- Historical data (event log)

### Dependencies
- **ONLY:** Type definitions
- **NOT:** Transport, protocol (receives results, not raw data)
- **NOT:** Firmware code
- **NOT:** Express, Socket.io (server uses state, not reverse)

### Public API
```typescript
// Getters
getState(): AppState;
getFirmwareState(): FirmwareState;
getConfig(): ConfigState;
getStatistics(): StatisticsState;
getEventLog(limit?: number): Event[];

// Setters (internal only)
setFirmwareStatus(status: FirmwareStatus): void;
setConfig(config: ConfigState): void;

// Subscriptions
subscribe(callback: (state: AppState) => void): Unsubscribe;
onEvent(handler: (event: Event) => void): void;
```

### Events Logged
- Device connected/disconnected
- Command sent/completed/failed
- Configuration changed
- Error occurred
- Temperature threshold crossed
- Drive added/removed

### Testing
✅ Unit testable (no external dependencies)
✅ Subscription behavior testable
✅ Event ordering testable
✅ State immutability verifiable

---

## Daemon Core (NASController.Core Library)

### Responsibilities
- Orchestrate protocol, transport, queue, state
- Manage system lifecycle (boot, shutdown)
- Capability discovery
- Atomic transactions for config changes
- Reconnection handling
- Error classification and recovery

### Inputs
- Configuration (port, timeout, etc.)
- Commands from API layer
- System signals (shutdown, error)

### Outputs
- State updates
- Events
- Responses to API

### Dependencies
- **ALL layers:** Protocol, Transport, Queue, State
- **NOT:** Express, Socket.io (thin API layer can inject)
- **NOT:** Firmware, UI code

### Public API
```typescript
class NASController {
  async boot(): Promise<void>;
  async shutdown(): Promise<void>;
  async sendCommand(cmd: Command): Promise<CommandResult>;
  subscribe(cb: (state: AppState) => void): Unsubscribe;
  getState(): AppState;
}
```

### Key Behavior
- **Boot sequence:** Load config → Connect → Ping → Capabilities → Get status
- **Shutdown sequence:** Drain queue → Close connection → Exit
- **Reconnect:** Detect disconnect → Auto-reconnect → Resync state
- **Transactions:** Begin → Send → Validate → Commit or Rollback

### Testing
✅ Unit testable (all dependencies mockable)
✅ Integration tests with MockTransport
✅ Reconnection scenarios testable
✅ Transaction rollback testable
✅ Full system testable without hardware

---

## WebSocket API Layer

### Responsibilities
- Accept WebSocket connections from frontend
- Route commands to daemon core
- Broadcast state updates
- Handle client disconnects
- Manage multiple concurrent clients (single user, but testing)
- Live packet inspector

### Inputs
- WebSocket messages from clients
- State updates from daemon
- Packet events (TX/RX)

### Outputs
- WebSocket responses to clients
- Commands to daemon core

### Dependencies
- **ONLY:** NASController.Core, Socket.io, Express
- **NOT:** Protocol, Transport directly
- **NOT:** Firmware code

### Public API
```typescript
// Client receives
{
  type: "state_update",
  state: AppState
}

{
  type: "command_result",
  result: CommandResult
}

{
  type: "packet_captured",
  packet: CapturedPacket
}
```

### Event Handlers
```typescript
socket.on("command", (cmd: Command) => {
  controller.sendCommand(cmd);
});

socket.on("get_state", () => {
  socket.emit("state", controller.getState());
});

socket.on("start_packet_inspector", () => {
  packetInspector.start();
});
```

### Testing
✅ Unit testable (mock Socket.io)
✅ Client-server message testing
✅ Broadcast testing

---

## REST API Layer (Minimal)

### Responsibilities
- Serve static content
- Download logs
- Download firmware files
- Diagnostic endpoints

### Inputs
- HTTP requests

### Outputs
- JSON responses
- File downloads

### Dependencies
- **ONLY:** Express, NASController.Core
- **NOT:** Protocol, Transport (state derived)

### Public API
```
GET  /api/logs              → Returns event log (JSON or CSV)
GET  /api/state             → Returns current state
GET  /api/config/backup     → Downloads config file
GET  /api/diagnostics       → Returns performance metrics
GET  /health                → Health check (200 if up)
```

### Note
**REST is minimal.** Real-time updates use WebSocket. REST is only for bulk/static data.

---

## Firmware: Hardware Abstraction Layer (HAL)

### Responsibilities
- Abstract ESP32 GPIO, UART, timers
- Provide hardware-independent interface
- Initialize hardware
- Handle hardware-specific quirks

### Inputs
- Configuration (pin assignments, etc.)
- Commands from application logic

### Outputs
- Hardware signals (GPIO, UART)
- Status/measurements

### Dependencies
- **ONLY:** ESP32 libraries, std C++
- **NOT:** Application logic
- **NOT:** Anything above HAL

### Public API
```cpp
class GPIO {
  void pinMode(int pin, Mode mode);
  void digitalWrite(int pin, bool value);
  bool digitalRead(int pin);
};

class UART {
  void begin(int baudrate);
  void write(const uint8_t* data, size_t len);
  size_t readAvailable(uint8_t* buffer, size_t maxLen);
};

class Timer {
  void start(uint32_t ms, Callback cb);
  void stop();
};
```

### Implementations
- **ESP32** - Actual GPIO, UART, timers
- **MockHAL** - Virtual hardware (for testing, future)

---

## Firmware: Protocol Handler

### Responsibilities
- Receive packets via UART
- Call appropriate handlers (relay, fan, LED, etc.)
- Send responses with status
- Handle CRC validation

### Inputs
- Binary packets from UART

### Outputs
- Binary responses (ACK, status, data)
- Commands to hardware controllers

### Dependencies
- **ONLY:** HAL, application controllers, standard library
- **NOT:** Anything outside firmware

### Public API (via protocol)
```
0x55AA 01 0x10 01 01 [CRC16]     ← Relay ON command
0x55AA 01 0x10 01 [CRC16]        → ACK response
```

---

## Firmware: Controllers (Relay, Fan, LED, Sensor)

### Responsibilities (each controller)
- Own one hardware subsystem
- Respond to protocol commands
- Report status
- Handle failures
- Coordinate with other controllers if needed

### Inputs
- Protocol commands
- Hardware feedback

### Outputs
- Hardware commands (via HAL)
- Status reports

### Dependencies
- **ONLY:** HAL, application types
- **NOT:** Other controllers (loose coupling)
- **NOT:** Anything outside firmware

### Example: RelayController
```cpp
class RelayController {
public:
  void init(Config& cfg);
  void handleCommand(RelayCommand cmd);
  RelayStatus getStatus(uint8_t relayId);
private:
  GPIO& gpio;
  uint8_t pinMap[4];
};
```

---

## Frontend: Pages

### Responsibilities (each page)
- Render UI for one feature area
- Handle user interactions
- Call hooks for data and commands
- Display state from daemon

### Inputs
- User interactions
- State from hooks

### Outputs
- Commands (via WebSocket hook)
- Rendered UI

### Dependencies
- **ONLY:** React, hooks, component library
- **NOT:** Direct WebSocket (use hooks)
- **NOT:** Daemon code

### Example: Dashboard Page
```typescript
function Dashboard() {
  const { firmware, sendCommand } = useFirmware();
  
  return (
    <div>
      <TemperatureChart data={firmware.temperature} />
      <RelayStatus relays={firmware.relays} />
      <FanStatus fans={firmware.fans} />
    </div>
  );
}
```

---

## Frontend: Hooks

### Responsibilities
- Connect to WebSocket
- Subscribe to state updates
- Provide data and command functions
- Handle reconnection

### Inputs
- WebSocket messages
- State snapshots

### Outputs
- Current state
- Command functions

### Dependencies
- **ONLY:** React, WebSocket service
- **NOT:** Pages directly (used by pages)
- **NOT:** Daemon code

### Example: useFirmware Hook
```typescript
function useFirmware() {
  const [firmware, setFirmware] = useState(initialState);
  
  useEffect(() => {
    socket.on("state_update", (state) => {
      setFirmware(state.firmware);
    });
  }, []);
  
  const sendCommand = (cmd) => {
    socket.emit("command", cmd);
  };
  
  return { firmware, sendCommand };
}
```

---

## Frontend: Services

### Responsibilities
- WebSocket connection management
- HTTP requests (for files, logs)
- Data serialization/deserialization

### Inputs
- Network events
- HTTP responses

### Outputs
- Parsed data
- Event callbacks

### Dependencies
- **ONLY:** Standard library, Socket.io client
- **NOT:** React (used by hooks)
- **NOT:** Pages (used by hooks)

---

## Dependency Summary

### Allowed (Downward)
```
UI components
  ↓
UI hooks
  ↓
WebSocket services
  ↓
Daemon core
  ↓
Protocol + Transport + Queue + State
  ↓
Standard library + Hardware (firmware)
```

### Forbidden (Upward or Circular)
```
❌ Firmware → Daemon
❌ Daemon → UI
❌ Protocol ↔ Queue (bi-directional)
❌ State → State store (circular)
```

---

## Adding New Modules

When adding a new module, ask:

1. **What is its single responsibility?**
2. **What does it depend on?** (Should be fewer things, not more)
3. **Who depends on it?** (Only things above it)
4. **Can it be tested independently?**
5. **Is its API clear?** (Public/internal separation)

If you answer these clearly, your module fits the system.

---

**Keep modules focused. Respect boundaries. The system stays understandable.**
