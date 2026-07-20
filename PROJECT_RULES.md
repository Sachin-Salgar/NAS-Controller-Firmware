# Project Rules

**NAS Controller Firmware V3**

This document establishes the non-negotiable rules governing development, protocol governance, and system verification.

---

## Core Principle

**"Architecture is Policy."**

The system is deliberately designed to enforce correct behavior through structure, not convention.

---

## Layer Architecture Rules (Rules 1-4)

### Rule 1: Strict Dependency Direction

**Code must depend only downward through the layer stack.**

```
Application ← All direction must be this way ← Core
System      ← Only downward allowed
Protocol    ← Violations are structural errors
Services
Objects
Drivers
Platform
Core
```

**Consequences of Violation:**
- Circular dependencies
- Impossible to test independently
- Hidden coupling and brittleness
- Architectural debt

**Enforcement:** Code review. No exceptions.

---

### Rule 2: Layer Boundary Integrity

**Each layer has defined boundaries. Crossing is forbidden.**

| Layer | Entry Points | Clients |
|-------|--------------|---------|
| Application | main() | External entry |
| System | Lifecycle managers | Application |
| Protocol | CommandDispatcher | System |
| Services | Event subscriptions | Protocol, Services |
| Objects | Getters/Setters, Events | Services, Objects |
| Drivers | Constructor injection | Objects, Platform HAL |
| Platform | Direct calls from drivers | Drivers |
| Core | Utilities (no dependencies) | All layers |

**Examples of Boundary Violations:**
- Service calling a driver directly (skip Objects)
- Driver creating Objects (skip Services)
- Protocol calling Platform HAL (skip everything)

**Rule:** Each layer communicates only with immediately adjacent layers, except:
- Core utilities may be used by any layer
- Event Bus enables optional cross-cutting communication

---

### Rule 3: No Bidirectional Communication

**Objects do not call Services. Drivers do not call Drivers.**

Once control passes downward, communication returns via:
- Return values (synchronous)
- Event publications (asynchronous)
- Result objects (error handling)

Upward calls after initialization is complete create hidden couplings.

---

### Rule 4: Architecture is Frozen

**The layered structure cannot be modified without formal ADR and technical review.**

For Firmware Version 1.x:
- ✅ Implementation within layers: Allowed
- ✅ New services: Allowed
- ✅ New drivers: Allowed
- ❌ Adding a layer: Forbidden
- ❌ Reordering layers: Forbidden
- ❌ Changing layer responsibilities: Requires ADR

Any modification to architecture requires:
1. Architecture Decision Record (ADR)
2. Technical lead approval
3. Documentation of rationale
4. Explicit version number increment
5. Communication to all teams

---

## Protocol Rules (Rules 5-15)

### Rule 5: Binary Protocol Specification

**All packets conform exactly to the specified format:**

```
Header       (0x55AA)           2 bytes
Sequence     (host-controlled)  2 bytes
Command      (enumerated)       2 bytes
Length       (payload bytes)    2 bytes
Payload      (per command spec) 0-500 bytes
CRC16        (Modbus)           2 bytes
```

No deviations. No exceptions. No extensions.

---

### Rule 6: Little-Endian Byte Order

**All multi-byte fields use little-endian (LSB-first) byte order.**

Examples:
- Header `0x55AA` → transmitted as `[0x55, 0xAA]` in little-endian: `[0xAA, 0x55]`
- Sequence `0x1234` → transmitted as `[0x34, 0x12]`
- CRC → LSB first

**Exception:** None. This is platform-native byte order for ESP32.

---

### Rule 7: CRC16-Modbus Algorithm

**All implementations must use identical CRC16-Modbus:**

```
Algorithm:     CRC16-Modbus (Reflected)
Polynomial:    0xA001
Initial Value: 0xFFFF
Reflection:    Yes (right-shift, reflect input/output)
Final XOR:     No (0x0000)
Byte Order:    Little-endian (LSB first in transmission)
```

**Calculation Parameters:**
- Input: All bytes from header through payload (12 bytes minimum for empty payload)
- Output: 2-byte CRC appended to packet
- Verification: CRC of header+sequence+command+length+payload == received CRC

**Reference Implementation:** `src/Utilities/CRC16.cpp` (firmware)

---

### Rule 8: Packet Constraints

**Every packet must satisfy:**

| Constraint | Value |
|-----------|-------|
| Minimum Size | 12 bytes (no payload) |
| Maximum Size | 512 bytes (total) |
| Maximum Payload | 500 bytes |
| Header | Must be 0x55AA |
| Sequence | 2 bytes, any value |
| Command | 2 bytes, enumerated in PROTOCOL_REGISTRY.md |
| Length | 0-500 (exact match payload size) |
| CRC | 2 bytes, little-endian |

**Validation:**
- Header mismatch → Reject
- Length mismatch → Reject
- Payload + 12 bytes ≠ packet length → Reject
- CRC mismatch → Reject

No packet with size 11 or less is valid. No packet over 512 bytes is valid.

---

### Rule 9: Command Enumeration

**All commands must be defined in PROTOCOL_REGISTRY.md before use.**

- No ad-hoc command codes
- No reserved ranges
- No "undefined behavior" commands
- Unknown commands receive `ResultCode::UnknownCommand`

Process for new commands:
1. Document in PROTOCOL_REGISTRY.md
2. Define payload structure
3. Define response structure
4. Implement in firmware
5. Add canonical test vector
6. Increment protocol version

---

### Rule 10: Response Code Standardization

**All responses use standardized result codes.**

Common codes:
- `0x0000` — OK (success)
- `0x0001` — InvalidHeader
- `0x0002` — InvalidLength
- `0x0003` — CrcError
- `0x0004` — UnknownCommand
- `0x0005` — InvalidPayload
- `0x0006` — Busy
- `0x0007` — InternalError

All result codes are defined in `src/Core/Result.h` and listed in PROTOCOL_REGISTRY.md.

---

### Rule 11: Sequence Number Handling

**The host controls sequence numbers. The firmware echoes them.**

- Host increments sequence for each request
- Firmware copies received sequence into response
- Allows matching responses to requests (especially important over slow transports)
- Sequence may wrap around (0xFFFF → 0x0000)

---

### Rule 12: Payload Format Specification

**Each command specifies its payload structure exactly:**

Format definition includes:
- Byte offset for each field
- Data type (uint8, uint16, uint32, string, etc.)
- Byte order (little-endian)
- Constraints (ranges, valid values, etc.)
- Example values

Ambiguous payload formats are not permitted.

---

### Rule 13: Request/Response Symmetry

**Every request receives exactly one response.**

- No unsolicited messages from firmware
- No multi-packet responses (fragment if necessary)
- Response includes original sequence number
- Response includes result code

---

### Rule 14: Protocol Verification Chain (Enhanced)

**A protocol version is frozen only when this complete chain succeeds:**

```
Stage 1: Architecture Decision
         ↓ (ADR document approved)
Stage 2: Protocol Specification
         ↓ (PROTOCOL_SPEC.md written)
Stage 3: Firmware Reference Implementation
         ↓ (Algorithm implemented and verified)
Stage 4: Canonical Protocol Test Vectors
         ↓ (At least 5 vectors generated from firmware)
Stage 5: Daemon Implementation
         ↓ (Independent implementation created)
Stage 6: Integration Test
         ↓ (Daemon passes all canonical vectors)
Stage 7: Protocol Freeze
         ✓ (Protocol version locked)
```

**Each stage requires:**
- Completion of all previous stages
- Documentation of results
- Verification against source of truth (firmware)
- Explicit approval before proceeding

**Verification Requirements:**
- Firmware verification: Execute firmware implementation, capture actual bytes
- Daemon verification: Run daemon against firmware implementation, verify packet bytes match
- Vector verification: Each vector includes firmware-generated CRC
- Byte-level verification: Entire packet bytes must match, not just CRC values

**No Shortcuts:** Rule 14 cannot be bypassed with calculated values or theoretical verification. Only firmware execution and actual packet capture count.

---

### Rule 15: Protocol Governance & Versioning

**Any modification to a frozen protocol requires formal governance:**

**Process for Protocol Changes (e.g., 1.0.0 → 1.1.0):**

1. **Decision Phase**
   - Document rationale in ADR
   - Identify all affected components
   - Obtain technical approval

2. **Specification Phase**
   - Update PROTOCOL_SPEC.md
   - Update PROTOCOL_REGISTRY.md
   - Document all changes explicitly

3. **Implementation Phase**
   - Implement in firmware
   - Execute on hardware
   - Capture actual packet bytes

4. **Test Vector Phase**
   - Generate new canonical vectors
   - Include firmware-generated CRC values
   - Document compatibility with prior version

5. **Verification Phase**
   - Daemon implementation against new vectors
   - Cross-reference with old vectors if applicable
   - Explicit incompatibility or compatibility statement

6. **Freeze Phase**
   - Update PROJECT_STATUS.md
   - Tag version in git
   - Document in CHANGELOG.md

**Backward Compatibility:**
- Within 1.x cycle: Backward compatibility required
- Version 2.0.0: Breaking changes allowed (with deprecation period)

---

## Implementation Rules (Rules 16-20)

### Rule 16: No Implementation Without Specification

**Every feature must be specified before implementation.**

Order:
1. Write specification (document requirements)
2. Create test vectors (if protocol-related)
3. Implement in firmware
4. Implement in daemon
5. Test against vectors

Skipping to #4 is prohibited.

---

### Rule 17: Source of Truth Hierarchy

**In case of conflict, resolve using this hierarchy:**

1. **Firmware Execution** (highest authority)
   - What the firmware actually does is the source of truth
   - Use packet captures and hardware testing

2. **PROTOCOL_SPEC.md**
   - Specification must match firmware behavior
   - If firmware and spec disagree, spec is wrong

3. **Code Comments**
   - Document design intent
   - Clarify non-obvious implementation choices

4. **Assumptions**
   - Never assume behavior not explicitly tested

---

### Rule 18: Testing Requirements

**All protocol implementations must validate against canonical test vectors.**

- Vectors are in `shared/docs/protocol/test_vectors/`
- Each vector is a JSON file with metadata
- Implementations must:
  - Parse input from vector
  - Generate packet
  - Verify packet matches expected output
  - Verify CRC matches expected CRC
  - Report pass/fail for each vector

**Failure on any vector = implementation is non-compliant.**

---

### Rule 19: Configuration Lock

**Once locked, configuration parameters cannot be changed without version increment.**

Locked parameters:
- CRC16 polynomial (0xA001)
- Packet header (0x55AA)
- Maximum packet size (512)
- Byte order (little-endian)
- Protocol version numbering

Any change to these requires protocol version increment.

---

### Rule 20: Documentation Lock

**Frozen documents cannot be modified without formal ADR approval.**

Frozen documents:
- `docs/Architecture.md` (layer structure)
- `docs/Hardware.md` (pinouts, ESP32 mapping)
- `docs/CodingStandard.md` (conventions)
- `shared/docs/PROTOCOL_SPEC.md` (for frozen versions)
- All ADRs in `docs/Decisions/`

Process for modifications:
1. Create new ADR (0009-XXX.md)
2. Explain rationale for change
3. Obtain technical lead approval
4. Document superseded rules
5. Update PROJECT_STATUS.md

---

## Summary Table

| Rule | Type | Topic | Enforcement |
|------|------|-------|-------------|
| 1-4 | Architecture | Layer structure, boundaries, direction | Code review, ADR |
| 5-13 | Protocol | Packet format, fields, constraints | Specification, testing |
| 14 | Verification | Protocol freeze requirements | Integration testing |
| 15 | Governance | Protocol versioning and change process | Technical review |
| 16-17 | Implementation | Specification-first, source of truth | Code review |
| 18-20 | Quality | Testing, configuration, documentation | Automated validation |

---

## Exception Process

**Rules cannot have exceptions. To change a rule:**

1. Create Architecture Decision Record explaining the change
2. Document the rationale and consequences
3. Obtain approval from technical lead
4. Update this document
5. Communicate change to all teams
6. Increment project version

**No ad-hoc workarounds. No "just this once" changes.**

---

## Enforcement

**Rules are enforced through:**
- Architecture review (ADRs)
- Code review (layer dependencies, rule compliance)
- Specification compliance (protocol conformance)
- Integration testing (vector validation)
- Automated validation (where possible)

**Violating a rule is not a "code smell"—it's a structural error that prevents the system from working correctly.**

---

## Last Updated

- **Date:** July 2026
- **Phase:** Protocol Verification & Governance
- **Version:** 1.0
- **Status:** Active (Rule 14 enhanced this session)
