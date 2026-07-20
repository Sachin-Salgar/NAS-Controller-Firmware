# ADR-0010: CRC16-Modbus Protocol Alignment

Status: Accepted

Date: 2026-07-20

Decision ID: ADR-0010

---

# Context

During protocol implementation verification, an inconsistency was discovered between the protocol specification and the firmware implementation:

- **Protocol Specification** (PROTOCOL_SPEC.md): Defined CRC16-CCITT with polynomial 0x1021
- **Firmware Implementation**: PacketBuilder, PacketValidator, and utility functions all implement CRC16-Modbus with polynomial 0xA001

Investigation into the firmware codebase revealed:

1. No evidence that CRC16-CCITT was ever implemented
2. All CRC calculations throughout the packet path use the Modbus variant consistently
3. The utility implementation also matches CRC16-Modbus
4. No mixing of algorithms—complete consistency in firmware

This discrepancy posed a critical risk:

- Existing firmware is production-ready and uses CRC16-Modbus
- Reimplementing firmware to match CCITT would require changes across: packet generation, packet validation, firmware tests, daemon implementation, all protocol captures, and documentation
- The cost of changing the specification (one paragraph) is far lower than changing working firmware

The firmware is no longer a prototype; it is the hardware reference implementation. Once the daemon, encoder, decoder, and UI are built against this firmware, discovering the mismatch would require reimplementation of all downstream systems.

---

# Decision

The protocol specification is hereby aligned to match the verified firmware implementation.

**CRC16 Specification:**

- **Algorithm:** CRC16-Modbus (reflected)
- **Polynomial:** 0xA001 (reflected form of 0x8005)
- **Initial Value:** 0xFFFF
- **Input Reflection:** Yes (data bytes XORed LSB-first)
- **Output Reflection:** Yes (result bit-reversed)
- **Final XOR:** 0x0000

**Coverage:** Header | Seq | Cmd | Len | Payload (NOT CRC or Footer)

**Rationale:**

- Firmware has one consistent CRC algorithm throughout the packet path
- PacketBuilder and PacketValidator both use identical implementation
- Utility functions confirm the same algorithm
- No firmware changes required; only documentation updates
- This decision aligns protocol specification with the verified firmware implementation
- Cost-benefit: Minimal (spec update) vs. maximum (firmware + all downstream systems)

---

# Alternatives Considered

## Option A: Update Protocol Spec to CRC16-Modbus (Selected)

**Advantages:**
- Matches verified firmware implementation exactly
- No firmware changes required
- Lower risk, lower cost
- Single document change propagates to daemon/encoder/decoder
- Preserves all existing firmware tests and validation

**Disadvantages:**
- Requires updating protocol documentation
- Requires retraining on actual algorithm

**Reason Selected:**
Firmware is production-ready with one consistent algorithm. Cost of changing spec is trivial vs. cost of changing firmware and all downstream systems.

---

## Option B: Update Firmware to CRC16-CCITT

**Advantages:**
- Would match original protocol design intent

**Disadvantages:**
- Firmware is already complete and tested
- Would require changes to PacketBuilder (generation)
- Would require changes to PacketValidator (validation)
- Would require changes to all firmware tests
- Would require changes to daemon implementation
- Would require re-verification against all protocol captures
- Would require regeneration of any documentation examples
- Higher risk, higher cost, longer delay
- Introduces instability into production-ready code

**Reason Rejected:**
The cost-benefit analysis strongly favors spec alignment. Firmware consistency is verified; specification is a single document.

---

## Option C: Investigate Further (Previously Considered)

Investigation has been completed. Findings are conclusive:

- Firmware CRC implementation is consistent across the entire packet path
- Investigation report documented all evidence
- No additional investigation required

---

# Consequences

## Positive

- Protocol specification now matches verified firmware implementation
- Eliminates ambiguity in daemon/encoder/decoder implementation
- Reduces risk of mismatched implementations in downstream systems
- Firmware requires no changes; tests remain valid
- Clear baseline for future reference implementations

## Trade-offs

- Requires updating PROTOCOL_SPEC.md
- Requires verifying any references in PROTOCOL_REGISTRY.md
- Future developers must understand this decision was made after investigation

---

# Scope

Affects:

- Protocol Specification (PROTOCOL_SPEC.md)
- Protocol Registry (PROTOCOL_REGISTRY.md) — if CRC details are referenced
- All CRC implementations in daemon/encoder/decoder (must implement CRC16-Modbus)
- Firmware Version 1.x+
- Future protocol test vectors (must use CRC16-Modbus algorithm)

Does NOT affect:

- Firmware implementation (already uses CRC16-Modbus)
- Existing firmware tests (already validated against Modbus)
- Existing firmware validation (already uses Modbus)

---

# Related Documents

- firmware/docs/PROTOCOL_SPEC.md — Primary specification document
- shared/docs/PROTOCOL_REGISTRY.md — Command registry (may reference CRC)
- firmware/src/Protocol/PacketValidator.cpp — Reference firmware implementation
- firmware/src/Protocol/PacketBuilder.cpp — Firmware packet generation
- shared/docs/INVESTIGATION_REPORT.md — Detailed technical analysis

---

# Implementation Notes

After this ADR is approved:

1. Update PROTOCOL_SPEC.md CRC section to specify CRC16-Modbus
2. Include test vector: Input `55 AA 01 10 00 02` → Expected CRC16 `0xB8 0x44`
3. Include example calculation (C-like pseudocode)
4. Include byte-order documentation
5. Verify PROTOCOL_REGISTRY.md has no conflicting CRC references
6. When implementing daemon CRC, reimplement fresh against the specification (do not port firmware code)
7. Validate daemon CRC against protocol test vectors before proceeding to Task 3

This ensures firmware → specification → daemon all align exactly.

---

# Verification Rule (Rule 14)

To prevent similar inconsistencies in the future:

**Implementation Verification Rule**

Before any protocol implementation is declared complete, all three must agree:

1. Protocol specification
2. Firmware reference implementation  
3. Protocol test vectors

A task is not complete until all three match exactly.

This rule applies to:

- New commands added in future firmware versions
- Changes to existing command behavior
- Addition of new data types or structures
- Any protocol modification

---

# Supersedes

None (first CRC decision for this protocol)

---

# Future Considerations

- If hardware is redesigned or protocol is versioned, CRC algorithm choice should be revisited for suitability to new platform
- Consider whether CRC16-Modbus remains appropriate if Ethernet is added in future versions
- If firmware architecture is substantially redesigned, verify CRC implementation consistency during refactoring

---

# Approval

Status: Accepted

Approved By: Project Owner

Effective Version: Firmware Version 1.0+

Date Approved: 2026-07-20

---

# Revision History

| Version | Date | Description |
|----------|------|-------------|
| 1.0 | 2026-07-20 | Initial ADR documenting CRC16-Modbus alignment decision |
