# NAS Controller V3 — Project Overview

**Status:** In Active Development (Protocol Verification Phase)  
**Architecture:** Frozen for 1.x Release Cycle  
**Protocol Version:** 1.0.0 (Verification in Progress)

---

## System Architecture

The NAS Controller is organized into four complementary subsystems, each with distinct responsibilities:

```
┌─────────────────────────────────────────────────┐
│          Host Application (PC/Device)            │
└──────────────────┬──────────────────────────────┘
                   │
              USB CDC Binary Protocol
                   │
┌──────────────────┴──────────────────────────────┐
│         ESP32 Firmware (Microcontroller)         │
│  ┌─────────────────────────────────────────┐   │
│  │    Application Layer (Commands)         │   │
│  └───────────────────┬─────────────────────┘   │
│                      │                         │
│  ┌───────────────────┴─────────────────────┐   │
│  │    System & Lifecycle Management        │   │
│  └───────────────────┬─────────────────────┘   │
│                      │                         │
│  ┌───────────────────┴─────────────────────┐   │
│  │    Protocol Layer (Packet Handling)     │   │
│  └───────────────────┬─────────────────────┘   │
│                      │                         │
│  ┌───────────────────┴─────────────────────┐   │
│  │    Services (Business Logic)            │   │
│  │    - RelayService                       │   │
│  │    - FanService                         │   │
│  │    - ProtocolService                    │   │
│  │    - HealthService                      │   │
│  └───────────────────┬─────────────────────┘   │
│                      │                         │
│  ┌───────────────────┴─────────────────────┐   │
│  │    Objects (Runtime State)              │   │
│  │    - Relay, Fan, LED, Temperature       │   │
│  │    - Drive, Configuration, Statistics   │   │
│  └───────────────────┬─────────────────────┘   │
│                      │                         │
│  ┌───────────────────┴─────────────────────┐   │
│  │    Drivers (Hardware Control)           │   │
│  │    - RelayDriver                        │   │
│  │    - FanDriver                          │   │
│  │    - LEDDriver                          │   │
│  │    - TemperatureDriver                  │   │
│  │    - WatchdogDriver                     │   │
│  └───────────────────┬─────────────────────┘   │
│                      │                         │
│  ┌───────────────────┴─────────────────────┐   │
│  │    Platform HAL (Hardware Abstraction)  │   │
│  │    - GPIO, PWM, OneWire, USB, Flash     │   │
│  └───────────────────┬─────────────────────┘   │
│                      │                         │
│  ┌───────────────────┴─────────────────────┐   │
│  │    Core (Fundamentals)                  │   │
│  │    - Result, Logger, EventBus, Types    │   │
│  └─────────────────────────────────────────┘   │
└─────────────────────────────────────────────────┘
```

---

## Core Subsystems

### 1. Firmware (ESP32)

**Purpose:** Embedded control plane managing hardware and protocol.

**Responsibilities:**
- Hardware interface (relays, fans, sensors, LEDs)
- Binary protocol packet handling
- Command dispatch and execution
- Real-time health monitoring
- Persistent configuration storage

**Technology:**
- Microcontroller: ESP32 (WROOM-32 variant)
- Framework: Arduino
- Language: C++17
- Build: PlatformIO

**Key Characteristics:**
- Layered architecture (Core → Platform → Drivers → Objects → Services → Protocol → System)
- Event-driven inter-component communication
- Deterministic task scheduling
- Strict dependency direction enforcement

---

### 2. Binary Protocol (Shared)

**Purpose:** Standardized packet format for all implementations.

**Characteristics:**
- Transport-independent (USB CDC, UART, TCP/IP, etc.)
- Request/response pattern
- Little-endian byte order
- CRC16-Modbus protection
- Command-based architecture

**Packet Structure:**
```
Header       (0x55AA)           2 bytes
Sequence     (incremented)      2 bytes
Command      (command ID)       2 bytes
Length       (payload size)     2 bytes
Payload      (variable)         N bytes
CRC16        (Modbus)           2 bytes
```

**Protocol Version:** 1.0.0  
**Verification Status:** ~90% (In Progress)

---

### 3. Daemon (Planned)

**Purpose:** Host-side service communicating with firmware.

**Planned Responsibilities:**
- USB/Serial transport management
- Packet assembly and validation
- Command API for client applications
- Event aggregation and logging

**Status:** ⬜ Awaiting protocol freeze

---

### 4. Frontend (Planned)

**Purpose:** User-facing application for monitoring and control.

**Planned Capabilities:**
- Dashboard view of relay/fan/temperature state
- Command execution interface
- Configuration management
- Event history and logging

**Status:** ⬜ Awaiting daemon completion

---

## Shared Contracts (Frozen)

All implementations must conform to these specifications:

### Packet Format
- Header: `0x55AA` (fixed)
- Sequence: 2 bytes (host-incremented, firmware-echoed)
- Command: 2 bytes (enumerated in PROTOCOL_REGISTRY.md)
- Length: 2 bytes (payload bytes only, 0-500)
- Payload: Variable (format per command definition)
- CRC: 2 bytes (CRC16-Modbus calculated over header through payload)

### CRC Algorithm
- **Type:** CRC16-Modbus
- **Polynomial:** 0xA001
- **Initial Value:** 0xFFFF
- **Reflection:** Enabled (right-shift)
- **Final XOR:** No (0x0000)
- **Byte Order:** Little-endian (LSB transmitted first)

### Packet Constraints
- Minimum Size: 12 bytes (header + seq + cmd + len + crc, no payload)
- Maximum Size: 512 bytes (total)
- Maximum Payload: 500 bytes
- Padding: None (exact length required)

### Command Categories
- **System** — Ping, Version, Status, Reset
- **Relay** — Get, Set, Enable, Disable
- **Fan** — Get, SetSpeed, SetMode
- **Temperature** — Get, Calibrate, SetThreshold
- **LED** — Get, SetColor, SetMode, SetBrightness
- **Drive** — Get, Scan, GetHealth
- **Configuration** — Get, Set, Save, Load
- **Statistics** — Get, Reset
- **Event** — Read, Clear

---

## Interoperability Requirement

**CRITICAL:** Any independent implementation of Protocol v1.0.0 must:

1. Produce identical packet bytes for every canonical protocol test vector
2. Validate using the same CRC16-Modbus algorithm
3. Interpret commands according to PROTOCOL_REGISTRY.md
4. Handle response codes consistently

**This applies to:**
- ✅ Firmware (reference implementation)
- ⬜ Daemon
- ⬜ Python utilities
- ⬜ Mobile applications
- ⬜ WebAssembly implementations
- ⬜ Future implementations

**Verification Method:** All implementations must successfully validate all vectors in `shared/docs/protocol/test_vectors/`.

---

## Development Phases

### Phase 1 ✅ Complete
- Architecture design and documentation
- Layer structure implementation
- Core utility framework
- Foundation infrastructure

### Phase 2 ✅ Complete
- Firmware core implementation
- Hardware driver integration
- Service layer completion
- Protocol scaffolding

### Phase 3 🔄 In Progress
- Protocol verification (firmware → documentation mapping)
- Canonical test vector generation
- Protocol governance documentation
- Rule enhancement and formalization
- Protocol freeze

### Phase 4 ⬜ Pending
- Daemon implementation (CRC, packet handling, USB)
- Integration testing
- System-level validation
- Performance benchmarking

### Phase 5 ⬜ Planned
- Frontend development
- User interface design
- Application testing
- Deployment preparation

---

## Key Design Decisions

### 1. Strict Layered Architecture (ADR-0001)
All dependencies point downward, enabling:
- Clear separation of concerns
- Easy testing at layer boundaries
- Hardware abstraction
- Platform independence

### 2. Event Bus for Inter-Layer Communication (ADR-0006)
Services communicate via event publications instead of direct calls:
- Decoupled component interaction
- Extensibility without modification
- Clean dependency graph

### 3. CRC16-Modbus for Protocol Protection (ADR-0003)
Selected for:
- Firmware efficiency (no table lookup required)
- Wide compatibility
- Adequate error detection for control protocols
- Little-endian byte order matches ESP32 native

### 4. Deterministic Task Scheduling
No interrupt-driven business logic:
- Predictable execution
- Easier debugging and testing
- Configurable polling intervals
- System health monitoring

### 5. Transport-Independent Protocol
Same packet format works with:
- USB CDC (current)
- UART serial (planned)
- Ethernet / TCP-IP (future)
- Wi-Fi / UDP (future)
- Bluetooth (future)

---

## Constraints & Rules

### Architecture Lock-In
The layered architecture is **frozen for all 1.x releases**. Any changes require formal ADR and technical review.

### Protocol Stability
Once Protocol 1.0.0 is frozen, backward compatibility must be maintained within the 1.x cycle. Breaking changes require version increment (2.0.0).

### Dependency Direction
Code must always depend downward through the layer stack. Upward dependencies create circular problems and are prohibited.

### No Exceptions
Rules are absolute. No workarounds, hacks, or exceptions are permitted without documented ADR approval.

---

## Quality Metrics

### Code Coverage
- Protocol layer: 100% (critical)
- Service layer: >90% (business logic)
- Driver layer: >80% (hardware dependent)

### Documentation
- All ADRs completed and approved
- All modules documented with purpose and usage
- All public APIs documented
- All decision rationale documented

### Verification
- All canonical test vectors passing
- CRC implementation verified against firmware
- Packet format validated
- Interoperability confirmed

---

## Next Steps (Current Session)

1. ✅ Complete CRC verification documentation
2. ✅ Generate canonical protocol test vectors
3. ✅ Create protocol interoperability requirement
4. ✅ Establish protocol freeze checklist
5. ✅ Enhance Rule 14 verification chain
6. 🔄 Obtain protocol freeze approval
7. ⬜ Begin Phase 4 (daemon implementation)

---

## Contact & Governance

**Technical Lead:** Sachin Salgar  
**Architecture Oversight:** Technical Team  
**Protocol Governance:** Engineering Committee  
**Freeze Authority:** Technical Lead + Stakeholders

All changes to frozen documents require documented approval.
