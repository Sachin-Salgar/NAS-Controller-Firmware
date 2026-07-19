# Shared Types & Protocol Documentation

This folder contains protocol specifications and shared type definitions used by both daemon and frontend.

## Core Protocol Documentation

- **[PROTOCOL_SPEC.md](PROTOCOL_SPEC.md)** - Binary protocol specification
  - Packet format, header/footer, sequence numbers
  - CRC16 validation, ACK/NAK handling
  - Command structure and payloads
  - Timeouts and retry policies

- **[COMMAND_SET.md](COMMAND_SET.md)** - Complete command reference
  - Relay commands
  - Fan control commands
  - LED control commands
  - Configuration commands
  - System commands
  - Sensor/status commands

- **[DATA_TYPES.md](DATA_TYPES.md)** - Shared TypeScript type definitions
  - Protocol types
  - Command definitions
  - Event types
  - Capability schema
  - Configuration schema

## API Contracts

- **[WEBSOCKET_PROTOCOL.md](WEBSOCKET_PROTOCOL.md)** - WebSocket message format and commands
- **[STATE_SCHEMA.md](STATE_SCHEMA.md)** - Global state structure shared between daemon and frontend
- **[EVENT_TYPES.md](EVENT_TYPES.md)** - Event definitions and logging schema

## Versioning & Compatibility

- **[VERSIONING.md](VERSIONING.md)** - Protocol versioning strategy
  - Protocol v1.0
  - Config v1.0
  - Events v1.0
  - Capabilities v1.0

## Architecture Decisions (ADR)

Shared architecture decisions documented in `adr/`:

- `0001-websocket-first.md` - WebSocket-first communication strategy
- `0002-protocol-versioning.md` - Versioning approach for protocols
- `0003-transport-abstraction.md` - Transport layer abstraction
- ... (additional ADRs as decisions are made)

## Quick Links

- **Source Code:** `../src/`
  - `protocol.ts` - Protocol types
  - `commands.ts` - Command definitions
  - `events.ts` - Event types
  - `capabilities.ts` - Firmware capability schema
  - `config.ts` - Configuration schema

## Usage

### In Daemon
```typescript
import { Protocol, Commands, Events, Capabilities } from '@nas-controller/shared';
```

### In Frontend
```typescript
import { Protocol, Commands, Events, Capabilities } from '@nas-controller/shared';
```

Both daemon and frontend import types from this shared package to ensure consistency.

## Contributing

When adding new commands or events:
1. Update `PROTOCOL_SPEC.md` with binary format details
2. Update `COMMAND_SET.md` with documentation
3. Add TypeScript types to `src/commands.ts` or `src/events.ts`
4. Update `DATA_TYPES.md` with new type definitions
5. If breaking change, document in `VERSIONING.md`
6. Update relevant ADRs or create new ADR if architectural decision made

## Project Integration

This shared package is part of the NAS Controller project. See [Project Overview](../../docs/README.md) for system-wide documentation.

## Publishing

```bash
cd shared
npm install
npm run build      # Compile TypeScript
npm publish        # Publish to npm registry
```
