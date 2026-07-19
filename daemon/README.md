# NAS Controller Host Daemon

The host daemon bridges the ESP32 firmware and the web dashboard, providing:

- **REST API** - HTTP endpoints for control and monitoring
- **WebSocket Server** - Real-time state updates to connected clients
- **USB Protocol Translator** - Converts web commands to firmware protocol and parses responses
- **Command Queue Manager** - Serializes and manages command execution
- **State Synchronizer** - Maintains synchronized state with the firmware
- **Configuration Persistence** - Saves and loads controller settings

## Status

Currently in the **planning phase**. The architecture is documented in `WEB_INTERFACE_PLAN.md` (at project root).

Implementation details TBD based on technology stack selection (Node.js, Python, or C#).

## Documentation

- **Architecture**: See `/WEB_INTERFACE_PLAN.md` for the complete system design
- **Protocol**: See `/shared/docs/PROTOCOL_SPEC.md` for the firmware communication protocol
- **Decisions**: See `docs/adr/` for architectural decisions

## Key Responsibilities

1. **USB Communication** - Read/write binary packets to/from the ESP32
2. **Command Processing** - Queue commands and handle responses
3. **State Management** - Maintain controller state synchronized with firmware
4. **API Endpoints** - Provide REST endpoints for the web dashboard
5. **WebSocket Server** - Push real-time updates to connected clients
6. **Configuration** - Manage persistent daemon configuration

## Getting Started

(Instructions TBD once implementation begins)

## Contributing

Please refer to `/docs/CONTRIBUTING.md` for contribution guidelines.
