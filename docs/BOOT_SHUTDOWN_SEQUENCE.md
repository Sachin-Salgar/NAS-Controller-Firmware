# Boot & Shutdown Sequence

Exact sequence from daemon startup to shutdown. Prevents ambiguity and race conditions.

---

## Daemon Boot Sequence

### Step 1: Load Configuration
```
Load config from file or environment
├─ Port/baudrate
├─ Timeouts (command, reconnect)
├─ Retry counts
└─ Hardware pin mappings
```

### Step 2: Open Serial Connection
```
Connect to ESP32 via USB/serial
├─ Open port at configured baudrate
├─ Set timeouts
└─ State: CONNECTING
```

### Step 3: Ping Firmware
```
Send PING command → Wait for ACK
├─ Timeout: 1 second
├─ Retries: 3 attempts
└─ State: WAITING_PING_ACK
```

### Step 4: Discover Capabilities
```
Query firmware capabilities
├─ Relay count
├─ Fan count
├─ LED type
├─ Sensor types
└─ Firmware version
→ Store in state (immutable during session)
```

### Step 5: Read Configuration
```
Get current config from firmware
├─ Thresholds
├─ Intervals
├─ Calibration data
└─ Store in state
```

### Step 6: Read Current Status
```
Get all sensor/status data
├─ Temperature, humidity
├─ Relay states
├─ Fan speeds
├─ Drive status
└─ Update state store
```

### Step 7: Start WebSocket Server
```
Listen for browser connections
├─ HTTP/WebSocket on :3000
├─ Ready to accept clients
└─ State: READY
```

### Step 8: Browser Connects
```
Client opens WebSocket connection
├─ Send initial state snapshot
├─ Subscribe to updates
└─ Client fully initialized
```

---

## Firmware Boot Sequence

### After Power-On
```
1. Initializers (HAL, drivers)
2. Load saved config from flash
3. Initialize all controllers
4. Set up serial communication
5. Enter main task loop
6. Wait for commands from USB
```

### On First USB Connection
```
1. Firmware ready to receive commands
2. Wait for PING from daemon
3. Send ACK
4. Ready for capability discovery
```

---

## Browser Connection Sequence

### User Opens Dashboard
```
1. Browser establishes WebSocket
2. Sends "connect" message
3. Daemon sends state snapshot
4. Dashboard renders
5. Subscribe to real-time updates
```

---

## Daemon Shutdown Sequence

### Step 1: Receive Shutdown Signal
```
User presses Ctrl+C or kill signal
├─ Save current state to disk (optional)
└─ State: SHUTTING_DOWN
```

### Step 2: Close Browser Connections
```
Close all active WebSocket connections
├─ Notify clients: "Server shutting down"
└─ Wait for graceful close (timeout: 5s)
```

### Step 3: Drain Command Queue
```
Wait for pending commands to complete
├─ Do NOT accept new commands
├─ Let in-flight commands finish
├─ Timeout: 10 seconds (or config)
└─ Force-cancel remaining
```

### Step 4: Drain Pending Transactions
```
Commit in-progress config changes
├─ Send any pending config to firmware
├─ Wait for ACK
└─ Timeout: 5 seconds per command
```

### Step 5: Close Serial Connection
```
Close USB/serial port
├─ Flush pending data
├─ Send goodbye packet (optional)
└─ Close port gracefully
```

### Step 6: Save Final State (Optional)
```
Persist event log and statistics
├─ Save to file for recovery
└─ Optional depending on config
```

### Step 7: Exit Process
```
Daemon exits cleanly
├─ Exit code 0 (success)
└─ Cleanup complete
```

---

## Firmware Shutdown Sequence

### On Timeout
```
No data from daemon for > 30 seconds
├─ Set safe defaults (fans off, lights off)
├─ Optionally reset to known state
└─ Remain powered (don't shut down)
```

### On Power Loss
```
Power goes away
├─ No sequence needed (hard shutdown)
└─ State lost (recovered on reboot)
```

---

## Browser Disconnect Sequence

### User Closes Tab or Loses Connection
```
1. WebSocket closes
2. Daemon detects client gone
3. Unsubscribe from client updates
4. (Daemon continues running)
5. Other browsers unaffected
```

### User Refreshes Page
```
1. Old WebSocket closes
2. New WebSocket opens
3. Send fresh state snapshot
4. Resubscribe to updates
```

---

## Error Recovery Sequences

### Firmware Disconnects (USB unplugged)
```
Daemon detects: No response to PING
├─ State: DISCONNECTED
├─ Log warning
├─ Notify all WebSocket clients
├─ WebSocket clients show "Disconnected"
└─ Auto-reconnect every 1 second
```

### Daemon Auto-Reconnect
```
On connection re-established:
├─ PING → ACK
├─ Capabilities (may have changed)
├─ Read full config
├─ Read all status
├─ State: CONNECTED
├─ Notify clients
└─ Resume normal operation
```

### Browser Reconnects After Daemon Restart
```
1. Browser was disconnected
2. Daemon restarted and booted
3. Browser connects
4. Receive full state snapshot
5. All events replayed from log (recent)
```

---

## Timing Constraints

| Operation | Target | Timeout | Retries |
|-----------|--------|---------|---------|
| PING | 100ms | 1s | 3 |
| Capabilities | 500ms | 2s | 2 |
| Status read | 500ms | 2s | 2 |
| Command send | 100ms | 1s | 3 |
| Boot complete | < 5s | 10s | N/A |
| Shutdown | < 5s | 10s | N/A |
| Reconnect attempt | 1s interval | Forever | Infinite |

---

## State Transitions

```
INIT
  ↓ (config loaded)
CONNECTING
  ↓ (USB open)
PINGING
  ↓ (PING ACK'd)
DISCOVERING_CAPS
  ↓ (capabilities received)
READING_CONFIG
  ↓ (config received)
READING_STATUS
  ↓ (status received)
READY
  ├→ (command processed)
  ├→ (status updated)
  ├→ (error) → ERROR
  └→ (shutdown) → SHUTTING_DOWN
    ↓
  DISCONNECTED
```

---

## Checklist

### Daemon Startup
- [ ] Config loaded
- [ ] Serial port opened
- [ ] Firmware responds to PING
- [ ] Capabilities known
- [ ] Config cached
- [ ] Status synced
- [ ] WebSocket listening
- [ ] Ready for clients

### Daemon Shutdown
- [ ] Signal received
- [ ] Clients notified
- [ ] Queue drained
- [ ] Transactions committed
- [ ] Port closed
- [ ] Process exits

### Firmware Ready
- [ ] Initializers complete
- [ ] Config loaded
- [ ] Controllers initialized
- [ ] Serial ready
- [ ] Responding to PING
- [ ] Capabilities available

---

**Follow these sequences exactly. They ensure correct initialization and shutdown.**
