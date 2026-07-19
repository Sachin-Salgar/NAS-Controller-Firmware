Document Version: 1.0  
Project Version: 1.0  
Last Updated: 2026-07-19  
Owner: NAS Controller  
Status: Frozen

# Sequence Diagrams - Key Interaction Flows

This document provides sequence diagrams for critical interactions in the NAS Controller system. These diagrams help developers understand the flow of control and data between components.

---

## 1. Startup Sequence (Daemon + Firmware)

When the daemon starts, it performs capability discovery to understand the firmware's capabilities.

```
Daemon          Transport       Firmware
  |                |                |
  |-- connect() -->|                |
  |                |--- USB Open --->|
  |                |<-- Ready -------|
  |
  |-- send(PING) ->|
  |                |--- 0x55AA01... ->|
  |                |                   |
  |                |<- ACK 0x8001... ---|
  |                |
  |-- send(GET_CAPABILITIES) ->|
  |                |--- 0x55AA02... ->|
  |                |                   |
  |                |<- Data (v1.0, 4 relays, 2 fans, 24 LEDs) ---|
  |                |
  |-- send(GET_ALL_CONFIG) ->|
  |                |--- 0x55AA03... ->|
  |                |                   |
  |                |<- Config data ---|
  |                |
  |-- send(GET_ALL_STATUS) ->|
  |                |--- 0x55AA04... ->|
  |                |                   |
  |                |<- Status data (relay states, fan speeds, etc.) ---|
  |                |
  |-- StateStore updated
  |-- Broadcast to all WebSocket clients
  |
UI <-- State Update (relay states, capabilities)
```

**Key Points:**
- Sequence numbers: 1 (PING), 2 (GET_CAPABILITIES), 3 (GET_ALL_CONFIG), 4 (GET_ALL_STATUS)
- PING verifies connection is alive
- Capabilities determine what UI elements show
- Config and Status populate initial state
- Broadcast to WebSocket ensures UI gets fresh data

**Timeline:**
- Connect: ~50ms
- PING → ACK: ~10ms
- Capabilities: ~20ms
- Config: ~50ms
- Status: ~50ms
- **Total: ~200ms**

---

## 2. User Turns Relay ON (Normal Case)

User clicks "Relay 1 ON" button in the UI. The relay turns on successfully.

```
User (UI)       WebSocket       Daemon          Transport       Firmware
  |                 |              |                |               |
  |-- Click Button ->|              |                |               |
  |                  |-- emit() --->|                |               |
  |                  |              |-- queue(CMD) ->|               |
  |                  |              |                |-- send() ---->|
  |                  |              |<- Sending ------|               |
  |                  |              |                |-- 0x55AA... ->|
  |                  |              |                |               |
  |                  |              |                |<- ACK -------|
  |                  |              |<- Completed ---|               |
  |                  |              |                               |
  |                  |              |-- StateStore.relay1 = ON
  |                  |              |
  |                  |<-- broadcast() ---|
  |<-- State Update (relay1: ON) ---|
  |
  |-- Button shows "ON"
```

**Key Points:**
- UI sends command via WebSocket emit
- Daemon receives and queues command
- Transport sends to firmware
- Firmware executes and responds with ACK
- Daemon updates state and broadcasts
- UI receives state update and updates immediately

**Timeline:**
- WebSocket emit: <1ms
- Queue and send: ~5ms
- Firmware processing: ~10ms
- State broadcast: ~5ms
- **Total: ~25ms** (all latency <500ms)

---

## 3. USB Disconnect and Reconnect

USB cable is unplugged, daemon detects, and firmware reconnects.

```
Daemon                Transport                  Firmware
  |                      |                          |
  |-- PING (periodic) -->|                          |
  |                      |--- USB Error (disconnected)
  |                      |<-- TIMEOUT ------|
  |<-- Error --------|    
  |                  
  |-- Reconnect logic:
  |-- Wait 1s
  |-- retry_count = 0
  |
  |-- while retry_count < MAX_RETRIES:
  |      try connect()
  |      if success: break
  |      wait (1s * exponential_backoff)
  |
  |-- connect() -->|
  |                |--- USB Re-enumerated (plugged back in)
  |                |<-- Ready ---|
  |
  |-- Startup sequence (as in section 1)
  |-- PING → ACK (connection verified)
  |-- GET_CAPABILITIES → Data
  |-- GET_ALL_CONFIG → Config
  |-- GET_ALL_STATUS → Status
  |
  |-- StateStore sync complete
  |-- Broadcast CONNECTED event
  |
UI <-- CONNECTED event (connection restored)
UI re-subscribes to updates
```

**Key Points:**
- Periodic PING detects disconnect quickly
- Exponential backoff prevents USB driver thrashing
- Full state resync ensures UI reflects firmware state
- Automatic recovery requires no user action

**Timeline:**
- Disconnect detection: ~1-2 seconds (next PING interval)
- Reconnection attempts: ~5-10 seconds (depends on reconnect delay)
- State resync: ~200ms
- **Total: ~10 seconds** until full reconnection

---

## 4. Configuration Save (Atomic Transaction)

User changes fan threshold and clicks "Save Configuration".

```
User (UI)          WebSocket          Daemon              Firmware
  |                    |                 |                   |
  |-- Edit config ---->|                 |                   |
  |                    |-- begin() ----->|                   |
  |                    |<-- ACK ---------|                   |
  |                    |                 |                   |
  |                    |-- send(CONFIG_SET) ->|              |
  |                    |                 |-- queue() ->|
  |                    |                 |            |-- send() -->|
  |                    |                 |            |             |
  |                    |                 |            |<- ACK -------|
  |                    |                 |<- Completed ------|
  |                    |                 |                   |
  |                    |-- validate() -->|                   |
  |                    |<-- Valid -------|                   |
  |                    |                 |                   |
  |                    |-- send(CONFIG_COMMIT) ->|           |
  |                    |                 |-- queue() ->|
  |                    |                 |            |-- send() -->|
  |                    |                 |            |             |
  |                    |                 |            |<- ACK -------|
  |                    |                 |<- Completed ------|
  |                    |                 |                   |
  |                    |                 |-- StateStore.config = new_config
  |                    |<-- broadcast() ---|
  |<-- SAVED event ----|
  |
  |-- Show "Configuration saved"
```

**Key Points:**
- Transaction has three phases: Begin, Send, Commit
- If any phase fails, entire transaction rolls back
- Validation prevents invalid configs reaching firmware
- Atomic: either all changes applied or none

**Timeline:**
- Begin: ~10ms
- Send and validate: ~50ms
- Commit: ~10ms
- Broadcast: ~5ms
- **Total: ~75ms**

---

## 5. Fan Speed Slider (Real-Time Control)

User drags fan speed slider from 30% to 50%.

```
User (UI)           WebSocket          Daemon          Transport    Firmware
  |                     |                |                |           |
  |-- Drag slider ----->|                |                |           |
  |  (position: 40%)    |-- send() ----->|                |           |
  |                     |                |-- queue() ---->|           |
  |                     |                |                |-- send() ->|
  |                     |                |                |           |
  |                     |                |                |<- ACK ----|
  |                     |                |<- Completed ----|
  |                     |                |                |           |
  |                     |                |-- broadcast() -|           |
  |<-- speed: 40% ------|                |                |           |
  |                     |
  |-- Slider shows 40%
  |
  |-- Drag to 45%
  |                     |-- send() ----->|
  |                     |                |-- queue() ---->|
  |                     |                |                |-- send() ->|
  |                     |                |                |<- ACK ----|
  |                     |                |<-- Completed --|
  |                     |                |-- broadcast()
  |<-- speed: 45% ------|
  |
  |-- Slider shows 45%
  |
  | ... (each slider movement = one command)
```

**Key Points:**
- Each slider movement generates a command
- Commands queue and execute serially
- UI receives feedback via state broadcast
- If queue backs up, user feels lag
- Debouncing recommended for many small changes

**Timeline:**
- Per command: ~25-50ms
- With debouncing (100ms): 1-2 commands per second = responsive feel

---

## 6. Command Timeout and Retry

A command times out (no ACK received) and is retried.

```
Daemon          Transport          Firmware
  |                |                   |
  |-- send() ----->|                   |
  |                |--- Packet ------>| (firmware not responding)
  |                |                   |
  |-- Wait 1000ms
  |
  |-- Timeout!
  |-- retry_count = 1
  |
  |-- wait(exponential_backoff: 1s * 1)
  |-- send() ----->|
  |                |--- Retry 1 ----->| (still no response)
  |                |
  |-- Wait 1000ms
  |-- Timeout!
  |-- retry_count = 2
  |
  |-- wait(exponential_backoff: 1s * 2)
  |-- send() ----->|
  |                |--- Retry 2 ---->| (firmware recovers)
  |                |                   |
  |                |<- ACK ----------|
  |
  |-- Completed (3rd attempt)
  |-- StateStore updated
```

**Key Points:**
- Timeout: 1000ms per command
- Retry: up to 3 attempts total
- Backoff: exponential (1s, 2s, 3s)
- After 3 failures: command marked FAILED
- Rare condition but handles transient USB issues

**Timeline:**
- First attempt: 1000ms
- Second attempt: 1000ms + 1000ms backoff = 2000ms
- Third attempt: 1000ms + 2000ms backoff = 3000ms
- **Total for 3 retries: ~6000ms** if all timeout

---

## 7. Error Response (NAK)

Firmware rejects command with error code.

```
Daemon          Transport          Firmware
  |                |                   |
  |-- send(CMD) -->|                   |
  |                |--- 0x55AA... ---->|
  |                |                   |
  |                |<-- NAK (0x40) ---|
  |                |    error: OUT_OF_RANGE
  |
  |-- Classify Error
  |   Type: Firmware Error
  |   Code: OUT_OF_RANGE (0x03)
  |
  |-- StateStore.lastError = {
  |     type: "FIRMWARE",
  |     code: 0x03,
  |     message: "Value out of range"
  |   }
  |
  |-- broadcast()
  |
UI <-- Error event (out of range)
UI shows toast: "Fan speed out of range (0-100)"
```

**Key Points:**
- NAK identified by Cmd bit 0x40
- Error code in payload
- Daemon classifies as Firmware error
- UI displays user-friendly message
- No retry on firmware errors (invalid input)

**Timeline:**
- NAK detection: <5ms
- Error classification: <5ms
- Broadcast: ~5ms
- **Total: ~15ms**

---

## 8. Live Packet Inspector

Packet capture shows TX and RX in real-time.

```
Daemon                                UI (Packet Inspector)
  |                                        |
  |-- send(0x55AA01...) -->|--- Capture
  |                         |-- TX: 0x55AA01... (PING)
  |                         |-- Timestamp: 12:34:56.123
  |                         |-- Display in list
  |
  |<-- receive(0x8001...)  |--- Capture
  |                         |-- RX: 0x8001... (ACK)
  |                         |-- Timestamp: 12:34:56.134
  |                         |-- Decode: PING ACK
  |                         |-- Display in list
  |
  | (real-time stream continues)
```

**Key Points:**
- Packets captured before and after transmission
- Hex dump with decoded meaning
- Timestamps for latency analysis
- Can filter by packet type
- Can export trace for debugging

**Timeline:**
- Packet capture: <1ms overhead
- UI update: ~10-20ms per packet

---

## 9. Multi-Client Broadcast

Multiple browsers connected; daemon broadcasts state change to all.

```
Browser 1         Browser 2        Daemon         Firmware
  |                 |               |               |
  |-- send() ------>|               |               |
  |                 |               |--- send() --->|
  |                 |               |<-- ACK -------|
  |                 |               |
  |                 |               |-- broadcast()
  |                 |<--- to all connected clients
  |                 |
  |<--- State Update |
  |-- Update UI      |-- Update UI
```

**Key Points:**
- One client sends command
- Daemon processes and broadcasts to ALL clients
- All UIs stay synchronized
- No multi-user conflict in v1 (single-user lock)

---

## 10. WebSocket Reconnection (Client-Side)

UI client loses connection and reconnects.

```
UI              WebSocket           Daemon
  |                |                  |
  |-- connected -->|                  |
  |                |--- normal operation
  |
  |                |[Network loss or timeout]
  |                |
  |<-- disconnect ---|
  |-- Show "offline"
  |
  |-- reconnect() (auto)
  |-- wait 1s exponential backoff
  |-- connect()
  |                |--- new connection
  |                |--- subscribe to updates
  |                |
  |                |<-- broadcast (current state)
  |<-- state update
  |-- Sync UI with current state
  |-- Show "online"
```

**Key Points:**
- Client-side reconnection logic (Socket.io handles automatically)
- Full state broadcast after reconnection ensures consistency
- User sees "offline" briefly, then "online"
- No data loss if queued before disconnect

---

## Usage Guide

These diagrams show the happy path and common error cases. When implementing:

1. **Reference the diagram** for your feature
2. **Trace through each step** to understand data flow
3. **Identify potential errors** at each step
4. **Plan error handling** based on failure modes
5. **Test timeout and retry** scenarios

### Common Bottlenecks

1. **Long firmware processing:** Increases latency
2. **Serial port congestion:** Multiple commands queued
3. **Network latency:** If extending to network daemon
4. **WebSocket broadcast:** Many clients receiving updates

### Optimization Opportunities

1. **Command batching:** Send multiple commands in one packet
2. **Debouncing:** Reduce slider/input update frequency
3. **Lazy state:** Only broadcast changed fields
4. **Compression:** Compress large responses

---

**Purpose:** Help developers understand system interactions  
**Status:** Current (updated as architecture changes)  
**Maintenance:** Add new diagrams as new features are implemented
