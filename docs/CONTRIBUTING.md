# Contributing to NAS Controller

Thank you for contributing to the NAS Controller project. This guide explains how to get involved and maintain quality across the codebase.

---

## Before You Start

1. **Read the architecture overview** (`docs/PROJECT_OVERVIEW.md`)
2. **Understand the design principles** (read the ADRs in respective component docs)
3. **Review the relevant component docs** (firmware/docs/, daemon/docs/, frontend/docs/)
4. **Check the roadmap** (`docs/ROADMAP.md`)

---

## Development Environment Setup

```bash
# Clone the repository
git clone https://github.com/Sachin-Salgar/NAS-Controller-Firmware.git
cd NAS-Controller-Firmware

# Run setup script
bash scripts/setup.sh

# Install dependencies for each component
cd daemon && npm install
cd ../frontend && npm install
cd ../shared && npm install
```

---

## Architecture Principles

Keep these principles in mind throughout development:

> **The daemon owns all state. The firmware owns all hardware. The UI owns only presentation.**

- **Firmware:** Focused on deterministic hardware control. No UI logic.
- **Daemon:** Pure library (NASController.Core) + thin Express API. Testable without server.
- **Frontend:** Presentation only. All state comes from daemon via WebSocket.

---

## When Contributing

### 1. Firmware Changes
- **Edit:** `firmware/src/`
- **Document:** Update `firmware/docs/ARCHITECTURE.md` if design changes
- **Reference:** `shared/docs/PROTOCOL_SPEC.md` for command definitions
- **Test:** On real hardware before submitting

### 2. Daemon Changes
- **Edit:** `daemon/src/` (prefer daemon/src/core/ for logic)
- **Document:** Update `daemon/docs/ARCHITECTURE.md` if design changes
- **Test:** `npm test` (jest tests run without hardware via MockTransport)
- **Type Safety:** Use strict TypeScript. Update `shared/` types as needed.

### 3. Frontend Changes
- **Edit:** `frontend/src/`
- **Document:** Update `frontend/docs/COMPONENTS.md` for new components
- **Test:** Run dev server, manual testing in browser
- **Types:** Import types from `shared/` package

### 4. Shared Types Changes
- **Edit:** `shared/src/`
- **Document:** Update `shared/docs/DATA_TYPES.md`
- **Version:** If breaking change, increment version (e.g., protocol v1 → v2)
- **Publish:** Update daemon and frontend package.json to use new version

---

## Code Style

### TypeScript

- **Strict mode:** Always use `"strict": true` in tsconfig.json
- **No `any`:** Prefer explicit types
- **Naming:** camelCase for variables/functions, PascalCase for types/classes
- **Comments:** Only explain WHY, not WHAT. Code should be self-documenting.
- **Imports:** Group imports (standard library, npm packages, local)

Example:
```typescript
import { EventEmitter } from 'events'
import { serialport } from 'serialport'

import { ITransport } from './transport'

export class MyClass implements ITransport {
  // No comments for obvious things
  async connect(): Promise<void> {
    // Only comment non-obvious decisions
    // We use exponential backoff to avoid overwhelming the device
    await this.retryWithBackoff()
  }
}
```

### Documentation

- **Location:** Colocate docs with code (daemon/docs/ for daemon, etc.)
- **Format:** Markdown with clear headings
- **Audience:** Write for someone unfamiliar with the code
- **Links:** Reference other docs and code locations clearly
- **Examples:** Include code examples for complex features

---

## Adding New Features

### 1. Update Protocol Spec (if needed)
- Edit `shared/docs/PROTOCOL_SPEC.md`
- Add command definition
- Document payload format
- Define ACK/NAK behavior

### 2. Update Shared Types
- Edit `shared/src/commands.ts` (add command type)
- Edit `shared/src/events.ts` (add event type if needed)
- Bump version if breaking change

### 3. Implement in Daemon Core
- Add handler in `daemon/src/core/controllers/`
- Update `daemon/src/core/protocol/` if new command type
- Write unit tests in `daemon/tests/`
- Test with MockTransport (no hardware needed)

### 4. Update WebSocket API
- Add Socket.io handler in `daemon/src/api/`
- Wire daemon core to Socket.io (thin bridge)

### 5. Implement in Frontend
- Create component in `frontend/src/components/`
- Update page in `frontend/src/pages/`
- Import types from `shared/`
- Manual testing in browser

### 6. Document
- Update relevant docs (firmware/docs/, daemon/docs/, frontend/docs/)
- Create ADR if architectural decision
- Update CHANGELOG.md

---

## Testing Strategy

### Daemon (No Hardware Required)

```bash
cd daemon
npm test              # Runs all unit tests
npm test -- --watch  # Watch mode for development
```

Tests use MockTransport by default, so you don't need ESP32 connected.

### Frontend

```bash
cd frontend
npm run dev           # Start dev server
# Manual testing in browser (localhost:5173)
```

### Integration Tests

```bash
cd daemon
npm test -- integration.test.ts  # Tests daemon core + MockTransport
```

### Hardware Testing

```bash
# Connect real ESP32 via USB
cd daemon
npm run test:hardware  # Runs integration tests against real device
```

---

## Commit Messages

Use conventional commit format:

```
type(scope): subject

body

footer
```

**Types:** feat, fix, docs, style, refactor, test, chore  
**Scope:** daemon, frontend, firmware, shared, etc.  
**Subject:** Imperative, lowercase, no period

Example:
```
feat(daemon): add auto-reconnect with state resync

When USB connection is lost, the daemon now automatically
attempts reconnection every second and requests a full
state synchronization from the firmware.

Fixes #123
```

---

## Pull Request Process

1. **Fork and branch** from `main` with descriptive name: `feature/auto-reconnect`
2. **Keep commits clean** - each commit should be a logical unit
3. **Write tests** - new code should have corresponding tests
4. **Update docs** - document your changes (ADRs for architecture)
5. **Describe changes** - PR description should explain what and why
6. **Request review** - at least one approval before merge

---

## Reporting Issues

Use GitHub Issues with:

1. **Clear title** - what's broken
2. **Environment** - OS, firmware version, etc.
3. **Steps to reproduce** - be specific
4. **Expected behavior** - what should happen
5. **Actual behavior** - what actually happens
6. **Logs** - include relevant logs (enable DIAGNOSTICS_ENABLE)

---

## Questions?

1. **Architecture questions:** Read the ADRs and design docs first
2. **Implementation questions:** Check relevant component docs
3. **Still stuck:** Open a GitHub Discussion or Issue

---

**Happy coding!** The NAS Controller project values quality contributions at every level. Thank you for helping make this better.
