# Protocol Test Vector Standard

**Status:** Active  
**Version:** 1.0  
**Owner:** Protocol Specification  
**Purpose:** Define the standard format and verification process for all protocol test vectors

---

## Purpose

Test vectors are the ground truth for protocol implementation verification. This document defines:
- The standard metadata for every test vector
- The verification workflow (firmware first)
- How vectors are documented in PROTOCOL_SPEC.md
- When vectors can be frozen into the specification

---

## Standard Test Vector Format

Every test vector in the protocol specification SHALL include:

```markdown
### Test Vector: [Description]

**Input Bytes (hex):**
[byte sequence, e.g., 55 AA 01 10 00 02]

**Algorithm:**
[Name and parameters, e.g., CRC16-Modbus with polynomial 0xA001]

**Expected Output:**
[Result, e.g., 0xB844]

**Verified Against:**
- Firmware Version: [e.g., 1.0.0]
- Protocol Version: [e.g., 1.0.0]
- Implementation: [File path, e.g., firmware/src/Utilities/CRC16.cpp]

**Verification Date:**
[ISO 8601, e.g., 2026-07-20]

**Status:**
[VERIFIED or PENDING VERIFICATION]
```

---

## Verification Workflow

The official verification workflow for protocol test vectors:

```
Reference Packet Bytes
        ↓
Run through Firmware Reference Implementation
        ↓
Record Output/Expected CRC Value
        ↓
Document in PROTOCOL_SPEC.md
        ↓
Create Daemon Unit Test
        ↓
Daemon Implementation Must Match
        ↓
Integration Test Validates Round-Trip
```

### Key Principle

**Firmware is the authority.**

The firmware reference implementation generates the official test vector output. The daemon never generates the official value first.

### Verification Steps

1. **Identify the input bytes** (defined in protocol spec)
2. **Run through firmware implementation** (actual code in firmware/src/)
3. **Record the output** (the expected value)
4. **Document in PROTOCOL_SPEC.md** using the standard format above
5. **Create daemon unit test** using the verified vector
6. **Run daemon implementation** against the vector
7. **Daemon output must match firmware output** exactly
8. **Integration test** validates the complete round-trip

---

## Example: CRC16-Modbus Test Vector

### Test Vector: Relay Set Command Header

**Input Bytes (hex):**
`55 AA 01 10 00 02`

This is a RELAY_SET command packet header without payload.

**Algorithm:**
CRC16-Modbus (reflected variant)
- Polynomial: 0xA001
- Initial Value: 0xFFFF
- Input Reflection: Yes
- Output Reflection: Yes
- Final XOR: 0x0000

**Expected Output:**
[TO BE FILLED: Run input through firmware/src/Utilities/CRC16.cpp]

**Verified Against:**
- Firmware Version: 1.0.0 (or current version when verified)
- Protocol Version: 1.0.0
- Implementation: firmware/src/Utilities/CRC16.cpp::CRC16::Calculate()

**Verification Date:**
[To be filled when verification session completes]

**Status:**
⏳ PENDING VERIFICATION

**Verification Process:**
1. Execute: `CRC16::Calculate([0x55, 0xAA, 0x01, 0x10, 0x00, 0x02], 6)`
2. Record the u16 return value
3. Update "Expected Output" above
4. Change status to "VERIFIED"
5. Proceed with daemon implementation

---

## Metadata Requirements

Every test vector must include:

| Field | Purpose | Example |
|-------|---------|---------|
| **Input Bytes** | The data being tested | `55 AA 01 10 00 02` |
| **Algorithm** | Precise specification with parameters | CRC16-Modbus (0xA001, reflected) |
| **Expected Output** | The result from firmware | `0xB844` or similar |
| **Firmware Version** | Which firmware version was the source | 1.0.0 |
| **Protocol Version** | Which protocol version defines this | 1.0.0 |
| **Implementation** | File path to the reference code | firmware/src/Utilities/CRC16.cpp |
| **Verification Date** | When it was verified | 2026-07-20 |
| **Status** | VERIFIED or PENDING VERIFICATION | VERIFIED |

---

## Why This Matters

Years from now:

- **"Why does the daemon produce this value?"** → Check the test vector metadata
- **"Which firmware did this test vector come from?"** → Firmware Version field
- **"When was this last verified?"** → Verification Date field
- **"Does this still apply?"** → Protocol Version field

Without this metadata, test vectors become meaningless over time.

---

## Lifecycle of a Test Vector

### Phase 1: Proposal (PENDING VERIFICATION)
- Input bytes defined in PROTOCOL_SPEC.md
- Algorithm specified with full parameters
- Status: PENDING VERIFICATION
- Firmware version: not yet recorded
- Cannot be used for daemon implementation

### Phase 2: Verification (IN PROGRESS)
- Run input through firmware reference implementation
- Record output value
- Firmware version and date recorded
- Status still: PENDING VERIFICATION

### Phase 3: Frozen (VERIFIED)
- Output value confirmed and documented
- Status: VERIFIED
- Firmware and protocol versions locked
- Daemon implementation can proceed
- Vector locked (cannot change without new verification)

---

## Test Vector Governance

### Creating a New Test Vector

1. **Propose** the input bytes and algorithm
2. **Mark as PENDING VERIFICATION**
3. **Run verification** against firmware
4. **Record the output**
5. **Update metadata**
6. **Mark as VERIFIED**
7. **Only then** proceed with daemon implementation

### Changing an Existing Test Vector

⚠️ **Do not modify verified test vectors.**

If a test vector output is incorrect:
1. Create a new test vector with corrected input/algorithm
2. Verify the new vector
3. Deprecate the old vector (mark as superseded)
4. Update all references to use the new vector

### Test Vector Audit

Periodically (quarterly or when versions change):
1. Re-verify all VERIFIED test vectors
2. Confirm firmware still produces the same output
3. Update Verification Date if confirmed
4. Flag any vectors where firmware behavior changed (indicates protocol change)

---

## Test Vector Standards in PROTOCOL_SPEC.md

### Requirement
Every algorithm or command that uses test vectors SHALL document them using the standard format above.

### Examples
- CRC algorithms: Test input → Expected CRC
- Packet encoding: Command bytes → Packet bytes
- State transitions: Current state + command → Expected new state

### Rule: No Theoretical Vectors

❌ **Do not include:**
- Calculated/theoretical expected values
- "Should be" values
- Values from other implementations

✅ **Do include:**
- Verified values from firmware
- Values confirmed by testing
- Values with known provenance

---

## Benefits of This Standard

1. **Traceability:** Every vector traces back to a specific firmware version
2. **Maintainability:** Future developers know exactly how to verify
3. **Auditability:** Clear record of what was verified and when
4. **Stability:** Vectors don't change arbitrarily
5. **Integration:** Daemon can validate against known-good values

---

## Related Documents

- `shared/docs/PROTOCOL_SPEC.md` - Protocol specification with embedded test vectors
- `firmware/docs/` - Firmware implementation that generates official vectors
- `docs/PROJECT_RULES.md` - Rule 14: Implementation Verification

---

**Status:** Active and required for all protocol test vectors  
**Effective:** Immediately for all new test vectors  
**Revision:** Update this document if the format or process changes
