# CRC16 Implementation Investigation Report

**Date:** 2026-07-20  
**Task:** Verify which CRC algorithm is actually used for protocol packet validation  

---

## Key Findings

### 1. Firmware Contains TWO CRC Implementations (Both Modbus)

#### A. Utilities/CRC16
- **File:** `firmware/src/Utilities/CRC16.h`
- **Comment:** "CRC-16 (Modbus) utility"
- **Polynomial:** 0xA001 (Modbus)
- **Usage:** Tests and general utility

#### B. Protocol/PacketValidator  
- **File:** `firmware/src/Protocol/PacketValidator.cpp`
- **Polynomial:** 0xA001 (Modbus)
- **Usage:** **Actual protocol packet CRC** (PacketBuilder and PacketValidator)

**Key Evidence:** Both use 0xA001 with LSB-first bit order (reflected).

---

### 2. Protocol Packet CRC Flow

```
PacketBuilder (line 88-91):
  → PacketValidator::CalculateCrc16()
  → Polynomial 0xA001
  → Encodes packets with CRC16-Modbus

PacketValidator (line 66-77):
  → PacketValidator::CalculateCrc16()
  → Polynomial 0xA001
  → Validates received packets with CRC16-Modbus
```

**Conclusion:** Firmware uses **CRC16-Modbus (0xA001)** for all protocol packets.

---

### 3. Storage/Configuration

**Finding:** No CRC usage in storage, EEPROM, or flash operations.
- StorageDriver uses Flash driver
- No CRC16 calls in configuration code
- Confirmed by full codebase search

---

### 4. Protocol Specification vs. Firmware

| Aspect | PROTOCOL_SPEC.md | Firmware |
|--------|------------------|----------|
| **Polynomial** | 0x1021 (CCITT) | 0xA001 (Modbus) |
| **Input Reflection** | No | Yes |
| **Algorithm Name** | CRC16-CCITT | CRC16-Modbus |

**Critical Mismatch:** Different polynomials produce different CRCs.

---

### 5. Evidence Summary

**Firmware CRC is Modbus because:**
- ✅ Utilities/CRC16.h explicitly labeled "CRC-16 (Modbus)"
- ✅ Both PacketBuilder and PacketValidator use identical 0xA001 algorithm
- ✅ No CCITT implementation exists in firmware
- ✅ Firmware tests use the same Modbus algorithm
- ✅ Consistent across all packet operations

**Protocol Spec says CCITT because:**
- ✅ Explicitly defined in PROTOCOL_SPEC.md line 285-294
- ✅ Different design choice during planning phase

---

### 6. Root Cause Analysis

Most likely scenario:
1. Protocol spec was written during planning with CRC16-CCITT as design choice
2. During firmware implementation, engineer selected CRC16-Modbus (standard embedded algorithm)
3. Firmware was built, tested, and delivered with Modbus
4. Protocol spec was never updated to match actual implementation

---

## Three Options for Resolution

### Option A: Update Protocol Specification (Recommended)
- Change PROTOCOL_SPEC.md to specify CRC16-Modbus (0xA001)
- Add: Input Reflection = Yes, Output Reflection = Yes
- Update daemon crc16.ts to implement 0xA001
- Firmware requires no changes
- **Rationale:** Firmware is proven, tested, and deployed

### Option B: Update Firmware CRC
- Change firmware PacketValidator::CalculateCrc16() to use 0x1021
- Regenerate all firmware CRC tests
- Redeploy firmware
- Daemon crc16.ts stays as-is
- **Rationale:** Spec was the design choice

### Option C: Create ADR-0003
- Document formal decision about CRC algorithm
- Choose A or B above
- Record decision for future reference

---

## Daemon Implementation Status

**Current:** daemon/src/core/protocol/crc16.ts implements CRC16-CCITT (0x1021)
**Problem:** Does NOT match firmware (0xA001)
**Blocked:** Awaiting decision on which to fix

---

## Not Changed in This Investigation

- ❌ No code modifications
- ❌ No spec updates
- ❌ No architectural decisions
- ❌ No workarounds implemented

All changes await your decision.

---

## Next Steps

**Decision Required:**
1. Review evidence above
2. Choose Option A, B, or C
3. Provide direction to Builder AI
4. Builder AI will implement refinements and proceed to Task 3
