# Coding Standards

Standards for writing code that fits the NAS Controller project.

---

## Naming Conventions

### Variables and Constants

**Local variables:** camelCase
```typescript
const deviceCount = 5;
let isConnected = false;
let retryAttempts = 0;
```

**Class/module constants:** UPPER_SNAKE_CASE
```typescript
const MAX_PACKET_SIZE = 256;
const DEFAULT_TIMEOUT_MS = 5000;
const PROTOCOL_VERSION = 1;
```

**Enum values:** UPPER_SNAKE_CASE
```typescript
enum CommandStatus {
  QUEUED = 0,
  SENDING = 1,
  WAITING_ACK = 2,
  COMPLETED = 3,
  FAILED = 4,
}
```

**Private fields:** _leadingUnderscore
```typescript
class CommandQueue {
  private _queue: Command[] = [];
  private _maxSize = 100;
}
```

### Functions and Methods

**Function names:** camelCase, verb-first
```typescript
function sendCommand(cmd: Command): void { }
function calculateChecksum(data: Uint8Array): number { }
function isValid(packet: Packet): boolean { }
```

**Event handlers:** on + EventName
```typescript
function onPacketReceived(packet: Packet) { }
function onConnectionLost() { }
function onTimeoutOccurred() { }
```

### Classes and Types

**Classes:** PascalCase
```typescript
class CommandQueue { }
class StateStore { }
class SerialTransport { }
```

**Interfaces:** PascalCase (no I prefix)
```typescript
interface Transport { }
interface Packet { }
interface Command { }
```

**Type aliases:** PascalCase
```typescript
type CommandId = number;
type PacketData = Uint8Array;
```

### Files and Folders

**TypeScript files:** lowercase-with-hyphens
```
src/
├── protocol/
│   ├── packet-encoder.ts
│   ├── packet-decoder.ts
│   └── crc16.ts
├── transport/
│   ├── serial-transport.ts
│   └── mock-transport.ts
└── queue/
    └── command-queue.ts
```

**Folders:** lowercase-no-hyphens (but hyphens okay for clarity)
```
src/
├── protocol/     (good)
├── error-handling/  (also acceptable)
└── not_this      (never underscores)
```

**Test files:** same.test.ts or same.spec.ts
```
src/
├── packet-encoder.ts
└── packet-encoder.test.ts
```

---

## Code Style

### Formatting

**Indentation:** 2 spaces (no tabs)
```typescript
if (condition) {
  doSomething();
  if (nestedCondition) {
    doMore();
  }
}
```

**Line length:** 100 characters (hard limit: 120)
```typescript
// ✅ Good: 78 characters
const result = calculateValue(input1, input2, input3);

// ❌ Avoid: 130 characters (too long)
const veryLongVariableNameForSomeReasonThatMakesThisLineRidiculouslyLongAndHardToRead = doSomething(param1, param2, param3);
```

**Semicolons:** Always use them
```typescript
const x = 5;    // ✅ Good
const y = 6    // ❌ Bad (no semicolon)
```

**Quotes:** Double quotes for strings
```typescript
const message = "Hello, world";    // ✅ Good
const message = 'Hello, world';    // ❌ Bad
```

**Comma-dangle:** Always include trailing commas in multi-line structures
```typescript
const config = {
  host: "localhost",
  port: 3000,
  timeout: 5000,      // ✅ Trailing comma
};

const values = [
  1,
  2,
  3,              // ✅ Trailing comma
];
```

**Spacing:** One space after keywords
```typescript
if (condition) { }       // ✅ Space after if
if(condition) { }        // ❌ No space

for (let i = 0; i < 10; i++) { }    // ✅ Space after for
for(let i = 0; i < 10; i++) { }     // ❌ No space
```

### Imports and Exports

**Import order:** Standard library → Third-party → Local

```typescript
import { EventEmitter } from "events";
import * as fs from "fs";

import express from "express";
import { Socket } from "socket.io";

import { Protocol } from "./protocol";
import { CommandQueue } from "./queue";
```

**Default vs named:** Use named imports (except for modules that export only default)

```typescript
// ✅ Good
import { Protocol } from "./protocol";
import { serialport } from "serialport";

// ❌ Bad (unnamed when module has named exports)
import Protocol from "./protocol";
```

**Export order:** Most important first

```typescript
export class CommandQueue { }
export interface QueueOptions { }
export function createQueue() { }
```

---

## Type Safety

### TypeScript Strictness

**tsconfig.json:**
```json
{
  "compilerOptions": {
    "strict": true,
    "noImplicitAny": true,
    "noUnusedLocals": true,
    "noUnusedParameters": true,
    "noImplicitReturns": true
  }
}
```

This means:
- Always annotate types
- No `any` unless absolutely necessary (and documented)
- No unused variables or imports
- All code paths must return

### Type Annotations

**Always annotate function parameters and returns:**

```typescript
// ✅ Good
function send(packet: Packet): Promise<AckResponse> { }
function getId(): number { }

// ❌ Bad (implicit return type, missing param type)
function send(packet) { }
function getId() { }
```

**Use generics for reusable code:**

```typescript
// ✅ Good
class Queue<T> {
  items: T[] = [];
  enqueue(item: T): void { }
  dequeue(): T | undefined { }
}

// ❌ Bad (returns any, loses type info)
class Queue {
  items: any[] = [];
  enqueue(item: any): void { }
  dequeue(): any { }
}
```

**Avoid union types when possible:**

```typescript
// ✅ Better
interface RelayCommand {
  type: "relay";
  id: number;
  value: boolean;
}

interface FanCommand {
  type: "fan";
  speed: number;
}

type Command = RelayCommand | FanCommand;

// ❌ Less clear
type Command = {
  type: string;
  id?: number;
  value?: boolean;
  speed?: number;
};
```

---

## Comments

### When to Comment

**DON'T comment the obvious:**
```typescript
// ❌ Bad: Comment explains what the code does
let x = 5;  // Set x to 5

function getName() {  // Return the name
  return this.name;
}
```

**DO comment the WHY:**
```typescript
// ✅ Good: Explains why this is done
let maxRetries = 3;  // Firmware needs 3 attempts max before disconnecting

function getName() {
  // Return local copy to prevent external modification
  return [...this.name];
}
```

**DO comment non-obvious logic:**
```typescript
// ✅ Good: Explains why this specific value
const RECONNECT_DELAY_MS = 1000;  // Exponential backoff starts at 1s

// Drain pending commands before reconnect
// (firmware may have processed them even if we didn't get ACK)
await queue.drain();
```

**DO comment workarounds and hacks:**
```typescript
// ✅ Good: Explains why this hack exists
// HACK: ESP32 firmware has bug where it sends extra padding bytes
// Remove them manually. Fixed in v2.1. Remove this when firmware updated.
if (packet.length > expectedLength) {
  packet = packet.slice(0, expectedLength);
}
```

### Comment Style

**Use //, not /* */**
```typescript
// ✅ Good
// This is a comment
// spanning multiple lines

// ❌ Bad for multi-line comments
/* This is a comment
   spanning multiple lines */
```

**One sentence per line:**
```typescript
// ✅ Good
// Connection lost due to timeout.
// Will automatically reconnect in 5 seconds.

// ❌ Bad
// Connection lost due to timeout. Will automatically reconnect in 5 seconds.
```

**No comment blocks:**
```typescript
// ❌ Bad: Too much ceremony for a comment
// =========================
// This is a section header
// =========================

// ✅ Good: Simple, clear comment
// State synchronization
```

---

## Error Handling

### Errors Should Be Specific

```typescript
// ✅ Good: Specific error types
class ValidationError extends Error { }
class TimeoutError extends Error { }
class DeviceDisconnectedError extends Error { }

throw new ValidationError("Packet CRC invalid");

// ❌ Bad: Generic error, no info
throw new Error("Error");
```

### Always Handle Errors

```typescript
// ✅ Good: Error handled
try {
  await sendPacket(packet);
} catch (error) {
  logger.error("Failed to send packet", { error, packet });
  throw new DeviceError("Device unreachable");
}

// ❌ Bad: Error silently ignored
try {
  await sendPacket(packet);
} catch (error) {
  // ignore
}
```

### Use Finally for Cleanup

```typescript
// ✅ Good: Ensures cleanup happens
let transaction = null;
try {
  transaction = beginTransaction();
  await transaction.send(commands);
  await transaction.commit();
} catch (error) {
  logger.error("Transaction failed", error);
  throw error;
} finally {
  transaction?.rollback();  // Always cleanup
}
```

---

## Logging

### Log Levels

| Level | When | Example |
|-------|------|---------|
| ERROR | Something failed and needs attention | Command timed out, reconnection failed |
| WARN | Something unexpected but recoverable | Packet dropped, retry attempted |
| INFO | Significant event | Device connected, configuration saved |
| DEBUG | Flow information for developers | State changed, packet sent, event received |
| TRACE | Every step (rarely used) | Loop iterations, internal state updates |

### Logging Standards

**Always include context:**
```typescript
// ✅ Good: Shows what and why
logger.info("Device connected", {
  port: "/dev/ttyUSB0",
  baudRate: 115200,
  timestamp: new Date().toISOString(),
});

// ❌ Bad: Too vague
logger.info("Device connected");
```

**Use structured logging (objects, not string concat):**
```typescript
// ✅ Good: Queryable, structured
logger.error("Command failed", {
  command: "relay_on",
  relayId: 1,
  error: err.message,
  duration: 234,
});

// ❌ Bad: String concatenation, hard to parse
logger.error("Command relay_on failed: " + err.message);
```

**Include error stack traces:**
```typescript
// ✅ Good: Includes stack
logger.error("Unexpected error", {
  error: err.stack,
  context: { operation: "sync" },
});

// ❌ Bad: No stack trace
logger.error("Error: " + err.message);
```

### Sensitive Data

**Never log:**
- Passwords
- API keys
- Personal information
- Firmware binary data

**OK to log:**
- Command types and IDs
- Status values
- Timing information
- Configuration keys (not values if sensitive)

---

## Testing

### Test File Structure

```typescript
describe("CommandQueue", () => {
  let queue: CommandQueue;

  beforeEach(() => {
    queue = new CommandQueue();
  });

  afterEach(() => {
    queue.clear();
  });

  describe("add()", () => {
    it("should add command to queue", () => {
      const cmd = new RelayCommand(1, true);
      queue.add(cmd);
      expect(queue.size()).toBe(1);
    });

    it("should throw if queue is full", () => {
      // Fill queue
      for (let i = 0; i < 100; i++) {
        queue.add(new RelayCommand(1, true));
      }
      // This should throw
      expect(() => queue.add(new RelayCommand(1, true)))
        .toThrow(QueueFullError);
    });
  });

  describe("process()", () => {
    it("should process commands in order", async () => {
      // Test implementation
    });
  });
});
```

### Test Naming

**Test names should describe the behavior:**
```typescript
// ✅ Good: Clear behavior
it("should return the name when getName is called")
it("should throw ValidationError when packet CRC is invalid")
it("should retry up to 3 times before giving up")

// ❌ Bad: Too generic or incomplete
it("should work")
it("test getName")
it("error handling")
```

### Test Coverage

Aim for:
- **Unit tests:** 80%+ coverage of logic
- **Integration tests:** Critical paths 100%
- **E2E tests:** Happy path + main error scenarios

```typescript
// ✅ Good: Tests both success and failure
it("should send command successfully", async () => {
  const result = await transport.send(packet);
  expect(result.status).toBe("success");
});

it("should retry on timeout", async () => {
  transport.setTimeout(10);  // Very short timeout
  const result = await transport.send(packet);
  expect(result.attempts).toBe(3);  // Retried 3 times
});

it("should throw on permanent failure", async () => {
  transport.disconnect();
  expect(() => transport.send(packet))
    .toThrow(DeviceDisconnectedError);
});
```

---

## Commit Messages

### Format

```
<type>: <subject>

<body>

<footer>
```

### Type

One of:
- **feat:** New feature
- **fix:** Bug fix
- **refactor:** Code change without feature/fix
- **test:** Adding or updating tests
- **docs:** Documentation only
- **perf:** Performance improvement
- **chore:** Build, CI, dependencies

### Subject

- Imperative mood: "add" not "added" or "adds"
- Don't capitalize first letter
- No period at end
- Max 50 characters

```
✅ feat: add packet inspector to web UI
✅ fix: resolve memory leak in command queue
✅ docs: clarify boot sequence in architecture

❌ feat: Added packet inspector to web UI
❌ fix: resolved memory leak in command queue.
❌ docs: Clarify boot sequence in architecture (70 chars is too long)
```

### Body (for non-trivial commits)

- Explain WHAT and WHY, not HOW
- Wrap at 72 characters
- Separate from subject with blank line

```
feat: add exponential backoff for reconnection

Previously, reconnection attempts used fixed 1-second delays,
causing rapid battery drain on battery-powered devices.

Now uses exponential backoff: 1s → 2s → 4s → 8s (max).
Resets when connection succeeds.

Fixes #42 (excessive power consumption on disconnect)
```

### Footer

Reference issues:
```
Fixes #123
Relates to #456
Closes #789
```

---

## File Structure

### Organize by Feature, Not by Type

❌ **Don't do this:**
```
src/
├── components/
│   ├── Dashboard.tsx
│   ├── ControlPanel.tsx
│   └── StatusIndicator.tsx
├── hooks/
│   ├── useFirmware.ts
│   ├── useWebSocket.ts
│   └── useState.ts
├── services/
│   └── api.ts
```

✅ **Do this (feature-based):**
```
src/
├── dashboard/
│   ├── Dashboard.tsx
│   ├── useStatusIndicators.ts
│   └── dashboard.test.ts
├── controls/
│   ├── ControlPanel.tsx
│   ├── useRelayControl.ts
│   └── controls.test.ts
├── shared/
│   ├── api.ts
│   ├── websocket.ts
│   └── types.ts
```

**Why:** Related code is together. Easier to understand a feature. Easier to delete a feature (just delete the folder).

---

## Performance

### Optimization Rules

1. **Write correct code first.** Performance premature optimization creates bugs.
2. **Measure before optimizing.** Don't guess at bottlenecks.
3. **Optimize algorithmically first.** A better algorithm beats micro-optimizations.
4. **Document performance decisions.** Why this approach?

### Performance Targets

| Operation | Target | Acceptable |
|-----------|--------|-----------|
| Relay command | < 50ms | < 100ms |
| Dashboard refresh | < 100ms | < 200ms |
| Configuration save | < 500ms | < 1000ms |
| Reconnection | < 1000ms | < 2000ms |
| UI frame render | < 16ms | < 32ms |

If you're hitting targets, stop optimizing. If not, profile first.

---

## Security Checklist

Before submitting code:

- [ ] No hardcoded secrets (API keys, passwords)
- [ ] Input validated (all user input)
- [ ] Output escaped (prevent injection)
- [ ] Error messages don't leak info
- [ ] No infinite loops (could DoS)
- [ ] No timing attacks (response time varies by secret)
- [ ] Dependencies checked for vulnerabilities
- [ ] No eval or dynamic code execution

---

## Summary Table

| Category | Standard |
|----------|----------|
| Variable names | camelCase |
| Constants | UPPER_SNAKE_CASE |
| Class names | PascalCase |
| File names | lowercase-with-hyphens |
| Indentation | 2 spaces |
| Line length | 100 chars soft, 120 hard |
| Quotes | Double quotes |
| Semicolons | Always |
| Trailing commas | Always in multi-line |
| Comments | Explain WHY, not WHAT |
| Errors | Always handle, specific types |
| Logging | Structured, with context |
| Tests | describe/it, clear names |
| Commits | Imperative, < 50 chars |
| Files | Feature-based organization |

---

**Follow these standards. They keep the code readable and maintainable.**
