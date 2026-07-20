# Project Rules

**14 core rules that guide every implementation decision.**

These rules are non-negotiable. When in doubt, check here.

---

## Rule 1: Firmware Never Knows the UI Exists

### The Rule
Firmware code contains zero references to the UI, web protocols, HTTP, WebSocket, JSON, or any web-specific concepts.

### Why
- Firmware is portable—if you swap the UI later, firmware doesn't break
- Firmware focus remains on hardware control and real-time performance
- Firmware never depends on browser technology

### What This Means
✅ **Allowed:**
- Firmware sends raw sensor data
- Firmware sends status updates
- Firmware receives binary commands

❌ **Forbidden:**
- Firmware sending JSON
- Firmware parsing HTTP headers
- Firmware referencing UI state
- Firmware aware of "admin user" or "web request"

### Example
```cpp
// ✅ GOOD: Firmware sends raw data
sendStatus(temperature, humidity, pressure);

// ❌ BAD: Firmware knows about JSON
sendJSON("{\"temperature\": 25.3}");
```

---

## Rule 2: UI Never Talks Directly to ESP32

### The Rule
The web frontend never makes direct connections to the ESP32. All communication goes through the daemon.

### Why
- Single point of control (the daemon manages all state)
- Easier to add features (authentication, logging, filtering) later
- Prevents multiple clients from conflicting
- Daemon can queue and prioritize commands

### What This Means
✅ **Allowed:**
- Frontend talks to daemon via WebSocket
- Frontend receives updates from daemon
- Daemon translates to/from ESP32

❌ **Forbidden:**
- Frontend connecting directly to ESP32 serial
- Frontend knowing USB port name
- Frontend making raw binary requests
- Frontend bypassing the daemon

### Enforcement
The frontend code has no `serialport` dependency, no serial communication code, and no USB access.

---

## Rule 3: Daemon Owns All State

### The Rule
The single source of truth for application state is the daemon. Not the UI. Not the firmware. The daemon.

### Why
- Prevents inconsistency (if multiple clients connected, all see the same state)
- Easier debugging (look in one place)
- Testable without hardware (daemon testable via MockTransport)
- Recovery on reconnect is straightforward (resync from daemon state)

### What This Means
✅ **Allowed:**
- Daemon maintains FirmwareState, ConfigState, StatisticsState
- Frontend caches state for performance (but it's read-only)
- Firmware maintains hardware state (but reports to daemon)

❌ **Forbidden:**
- Frontend changing state without daemon approval
- Frontend storing "source of truth" for any data
- Firmware storing state that daemon doesn't know about
- Different clients seeing different state

### Example
```typescript
// ✅ GOOD: Daemon owns the state
class StateStore {
  relayStatus = [false, false, false, false];  // Single source
  setRelay(id, value) {
    this.relayStatus[id] = value;
    this.broadcastToAllClients();
  }
}

// ❌ BAD: Frontend owns state
const [relayStatus, setRelayStatus] = useState([...]);
```

---

## Rule 4: Everything is Logged

### The Rule
Every significant operation is logged with timestamp, event type, and details.

### Why
- Debugging is possible (what happened when?)
- Compliance (audit trail)
- Performance monitoring (can identify bottlenecks)
- User understanding (what did the system do?)

### What This Means
✅ **Log:**
- Command sent to firmware
- Firmware response received
- State changes
- Errors and warnings
- Connection/disconnection events
- Configuration changes
- All user actions (if local only)

❌ **Don't log:**
- Every iteration of a loop
- Internal function calls (unless they change state)
- Debug variables (use debugger instead)

### Logging Levels
```
ERROR   - Something failed and needs attention
WARN    - Something unexpected but recoverable
INFO    - Significant event (user action, state change)
DEBUG   - Detailed flow information (for developers)
TRACE   - Every step (rarely used, performance hit)
```

---

## Rule 5: No Feature Bypasses the Command Queue

### The Rule
Every command (relay on/off, fan speed change, LED animation) goes through the daemon's command queue. No shortcuts.

### Why
- Consistent ordering (commands execute in order received)
- State always matches what was sent (no race conditions)
- Can implement retries, rate limiting, timeouts uniformly
- Diagnostic/replay features work everywhere

### What This Means
✅ **Allowed:**
- Query/read operations can be prioritized (read latest status)
- But configuration changes must queue

❌ **Forbidden:**
- Sending command directly to ESP32 (even if you think it's faster)
- Bypassing queue for "urgent" commands
- Mixing queued and non-queued operations

### Queue States
Every command has a state: `Queued → Sending → Waiting ACK → Completed/Timeout/Failed`

---

## Rule 6: Protocol is Backwards Compatible

### The Rule
New firmware must understand old packet formats. Versioning is built in. Never break the protocol for a feature.

### Why
- Can update firmware without updating UI (or vice versa)
- Users don't get stuck on old versions
- Graceful degradation (old UI can talk to new firmware safely)

### What This Means
✅ **Allowed:**
- Adding new command codes
- Adding new fields to responses (at the end)
- Increasing packet size for new data

❌ **Forbidden:**
- Removing old commands
- Changing meaning of existing command codes
- Inserting new fields in the middle of a packet
- Assuming firmware and UI versions match

### Versioning
```
Protocol Version: 1.0
Config Version: 1.0
Events Version: 1.0
Capabilities Version: 1.0
```

Each can evolve independently with clear versioning.

---

## Rule 7: No Magic Numbers

### The Rule
Every constant, especially protocol values, has a name.

### Why
- Code is self-documenting
- Easy to find where values are used
- Prevents typos
- Single point of change

### What This Means
✅ **GOOD:**
```cpp
const uint16_t PACKET_HEADER = 0x55AA;
const uint8_t CMD_RELAY_ON = 0x10;
const uint32_t SERIAL_BAUD = 115200;
```

❌ **BAD:**
```cpp
if (data[0] == 0x55 && data[1] == 0xAA) { ... }  // What is 0x55AA?
sendByte(0x10);  // What does 0x10 mean?
```

### Constants File
All protocol constants live in `shared/docs/PROTOCOL_SPEC.md` and `shared/src/protocol.ts`.

---

## Rule 8: Every Module is Independently Testable

### The Rule
Each module can be tested without running the full system. Protocol layer testable without hardware. Daemon testable without ESP32.

### Why
- Fast feedback (tests run in milliseconds, not seconds)
- Easy to find bugs (pinpoint which layer failed)
- Development possible without hardware
- CI/CD works without physical devices

### What This Means
✅ **Testable in isolation:**
- Protocol encoder/decoder (no serial needed)
- Daemon core (using MockTransport)
- UI components (using Storybook or mock WebSocket)

❌ **Not allowed:**
- Tests that require hardware to be connected
- Tests that hardcode port names
- Tests that take 30 seconds to run

### Testing Layers
1. **Unit tests** - Single function/class
2. **Integration tests** - Multiple modules together (using mocks)
3. **UI tests** - Components with mock data
4. **Real hardware tests** - Final validation only

---

## Rule 9: No Global Mutable State (Except State Store)

### The Rule
Don't use global variables. Don't use class static fields for state. Don't use module-level state that can be modified.

**Exception:** The daemon's State Store is intentionally global and shared.

### Why
- Easy to reason about behavior (change = function parameters)
- Tests don't interfere (no cleanup needed)
- Debugging is straightforward (state comes from args, not internals)
- Thread-safe by design

### What This Means
✅ **GOOD:**
```typescript
function setRelay(queue: CommandQueue, id: number, value: boolean) {
  queue.add(new SetRelayCommand(id, value));
}
```

❌ **BAD:**
```typescript
let globalQueue: CommandQueue;  // ← Don't do this

function setRelay(id: number, value: boolean) {
  globalQueue.add(new SetRelayCommand(id, value));  // Where does it come from?
}
```

### Exception: State Store
```typescript
// ✅ This is OK (intentional singleton)
class StateStore {
  private static instance = new StateStore();
  
  static getInstance() {
    return this.instance;
  }
}
```

---

## Rule 10: Respond Quickly, But Correctly

### The Rule
Performance matters, but correctness matters more. Don't optimize prematurely. Don't sacrifice correctness for speed.

### Why
- Bugs are more expensive than slowness
- Premature optimization creates complexity
- Hardware is fast enough for this application
- Profile before optimizing

### What This Means
✅ **Do:**
- Write clear code first
- Use appropriate data structures
- Cache if data is reused
- Profile if something is slow

❌ **Don't:**
- Sacrifice readability for 10% speed gain
- Use unsafe optimizations to save a few bytes
- Skip logging for performance
- Assume something is slow without measuring

### Performance Targets
- Dashboard refresh: < 100ms
- Command response: < 500ms
- UI responsiveness: < 16ms per frame
- Reconnection: < 1 second

If you're not hitting these, investigate. If you are, don't optimize further.

---

## Rule 11: Protocol First Rule

### The Rule

Every protocol modification MUST follow this order:

1. **PROTOCOL_SPEC.md** - Define the new command, packet structure, or error code
2. **PROTOCOL_REGISTRY.md** - Register the command with full specification
3. **shared/** - Add new types, constants, or enums
4. **daemon/** - Implement encoder, decoder, controller, handler
5. **firmware/** - Implement command handler, response generation
6. **frontend/** - Add UI component, WebSocket binding

**Never modify implementation before updating the protocol specification.**
Documentation is the source of truth.

### Why

- Prevents misalignment between layers
- Makes code reviews easier (compare code to spec)
- Enables async development (firmware can build off spec alone)
- Easy to catch architecture mistakes early
- Provides reference for future contributors

### Example: Adding Temperature Threshold Command

```
1. PROTOCOL_SPEC.md:
   Define CMD_TEMP_SET_THRESHOLD packet format

2. PROTOCOL_REGISTRY.md:
   Register with command code, payload format, error codes

3. shared/src/protocol.ts:
   Add TemperatureSetThresholdRequest type

4. daemon/src/core/controllers/temperature.ts:
   Implement handler

5. firmware/src/commands/temperature.cpp:
   Implement device logic

6. frontend/src/hooks/useTemperature.ts:
   Add WebSocket binding
```

---

## Enforcing the Rules

### Code Review
Every PR is reviewed against these rules:
1. Does it violate any rule?
2. Is the violation necessary? (Usually no)
3. Is there documentation explaining the exception? (Rarely yes)

### Static Checks
Some rules are enforced by tools:
- **No circular dependencies:** Build will fail
- **No magic numbers:** Linter warns
- **Naming consistency:** Linter checks

### Testing
Tests catch violations of rules:
- Tests fail if state is mutable everywhere
- Tests fail if modules aren't independently testable
- Tests catch protocol breaks

---

## When Rules Conflict

Sometimes rules seem to conflict. Here's the priority:

1. **Rule 3 (Daemon owns state)** - Highest priority. Always wins.
2. **Rule 2 (UI never talks direct)** - High priority. Prevents shortcuts.
3. **Rule 5 (Command queue)** - Medium priority. Ensures consistency.
4. **Other rules** - Lower priority. But still important.

If you think you found a conflict, ask. Don't bypass a rule.

---

## Rule 12: Documentation Hierarchy

### Authority Order (Highest to Lowest)

1. **PROTOCOL_SPEC.md** - Binary protocol definition (highest authority)
2. **PROTOCOL_REGISTRY.md** - Command registry
3. **PROJECT_RULES.md** - Core project principles
4. **CODING_STANDARDS.md** - Code style and naming
5. **IMPLEMENTATION_ORDER.md** - Build sequence
6. **Architecture Decision Records** - Technical decisions
7. **Implementation Code** - Actual implementation (must match docs)

### What This Means

- If implementation conflicts with PROTOCOL_SPEC.md, implementation is wrong
- If code conflicts with PROJECT_RULES.md, code must be fixed
- If code conflicts with CODING_STANDARDS.md, code must be reformatted
- Documentation is always the source of truth
- Never implement before documenting

### Example

❌ **Wrong:**
```
Firmware implements RELAY_SET command without updating PROTOCOL_SPEC.md
```

✅ **Right:**
```
1. Update PROTOCOL_SPEC.md with new command
2. Register in PROTOCOL_REGISTRY.md
3. Update shared/src/protocol.ts
4. Implement in firmware
5. Code review verifies implementation matches spec
```

---

## Rule 13: Architecture Freeze Rule

### The Rule

After planning is complete, architecture changes require an **Architecture Decision Record (ADR)** documenting:

1. **Problem** - What issue needs solving?
2. **Decision** - What solution was chosen?
3. **Consequences** - What are the tradeoffs?
4. **Alternatives** - What else was considered?

No architectural refactoring is allowed without documenting these elements. This prevents uncontrolled design changes and keeps the architecture stable and intentional.

### Why

- Prevents architecture drift (death by a thousand cuts)
- Forces stakeholders to evaluate tradeoffs
- Creates audit trail of design decisions
- Makes future changes traceable
- Prevents repeating old mistakes

### What This Means

✅ **Allowed:**
- Bug fixes within existing architecture
- New features following established patterns
- Performance optimizations that don't change structure
- Internal refactoring that preserves interface

❌ **Forbidden:**
- Splitting/merging modules without ADR
- Changing layering (e.g., frontend talking directly to firmware)
- New architectural patterns without review
- Moving responsibilities between components
- Changing how state flows through system

### Example: Proposed Change

❌ **Wrong:**
```
"Let me move state management from daemon to firmware
 to reduce network latency"
```

✅ **Right:**
```
1. Propose ADR-0003: Distributed State Model
2. Document problem, decision, consequences
3. Get stakeholder approval
4. Update DECISIONS.md with new ADR
5. Update IMPLEMENTATION_ORDER.md if affected
6. Implement changes
```

### When to Create an ADR

| Scenario | Action |
|----------|--------|
| New major feature | Might need ADR if it introduces new patterns |
| Bug fix | No ADR needed |
| Performance optimization | No ADR unless architecture changes |
| Changing module boundaries | ADR required |
| Adding new transport layer | ADR required |
| Changing error handling strategy | ADR might be needed |
| Adding logging or diagnostics | No ADR needed |

### ADR Format

Location: `docs/adr/00XX-title.md` or `[component]/docs/adr/00XX-title.md`

```markdown
# ADR-NNNN: Title

## Context
What was the problem?

## Decision
What did we choose?

## Consequences
- Positive impacts
- Negative impacts
- Tradeoffs

## Rationale
Why this choice?

## Alternatives Considered
- Option 1: [why not]
- Option 2: [why not]
```

### Enforcement

- **Code review:** "This violates Rule 13 - need ADR"
- **Architecture check:** "No ADR found for this change"
- **Stakeholder approval:** ADR must be reviewed before implementation

---

## Rule 14: Implementation Verification

### The Rule

Before any protocol implementation is declared complete, all three must agree:

1. **Protocol Specification** (`shared/docs/PROTOCOL_SPEC.md`)
2. **Firmware Reference Implementation** (firmware source code)
3. **Protocol Test Vectors** (known good inputs/outputs)

A task is not complete until all three match exactly.

### Why

Mismatches between specification, firmware, and daemon can remain undetected until late development when they're most expensive to fix. This rule catches inconsistencies at Phase 1 when cost is minimal.

### What This Means

✅ **Do:**
- For new commands: define in PROTOCOL_SPEC.md first
- Implement in firmware
- Create test vectors for the command
- Implement in daemon and validate against test vectors
- Only then mark task complete

❌ **Don't:**
- Implement in firmware without updating spec first
- Assume daemon implementation is correct without verifying against test vectors
- Mark task complete without verifying all three match

### Example: Adding a New Command

```
1. Edit PROTOCOL_SPEC.md
   - Define command byte
   - Define payload format
   - Add example and test vector

2. Implement in firmware
   - Add command handler
   - Implement packet generation/validation

3. Create test vector
   - Input: raw bytes
   - Expected output: known good response

4. Implement in daemon
   - Encoder: command → packet
   - Decoder: packet → response
   - Test against protocol vector

5. Mark complete
   - All three (spec, firmware, daemon) agree
   - Test vectors pass
   - Code review approved
```

### Verification Checklist

Before marking any protocol task complete:

- [ ] Change documented in PROTOCOL_SPEC.md?
- [ ] Change implemented in firmware?
- [ ] Test vector added to PROTOCOL_SPEC.md?
- [ ] Daemon implementation validates against vector?
- [ ] Code passes all tests?
- [ ] Spec and firmware agree (byte-for-byte)?

### Enforcement

- **Code review:** "Where's the test vector?"
- **Task completion:** Task not closed until verification complete
- **Specification freeze:** Can't change protocol without updating docs

---

## Summary

| Rule | Essence | Enforced By |
|------|---------|------------|
| 1. Firmware ≠ UI | Firmware portable, UI-agnostic | Code review |
| 2. No direct firmware-UI | Single control point (daemon) | Architecture, dependencies |
| 3. Daemon owns state | Single source of truth | Code review, tests |
| 4. Log everything | Debugging and compliance | Code review, logging review |
| 5. Queue all commands | Consistent ordering, retries | Code review, tests |
| 6. Backwards compatible | Graceful updates | Protocol spec review |
| 7. No magic numbers | Readable, maintainable code | Linter, code review |
| 8. Independent testability | Fast, reliable development | Test suite passes locally |
| 9. No global mutable state | Predictable behavior | Linter, code review |
| 10. Correct > Fast | Quality over premature optimization | Code review, testing |
| 11. Protocol First | Documentation is source of truth | Code review, spec review |
| 12. Documentation Hierarchy | Clear authority order | Code review |
| 13. Architecture Freeze | Changes require ADR | Stakeholder review |
| 14. Implementation Verification | Spec/firmware/tests align | Code review, test vectors |

---

## Reading These Rules

**First time?**
- Read all 14 rules
- Notice which ones apply to your component
- Re-read those

**Before coding?**
- Skim all 14 rules (30-45 seconds)
- Focus on the ones your feature touches

**In code review?**
- Reference the rule number (e.g., "This violates Rule 3")
- Link to the rule

**Unsure?**
- "Does this violate a rule?" → Check here
- "How should I do this?" → Check here

---

These rules prevent architecture drift and keep the codebase maintainable as it grows. Follow them.
