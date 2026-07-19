# Daemon Documentation

This folder contains all Node.js/TypeScript daemon (backend) documentation.

## Core Documentation

- **[ARCHITECTURE.md](ARCHITECTURE.md)** - Daemon architecture and design patterns
- **[WEBSOCKET_API.md](WEBSOCKET_API.md)** - WebSocket protocol and command specification
- **[STATE_STORE.md](STATE_STORE.md)** - Global state management and subscriptions
- **[TRANSPORT_LAYER.md](TRANSPORT_LAYER.md)** - Transport abstraction (Serial, TCP, Mock, etc.)

## System Components

- **[COMMAND_QUEUE.md](COMMAND_QUEUE.md)** - Command queue state machine and management
- **[PACKET_INSPECTOR.md](PACKET_INSPECTOR.md)** - Packet inspection and debugging tools
- **[DIAGNOSTIC_MODE.md](DIAGNOSTIC_MODE.md)** - Diagnostic and performance metrics
- **[ERROR_HANDLING.md](ERROR_HANDLING.md)** - Error classification and recovery

## Testing

- **[TESTING.md](TESTING.md)** - Testing strategy and mock device usage
- **[MOCK_DEVICE.md](MOCK_DEVICE.md)** - Virtual device for development without hardware

## Architecture Decisions (ADR)

All significant architecture decisions are documented in `adr/`:

- `0001-library-first-daemon.md` - Library-first architecture (core as reusable library)
- `0002-websocket-first.md` - WebSocket-first communication strategy
- `0003-capability-discovery.md` - Firmware capability discovery
- `0004-atomic-transactions.md` - Atomic configuration transactions
- ... (additional ADRs as decisions are made)

## Quick Links

- **Source Code:** `../src/`
- **Tests:** `../tests/`
- **Configuration:** `../package.json`

## Contributing to Daemon

Before modifying daemon code, read:
1. [ARCHITECTURE.md](ARCHITECTURE.md)
2. [TRANSPORT_LAYER.md](TRANSPORT_LAYER.md)
3. Relevant ADR in `adr/` folder
4. Project-level [DEVELOPER_GUIDE.md](../../docs/DEVELOPER_GUIDE.md)

## Project Integration

This daemon is part of the NAS Controller project. See [Project Overview](../../docs/README.md) for system-wide documentation.

## Building & Running

```bash
cd daemon
npm install
npm run dev        # Development mode with hot reload
npm run build      # Build for production
npm test           # Run tests
```
