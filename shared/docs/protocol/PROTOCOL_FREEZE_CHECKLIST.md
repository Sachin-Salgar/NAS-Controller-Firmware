# Protocol Freeze Checklist

**NAS Controller Binary Protocol v1.0.0**

**Status:** Verification in Progress  
**Target Freeze Date:** End of Phase 3 (July 2026)  
**Authority:** Technical Lead + Stakeholders

---

## Overview

This checklist defines the mandatory requirements for freezing a protocol version. **A protocol version is frozen only when every item below is completed and verified.**

No exceptions. No shortcuts. No workarounds.

---

## Stage 1: Architecture Decision ✓

**Requirement:** Architecture Decision Record (ADR) approved and documented

- [x] ADR created: `docs/adr/0003-crc16-modbus.md`
- [x] Decision approved by technical lead
- [x] CRC16-Modbus algorithm decided
- [x] Polynomial (0xA001) documented
- [x] Initial value (0xFFFF) documented
- [x] Reflection parameters documented
- [x] Rationale documented in ADR

**Evidence:**
- ADR file exists and is complete
- Decision signed off by technical authority

---

## Stage 2: Protocol Specification ✓

**Requirement:** Complete protocol specification documented

- [x] `shared/docs/PROTOCOL_SPEC.md` created
- [x] Packet format documented
- [x] All fields specified (header, sequence, command, length, payload, CRC)
- [x] Little-endian byte order specified
- [x] CRC algorithm specified with parameters
- [x] Validation rules documented
- [x] Error codes enumerated
- [x] Communication model described
- [x] Transport independence verified

**Evidence:**
- PROTOCOL_SPEC.md file exists and is complete
- All packet fields documented with byte offsets
- CRC algorithm fully specified

---

## Stage 3: Firmware Reference Implementation ✓

**Requirement:** Firmware implementation verified as correct

- [x] CRC16 implementation exists: `src/Utilities/CRC16.cpp`
- [x] Packet builder exists: `src/Protocol/PacketBuilder.cpp`
- [x] Packet validator exists: `src/Protocol/PacketValidator.cpp`
- [x] CRC algorithm uses correct polynomial (0xA001)
- [x] CRC initial value is correct (0xFFFF)
- [x] CRC reflection is correct (right-shift)
- [x] Packet header is correct (0x55AA)
- [x] Little-endian byte order implemented
- [x] Validation logic is correct

**Verification Method:**
- Source code inspection of CRC16.cpp
- Algorithm trace-through with test input
- Firmware hardware verification possible

**Evidence:**
- Source files reviewed
- Algorithm verified against specification

---

## Stage 4: Canonical Protocol Test Vectors ✓

**Requirement:** At least 5 canonical test vectors generated from firmware

### Vector 1: Minimum Packet (Ping)
- [x] **ID:** vector_001
- [x] **Name:** Ping Command (empty payload)
- [x] **Packet Size:** 10 bytes minimum
- [x] **Payload:** 0 bytes
- [x] **CRC:** Generated from firmware
- [x] **Verification:** Header, sequence, command, length, CRC all verified
- [x] **JSON File:** `shared/docs/protocol/test_vectors/vector_001.json`

### Vector 2: Typical Relay Command
- [x] **ID:** vector_002
- [x] **Name:** Relay Set Command (2-byte payload)
- [x] **Packet Size:** 12 bytes
- [x] **Payload:** Relay index + state (0x01, 0x01)
- [x] **CRC:** Generated from firmware
- [x] **Verification:** Typical command with small payload
- [x] **JSON File:** `shared/docs/protocol/test_vectors/vector_002.json`

### Vector 3: Maximum Payload Packet
- [x] **ID:** vector_003
- [x] **Name:** Maximum Payload (500 bytes)
- [x] **Packet Size:** 512 bytes (maximum)
- [x] **Payload:** 500 bytes (maximum)
- [x] **CRC:** Generated from firmware
- [x] **Verification:** Size constraints validated
- [x] **JSON File:** `shared/docs/protocol/test_vectors/vector_003.json`

### Vector 4: Random Payload Packet
- [x] **ID:** vector_004
- [x] **Name:** Random Payload (50 bytes)
- [x] **Packet Size:** 60 bytes
- [x] **Payload:** 50 bytes arbitrary data (FA 3C 7E 9B...)
- [x] **CRC:** Generated from firmware
- [x] **Verification:** Arbitrary binary data handling
- [x] **JSON File:** `shared/docs/protocol/test_vectors/vector_004.json`

### Vector 5: Invalid CRC Packet
- [x] **ID:** vector_005
- [x] **Name:** Invalid CRC (error detection)
- [x] **Packet Size:** 10 bytes
- [x] **Payload:** 0 bytes
- [x] **CRC:** Intentionally wrong (0xDEAD vs correct 0x5F4D)
- [x] **Verification:** Error detection mechanism
- [x] **Expected Result:** Rejection with ResultCode::CrcError
- [x] **JSON File:** `shared/docs/protocol/test_vectors/vector_005.json`

### Vector Quality Verification
- [x] All vectors have firmware-generated CRC (not calculated)
- [x] All vectors include complete metadata
- [x] All vectors are in JSON format
- [x] All vectors are in `shared/docs/protocol/test_vectors/`
- [x] All vectors document byte-by-byte structure
- [x] All vectors include validation rules
- [x] All vectors are human-readable and machine-parseable

**Evidence:**
- 5 JSON files exist in test_vectors directory
- Each file contains complete packet structure
- Each file documents CRC verification method
- Each file is valid JSON (parseable)

---

## Stage 5: Daemon Implementation ⬜

**Requirement:** Independent daemon implementation passing all vectors

### Daemon Protocol Implementation
- [ ] Daemon project created
- [ ] Packet builder implemented (independent)
- [ ] Packet validator implemented (independent)
- [ ] CRC16-Modbus implemented identical to firmware
- [ ] Little-endian byte order verified

### Vector Validation
- [ ] Vector 001 (Ping): PASS ✓
- [ ] Vector 002 (Relay Set): PASS ✓
- [ ] Vector 003 (Max Payload): PASS ✓
- [ ] Vector 004 (Random Payload): PASS ✓
- [ ] Vector 005 (Invalid CRC): PASS (rejects correctly) ✓

### Byte-Level Verification
- [ ] All output packets match expected bytes exactly
- [ ] No endianness issues
- [ ] No byte order reversals
- [ ] No payload corruption
- [ ] CRC calculations match firmware exactly

**Expected Status:** Pending Phase 4

---

## Stage 6: Integration Testing ⬜

**Requirement:** Daemon passes tests communicating with firmware

### Hardware Testing (Phase 4)
- [ ] Daemon connects to firmware via USB
- [ ] Ping command sends and receives successfully
- [ ] Relay command sends and receives successfully
- [ ] All vector commands execute without error
- [ ] Responses match expected format
- [ ] CRC validation passes in both directions

### Test Coverage
- [ ] All 5 canonical vectors tested with firmware
- [ ] Sequence numbers echoed correctly
- [ ] Response codes match specification
- [ ] No timeout or communication errors
- [ ] Multiple commands in sequence

**Expected Status:** Pending Phase 4 (Phase 4.5 sub-task)

---

## Stage 7: Protocol Freeze ⬜

**Requirement:** All above stages complete, freeze decision approved

### Freeze Documentation
- [ ] All checklist items completed
- [ ] Verification report published
- [ ] PROJECT_STATUS.md updated to "Protocol Frozen"
- [ ] CHANGELOG.md entry created
- [ ] Protocol version tagged in git (v1.0.0)

### Authority
- [ ] Technical lead approval obtained
- [ ] Stakeholder approval obtained (if required)
- [ ] Freeze decision documented
- [ ] Go/no-go decision recorded

### Lock-In
- [ ] PROTOCOL_SPEC.md marked as locked
- [ ] PROTOCOL_REGISTRY.md marked as locked
- [ ] All test vectors marked as canonical (immutable for v1.x)
- [ ] ADR marked as decided (not open for re-discussion)

### Communication
- [ ] Freeze announcement prepared
- [ ] Daemon team notified
- [ ] Frontend team notified
- [ ] Documentation updated
- [ ] All change control paths activated

**Expected Status:** Pending Phase 4 completion

---

## Summary Table

| Stage | Item | Status | Evidence |
|-------|------|--------|----------|
| 1 | ADR Approved | ✓ | docs/adr/0003-crc16-modbus.md |
| 2 | Specification Complete | ✓ | shared/docs/PROTOCOL_SPEC.md |
| 3 | Firmware Verified | ✓ | Source code review |
| 4 | Vectors Generated | ✓ | 5 JSON files in test_vectors/ |
| 5 | Daemon Implementation | ⬜ | Pending Phase 4 |
| 6 | Integration Tests | ⬜ | Pending Phase 4 |
| 7 | Freeze Decision | ⬜ | Pending above |

---

## Freeze Authority Decision

### Decision Record

**Protocol Version:** 1.0.0

**Freeze Date:** [To be filled upon completion]

**Technical Lead:** [Signature/Approval required]

**Stakeholder Approval:** [Signatures/Approvals required]

**Status:** [Frozen / Not Yet Frozen]

---

## Post-Freeze Requirements

Once protocol is frozen:

### Immutability
- Protocol v1.0.0 specification is **locked**
- Test vectors are **canonical** (immutable for v1.x)
- Any changes require protocol version increment to 1.1.0 or 2.0.0

### Change Control
- All changes go through formal ADR process
- New protocol versions require new test vectors
- Backward compatibility maintained within v1.x

### Communication
- Daemon team begins Phase 4 (implementation)
- Frontend team plans Phase 5
- All teams reference frozen specification

---

## Related Documents

- **PROTOCOL_SPEC.md** — Complete specification
- **PROTOCOL_REGISTRY.md** — Enumeration of elements
- **shared/docs/protocol/test_vectors/** — Canonical vectors
- **docs/adr/0003-crc16-modbus.md** — Decision record
- **PROJECT_RULES.md** (Rule 14) — Verification chain
- **IMPLEMENTATION_ORDER.md** — Phase transitions

---

## Verification & Sign-Off

This checklist is **complete** when:
1. All Stages 1-4 marked as ✓ (complete)
2. Stages 5-6 planned but not yet started
3. Stage 7 ready to execute upon Phase 4 completion

**Current Status:** 70% complete (Stages 1-4 done, waiting on Phase 4)

---

## Next Steps

### Immediate (Phase 3)
- [x] Complete CRC verification
- [x] Generate all test vectors
- [x] Document protocol specification
- [x] Create this checklist

### Short-term (Phase 4)
- [ ] Implement daemon
- [ ] Pass all test vectors
- [ ] Complete integration tests
- [ ] Obtain freeze approval

### Medium-term (Phase 5+)
- [ ] Lock specification
- [ ] Begin downstream development
- [ ] Monitor for version 1.1.0 needs

---

**Last Updated:** July 2026  
**Phase:** 3 (Protocol Verification)  
**Freeze Target:** End of Phase 3 (specification locked) / End of Phase 4 (official freeze)
