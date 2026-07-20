# Verification Session Checklist: CRC16 Before Task 2 Reimplementation

**Purpose:** Verify all documentation, decisions, and specifications are correct before reimplementing the daemon CRC16 library.

**Status:** Ready for execution  
**Effort:** 30-45 minutes (no code changes, verification only)  
**Outcome:** Lock in verified test vector and confirm everything aligns

---

## Pre-Verification State

### What's In Place ✅
- ADR-0003 created (docs/adr/0003-crc16-modbus.md) documenting decision
- PROTOCOL_SPEC.md updated to specify CRC16-Modbus algorithm
- Rule 14 added: Implementation Verification Rule (6-element chain)
- Project status documents updated
- Old ADR-0010 deleted (was incorrectly numbered)

### What Needs Verification 🔍
1. ADR numbering is correct (0001, 0002, 0003 sequence)
2. CRC test vector verified against actual firmware
3. All documents reference the same algorithm (CRC16-Modbus)
4. Old CCITT daemon implementation exists and ready to delete
5. PROJECT_STATUS.md reflects the restart of Task 2

---

## Verification Checklist

### 1. Verify ADR Numbering ✓

**Task:** Confirm ADRs are sequentially numbered

**Locations to check:**
- `docs/adr/` directory (project-level ADRs)
- `firmware/docs/Decisions/` directory (firmware ADRs)

**What we expect:**
- Project ADRs: 0001, 0002, 0003 (exist at docs/adr/)
- Firmware ADRs: 0001-0009 (exist at firmware/docs/Decisions/)
- No gaps or duplicate numbering

**Verification steps:**
```bash
ls -1 docs/adr/ | sort
ls -1 firmware/docs/Decisions/ | grep "^[0-9]" | sort
```

**✅ Pass if:**
- docs/adr contains: 0001, 0002, 0003 exactly
- firmware/docs/Decisions/ contains: 0001-0009 in order
- No ADR-0010 in firmware/docs/Decisions/ (was deleted)

---

### 2. Verify CRC Test Vector Against Firmware ✓

**Task:** Generate and verify the CRC test vector by running the test data through the actual firmware implementation

**Input data:** `[0x55, 0xAA, 0x01, 0x10, 0x00, 0x02]`

**Location of firmware CRC implementation:**
- `firmware/src/Utilities/CRC16.cpp` - Contains `CRC16::Calculate()`
- `firmware/src/Protocol/PacketValidator.cpp` - Contains `PacketValidator::CalculateCrc16()`

**Verification steps:**
1. Read the CRC16 implementation in firmware
2. Manually trace through the algorithm with input bytes:
   - Start: crc = 0xFFFF
   - For each byte, XOR into LSB and process 8 bits
   - Polynomial: 0xA001
3. OR: Create a simple test program to run the firmware code with the input
4. Record the output CRC value (should be 16-bit, big-endian)

**Example expected output format:**
```
Input:  55 AA 01 10 00 02
CRC16:  0x???? (fill in the verified value)
Bytes:  0x?? 0x?? (in big-endian order)
```

**✅ Pass if:**
- The test vector is calculated and verified
- The CRC value is recorded (e.g., `0xB844` or whatever the actual value is)
- Value is confirmed to match firmware calculation

**⚠️ Note:** 
- The value `0xB8 0x44` in the old spec was theoretical, not verified
- This step locks in the correct verified value
- After this, the vector cannot change without re-verification

---

### 3. Verify All Documents Reference CRC16-Modbus ✓

**Task:** Scan all relevant documents for algorithm references

**Documents to check:**
- `shared/docs/PROTOCOL_SPEC.md` - CRC section
- `shared/docs/PROTOCOL_REGISTRY.md` - Any CRC references
- `docs/PROJECT_RULES.md` - Rule 14 verification chain
- `docs/IMPLEMENTATION_ORDER.md` - Task 2 description
- `docs/PROJECT_STATUS.md` - Current status
- `docs/adr/0003-crc16-modbus.md` - Decision record
- `firmware/docs/Decisions/` - Any existing ADRs about CRC

**Verification steps:**
```bash
grep -r "CCITT\|0x1021" docs/ shared/ firmware/docs/Decisions/ --include="*.md"
```

**✅ Pass if:**
- Zero results for "CCITT" in frozen documents (only in history/rejected alternatives)
- Zero results for "0x1021" polynomial
- All algorithm references say "CRC16-Modbus" (not CCITT)
- If CCITT appears, it's only in "Options Considered" or "Why Rejected" sections

---

### 4. Verify Old Daemon CRC Implementation Exists ✓

**Task:** Confirm the old CCITT implementation exists and is ready for deletion

**Location to check:**
- `daemon/src/core/protocol/crc16.ts` - Old implementation
- `daemon/src/core/protocol/__tests__/crc16.test.ts` - Old tests

**Verification steps:**
```bash
ls -la daemon/src/core/protocol/crc16.ts
ls -la daemon/src/core/protocol/__tests__/crc16.test.ts
head -20 daemon/src/core/protocol/crc16.ts  # Should mention CCITT
```

**✅ Pass if:**
- Both files exist
- `crc16.ts` contains CRC16-CCITT implementation (polynomial 0x1021)
- Tests reference CCITT or 0x1021
- Files are ready for deletion (nothing else imports them critically)

**⚠️ Note:**
- These files will be completely deleted, not modified
- A fresh implementation will be written in Task 2

---

### 5. Verify PROJECT_STATUS.md Reflects Task 2 Restart ✓

**Task:** Confirm status document accurately reflects the current state

**File to check:** `docs/PROJECT_STATUS.md`

**Verification steps:**
1. Search for "Task 2" or "CRC16" in the file
2. Check the "Current Position" table
3. Check the "Session History" section

**✅ Pass if:**
- Task 2 status indicates: "⏭️ Reimplementation Pending" (not "Complete")
- References ADR-0003 (not ADR-0010 or other numbers)
- Mentions "Verification Session" as the current step
- "Next Implementation Task" points to CRC16 reimplementation
- No references to CCITT in current status

---

### 6. Verify ADR-0003 Contains All Required Sections ✓

**File to check:** `docs/adr/0003-crc16-modbus.md`

**Verification steps:**
1. Check file exists at `docs/adr/0003-crc16-modbus.md`
2. Verify it contains:
   - Context section (explains the problem)
   - Decision section (what was chosen)
   - Alternatives Considered (options evaluated)
   - Consequences (impacts)
   - Rationale (why this choice)

**✅ Pass if:**
- All sections present and filled in
- Status is "Accepted" (not "Proposed")
- Supersedes field is filled in
- Clearly documents why firmware implementation takes precedence

---

## Final Checklist Before Proceeding

### Documentation Alignment ✅
- [ ] ADR numbering verified (0001, 0002, 0003)
- [ ] ADR-0003 exists and is complete
- [ ] No ADR-0010 in firmware/docs/Decisions/
- [ ] PROTOCOL_SPEC.md specifies CRC16-Modbus (not CCITT)
- [ ] PROTOCOL_REGISTRY.md checked (no changes needed)
- [ ] PROJECT_RULES.md includes Rule 14 with 6-element verification chain
- [ ] All documents reference CRC16-Modbus consistently

### Test Vector Verification ✅
- [ ] Test vector calculated and verified against firmware
- [ ] CRC value recorded (e.g., 0x???? in big-endian)
- [ ] PROTOCOL_SPEC.md test vector section updated with verified value
- [ ] Test vector marked as "Verified" (not "Pending Verification")

### Implementation Readiness ✅
- [ ] Old `daemon/src/core/protocol/crc16.ts` confirmed to exist
- [ ] Old `daemon/src/core/protocol/__tests__/crc16.test.ts` confirmed to exist
- [ ] Old implementation identified as CRC16-CCITT (ready for deletion)
- [ ] PROJECT_STATUS.md shows Task 2 as "⏭️ Reimplementation Pending"

### Rule 14 Compliance ✅
- [ ] ADR created and approved ✓ (ADR-0003)
- [ ] Protocol specification updated ✓ (PROTOCOL_SPEC.md)
- [ ] Firmware implementation verified ✓ (firmware code audited)
- [ ] Test vector verified ✓ (against actual firmware)
- [ ] Document references synchronized ✓ (all say Modbus)
- [ ] Ready for daemon implementation

---

## After Verification Complete

### Next Session: Task 2 Reimplementation

1. **Delete old implementation:**
   - `rm daemon/src/core/protocol/crc16.ts`
   - `rm daemon/src/core/protocol/__tests__/crc16.test.ts`

2. **Implement fresh:**
   - Create new `daemon/src/core/protocol/crc16.ts`
   - Implement CRC16-Modbus from PROTOCOL_SPEC.md
   - Algorithm: 0xA001 polynomial, reflected, initial 0xFFFF

3. **Test:**
   - Create comprehensive test suite
   - Include verified test vector
   - Validate daemon output matches firmware output
   - 100% code coverage

4. **Verify:**
   - Test passes
   - Daemon CRC matches firmware CRC
   - Update PROJECT_STATUS.md: Task 2 → ✅ Complete

---

## Scope: What This Session Does NOT Do

❌ Write any code  
❌ Modify implementations  
❌ Delete anything (just verify it exists)  
❌ Change specifications (only verify they're correct)  
❌ Implement CRC in daemon  

This is purely a **verification session** to ensure all documentation and decisions are locked in correctly before Task 2 begins.

---

## Success Criteria

✅ **Pass:** All 6 checklist sections pass verification  
✅ **Test vector:** Verified against firmware and recorded  
✅ **Documents:** All aligned and internally consistent  
✅ **Status:** Ready to proceed with Task 2 reimplementation  

❌ **Fail:** If any verification fails, create a new task to fix it before proceeding

---

**Time Estimate:** 30-45 minutes  
**Effort Level:** Low (verification, no implementation)  
**Risk Level:** Very low (read-only, no changes to code)  
**Outcome:** Locked-in, verified baseline for Task 2 reimplementation
