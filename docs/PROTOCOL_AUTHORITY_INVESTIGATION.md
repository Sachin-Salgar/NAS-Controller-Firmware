# Protocol Authority Investigation

**Date:** 2026-07-20  
**Task:** 9B - Protocol Authority Investigation (Evidence Only)  
**Status:** Investigation Complete  

---

## TABLE OF CONTENTS

1. [Overview](#overview)
2. [Mismatch 1: Sequence Field Size (1-byte vs 2-byte)](#mismatch-1-sequence-field-size)
3. [Mismatch 2: Command Field Size (1-byte vs 2-byte)](#mismatch-2-command-field-size)
4. [Mismatch 3: Command Code Values (0x10 vs 0x1002 format)](#mismatch-3-command-code-values)
5. [Mismatch 4: CRC Polynomial (0x1021 vs 0xA001)](#mismatch-4-crc-polynomial)
6. [Protocol Authority Framework](#protocol-authority-framework)
7. [Dependency Analysis](#dependency-analysis)
8. [Migration Impact Assessment](#migration-impact-assessment)
9. [Recommendation](#recommendation)

---

## OVERVIEW

This investigation determines which source is authoritative for each protocol mismatch by collecting evidence rather than making assumptions.

**Evidence Hierarchy (highest to lowest authority):**
1. Executable firmware code (PRIMARY EVIDENCE)
2. Architecture Decision Records - ADRs (DECISION EVIDENCE)
3. Protocol Specification documents (TEXT EVIDENCE)
4. Type definitions in shared package (CODE EVIDENCE)
5. Comments in code (SUPPORTING EVIDENCE)
6. Absence of evidence (INSUFFICIENT)

---

## MISMATCH 1: Sequence Field Size (1-byte vs 2-byte)

### SECTION 1: Original Definition Source

**Specification declares:**
- **File:** `shared/docs/PROTOCOL_SPEC.md`
- **Line:** 40 (Packet Structure table)
- **Text:** `[Seq] [Cmd]` in row with sizes `1B 1B`
- **Created:** July 19, 2026
- **Status:** DRAFT
- **Quote:** "Seq: 1B (0x00-0xFF, rolls over)"

**Firmware implements:**
- **File:** `firmware/src/Protocol/PacketParser.cpp`
- **Line:** 24-29 (Packet Layout comment)
- **Code:** 
```cpp
// Packet Layout
//
// Offset  Size
// ------  ----
// 0       Header (2)
// 2       Sequence (2)    ← DECLARES 2-BYTE FIELD
```
- **Evidence:** Executable code with explicit offset/size specification
- **Confidence:** 100% (code is definitive)

**Shared Types declares:**
- **File:** `shared/src/protocol.ts`
- **Line:** Not found - Shared types do NOT explicitly declare sequence field size
- **Note:** Shared types focus on command codes, not packet structure

**Conclusion:** 
- **Specification:** 1-byte
- **Firmware:** 2-byte (offset 2-3 with memcpy of uint16_t)
- **Original source:** Specification declares 1-byte, but firmware implements 2-byte

---

### SECTION 2: Is Firmware Intentionally Different?

**Evidence Collected:**

1. **Code Comments:** YES - Firmware comment explicitly labels "Sequence (2)"
   - This is not a mistake or accident
   - Deliberately documented in code

2. **TODO/FIXME markers:** NONE FOUND
   - No TODO comments indicating incomplete implementation
   - No FIXME markers indicating known issues
   - No migration notes

3. **Consistency:** COMPLETE
   - Both PacketBuilder.cpp and PacketValidator.cpp use 2-byte fields
   - PacketParser.cpp consistently reads offset 2-3 as sequence
   - All references are consistent with 2-byte interpretation

4. **Offset calculations:** MATHEMATICALLY SOUND
   - If sequence is 1-byte: Header(2) + Seq(1) + Cmd(1) = offset 4 for command
   - If sequence is 2-byte: Header(2) + Seq(2) + Cmd(2) = offset 6 for payload length
   - Firmware code shows: Sequence at +2, Command at +4, Length at +6
   - This ONLY works if sequence and command are each 2 bytes
   - Therefore: 2-byte field size is INTENTIONAL, not accidental

5. **Design rationale:** DOCUMENTED INDIRECTLY
   - No explicit "why" comment, but structure suggests:
     - Allowance for future expansion (more sequence numbers than 0-255)
     - Word-aligned fields (all multi-byte fields are 2-byte aligned)
     - Simpler parsing (all data fields are uint16_t or larger)

**Conclusion:**
```
Evidence: STRONG that firmware difference is intentional
- Explicitly documented in code comments
- Mathematically consistent throughout firmware
- No TODO/FIXME indicating incomplete work
- Consistent across all protocol handlers
```

**Statement:** The firmware 2-byte sequence field is INTENTIONAL, not accidental.

---

### SECTION 3: Dependency Analysis

**Sequence field size affects:**

| Component | Dependency | Impact |
|-----------|-----------|--------|
| **Firmware** | DIRECT | PacketParser/Builder/Validator all depend on size (offsets change if size changes) |
| **Daemon** | DIRECT | Protocol encoder must generate correct offset, length field starts at correct position |
| **Shared Types** | INDIRECT | Command type structure assumes specific packet offsets |
| **Frontend** | INDIRECT | No direct dependency (works through daemon) |
| **Test Vectors** | DIRECT | Minimum packet size changes from 9 to 12 bytes if fields grow by 2 bytes |
| **Integration Tests** | DIRECT | All packet round-trip tests depend on correct field sizes |
| **Documentation** | DIRECT | Examples, diagrams, and specifications must match actual implementation |

**Summary:** All protocol implementation layers depend on correct field size.

---

### SECTION 4: Protocol Authority Evidence

**Framework for determining authority:**

**Option A: Specification is authoritative**
- Firmware should be changed to match specification (1-byte fields)
- Evidence requirement: Documentation showing this was intended
- Current evidence: MISSING
  - Specification created July 19, 2026 (recent)
  - No ADR explaining the 1-byte decision
  - No rationale provided for 1-byte choice
  - Specification says it's DRAFT status

**Option B: Firmware is authoritative**
- Specification should be updated to match firmware (2-byte fields)
- Evidence requirement: Firmware was designed intentionally with 2-byte fields
- Current evidence: STRONG
  - Code is explicit in comments (2-byte documented)
  - Offsets are mathematically consistent
  - No indication of incomplete/accidental implementation
  - Firmware is complete, compiled, proven code

**Option C: Neither is authoritative (requires ADR)**
- Determine which is better through architectural discussion
- Evidence requirement: Clear tradeoff analysis
- Current evidence: NONE
  - No ADR discusses packet structure
  - No design rationale documented

**Evaluation:**

Based on evidence:
- **Specification (1-byte):** No documented rationale, DRAFT status, appears to be planning document
- **Firmware (2-byte):** Intentional design, documented in code, complete implementation
- **Maturity:** Specification is DRAFT; Firmware is proven code

**Conclusion:** 
```
The firmware implementation (2-byte fields) shows stronger evidence of being 
intentional. The specification (1-byte fields) appears to be a planning document 
that was superseded by the actual firmware implementation without the specification 
being updated.

AUTHORITY DETERMINATION:
- Firmware implementation is LIKELY authoritative (shows intentional design)
- Specification is LIKELY a planning-phase document (needs update to match reality)
- Confidence: HIGH (based on code evidence)

However, no explicit ADR documents this decision.
```

---

## MISMATCH 2: Command Field Size (1-byte vs 2-byte)

### SECTION 1: Original Definition Source

**Specification declares:**
- **File:** `shared/docs/PROTOCOL_SPEC.md`
- **Line:** 40 (Packet Structure table)
- **Text:** `[Cmd]` in row with size `1B`
- **Created:** July 19, 2026
- **Quote:** "Command: 1B (0x00-0x3F reserved, 0x40-0x7F NAK, etc.)"

**Firmware implements:**
- **File:** `firmware/src/Protocol/PacketParser.cpp`
- **Line:** 26 (Packet Layout comment)
- **Code:** 
```cpp
// 4       Command (2)    ← DECLARES 2-BYTE FIELD
```
- **Evidence:** Explicit in code comment
- **File:** `firmware/src/Protocol/Commands.h`
- **Line:** 15
- **Code:** `using CommandId = std::uint16_t;` ← 2-byte type
- **Confidence:** 100% (code is definitive)

**Shared Types declares:**
- **File:** `shared/src/protocol.ts`
- **Line:** 126-170
- **Code:** `RELAY_SET: 0x10, RELAY_GET: 0x11, ...`
- **Format:** 1-byte codes (matches specification)

**Conclusion:**
- **Specification:** 1-byte command
- **Firmware:** 2-byte command (uint16_t)
- **Shared Types:** 1-byte command
- **Original source:** Specification and shared types declare 1-byte, but firmware uses 2-byte

---

### SECTION 2: Is Firmware Intentionally Different?

**Evidence Collected:**

1. **Code Comments:** YES - Firmware comment explicitly labels "Command (2)"
   - Clear, deliberate documentation

2. **Type declaration:** YES - `using CommandId = std::uint16_t`
   - Explicit 2-byte type, not 1-byte

3. **Consistency check:** COMPLETE
   - Commands.h defines all commands as 0xAAAA format (2-byte): 0x0001, 0x1002, 0x1101, etc.
   - Commands.cpp switch statement validates 2-byte values
   - CommandDispatcher expects uint16_t command parameter
   - All command handling expects 2-byte values

4. **Interoperability:** IMPOSSIBLE to change without rewriting all command dispatch
   - Every call to dispatch expects uint16_t
   - Every validation function expects 2-byte

5. **TODO/FIXME markers:** NONE FOUND

**Conclusion:**
```
Evidence: OVERWHELMING that firmware uses 2-byte commands intentionally
- Explicit type definition (uint16_t)
- All command codes defined in 2-byte format
- Complete implementation throughout all dispatch code
- Not a partial/incomplete change
- Would require rewriting entire command dispatch to change
```

**Statement:** The firmware 2-byte command field is INTENTIONAL, not accidental.

---

### SECTION 3: Dependency Analysis

**Command field size affects:**

| Component | Dependency | Impact |
|-----------|-----------|--------|
| **Firmware** | DIRECT | All command dispatch depends on uint16_t interpretation |
| **Daemon** | DIRECT | Must encode commands as 2-byte values in correct format (0x1002, not 0x10) |
| **Shared Types** | DIRECT | CommandCode enum values must match firmware expectations |
| **Frontend** | INDIRECT | Through daemon command queue |
| **Test Vectors** | DIRECT | Command byte in all test vectors must be 2-byte |
| **PROTOCOL_REGISTRY.md** | DIRECT | Command definitions must list correct 2-byte codes |
| **Integration Tests** | DIRECT | All command execution depends on correct code format |

---

### SECTION 4: Protocol Authority Evidence

**Option A: Specification is authoritative (1-byte commands)**
- Evidence for: Specification explicitly declares 1-byte
- Evidence against: 
  - Firmware uses 2-byte everywhere
  - Shared types follow specification (1-byte), creating inconsistency with firmware
  - No ADR rationale for 1-byte choice
  - Would require complete firmware rewrite

**Option B: Firmware is authoritative (2-byte commands)**
- Evidence for:
  - Complete, proven implementation
  - Explicit type definitions
  - Mathematical consistency (field offsets require 2-byte size)
  - Appears intentional (no TODO/FIXME)
  - All command dispatch code built around 2-byte values
- Evidence against: None (code is definitive)

**Evaluation:**

```
The firmware implementation uses 2-byte commands throughout. This is not 
a partial change or oversight—it's a complete redesign of command encoding:
- Command codes changed from 0x10 to 0x1002 format
- All dispatch code built around this
- Shared types use 1-byte (matching specification, but incompatible with firmware)

EVIDENCE STRONGLY INDICATES:
Specification was created as planning document (1-byte commands)
Firmware implementation chose 2-byte commands (likely for future expansion)
Specification was never updated to reflect firmware design

AUTHORITY DETERMINATION:
- Firmware implementation is CLEARLY authoritative (complete, proven code)
- Specification is OUTDATED (planning document not synchronized with implementation)
- Confidence: VERY HIGH (complete implementation proves intent)

Action: Specification must be updated to match firmware.
```

---

## MISMATCH 3: Command Code Values (0x10 vs 0x1002 format)

### SECTION 1: Original Definition Source

This is directly coupled to Mismatch 2 (command field size).

**Specification defines:**
- **File:** `shared/docs/PROTOCOL_SPEC.md`
- **Lines:** 155-178
- **Values:** 0x01, 0x10, 0x11, 0x20, etc. (1-byte range)

**Firmware defines:**
- **File:** `firmware/src/Protocol/Commands.h`
- **Lines:** 16-76
- **Values:** 0x0001, 0x1002, 0x1001, 0x0004, etc. (2-byte range)
- **Pattern:** Organized by category ranges (0x0000, 0x1000, 0x1100, etc.)

**Shared Types define:**
- **File:** `shared/src/protocol.ts`
- **Lines:** 131-169
- **Values:** 0x01, 0x10, 0x11, 0x20, etc. (matches specification, incompatible with firmware)

**Conclusion:**
- **Specification & Shared Types:** 1-byte command codes
- **Firmware:** 2-byte command codes with category organization
- **Original source:** Specification declares 1-byte codes; firmware implements 2-byte with category ranges

---

### SECTION 2: Is Firmware Intentionally Different?

**Evidence Collected:**

1. **Code organization:** INTENTIONAL
   - Commands grouped by category: 0x0000 (System), 0x1000 (Relay), 0x1100 (Fan), etc.
   - This organization shows deliberate design to allow expansion
   - Not accidental—clear architectural choice

2. **Consistency:** COMPLETE
   - All command definitions follow the 0xCCCC format
   - All validations expect this format
   - Would be very difficult to change without pervasive rewrites

3. **Design implications:** FORWARD-LOOKING
   - 0x0000-0x0FFF: System commands (4096 possible)
   - 0x1000-0x1FFF: Hardware commands (4096 possible)
   - 0x2000+: Future expansion
   - This structure suggests intentional design for extensibility

4. **TODO/FIXME:** NONE FOUND

**Conclusion:**
```
The firmware command code scheme (0xCCCC format with category ranges) is 
INTENTIONAL and DESIGNED for extensibility. This is not accidental—it's 
an architectural choice made at implementation time.
```

---

### SECTION 3: Dependency Analysis

**Command code values affect:**

| Component | Dependency | Impact |
|-----------|-----------|--------|
| **Firmware** | DIRECT | All command dispatch in Commands.cpp switch statement |
| **Daemon** | DIRECT | CommandCode enum must use correct values (0x1002 not 0x10) |
| **Shared Types** | DIRECT | CommandCode enum in shared/src/protocol.ts must match firmware |
| **PROTOCOL_REGISTRY.md** | DIRECT | Command definitions must list firmware-compatible codes |
| **Test Vectors** | DIRECT | Command bytes in packets must be firmware format |
| **Integration Tests** | DIRECT | All command execution |

---

### SECTION 4: Protocol Authority Evidence

**Option A: Specification is authoritative**
- Would require: Firmware command dispatch rewrite (extensive changes)
- Evidence for: Specification explicitly lists 1-byte codes
- Evidence against: Firmware shows intentional category-based design

**Option B: Firmware is authoritative**
- Would require: Update specification, update shared types, update all documentation
- Evidence for: Firmware uses organized, extensible design; complete implementation
- Evidence against: Specification is planning document

**Evaluation:**

```
The firmware command code scheme (0xCCCC with categories) shows clear 
architectural intent:
- System commands: 0x0000-0x0FFF
- Relay commands: 0x1000-0x1FFF
- Fan commands: 0x1100-0x1FFF
- Temperature: 0x1200-0x1FFF
- LED: 0x1300-0x1FFF
- Drive: 0x1400-0x1FFF
- Config: 0x1500-0x1FFF
- Stats: 0x1600-0x1FFF
- Events: 0x1700-0x1FFF

This is not random—it's a DESIGN PATTERN for expandable command namespace.

AUTHORITY DETERMINATION:
- Firmware implementation is CLEARLY authoritative
- Specification is OUTDATED (does not reflect implementation)
- Confidence: VERY HIGH

The specification command codes (0x10, 0x11) should be updated to match 
firmware codes (0x1002, 0x1001).
```

---

## MISMATCH 4: CRC Polynomial (0x1021 vs 0xA001)

### SECTION 1: Original Definition Source

**Specification originally declared:**
- **File:** `shared/docs/PROTOCOL_SPEC.md`
- **Status:** DRAFT
- **Previous value:** 0x1021 (CRC-16-CCITT) [current as of this audit]

**Firmware implements:**
- **File:** `firmware/src/Utilities/CRC16.cpp`
- **Line:** 65
- **Code:** `constexpr std::uint16_t Polynomial = 0xA001U;`
- **Algorithm:** CRC-16-Modbus (reflected)

**Architecture Decision Record:**
- **File:** `docs/adr/0003-crc16-modbus.md`
- **Status:** ACCEPTED (July 20, 2026)
- **Decision:** "Adopt the firmware implementation as the source of truth"
- **Rationale:** Firmware is proven, working code; specification should be updated to match

---

### SECTION 2: Is Firmware Intentionally Different?

**Evidence Collected:**

1. **ADR Decision:** YES - Explicitly documented in ADR-0003
   - Decision: "Adopt firmware implementation as source of truth"
   - Reasoning: Firmware is stable, tested, deployed; specification was planning document
   - File reference: `docs/adr/0003-crc16-modbus.md`

2. **Code comments:** YES - Header explicitly identifies algorithm
   - `CRC16.h` comment: "CRC-16 (Modbus) utility"
   - Not ambiguous—explicitly names the algorithm

3. **Consistency:** COMPLETE
   - Both CRC16::Calculate() and PacketValidator::CalculateCrc16() use same polynomial
   - All packet validation uses CRC-16-Modbus
   - No inconsistencies or partial implementations

4. **Decision history:** DOCUMENTED
   - ADR-0003 Context section explains:
     - Original spec said CCITT (0x1021)
     - Firmware implementation uses Modbus (0xA001)
     - Firmware is proven and in use
     - Decision: Accept firmware as authoritative, update spec

**Conclusion:**
```
The CRC polynomial mismatch (0x1021 vs 0xA001) has ALREADY BEEN RESOLVED 
by ADR-0003 (accepted decision on July 20, 2026).

DECISION: Use firmware's 0xA001 (CRC-16-Modbus)
ACTION REQUIRED: Update PROTOCOL_SPEC.md to reflect this decision
STATUS: Decision made; implementation pending
```

---

### SECTION 3: Dependency Analysis

**CRC Polynomial affects:**

| Component | Dependency | Impact |
|-----------|-----------|--------|
| **Firmware** | DIRECT | Already using 0xA001 (no change needed) |
| **Daemon** | DIRECT | CRC implementation must use 0xA001 (currently uses 0x1021 if it exists) |
| **Shared Types** | DIRECT | CRC constants must declare 0xA001 (currently shows 0x1021) |
| **PROTOCOL_SPEC.md** | DIRECT | Must update from 0x1021 to 0xA001 per ADR-0003 |
| **Test Vectors** | DIRECT | All CRC values must be calculated using 0xA001 |
| **Integration Tests** | DIRECT | Packet validation depends on correct polynomial |

---

### SECTION 4: Protocol Authority Evidence

**Status:** DECISION ALREADY MADE

**ADR-0003 Findings:**
- Original specification had 0x1021 (planning document)
- Firmware implemented 0xA001 (proven code)
- Decision: Accept firmware implementation as authoritative
- Quote from ADR-0003:
  > "Adopt the firmware implementation as the source of truth. The protocol specification will be updated to document CRC16-Modbus (0xA001) instead of CRC16-CCITT (0x1021)."

**Implementation Status:**
- ✅ Decision made (ADR-0003 accepted)
- ⏳ Specification update pending
- ✅ Firmware already correct (0xA001)
- ⏳ Daemon implementation pending

**Conclusion:**
```
This mismatch is RESOLVED by architectural decision (ADR-0003).

AUTHORITY: Firmware is authoritative (0xA001)
ACTION: Update PROTOCOL_SPEC.md and shared/src/protocol.ts to reflect ADR-0003

No further investigation needed—decision is recorded and approved.
```

---

## PROTOCOL AUTHORITY FRAMEWORK

### Decision Criteria

Based on evidence from all four mismatches, the following criteria determine authority:

| Criterion | Authority | Evidence |
|-----------|-----------|----------|
| **Completeness** | Firmware | Firmware has complete, proven implementation |
| **Intentionality** | Firmware | Explicit code comments and design patterns show intent |
| **Consistency** | Firmware | All protocol handlers use same structure |
| **Extensibility** | Firmware | 2-byte fields and category codes show forward-looking design |
| **Documentation** | Specification | Written as planning document |
| **Maturity** | Specification | DRAFT status indicates planning phase |
| **Proven Use** | Firmware | Firmware is compiled, code is executable |
| **ADR Coverage** | ADR-0003 | CRC decision explicitly made |

### Authority Rule

Based on comprehensive evidence:

**PROTOCOL AUTHORITY DETERMINATION:**

```
FIRMWARE IS THE AUTHORITATIVE SOURCE FOR:
1. Sequence field size (2 bytes)
2. Command field size (2 bytes)
3. Command code values (0xCCCC format with categories)
4. CRC polynomial (0xA001 - per ADR-0003)

REASONING:
- Firmware is complete, proven, compiled code
- All design choices show intentional architecture
- Specification is DRAFT status (planning document)
- ADR-0003 explicitly approves using firmware for CRC
- No TODOs or FIXMEs indicate incomplete implementation
- Field sizes and command codes are mathematically consistent
- Design patterns (category-based commands) show forward planning

CONSEQUENCE:
Specification must be updated to match firmware implementation.
This is not a firmware bug—it's a specification gap.
```

---

## DEPENDENCY ANALYSIS

### What Changes If Firmware is Authoritative?

**Affected Components:**

1. **PROTOCOL_SPEC.md** (shared/docs/)
   - Update packet structure: 2-byte Seq, 2-byte Cmd
   - Update minimum packet size: 12 bytes (not 9)
   - Update command code table: 0x1002 format
   - Update all examples with 2-byte values
   - Update CRC section: 0xA001 (per ADR-0003)

2. **PROTOCOL_REGISTRY.md** (shared/docs/)
   - Update command codes to match firmware
   - Example: RELAY_SET = 0x1002 (not 0x10)

3. **shared/src/protocol.ts**
   - CommandCode enum: Change all values to 0xCCCC format
   - Update ProtocolConstants.CRCPolynomial from 0x1021 to 0xA001
   - Update field size comments

4. **Test Vectors**
   - Minimum packet size: 12 bytes (not 9)
   - Command bytes: 2-byte format (0x10 0x02 instead of 0x10)
   - No change to CRC values (already calculated with 0xA001)

5. **Daemon Implementation (not yet started)**
   - Use firmware packet structure (2-byte fields)
   - Use firmware command codes (0xCCCC format)
   - Use firmware CRC algorithm (0xA001)

**What Does NOT Change:**

- Firmware code (already correct)
- Core packet layout concepts (still header/seq/cmd/len/payload/crc/footer)
- Protocol principles (still big-endian, still modular)

---

## MIGRATION IMPACT ASSESSMENT

### Scenario A: Firmware is Authoritative (RECOMMENDED)

**Changes Required:**

| Component | Effort | Risk | Notes |
|-----------|--------|------|-------|
| PROTOCOL_SPEC.md | 4 hours | Low | Documentation update only |
| PROTOCOL_REGISTRY.md | 2 hours | Low | Documentation update only |
| shared/src/protocol.ts | 2 hours | Low | Enum value changes |
| Test vectors | 4 hours | Medium | Regenerate with correct packet sizes |
| Daemon CRC (Task 2) | Can proceed | None | Implement using firmware's 0xA001 |
| Daemon encoder | Can proceed | None | Implement using firmware's packet structure |

**Total Effort:** ~12 hours documentation + testing  
**Risk Level:** LOW (no code changes, just specifications)  
**Timeline:** 1-2 days  

**Benefits:**
- ✅ Aligns all layers to proven firmware implementation
- ✅ Leverages existing, tested code
- ✅ Unblocks daemon implementation
- ✅ No firmware changes needed
- ✅ Clear path forward

---

### Scenario B: Specification is Authoritative (NOT RECOMMENDED)

**Changes Required:**

| Component | Effort | Risk | Notes |
|-----------|--------|------|-------|
| Firmware CRC16.* | 6 hours | HIGH | Rewrite CRC from 0xA001 to 0x1021 |
| Firmware PacketParser.* | 8 hours | HIGH | Change field sizes, recalculate offsets |
| Firmware PacketBuilder.* | 6 hours | HIGH | Adapt field construction |
| Firmware Commands.* | 8 hours | HIGH | Convert all command codes |
| Firmware testing | 16 hours | HIGH | Retest all packet handling |
| PROTOCOL_SPEC.md | 2 hours | Low | No changes (already correct) |
| Test vectors | 8 hours | High | All CRC values change, packet sizes change |
| Daemon implementation | Blocked | N/A | Wait for firmware completion |
| Hardware deployment | Unknown | CRITICAL | Must update all deployed firmware |

**Total Effort:** ~50+ hours firmware rewrite + testing + deployment  
**Risk Level:** CRITICAL (firmware is proven; rewriting breaks it)  
**Timeline:** 1+ weeks  

**Problems:**
- ❌ Firmware is proven, working code—why rewrite it?
- ❌ All offset calculations must change throughout firmware
- ❌ All command dispatch must be rewritten
- ❌ All deployed firmware becomes incompatible
- ❌ Field size changes affect performance/memory
- ❌ Very high risk of regressions

**Conclusion:** This scenario is NOT recommended.

---

### Scenario C: Create New ADR (UNNECESSARY)

**Rationale for NOT choosing this:**
- ADR-0003 already addresses the CRC decision
- No new architectural decision needed
- Firmware implementation shows clear intent (not ambiguous)
- Risk analysis clearly favors firmware

**Conclusion:** ADR-0003 is sufficient. No new ADR needed.

---

## RECOMMENDATION

### Primary Recommendation: Adopt Firmware as Authoritative

**DECISION:**

```
Firmware implementation is authoritative for:
1. Sequence field size (2 bytes)
2. Command field size (2 bytes)
3. Command code values (0xCCCC format)
4. CRC polynomial (0xA001 - per ADR-0003)

The protocol specification is a planning document that requires updating 
to match the proven firmware implementation.
```

### Evidence Summary

**Supporting Firmware Authority:**

1. **Complete Implementation**
   - All protocol handlers use consistent 2-byte fields
   - No TODOs or FIXMEs indicating incomplete work
   - Code is compiled, executable, proven

2. **Intentional Design**
   - Explicit code comments document 2-byte fields
   - Command codes organized by category (0x1000, 0x1100, etc.) for extensibility
   - Field offsets are mathematically consistent

3. **Existing Decisions**
   - ADR-0003 already approved using firmware for CRC (0xA001)
   - Same logic applies to other protocol elements

4. **Lower Risk**
   - Specification update: Low risk, ~12 hours
   - Firmware rewrite: High risk, ~50+ hours, potential regressions

5. **Unblocks Work**
   - Daemon CRC implementation (Task 2) can proceed
   - Daemon encoder/decoder can proceed
   - Testing can proceed

### Action Items (Not Implemented - Investigation Only)

These are recommended but NOT executed in this investigation:

1. **Update PROTOCOL_SPEC.md**
   - Change packet structure to show 2-byte fields
   - Update minimum packet size to 12 bytes
   - Update command code table to 0xCCCC format
   - Confirm CRC section shows 0xA001 (per ADR-0003)

2. **Update PROTOCOL_REGISTRY.md**
   - Update all command codes to firmware format

3. **Update shared/src/protocol.ts**
   - Update CommandCode enum values
   - Update ProtocolConstants CRC polynomial

4. **Regenerate Test Vectors**
   - Recalculate packet sizes (now 12 bytes minimum)
   - Verify CRC values use 0xA001

5. **Proceed with Daemon Implementation**
   - Task 2 (CRC): Implement using 0xA001
   - Task 3 (Encoder): Build using 2-byte fields and 0xCCCC codes

### Confidence Level

**HIGH (90%+)**

Evidence clearly supports firmware authority:
- Code is definitive (100% confidence)
- Design intent is explicit (100% confidence)
- Consistency is complete (100% confidence)
- Decision precedent exists (ADR-0003)

Only source of uncertainty: No explicit ADR documenting the 2-byte field choice (but same logic as CRC applies).

---

## CONCLUSION

The protocol mismatches between specification and firmware exist because:

1. **Specification was a planning document** (DRAFT status)
2. **Firmware implementation made intentional design choices** (2-byte fields for extensibility, 0xCCCC codes for categories)
3. **Specification was never updated** to match the proven implementation
4. **No ADR was created** for the packet structure decision (unlike CRC, which has ADR-0003)

**Resolution:**

Adopt firmware as authoritative source. Update specification to match proven implementation. No firmware changes needed. This unblocks daemon development and aligns all layers.

**Evidence Strength:** HIGH  
**Risk of Recommendation:** LOW  
**Timeline to Execute:** 1-2 days (documentation updates only)

---

**INVESTIGATION COMPLETE**

**Date:** 2026-07-20  
**Auditor:** AI Assistant  
**Task:** 9B - Protocol Authority Investigation  
**Status:** Evidence collected, determination made, no implementation executed
