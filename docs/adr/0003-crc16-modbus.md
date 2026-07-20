# ADR-0003: CRC16-Modbus for Protocol Packets

**Status:** Accepted  
**Date:** 2026-07-20  
**Deciders:** Lead Architect, Project Owner  
**Supersedes:** Original CRC16-CCITT specification in PROTOCOL_SPEC.md  

---

## Context

### Original Planning Decision

During the architecture planning phase, PROTOCOL_SPEC.md specified **CRC16-CCITT** (polynomial 0x1021) for packet validation. This was a deliberate design choice documented in the protocol specification.

### Implementation Reality

The ESP32 firmware implementation, completed during the firmware development phase, uses **CRC16-Modbus** (polynomial 0xA001, reflected) throughout:

**Evidence:**
- `firmware/src/Utilities/CRC16.h` explicitly labeled "CRC-16 (Modbus) utility"
- `firmware/src/Protocol/PacketValidator.cpp` implements 0xA001 for all packet validation
- `firmware/src/Protocol/PacketBuilder.cpp` uses the same 0xA001 algorithm for packet generation
- All firmware protocol tests use CRC16-Modbus
- Implementation is consistent across all packet operations
- No evidence that CCITT was ever implemented or considered during firmware development

### Investigation Findings

An independent investigation conducted during daemon development confirmed:
- Firmware uses CRC16-Modbus (0xA001) for ALL protocol packets
- No CRC usage in storage or configuration (protocol-only)
- Both send (PacketBuilder) and receive (PacketValidator) paths use identical algorithm
- The implementation is stable and tested

### Incompatibility

Different polynomials (0x1021 vs 0xA001) produce different CRC values. Any daemon implementation following the original PROTOCOL_SPEC.md specification would be incompatible with the firmware.

---

## Decision

**Adopt the firmware implementation as the source of truth.**

The protocol specification will be updated to document CRC16-Modbus (0xA001) instead of CRC16-CCITT (0x1021).

### Rationale

**Why firmware takes precedence:**

1. **Proven Implementation** - Firmware is no longer a prototype. It is a working, tested, deployed hardware reference implementation.

2. **Cost Analysis:**
   - Change firmware: Modify packet generation, packet validation, firmware tests, redeploy
   - Change specification: Update one document section
   - Change daemon: Reimplement CRC to match (minor effort)
   - **Verdict:** Specification change is lowest cost and lowest risk

3. **Consistency** - Firmware shows no evidence of mixed algorithms or incomplete implementation. Both builder and validator use identical code.

4. **Stability** - The firmware CRC implementation is based on standard CRC16-Modbus (IEC 61131-3 Annex C), a well-established algorithm used in industrial systems.

5. **Future Impact** - Changing firmware now would require updating:
   - All protocol test vectors
   - Future protocol captures and diagnostics
   - Any deployed firmware already in use
   - Firmware backward compatibility

---

## Consequences

### Positive
- Protocol specification now matches proven firmware implementation
- Eliminates incompatibility between daemon and firmware
- Daemon CRC implementation can be verified against actual hardware behavior
- Clear audit trail (this ADR) documents the decision and rationale
- Prevents cascading rework later in implementation

### Negative
- Original PROTOCOL_SPEC design choice (CCITT) is not implemented
- Requires updating protocol documentation
- Requires reimplementing daemon CRC code to match new specification

### Mitigations
- Update PROTOCOL_SPEC.md with complete CRC specification including algorithm parameters
- Include protocol test vectors in specification for verification
- Document this decision in ADR for future maintainers
- Implement verification rule (Rule 14) to prevent future mismatches

---

## Implementation

### Phase 1: Protocol Specification Update
Update `shared/docs/PROTOCOL_SPEC.md` to specify:
- **Scheme:** CRC-16-Modbus
- **Polynomial:** 0xA001
- **Initial Value:** 0xFFFF
- **Input Reflection:** Yes
- **Output Reflection:** Yes
- **Final XOR:** 0x0000
- Include algorithm description, examples, and test vectors

### Phase 2: Daemon CRC Reimplementation
Delete existing daemon CRC implementation and reimplement from scratch against the corrected specification. This guarantees:
- Firmware → Specification → Daemon implementation alignment
- All three sources agree exactly
- Clear verification path: test daemon CRC against protocol specification and firmware behavior

### Phase 3: Add Implementation Verification Rule
Add Rule 14 to PROJECT_RULES.md requiring verification of implementation against specification and firmware before task completion.

---

## Alternatives Considered

### Option A: Change Firmware to CCITT
- **Pros:** Honors original specification design choice
- **Cons:** Requires firmware modification, redeploy, test regeneration, protocol capture updates
- **Rejected:** Higher cost, higher risk, firmware is already proven and deployed

### Option B: Create Compatibility Layer
- **Pros:** Support both algorithms dynamically
- **Cons:** Permanently complicates protocol, obscures the inconsistency, added complexity for no benefit
- **Rejected:** Violates principle of single clear specification

---

## Related Decisions

This decision updates the implicit CRC specification decision from the architecture planning phase.

See also:
- PROTOCOL_SPEC.md (specification document)
- PROTOCOL_REGISTRY.md (command registry)
- ADR-0001 (WebSocket architecture)
- ADR-0002 (Library-first daemon)

---

## Sign-Off

| Role | Name | Date | Status |
|------|------|------|--------|
| Lead Architect | Project Owner | 2026-07-20 | Accepted |

---

## Documentation Updates Required

After approval, these documents must be updated:

1. **shared/docs/PROTOCOL_SPEC.md**
   - Replace CRC16-CCITT with CRC16-Modbus specification
   - Add algorithm parameters and examples
   - Include test vectors

2. **shared/docs/PROTOCOL_REGISTRY.md**
   - Update any CRC references

3. **docs/PROJECT_RULES.md**
   - Add Rule 14: Implementation Verification Rule

4. **daemon/src/core/protocol/crc16.ts**
   - Delete and reimplement from scratch

5. **daemon/src/core/protocol/__tests__/crc16.test.ts**
   - Delete and reimplement with correct test vectors

6. **docs/PROJECT_STATUS.md**
   - Update session history

---

**Purpose:** Document the decision to adopt firmware CRC16-Modbus implementation as the protocol standard  
**Status:** Frozen (unless a critical issue emerges requiring reconsideration)  
**Review Cycle:** Approved with this ADR
