Document Version: 1.0  
Project Version: 1.0  
Last Updated: 2026-07-19  
Owner: NAS Controller  
Status: Frozen

# Architecture Decision Records (ADRs) - Summary

This is a quick reference for all significant architectural decisions made on the NAS Controller project. For detailed context on each decision, see the individual ADR documents.

---

## Decision Summary Table

| # | ADR | Title | Decision | Status | Consequences |
|---|-----|-------|----------|--------|--------------|
| **ADR-0001** | WebSocket-First Architecture | Use WebSocket for all real-time commands; REST only for bulk operations | Accepted | Frozen | Low latency, persistent connection, easier real-time updates. REST adds complexity but provides fallback. |
| **ADR-0002** | Library-First Daemon Pattern | Implement daemon as pure TypeScript library (NASController.Core) with Express as optional thin adapter | Accepted | Frozen | Testable without running server, reusable in other contexts (CLI, Electron, etc.). Adds architectural discipline. |

---

## ADR-0001: WebSocket-First Architecture

**Location:** `shared/docs/adr/0001-websocket-first.md`

**Context:**
Daemon needs to support both real-time commands (low latency) and bulk operations (large payloads). How should communication be structured?

**Decision:**
- **Primary:** WebSocket for all real-time commands and state updates
- **Secondary:** REST for static/bulk operations (logs, files, diagnostics)
- **Pattern:** Daemon maintains persistent WebSocket connection to all clients

**Consequences:**
✅ **Advantages:**
- Low latency for command execution (no request-response round trip)
- Persistent connection enables server-initiated updates
- Natural fit for real-time status monitoring
- Socket.io provides fallback to long-polling if WebSocket unavailable

❌ **Disadvantages:**
- WebSocket connection must be actively maintained
- REST fallback adds some code complexity
- Requires reconnection logic on client and server

**Rationale:**
Web UI needs sub-second command execution (turn relay on/off, adjust fan). HTTP request-response adds ~100-200ms overhead. WebSocket matches real-time nature of hardware control.

**Evidence:**
- Real-time dashboards (e.g., AWS CloudWatch, Grafana) use WebSocket or Server-Sent Events
- Hardware control systems (e.g., Philips Hue, IKEA TRÅDFRI) use persistent connections
- REST is good for time-insensitive operations (downloading logs)

---

## ADR-0002: Library-First Daemon Pattern

**Location:** `daemon/docs/adr/0002-library-first-daemon.md`

**Context:**
Daemon needs to manage USB communication, state, commands, and error recovery. How should this be structured?

**Decision:**
- **Core:** Implement as pure TypeScript library (NASController.Core) with NO external dependencies except serialport
- **Adapter:** Wrap library with Express + Socket.io for HTTP/WebSocket API
- **Pattern:** Dependency injection for transport, command queue, state store

**Consequences:**
✅ **Advantages:**
- Core logic testable without running Express server
- No dependency on web framework (Express, Socket.io) for business logic
- Reusable in other contexts: CLI tool, Electron desktop app, headless automation
- Clear separation: business logic vs. HTTP plumbing
- Easy to test with MockTransport (no hardware needed)
- Better code organization and modularity

❌ **Disadvantages:**
- Extra abstraction layer (ITransport, dependency injection)
- Slightly more code upfront
- Requires discipline to keep library pure (no web framework imports)

**Rationale:**
Hardware control logic is valuable independently of the web interface. Separating concerns makes code more resilient and testable.

**Evidence:**
- Node.js libraries like Socket.io, Passport.js are pure libraries with framework adapters
- Microservices pattern: core service logic separate from HTTP wrapper
- Testing best practice: test business logic independently from web framework

---

## Related Architectural Decisions

### Implicit Decisions (not yet ADRs)

**1. ITransport Interface**
- USB communication abstracted via ITransport
- Enables SerialTransport (USB), MockTransport (testing), TcpTransport (future)
- Consequence: Easy to add new communication methods without core changes

**2. Centralized State Store**
- Single source of truth for all application state in daemon
- Published to UI via subscriptions
- Consequence: Consistent state across all clients, easy to implement features like undo/redo

**3. Command Queue State Machine**
- Commands transition: Queued → Sending → Waiting ACK → Completed/Timeout/Failed
- Timeout: 1000ms per command, retry up to 3 times
- Consequence: Reliable command execution, clear error states for UI

**4. Atomic Config Transactions**
- Config changes: Begin → Send → Validate → Commit
- All-or-nothing semantics
- Consequence: No partial/corrupted configurations

**5. Capability Discovery**
- Firmware reports capabilities at startup (relay count, fan count, etc.)
- Daemon adapts UI to hardware capabilities
- Consequence: UI works with different hardware versions without code changes

**6. Error Classification**
- 6 error types: Transport, Protocol, Firmware, Validation, User, Internal
- Each type has different recovery strategy
- Consequence: Better error handling, easier debugging

**7. Diagnostic Mode**
- Optional packet logging, timing info, metrics
- Lightweight enough for production
- Consequence: Production debugging without verbose logging overhead

---

## How to Create a New ADR

If you need to document a new architectural decision:

1. **Title:** Descriptive name (e.g., "Feature X Implementation")
2. **Context:** What was the problem?
3. **Decision:** What did we choose?
4. **Consequences:** Pros and cons
5. **Rationale:** Why this choice?
6. **Evidence:** Industry precedent or validation

**Location:** 
- Project-wide: `docs/adr/00XX-title.md`
- Component-specific: `[component]/docs/adr/00XX-title.md`

**Process:**
1. Draft ADR as comment in PR or discussion
2. Stakeholder review and approval
3. Implement decision
4. Move draft to final location in component docs
5. Reference in this summary (DECISIONS.md)

---

## Frozen Architecture Baseline

These ADRs define the frozen v1.0 architecture. Changes to these decisions require:

1. **Stakeholder consensus** (lead architect, tech lead)
2. **Updated ADR** with new context and consequences
3. **Impact analysis** of change across codebase
4. **Completion of affected tasks** in IMPLEMENTATION_CHECKLIST.md

---

## Related Documents

- **WebSocket-First ADR:** `shared/docs/adr/0001-websocket-first.md`
- **Library-First ADR:** `daemon/docs/adr/0002-library-first-daemon.md`
- **Frozen Architecture:** [ARCHITECTURE_FREEZE.md](./ARCHITECTURE_FREEZE.md)
- **Implementation Checklist:** [IMPLEMENTATION_CHECKLIST.md](./IMPLEMENTATION_CHECKLIST.md)

---

**Purpose:** Centralized decision log for architectural decisions  
**Status:** Frozen (additions require stakeholder approval)  
**Maintenance:** Update this table when new ADRs are created
