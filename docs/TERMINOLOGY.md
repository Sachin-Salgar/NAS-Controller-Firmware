Document Version: 1.0  
Project Version: 1.0  
Last Updated: 2026-07-19  
Owner: NAS Controller  
Status: Frozen

# Terminology & Glossary

This document defines key terms used throughout the project. Use these definitions to maintain consistency across code, documentation, and communication.

---

## Core Architecture

**Daemon** (Host Daemon)
- A Node.js/TypeScript service running on a host PC or server
- Bridges the ESP32 firmware and web UI
- Owns all application state
- Translates WebSocket commands to firmware protocol
- Examples: Host Daemon, NASController.Core

**Firmware** (ESP32 Firmware)
- Embedded C++ application running on the ESP32 microcontroller
- Manages hardware directly (relays, fans, LEDs, sensors)
- Receives commands via USB serial, executes them
- Reports hardware state back to daemon
- Owns all hardware operations

**UI** / **Web Interface** / **Frontend**
- React-based web application running in the browser
- Displays real-time hardware status
- Sends user commands to daemon via WebSocket
- Owns only presentation logic
- Examples: Dashboard, Control Panel, Test Console

---

## Communication

**Protocol** / **Binary Protocol**
- The contract between firmware and daemon
- Packet format: Header(0x55AA) | Seq | Cmd | Len | Payload | CRC16
- Defined in `shared/docs/PROTOCOL_SPEC.md`
- Version-aware for backwards compatibility

**Packet**
- A single message in the binary protocol
- Request: Command from daemon to firmware
- Response: ACK/NAK/Data from firmware to daemon
- Minimum size: 6 bytes (header, seq, cmd, crc)

**Command** / **Request**
- An instruction sent by daemon to firmware
- Examples: RELAY_SET, FAN_SPEED_GET, LED_MODE_SET
- Each command has a unique command byte (Cmd)
- Defined in firmware/docs/CommandSet.md

**Response**
- Data returned by firmware in answer to a command
- ACK: Positive acknowledgment (no data)
- NAK: Negative acknowledgment (with error code)
- Data Response: Command succeeded with returned data

**ACK** / **NAK**
- ACK (Acknowledge): Command processed successfully (Cmd bit 0x80 set)
- NAK (Negative Acknowledge): Command failed (Cmd bit 0x40 set)
- Error codes in NAK payload indicate failure reason

**Transport**
- Communication channel between daemon and firmware
- Implementations: SerialTransport (USB), MockTransport (testing), TcpTransport (future), BluetoothTransport (future)
- Abstracted by ITransport interface

**WebSocket** / **Socket.io**
- Real-time, bidirectional communication between UI and daemon
- Primary method for commands and state updates
- Persistent connection for low-latency interaction

**REST** / **HTTP**
- Traditional request-response protocol
- Used for bulk operations: download logs, files, static resources
- Secondary to WebSocket in this architecture

---

## State & Configuration

**State** / **Application State**
- Current hardware status maintained by daemon
- Examples: relay on/off, fan speed, LED color, temperature reading
- Single source of truth in daemon (UI has read-only copy)
- Updated via state store subscriptions

**State Store**
- Centralized data structure in daemon holding all application state
- Supports subscriptions for real-time updates
- Components: FirmwareState, HardwareState, ConfigState, StatisticsState
- Pattern: Daemon owns all state, UI subscribes to changes

**Configuration** / **Config**
- Persistent settings stored in firmware EEPROM
- Examples: hardware counts, task intervals, temperature thresholds, fan curves
- Can be read from or written to firmware
- Differs from runtime state

**Runtime State**
- Transient data that changes during operation
- Examples: relay status, current fan speed, temperature reading
- Lost on power cycle
- Contrasts with configuration (persistent)

**Capability** / **Firmware Capability**
- A feature the firmware supports and reports to daemon
- Examples: Relay count, Fan count, LED count, Max temperature
- Discovered at startup via GET_CAPABILITIES command
- Enables UI to adapt to different hardware versions

**Transaction** / **Config Transaction**
- Atomic change to configuration
- Sequence: Begin → Send Changes → Validate → Commit
- Ensures consistency: either all changes applied or none
- Prevents partially-applied configurations

---

## Command Queue & Processing

**Command Queue**
- FIFO queue in daemon managing all outgoing commands
- Ensures serial execution (one command at a time)
- Tracks command state for each queued command

**Command State**
- Queued: Command waiting in queue
- Sending: Packet transmitted to firmware
- Waiting ACK: Awaiting response from firmware
- Completed: Command executed successfully
- Timeout: No response within 1000ms
- Failed: Command rejected by firmware (NAK)

**Retry Logic**
- Automatic re-transmission of timed-out commands
- Up to 3 retries per command with exponential backoff
- After 3 failed attempts, command marked as FAILED
- Timeout per command: 1000ms

**Sequence Number** / **Seq**
- Byte value (0x00-0xFF) in packet header
- Incremented for each command
- Rolls over: 0xFF → 0x00
- Matches request to response

---

## Hardware

**Relay**
- Electrical switch controlled by GPIO pin
- States: ON (closed) or OFF (open)
- Controlled via CMD_RELAY_SET
- Examples: Power relay, pump relay, compressor relay

**Fan**
- Motor controlled via PWM (Pulse Width Modulation)
- Speed: 0-100% or RPM
- Modes: Manual, Auto, Off
- Examples: Cooling fan, intake fan, exhaust fan

**LED** / **Addressable LED**
- WS2812B digital RGB LED strip
- Controlled via one-wire protocol
- Supports color and brightness per LED
- Used for status indication and animation

**Sensor** / **Temperature Sensor**
- OneWire temperature sensor (DS18B20)
- Reads temperature in Celsius
- Multiple sensors supported
- Used for thermostat and monitoring

**Drive**
- Physical storage device detected by firmware
- Status: Present, Missing, Error
- Tracked by firmware for monitoring

**Hardware State**
- Real-time snapshot of all hardware
- Relays: on/off per relay
- Fans: current speed, mode
- LEDs: current color, brightness
- Temperatures: reading per sensor
- Drives: present/missing per slot

---

## Events & Logging

**Event**
- Significant occurrence in the system
- Examples: Relay ON, Temperature High, USB Reconnected, Config Saved
- Logged with timestamp, type (info/warning/error), details
- Forms the Event Log

**Event Log**
- Chronological record of all events
- Used for debugging, auditing, monitoring
- Supports filtering and export (CSV, JSON)
- Retained in memory and persisted to files

**Packet Inspector** / **Packet Capture**
- Real-time display of all TX/RX packets
- Shows: timestamp, direction (TX/RX), hex dump, decoded meaning
- Used for protocol debugging and validation
- Can be filtered by packet type and exported

**Diagnostic Mode**
- Special daemon mode for production debugging
- Enables: packet logging, timing info, queue transitions, CRC verification
- Outputs: timing metrics, queue depth, performance stats
- Lightweight overhead suitable for production

---

## Architecture Patterns

**Library-First Daemon**
- Daemon implemented as pure TypeScript library (NASController.Core)
- No dependency on Express or HTTP framework
- Testable without running server
- Express + Socket.io are thin adapters on top of library

**ITransport Interface**
- Abstract interface for communication with firmware
- Implementations: SerialTransport, MockTransport, TcpTransport, BluetoothTransport
- Enables testing without hardware (via MockTransport)
- Makes adding new transports easy without core changes

**Mock Device** / **MockTransport**
- Complete firmware simulator running in-process
- Provides deterministic test environment
- Fully testable: protocol, transport, queue, state management
- Enables UI development without ESP32 hardware

**Abstraction Layer**
- Clear separation between components
- Transport abstraction (ITransport)
- Protocol abstraction (packet encoder/decoder)
- State subscription (publish-subscribe pattern)

---

## Error Handling

**Error Type** / **Error Classification**
- Transport Error: Communication failure (USB disconnect, serial timeout)
- Protocol Error: Malformed packet (bad CRC, invalid format)
- Firmware Error: Command rejected by firmware (NAK response)
- Validation Error: User input invalid (e.g., out-of-range value)
- User Error: Invalid usage (e.g., command sent before connection)
- Internal Error: Daemon bug (unexpected state, logic error)

**Recovery** / **Error Recovery**
- Different strategies per error type
- Transport: Auto-reconnect with exponential backoff
- Protocol: Retry with backoff
- Firmware: Report to UI, allow user to retry
- User: Validate input before sending
- Internal: Log and fallback to safe state

**Watchdog** / **Health Check**
- Periodic PING command to verify firmware connectivity
- Detects and responds to USB disconnections
- Triggers reconnection sequence
- Ensures daemon always knows firmware state

---

## Development & Testing

**Unit Test**
- Tests a single component in isolation
- Example: CRC16 algorithm without hardware
- Uses Jest framework
- Should mock external dependencies

**Integration Test**
- Tests interaction between components
- Example: Command queue + protocol + MockTransport
- Verifies end-to-end behavior without hardware
- Still uses Jest with mock firmware

**Manual Test**
- Testing on real hardware
- Validates actual USB communication
- Checks real-time behavior
- Detects issues unit/integration tests miss

**Test Coverage**
- Percentage of code paths executed by tests
- Target: 100% for protocol, transport, core logic
- Acceptable: 80%+ for API layer, UI

---

## Documentation

**ADR** / **Architecture Decision Record**
- Document explaining a significant design decision
- Includes: context, decision, consequences
- Examples: ADR-0001 WebSocket-first, ADR-0002 Library-first daemon
- Helps future maintainers understand the "why"

**Specification** / **Spec**
- Detailed description of requirements and behavior
- Examples: PROTOCOL_SPEC.md, WEB_INTERFACE_PLAN.md
- More detailed than overview, less detailed than implementation

**README**
- Quick start guide for a component
- Covers: what it is, how to set up, key documents
- Entry point for developers new to that component

**Documentation Comment** / **Doc Comment**
- Inline code comment explaining non-obvious behavior
- Use sparingly: only for hidden constraints, subtle invariants, workarounds
- Examples: thread-safety, performance implications, workaround for specific bug

---

## Versioning & Compatibility

**Protocol Version**
- Semantic version of binary protocol
- Example: v1.0
- Embedded in capability discovery
- Supports multiple firmware versions

**Configuration Version**
- Version of config schema
- Enables migration when schema changes
- Example: config v1.0 → v1.1

**API Version**
- Version of WebSocket/REST API
- Enables multiple versions for backwards compatibility
- Example: /api/v1/, /api/v2/

**Capability Discovery**
- Startup sequence where firmware reports its capabilities
- Includes: protocol version, config version, hardware counts
- Allows daemon to adapt to different firmware versions
- Sequence: PING → GET_CAPABILITIES → GET_ALL_CONFIG → GET_ALL_STATUS

---

## Workflow & Process

**Feature** / **User Story**
- A capability from the user's perspective
- Example: "Turn relay on/off from web UI"
- Breaks down into tasks across firmware, daemon, UI

**Task** / **Implementation Task**
- Specific code work with clear acceptance criteria
- Example: "Implement RELAY_SET command in firmware"
- Assigned to developer with size estimate

**Pull Request** / **PR**
- Code review mechanism
- Includes: description, changes, tests, deployment notes
- Requires approval before merge

**Branch**
- Isolated development environment
- Example: feature/relay-control, fix/crc-validation
- Merged to main via PR

**Commit**
- Single logical change with descriptive message
- Atomic: one concern per commit
- Example: "Add CRC16 implementation"

---

## Scope & Extensions

**In Scope**
- Features explicitly planned for Phase 1, 2, 3
- See WEB_INTERFACE_PLAN.md, IMPLEMENTATION_CHECKLIST.md

**Out of Scope** / **Not Planned**
- Features intentionally excluded
- See NOT_PLANNED.md
- Prevents scope creep

**Extension Point**
- Intentional design for future features
- Example: Plugin architecture for controllers
- Allows adding new hardware without core changes
- See EXTENSION_POINTS.md

**Future** / **Backlog**
- Features considered but not yet planned
- Example: Remote network access, Mobile app
- Revisited after Phase 1-3 complete

---

## Quality

**Maintainability**
- Code ease of understanding and modification
- Achieved via: clear naming, documentation, separation of concerns

**Testability**
- Code ease of testing
- Achieved via: dependency injection, small functions, abstraction

**Reliability**
- Code correctness under various conditions
- Achieved via: error handling, timeouts, recovery strategies

**Performance**
- Code speed and resource efficiency
- Measured: latency, throughput, queue depth
- Diagnostics: timing metrics, bottleneck analysis

**Scalability**
- Ability to handle growth
- Example: extensible to more relays, fans, sensors
- Design consideration: plugin architecture, configuration-driven

---

**Purpose:** Ensure consistent terminology across the project  
**Status:** Frozen (additions require stakeholder approval)
