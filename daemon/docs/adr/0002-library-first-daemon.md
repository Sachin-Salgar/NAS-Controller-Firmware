# ADR 0002: Library-First Daemon Architecture

**Status:** ACCEPTED  
**Date:** July 19, 2026  
**Decision:** Daemon is a pure TypeScript library (NASController.Core) with optional Express/Socket.io API layer  

---

## Context

The daemon needs to orchestrate hardware control, manage state, and communicate with the ESP32 firmware. Initial design considered building it as an Express server from the start.

## Problem

- **Server-first approach** couples business logic to HTTP/WebSocket framework
- **Hard to test** without running Express server
- **No reuse** in other applications (CLI, Electron app, etc.)
- **Framework lock-in** to Express and Socket.io
- **Testing complexity** requires mocking HTTP/WebSocket layers

## Decision

Split the daemon into **two tiers:**

### Tier 1: NASController.Core (Pure Library)
- Protocol encoding/decoding
- Command queue and state machine
- State store with subscriptions
- Transport layer abstraction (ITransport)
- Event bus and packet inspector
- Diagnostic metrics
- **Zero dependencies** except serialport
- **Fully testable** without server

### Tier 2: Node API (Express + Socket.io)
- Thin HTTP/WebSocket bridge
- Serves React UI
- Exposes Core via Socket.io commands
- REST endpoints for bulk operations
- ~5% of total code

## Rationale

✅ **Testable:** Core logic works without server (use MockTransport)  
✅ **Reusable:** Core can be used in CLI tool, Electron app, etc.  
✅ **Clear separation:** Business logic ≠ HTTP plumbing  
✅ **Framework agnostic:** Replace Express with Fastify/Hono without rewriting Core  
✅ **Faster development:** Test Core independently before API integration  
✅ **Easier to understand:** Daemon responsibility is clear and bounded  

## Consequences

**Positive:**
- Unit tests run in milliseconds (no server)
- Integration tests use MockTransport (faster than USB)
- New transport types (TCP, BT) easy to add
- Future CLI tool reuses entire Core library
- Clear layering makes codebase maintainable

**Negative:**
- Slightly more code (library wrapper + API layer)
- Developers must understand library vs. API boundary
- Requires discipline not to add Express dependencies to Core

## Alternatives Considered

1. **Express-first (server as entry point)**
   - ❌ Tightly couples logic to framework
   - ❌ Hard to test without running server
   - ❌ No reuse outside Express context

2. **Monolithic daemon**
   - ❌ All code in one file/module
   - ❌ Harder to test components independently

## Implementation Pattern

```typescript
// daemon/src/core/index.ts
export class NASController {
  constructor(transport: ITransport) { ... }
  async connect() { ... }
  async sendCommand(cmd: Command) { ... }
  subscribe(listener: StateListener) { ... }
  // ... no Express or Socket.io imports
}

// daemon/src/api/index.ts
import { NASController } from '../core'
const controller = new NASController(new SerialTransport())
io.on('connection', (socket) => {
  socket.on('relay_on', (id) => controller.sendCommand(...))
})
```

## References

- WEB_INTERFACE_PLAN.md (daemon structure)
- PROTOCOL_SPEC.md (protocol layer)
- `daemon/docs/ARCHITECTURE.md` (implementation details)

---

This ADR documents the architectural principle: Daemon logic is agnostic to how it's used (server, CLI, desktop app).
