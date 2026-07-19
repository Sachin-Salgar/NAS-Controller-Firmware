# Dependency Diagram

Enforce the dependency direction rule: **UI → Daemon → Transport → Firmware**

---

## The Rule

Dependencies flow ONE DIRECTION only:

```
Web UI
  ↓ (imports from)
Daemon
  ↓ (imports from)
Transport
  ↓ (imports from)
Firmware
```

**Never go upward. Never go sideways. Always down.**

---

## Dependency Map

**Frontend (React)**
- Can import: React, Socket.io-client, utilities
- Cannot import: Daemon code, protocol, firmware
- Talks to daemon: Via WebSocket only (network boundary)

**Daemon (Node.js)**
- Can import: Protocol, Transport, Queue, State
- Cannot import: React, firmware code
- Depends on: Layers below it

**Transport Layer**
- Can import: Protocol, serialport, standard library
- Cannot import: Queue, State, daemon core, anything above
- Knows about: I/O only

**Protocol Layer**
- Can import: Standard library only
- Cannot import: Anything external
- Self-contained: Encodes/decodes, nothing else

**Firmware (ESP32)**
- Can import: ESP32 SDK, C++ stdlib, HAL, controllers
- Cannot import: Anything from daemon/frontend
- Standalone: Controls hardware only

---

## Frontend

✅ **CAN import:**
- React, Vite, TypeScript
- Socket.io-client
- Component libraries
- Utilities

❌ **CANNOT import:**
- Daemon code
- Protocol types
- Firmware code

---

## Daemon

✅ **CAN import:**
- Protocol layer
- Transport layer
- Command Queue (own)
- State Store (own)
- Express, Socket.io

❌ **CANNOT import:**
- React
- Firmware code

---

## Protocol Layer

✅ **CAN import:**
- Standard library
- Type definitions

❌ **CANNOT import:**
- ANYTHING external (self-contained)

---

## Transport Layer

✅ **CAN import:**
- Protocol layer
- serialport, net, os
- Standard library

❌ **CANNOT import:**
- Queue, State
- Daemon core
- Anything above

---

## Firmware

✅ **CAN import:**
- ESP32 SDK
- C++ stdlib
- HAL
- Controllers

❌ **CANNOT import:**
- ANYTHING from daemon/frontend

---

## Testing Rules

### Automated Checks
- TypeScript strict mode (catch type violations)
- ESLint rules (prevent imports from wrong layers)
- Manual code review (verify dependency directions)

### Code Review Checklist
- [ ] No upward imports
- [ ] No circular dependencies
- [ ] No skipping layers
- [ ] Can module be tested independently?
- [ ] Are dependencies minimal?

---

## Summary

| Layer | Imports From | Cannot Import |
|-------|-------------|---------------|
| Frontend | React, Socket.io-client | Daemon, firmware, protocol |
| Daemon | Transport, Protocol, Queue, State | Frontend, firmware |
| Transport | Protocol, standard lib | Queue, state, daemon |
| Protocol | Standard lib | Everything else |
| Firmware | ESP32 SDK, C++ stdlib | Everything else |

**One direction: DOWN. Never sideways. Never up.**
