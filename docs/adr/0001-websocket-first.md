# ADR 0001: WebSocket-First Architecture for Command/Control

**Status:** ACCEPTED  
**Date:** July 19, 2026  
**Decision:** Use WebSocket for all real-time command/control; reserve REST only for static/bulk operations  

---

## Context

The NAS Controller needs a low-latency communication channel between the web UI and the daemon. Initial design considered REST for commands plus WebSocket for status updates.

## Problem

- **REST + WebSocket hybrid** creates two communication channels
- **Higher latency:** Each command requires HTTP round-trip (overhead + TCP reconnect)
- **Duplicate code:** REST handlers mirror WebSocket handlers
- **Complex state sync:** Unclear which channel owns state

## Decision

Use **persistent WebSocket connection** for all real-time operations:
- Command execution
- Status updates
- Event broadcasting
- Diagnostic data

Reserve **REST only for:**
- Log downloads (bulk data)
- Firmware file uploads
- Static resources
- Metrics export

## Rationale

✅ **Single persistent connection** reduces latency (no TCP overhead per command)  
✅ **Bidirectional:** Server can push updates without polling  
✅ **Simpler architecture:** One code path for commands and responses  
✅ **Easier testing:** Mock WebSocket instead of HTTP mocks  
✅ **Scales better:** Fewer connections = lower server load  

## Consequences

**Positive:**
- Lower command latency (crucial for hardware control)
- Simpler daemon code (no REST controller duplication)
- Real-time status feels more responsive
- Easier to add new commands (single handler pattern)

**Negative:**
- WebSocket connection must be managed (reconnection handling)
- Browser WebSocket API has fewer debugging tools than HTTP
- Requires Socket.io or similar library (adds dependency)

## Alternatives Considered

1. **REST for commands, WebSocket for status**
   - ❌ Hybrid approach, higher latency, code duplication
   
2. **GraphQL with subscriptions**
   - ❌ Overkill for hardware control, adds complexity
   
3. **gRPC streaming**
   - ❌ Requires proxy layer, more complex than Socket.io

## References

- PROTOCOL_SPEC.md (section on WebSocket usage)
- WEB_INTERFACE_PLAN.md (architecture diagram)

---

This ADR documents the architectural principle: WebSocket-first communication for all real-time daemon ↔ UI interaction.
