# Developer Guide

**Welcome to the NAS Controller project.** This is the first document you should read before contributing.

---

## What is NAS Controller?

A sophisticated firmware platform for controlling network-attached storage hardware, with a complementary web-based control interface. Three-tier architecture:

- **Firmware** (ESP32): Hardware control, real-time monitoring
- **Daemon** (Node.js): USB bridge, state management, WebSocket server
- **Frontend** (React): Web UI for control and monitoring

---

## Getting Started (5 minutes)

### 1. Understand the Project Structure

```
firmware/          ← ESP32 firmware code
daemon/            ← Node.js backend
frontend/          ← React web UI
shared/            ← Shared type definitions
docs/              ← Project-wide documentation
```

Each component has its own `docs/README.md`. Start there after reading this guide.

### 2. Read the Core Rules

Before writing ANY code, read [PROJECT_RULES.md](PROJECT_RULES.md). These 10 rules guide every implementation decision.

**Key rules:**
- Firmware never knows the UI exists
- UI never talks directly to ESP32
- Daemon owns all state
- Everything is logged
- No global mutable state
- Every module is independently testable

### 3. Review Architecture Overview

Read [ARCHITECTURE_OVERVIEW.md](ARCHITECTURE_OVERVIEW.md) to understand how the three layers work together.

### 4. Check Coding Standards

Before writing code, review [CODING_STANDARDS.md](CODING_STANDARDS.md) for naming conventions, formatting, and style.

---

## Development Workflow

### Before Starting a Feature

1. **Check the roadmap** - Is it in the plan? (See [WEB_INTERFACE_PLAN.md](../frontend/docs/WEB_INTERFACE_PLAN.md) for web features)
2. **Review PROJECT_RULES** - Does your approach violate any core rules?
3. **Check MODULE_BOUNDARIES** - Is your change in the right module?
4. **Review DEPENDENCY_DIAGRAM** - Are your dependencies correct?

### While Coding

1. **Follow CODING_STANDARDS.md** - Naming, formatting, comments
2. **Write tests first** - See [TESTING_STRATEGY.md](TESTING_STRATEGY.md)
3. **Use ERROR_RECOVERY_MATRIX.md** - How do you handle failures?
4. **Check SECURITY_POLICY.md** - Is your code secure?

### Before Committing

1. **Tests pass locally** - Run test suite
2. **Follow commit format** - See [CODING_STANDARDS.md](CODING_STANDARDS.md#commit-messages)
3. **Update documentation** - If behavior changed
4. **Self-review** - Does it follow the rules?

### Before Pushing

1. **Run code review** - Use skill or manual review
2. **Verify no regressions** - Run full test suite
3. **Check BUILD_RELEASE_PROCESS** - If this goes to production

---

## Key Documents Quick Reference

### Rules & Standards (Read First)
- **[PROJECT_RULES.md](PROJECT_RULES.md)** - 10 core project rules
- **[CODING_STANDARDS.md](CODING_STANDARDS.md)** - How to write code
- **[MODULE_BOUNDARIES.md](MODULE_BOUNDARIES.md)** - What code goes where
- **[DEPENDENCY_DIAGRAM.md](DEPENDENCY_DIAGRAM.md)** - How components connect

### Design & Architecture (Read Before Building)
- **[ARCHITECTURE_OVERVIEW.md](ARCHITECTURE_OVERVIEW.md)** - System design
- **[ARCHITECTURE_FREEZE.md](ARCHITECTURE_FREEZE.md)** - What can/cannot change
- **[BOOT_SHUTDOWN_SEQUENCE.md](BOOT_SHUTDOWN_SEQUENCE.md)** - System lifecycle

### Operations & Quality (Read Before Testing/Releasing)
- **[TESTING_STRATEGY.md](TESTING_STRATEGY.md)** - How to test
- **[ERROR_RECOVERY_MATRIX.md](ERROR_RECOVERY_MATRIX.md)** - Error handling rules
- **[SECURITY_POLICY.md](SECURITY_POLICY.md)** - Security guidelines
- **[BUILD_RELEASE_PROCESS.md](BUILD_RELEASE_PROCESS.md)** - Release workflow

### Component-Specific (Read for Your Component)
- **[firmware/docs/README.md](../firmware/docs/README.md)** - Firmware overview
- **[daemon/docs/README.md](../daemon/docs/README.md)** - Daemon overview
- **[frontend/docs/README.md](../frontend/docs/README.md)** - Frontend overview
- **[shared/docs/README.md](../shared/docs/README.md)** - Protocol & types

---

## The Golden Rule

> **The daemon owns all state. The firmware owns all hardware. The UI owns only presentation.**

This separation ensures:
- **Testability** - Daemon testable without hardware (using MockTransport)
- **Consistency** - Single source of truth (the daemon)
- **Clarity** - Clear responsibilities = predictable behavior

If your code violates this, something is wrong. Stop and re-read [PROJECT_RULES.md](PROJECT_RULES.md).

---

## Architecture at a Glance

```
┌─────────────────────┐
│   Web Browser       │
│   (React UI)        │
└──────────┬──────────┘
           │ WebSocket
           ↓
┌──────────────────────────────────┐
│   Node.js Daemon                 │
│   (NASController.Core + Express) │
│   - State Store (single source)  │
│   - Command Queue                │
│   - Event Log                    │
└──────────┬───────────────────────┘
           │ USB Serial
           ↓
┌──────────────────────┐
│   ESP32 Firmware     │
│   (Hardware Control) │
└──────────────────────┘
```

**Data flow principle:** Changes flow UP (firmware → daemon → UI), commands flow DOWN (UI → daemon → firmware).

---

## Communication Patterns

### When User Clicks a Button (Command Flow)

```
Browser                Daemon              Firmware
  │                      │                    │
  ├─ WebSocket ─────────>│                    │
  │  "turn_relay_on"     │                    │
  │                      ├─ USB Serial ─────>│
  │                      │  [binary packet]   │
  │                      │                    │
  │                      │<─ ACK ─────────────┤
  │                      │                    │
  │  <─ WebSocket ───────┤                    │
  │  "relay_on_success"  │                    │
  │                      │                    │
```

### When Hardware Changes State (Status Flow)

```
Firmware            Daemon              Browser
  │                   │                    │
  ├─ USB Serial ─────>│                    │
  │  [temperature]    │                    │
  │                   ├─ Parse & Store    │
  │                   ├─ WebSocket ──────>│
  │                   │  "temp_changed"   │
  │                   │   value: 45.2     │
  │                   │                    │
```

All communication goes through the **daemon**. Never direct browser-to-firmware.

---

## Dependency Rules

### Allowed Dependencies (downward only)
```
UI       (frontend, React)
  ↓
Daemon   (Node.js, Express, Socket.io)
  ↓
Protocol (serialport, packet encoding)
  ↓
Firmware (ESP32, HAL, drivers)
```

### Forbidden Dependencies (upward)
```
❌ Firmware importing UI code
❌ Frontend importing daemon code
❌ Daemon importing firmware code
❌ Circular dependencies anywhere
```

See [DEPENDENCY_DIAGRAM.md](DEPENDENCY_DIAGRAM.md) for detailed rules.

---

## What Success Looks Like

### Code Quality
- ✅ Follows [CODING_STANDARDS.md](CODING_STANDARDS.md)
- ✅ No circular dependencies
- ✅ Every module independently testable
- ✅ Clear, descriptive names (no abbreviations)
- ✅ Comments only for WHY, not WHAT

### Testing
- ✅ Unit tests for all business logic
- ✅ Integration tests using MockTransport
- ✅ UI tested in browser
- ✅ Manual testing on real hardware

### Documentation
- ✅ Code changes documented in commit message
- ✅ New features documented
- ✅ Architecture decisions recorded in ADRs

### Architecture
- ✅ Follows [PROJECT_RULES.md](PROJECT_RULES.md)
- ✅ Respects [DEPENDENCY_DIAGRAM.md](DEPENDENCY_DIAGRAM.md)
- ✅ Handles errors per [ERROR_RECOVERY_MATRIX.md](ERROR_RECOVERY_MATRIX.md)
- ✅ Implements security per [SECURITY_POLICY.md](SECURITY_POLICY.md)

---

## Common Questions

**Q: Where do I add a new feature?**  
A: Check [MODULE_BOUNDARIES.md](MODULE_BOUNDARIES.md) to find the right module. If it's UI, go to `frontend/`. If it's hardware control, it might be daemon, not firmware.

**Q: How do I handle errors?**  
A: Check [ERROR_RECOVERY_MATRIX.md](ERROR_RECOVERY_MATRIX.md). Find your error type, follow the retry policy and user message.

**Q: Can I add a dependency?**  
A: Check [PROJECT_RULES.md](PROJECT_RULES.md#rule-9-no-global-mutable-state) and [DEPENDENCY_DIAGRAM.md](DEPENDENCY_DIAGRAM.md). Some layers are locked. If unsure, ask.

**Q: What should I test?**  
A: Read [TESTING_STRATEGY.md](TESTING_STRATEGY.md). The testing pyramid shows what's expected at each level.

**Q: How do I release?**  
A: Follow [BUILD_RELEASE_PROCESS.md](BUILD_RELEASE_PROCESS.md) exactly. It's a checklist.

---

## Getting Help

### Quick Answers
- **"Where does this code go?"** → [MODULE_BOUNDARIES.md](MODULE_BOUNDARIES.md)
- **"How do I name this?"** → [CODING_STANDARDS.md](CODING_STANDARDS.md)
- **"How do I handle this error?"** → [ERROR_RECOVERY_MATRIX.md](ERROR_RECOVERY_MATRIX.md)
- **"Is this architecture right?"** → [PROJECT_RULES.md](PROJECT_RULES.md)

### Detailed Information
- **Component-specific:** Read `component/docs/README.md` (firmware, daemon, frontend, or shared)
- **Architecture decisions:** Check `component/docs/adr/` folders
- **System design:** Read [ARCHITECTURE_OVERVIEW.md](ARCHITECTURE_OVERVIEW.md)

### Still Stuck?
- Review the relevant ADR (architecture decision record) in the component folder
- Check if similar code exists elsewhere—follow that pattern
- Ask a question in your commit or PR—document your thinking

---

## Environment Setup

### Firmware Development
```bash
cd firmware
# Install PlatformIO
platformio run -e esp32doit-devkit-v1 -t upload
```

### Daemon Development
```bash
cd daemon
npm install
npm run dev        # With hot reload
npm test           # Run tests
```

### Frontend Development
```bash
cd frontend
npm install
npm run dev        # Vite dev server
npm test           # Run component tests
```

See `GETTING_STARTED.md` for detailed setup.

---

## Onboarding Checklist

Before your first code contribution:

- [ ] Read this guide (DEVELOPER_GUIDE.md)
- [ ] Read [PROJECT_RULES.md](PROJECT_RULES.md)
- [ ] Read [ARCHITECTURE_OVERVIEW.md](ARCHITECTURE_OVERVIEW.md)
- [ ] Read [CODING_STANDARDS.md](CODING_STANDARDS.md)
- [ ] Read component-specific docs for your area
- [ ] Review an existing PR or two (understand the style)
- [ ] Run tests locally (verify setup works)
- [ ] Ask questions (better now than later)

---

## Key Takeaways

1. **Architecture is frozen.** Read [ARCHITECTURE_FREEZE.md](ARCHITECTURE_FREEZE.md) to understand what can change.

2. **Rules guide decisions.** When unsure, check [PROJECT_RULES.md](PROJECT_RULES.md).

3. **Dependencies matter.** Respect [DEPENDENCY_DIAGRAM.md](DEPENDENCY_DIAGRAM.md).

4. **Error handling is consistent.** Follow [ERROR_RECOVERY_MATRIX.md](ERROR_RECOVERY_MATRIX.md).

5. **Testing is mandatory.** See [TESTING_STRATEGY.md](TESTING_STRATEGY.md).

6. **Documentation is required.** Update docs when you change behavior.

7. **Releases are repeatable.** Follow [BUILD_RELEASE_PROCESS.md](BUILD_RELEASE_PROCESS.md) exactly.

---

## Document Map

All documentation is indexed at [docs/README.md](README.md). Bookmark that page—it's your navigation hub.

---

**Ready to start?**

1. Pick a task from the roadmap
2. Read the relevant docs for your component
3. Follow [CODING_STANDARDS.md](CODING_STANDARDS.md)
4. Write tests first
5. Verify against [ERROR_RECOVERY_MATRIX.md](ERROR_RECOVERY_MATRIX.md)
6. Commit with a clear message
7. Push and request review

Welcome aboard! 🚀
