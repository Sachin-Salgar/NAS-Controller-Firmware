# Error Recovery Matrix

For each failure scenario: define retry policy, user message, and recovery action.

---

## Serial Communication Errors

### Serial Disconnected (USB Unplugged)

| Aspect | Value |
|--------|-------|
| **Retry** | Yes, exponential backoff (1s → 2s → 4s → 8s) |
| **User Message** | "Device disconnected. Reconnecting..." |
| **Recovery** | Auto-reconnect every N seconds, full state resync |
| **Log** | ERROR + attempt count |
| **UI Effect** | Show "Disconnected" badge, disable controls |
| **Commands** | Queue them locally, send on reconnect |

### Serial Port Busy

| Aspect | Value |
|--------|-------|
| **Retry** | Yes (1 second wait, then retry) |
| **User Message** | "Device busy. Please wait..." |
| **Recovery** | Wait and retry |
| **Log** | WARN + port name |
| **UI Effect** | Disable controls temporarily |
| **Commands** | Queue and retry |

### Serial Read Timeout

| Aspect | Value |
|--------|-------|
| **Retry** | Yes (3 attempts) |
| **User Message** | "No response from device. Retrying..." |
| **Recovery** | Resend command |
| **Log** | WARN + command type |
| **UI Effect** | Show spinner while retrying |
| **Commands** | Cancel after 3 retries |

---

## Protocol Errors

### Invalid CRC

| Aspect | Value |
|--------|-------|
| **Retry** | Yes (request resend) |
| **User Message** | "Communication error. Retrying..." |
| **Recovery** | Request firmware resend packet |
| **Log** | WARN + expected vs actual CRC |
| **UI Effect** | Temporary freeze (< 1 second) |
| **Commands** | None (automatic) |

### Invalid Packet Format

| Aspect | Value |
|--------|-------|
| **Retry** | No (protocol corruption) |
| **User Message** | "Protocol error. Please restart." |
| **Recovery** | Attempt recovery: drain queue, reconnect |
| **Log** | ERROR + hex dump |
| **UI Effect** | Show error message, allow reconnect |
| **Commands** | Cancel all, require manual reconnect |

### Sequence Number Out of Order

| Aspect | Value |
|--------|-------|
| **Retry** | Yes (resync) |
| **User Message** | None (automatic recovery) |
| **Recovery** | Request capabilities/status to resync |
| **Log** | WARN + expected vs received seq |
| **UI Effect** | None (invisible recovery) |
| **Commands** | Continue, no command loss |

---

## Firmware Errors

### Firmware Timeout (No ACK)

| Aspect | Value |
|--------|-------|
| **Retry** | Yes (up to 3 times) |
| **User Message** | "Device busy. Retrying..." |
| **Recovery** | Resend same packet |
| **Log** | WARN + attempt count |
| **UI Effect** | Show spinner while retrying |
| **Commands** | Cancel after 3 retries, show error |

### Firmware Exception (Bad Response)

| Aspect | Value |
|--------|-------|
| **Retry** | No (firmware issue) |
| **User Message** | "Device error. Please restart device." |
| **Recovery** | Log error, offer reset option |
| **Log** | ERROR + response code + details |
| **UI Effect** | Show error dialog |
| **Commands** | Cancel all |

### Configuration Invalid (Firmware Rejects)

| Aspect | Value |
|--------|-------|
| **Retry** | No (user error) |
| **User Message** | "Invalid configuration: [reason]" |
| **Recovery** | Revert to previous config |
| **Log** | WARN + validation error |
| **UI Effect** | Show validation error, allow retry |
| **Commands** | Cancel transaction, rollback |

---

## Command Errors

### Command Rejected (Invalid Parameter)

| Aspect | Value |
|--------|-------|
| **Retry** | No |
| **User Message** | "Invalid: [description]" |
| **Recovery** | Return error to user |
| **Log** | WARN + command + error code |
| **UI Effect** | Show error tooltip near control |
| **Commands** | Fail gracefully |

### Command Timeout (No Response)

| Aspect | Value |
|--------|-------|
| **Retry** | Yes (up to 3 times) |
| **User Message** | "Device busy. Retrying..." |
| **Recovery** | Resend command |
| **Log** | WARN + attempt count |
| **UI Effect** | Show spinner, disable control |
| **Commands** | Cancel after 3 retries |

### Command Queue Full

| Aspect | Value |
|--------|-------|
| **Retry** | Yes (wait for space) |
| **User Message** | "Too many pending commands. Please wait..." |
| **Recovery** | Wait for queue to drain, retry |
| **Log** | WARN + queue size |
| **UI Effect** | Disable controls temporarily |
| **Commands** | Queue locally until space available |

---

## State Errors

### State Inconsistency Detected

| Aspect | Value |
|--------|-------|
| **Retry** | Yes (request full status) |
| **User Message** | None (automatic recovery) |
| **Recovery** | Full state resync from firmware |
| **Log** | WARN + what was inconsistent |
| **UI Effect** | Refresh (may flicker) |
| **Commands** | None (automatic) |

### Missing Event (Out of Sync)

| Aspect | Value |
|--------|-------|
| **Retry** | N/A (request full sync) |
| **User Message** | None (automatic recovery) |
| **Recovery** | Full status refresh |
| **Log** | WARN + missing event |
| **UI Effect** | Quick refresh |
| **Commands** | None (automatic) |

---

## Network Errors (WebSocket)

### Browser Disconnects

| Aspect | Value |
|--------|-------|
| **Retry** | Auto-reconnect (exponential backoff) |
| **User Message** | "Reconnecting..." |
| **Recovery** | Re-establish WebSocket, sync state |
| **Log** | INFO + reconnect attempt count |
| **UI Effect** | Show "Connecting" badge |
| **Commands** | Buffer in browser, send on reconnect |

### WebSocket Timeout

| Aspect | Value |
|--------|-------|
| **Retry** | Yes (auto-reconnect) |
| **User Message** | "Connection lost. Reconnecting..." |
| **Recovery** | Auto-reconnect |
| **Log** | WARN + timeout duration |
| **UI Effect** | Show "Reconnecting" badge |
| **Commands** | Buffer locally |

---

## Recovery Actions

### Retry Policy

```
Attempt 1: Immediate
Attempt 2: After 1 second
Attempt 3: After 2 seconds
Attempt 4+: Give up, show error
```

### Backoff Policy (Reconnection)

```
Attempt 1: 1 second
Attempt 2: 2 seconds
Attempt 3: 4 seconds
Attempt 4: 8 seconds
Attempt 5+: 8 seconds (cap)
Reset on success
```

### State Resync

```
Trigger conditions:
├─ Reconnection
├─ Sequence number reset
├─ State inconsistency detected
└─ Manual refresh request

Process:
├─ PING
├─ Get capabilities
├─ Get configuration
└─ Get all status
```

### Transaction Rollback

```
If config transaction fails:
├─ Revert to previous config
├─ Notify user of failure
├─ Log original config value
└─ Offer retry option
```

---

## Error Classification

### Transport Errors
- Serial disconnected
- Port busy
- Read/write timeout
- Protocol corruption

**Handling:** Auto-reconnect, queue commands

### Firmware Errors
- Timeout (device not responding)
- Exception (device error)
- Invalid response
- Rejected configuration

**Handling:** Log, notify user, offer reset

### Validation Errors
- Invalid command parameters
- Out-of-range values
- Conflicting settings
- Permission denied

**Handling:** Show validation error, allow retry

### User Errors
- Sending command while disconnected
- Invalid configuration
- Unsupported operation

**Handling:** Show friendly error message

### Internal Errors
- State corruption
- Logic errors
- Race conditions
- Memory issues

**Handling:** Log, attempt recovery, show error

---

## Error Messages

### Show to User
- High-level, non-technical
- Actionable ("Please restart device" not "CRC mismatch")
- Empathetic tone

### Log Only
- Low-level details
- Technical info
- Stack traces
- Hex dumps

### Never Log
- Sensitive data (passwords, secrets)
- Personal information
- Firmware binary data (too large)

---

## Testing Errors

### Simulate Serial Disconnect
```bash
# Unplug USB cable during operation
# Expect: Auto-reconnect, UI shows "Disconnected"
```

### Simulate Timeout
```bash
# Firmware delay response > configured timeout
# Expect: Retry up to 3 times, then error
```

### Simulate Invalid Data
```bash
# Modify packet CRC before sending
# Expect: Firmware rejects, daemon retries
```

### Simulate Queue Full
```bash
# Send 100 commands rapidly
# Expect: Queue fills, further commands wait
```

---

## Monitoring & Alerts

### When to Alert

- [ ] 3+ retries needed for single command
- [ ] Reconnection after 10+ seconds offline
- [ ] State inconsistency detected
- [ ] Firmware error response
- [ ] Queue depth > 50% capacity

### What to Log

```
ERROR   - Transaction failed
WARN    - Retry needed
INFO    - State changed, device connected
DEBUG   - Command queued, response received
```

---

**Follow this matrix. It ensures consistent error handling across the system.**
