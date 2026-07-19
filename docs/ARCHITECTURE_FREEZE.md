# Architecture Freeze v1.0

**Status:** FROZEN (July 19, 2026)

**Purpose:** Prevent architecture drift as the project grows. Define what can and cannot change without explicit stakeholder approval.

---

## What is Frozen?

### The Core Architecture ✅ FROZEN

```
┌─────────────┐
│  Web UI     │  React + Vite + TypeScript
│  (Frontend) │
└──────┬──────┘
       │ WebSocket
       ↓
┌──────────────────────────┐
│   Node.js Daemon         │  Library-first design
│   (Backend)              │  Express + Socket.io
│   State Store            │  Command Queue
└──────┬───────────────────┘
       │ USB Serial (CDC)
       ↓
┌──────────────┐
│ ESP32        │  Existing firmware
│ (Firmware)   │  Binary protocol
└──────────────┘
```

**This structure does not change.** No exceptions.

### Responsibility Model ✅ FROZEN

```
┌─ Firmware owns hardware
├─ Daemon owns state and coordination
└─ UI owns presentation only
```

**This responsibility separation does not change.**

### Communication Pattern ✅ FROZEN

```
Commands:   UI → Daemon → Firmware (via binary protocol)
Updates:    Firmware → Daemon → UI (via WebSocket)
```

**This pattern does not change.**

### Key Decisions ✅ FROZEN

These architectural decisions are locked:

1. **WebSocket-first communication** - REST only for bulk data
2. **Binary protocol** - Efficient, proven, existing
3. **Centralized state store** - Single source of truth in daemon
4. **Command queue** - Queued → Sending → Waiting ACK → Completed/Failed
5. **Abstracted transport layer** - ITransport interface (Serial, TCP, Mock, etc.)
6. **Capability discovery** - Firmware reports capabilities on connect
7. **Atomic transactions** - Config changes all-or-nothing
8. **Error classification** - Transport, Protocol, Firmware, Validation, User, Internal
9. **Shared type package** - Protocol definitions shared between daemon and frontend
10. **Library-first daemon** - NASController.Core is reusable library, Express is thin layer

---

## What CAN Change ✅

### Bug Fixes
- Fix a crash
- Fix logic error
- Fix memory leak
- Fix performance regression

**Approval needed:** Code review only (PR)

**Example:**
```
Bad:   "Add WebSocket-over-HTTP fallback"
Good:  "Fix memory leak in packet inspector"
```

### Clarifications
- Clarify ambiguous specification
- Reorganize documentation
- Better error messages
- Improve logging

**Approval needed:** Code review only (PR)

**Example:**
```
Bad:   "Add REST endpoint for real-time status"
Good:  "Clarify protocol spec for CRC16 validation"
```

### Missing Implementation
- Implement planned feature from roadmap
- Add module from architecture doc
- Implement documented behavior that's missing

**Approval needed:** Code review (PR) + acceptance criteria verification

**Example:**
```
Bad:   "Add mobile app support"
Good:  "Implement Configuration Editor (Phase 2 feature)"
```

### Performance Optimization
- Optimize algorithm (same behavior)
- Reduce memory usage (same behavior)
- Improve latency (same behavior)

**Approval needed:** Code review + performance benchmarks

**Example:**
```
Bad:   "Switch from JSON to MessagePack for web"
Good:  "Optimize relay command processing (< 50ms target)"
```

### Documentation Updates
- Update architecture docs
- Add ADRs for future decisions
- Improve code comments
- Add examples

**Approval needed:** Code review (no functional change)

---

## What CANNOT Change ✗

### New Features (Outside Roadmap)
Adding capabilities not in the approved roadmap.

**Must have:** Stakeholder approval + architecture review

**Example:**
```
❌ "Add MQTT support"
❌ "Add authentication to daemon"
❌ "Add mobile app"
```

### New Technologies
Introducing new languages, frameworks, or dependencies (beyond approved list).

**Must have:** Stakeholder approval + impact analysis

**Example:**
```
❌ "Switch frontend to Vue"
❌ "Use Rust for daemon instead of Node"
❌ "Switch to gRPC for communication"
```

### Architectural Restructuring
Changing how layers connect or responsibility boundaries.

**Must have:** Stakeholder approval + full redesign review

**Example:**
```
❌ "Make firmware talk directly to database"
❌ "Expose daemon state directly to UI (skip state sync)"
❌ "Remove command queue, send commands directly"
```

### Circular Dependencies
Introducing dependency direction violations.

**Must have:** Stakeholder approval + architectural review

**Example:**
```
❌ "Have firmware import UI types"
❌ "Have frontend send raw bytes to serial"
❌ "Have daemon depend on React"
```

### Protocol Changes (Breaking)
Removing commands, changing packet structure, breaking existing clients.

**Must have:** Stakeholder approval + migration path

**Example:**
```
❌ "Change packet header from 0x55AA to 0xABCD"
❌ "Remove relay on/off command"
❌ "Move CRC from end to beginning (old firmware breaks)"
```

### Core Principles Violations
Violating the 10 rules in PROJECT_RULES.md.

**Must have:** Stakeholder approval + exception documentation

**Example:**
```
❌ "Make firmware know about the UI"
❌ "Allow firmware to own application state"
❌ "Let UI talk directly to ESP32"
```

---

## Approval Process

### For Changes That Can Change (Bug Fixes, Clarifications)

```
Developer commits
    ↓
Code review (PR)
    ↓
If approved: Merge
If rejected: Fix and resubmit
```

**Time to approval:** 24 hours (standard PR process)

### For Changes That Cannot Change (New Features, Architecture)

```
Developer proposes change
    ↓
Stakeholder review meeting
    ↓
If rejected: Document reason, try alternative approach
If approved: Document decision in ADR
    ↓
Implement with ADR reference
    ↓
Code review (PR)
    ↓
Merge
```

**Time to approval:** 1 week minimum (requires discussion)

---

## How to Know Which Category

### Ask These Questions

1. **Does it change behavior users see?**
   - No → Probably OK (bug fix or clarification)
   - Yes → Check next question

2. **Is it in the approved roadmap?**
   - Yes → OK (implement as planned)
   - No → Cannot change without approval

3. **Does it affect how layers communicate?**
   - No → Probably OK
   - Yes → Cannot change without approval

4. **Does it add a new technology?**
   - No → Probably OK
   - Yes → Cannot change without approval

5. **Does it violate a Project Rule?**
   - No → Probably OK
   - Yes → Cannot change without approval

---

## Documenting Exceptions

### If Frozen Change Needed

**Step 1: Document why it's necessary**
```
Problem: [describe the actual problem, not the solution]
Impact: [who is affected, how severely]
Alternatives: [what other approaches were considered and why they don't work]
Proposed Solution: [the specific change]
Risks: [what could go wrong]
Rollback Plan: [how to undo if it fails]
```

**Step 2: Create ADR**
```
docs/adr/
└── XXXX-exception-to-freeze.md
    (explains the exception, when it's OK, when it's not)
```

**Step 3: Get stakeholder approval**

**Step 4: Implement with ADR reference**

---

## Architecture Reviewers

These people can approve changes to frozen architecture:

- Sachin Salgar (project lead)
- Senior firmware architect
- Senior backend architect
- Senior frontend architect

Minor clarifications can be approved by code review. Major changes need consensus.

---

## Review Checklist

### For Every PR

**Is this a bug fix?** → Approve if tests pass
**Is this clarification?** → Approve if accurate
**Is this new feature?** → Check roadmap
- ✅ In roadmap? Approve if tests pass
- ❌ Not in roadmap? Request stakeholder approval

**Is this architecture change?** → Request stakeholder approval
**Does it violate a rule?** → Request stakeholder approval or reject
**Does it change protocol?** → Request stakeholder approval

---

## When Architecture Can Unfreeze

The architecture can be unfrozen only if:

1. **Stakeholder consensus** - Unanimous decision to change
2. **Documented reason** - Why the frozen architecture no longer works
3. **Full impact analysis** - What breaks, how to migrate
4. **Timeline** - When will change be complete

This is a serious decision. Expect weeks of planning.

---

## Migration Path If Changes Needed

### Scenario: "We need to use REST instead of WebSocket"

1. **Propose exception** - Document why WebSocket doesn't work
2. **Get approval** - Stakeholder review meeting
3. **Create ADR** - Document the new decision
4. **Implement with dual support** - Both WebSocket and REST work during transition
5. **Migrate clients** - UI and daemon both support both protocols
6. **Deprecate old** - When all clients migrated, remove WebSocket (not for this project, just example)

This maintains stability while evolving.

---

## FAQ

**Q: Can I change the frontend framework?**  
A: No. That's a new technology. Requires stakeholder approval.

**Q: Can I optimize the command queue?**  
A: Yes, if behavior stays the same. Optimize away.

**Q: Can I add a new WebSocket command?**  
A: Yes, if it's in the roadmap. If you're extending beyond the plan, needs approval.

**Q: Can I use Protocol Buffers instead of binary protocol?**  
A: No. Protocol is frozen. Would break all firmware. Requires stakeholder approval.

**Q: Can I add logging to a module?**  
A: Yes. More logging is always OK (for debugging).

**Q: Can I refactor code that violates a rule?**  
A: Yes. In fact, please do. But respect the frozen architecture.

**Q: What if the firmware needs to change?**  
A: Firmware changes are fine if they don't change the protocol or responsibility boundaries. If protocol changes needed, request approval.

---

## Summary

### In Scope (Can Change)
✅ Bug fixes
✅ Clarifications
✅ Planned features
✅ Performance optimization
✅ More tests
✅ More documentation
✅ Logging improvements

### Out of Scope (Cannot Change Without Approval)
❌ New technologies
❌ New unplanned features
❌ Architecture restructuring
❌ Protocol changes (breaking)
❌ Responsibility boundary changes
❌ Circular dependencies

---

## Document Status

**Freeze Date:** July 19, 2026  
**Frozen By:** Architecture Review  
**Current Version:** 1.0  
**Next Review:** When major feature completion changes timeline

This freeze stands until explicitly unfrozen by stakeholder decision.

---

**Follow the freeze. It keeps the project stable.**
