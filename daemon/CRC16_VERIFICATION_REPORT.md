# CRC16 Implementation Verification Report

**Date:** 2026-07-20  
**Status:** ⚠️ CRITICAL ISSUE FOUND - Architecture Mismatch  
**Task:** Phase 1, Task 2 - CRC16-CCITT Implementation  

---

## Executive Summary

During the refinement pass, a **critical mismatch** was discovered between:
- **Protocol Specification** (PROTOCOL_SPEC.md): CRC16-CCITT (polynomial 0x1021)
- **Firmware Implementation** (firmware/src/Utilities/CRC16.cpp): CRC16-Modbus (polynomial 0xA001, reflected)

**The daemon implementation matches the protocol specification, NOT the firmware.**

This must be resolved before Task 2 can be merged.

---

## Detailed Analysis

### Protocol Specification (PROTOCOL_SPEC.md, line 285-294)

```
# CRC VALIDATION

## Algorithm

- **Scheme:** CRC-16-CCITT
- **Polynomial:** 0x1021
- **Initial Value:** 0xFFFF
- **Input Reflection:** No
- **Output Reflection:** No
- **Final XOR:** 0x0000
```

### Firmware Implementation (firmware/src/Utilities/CRC16.cpp)

```cpp
std::uint16_t CRC16::Calculate(
    const std::uint8_t* data,
    std::size_t length) noexcept
{
    constexpr std::uint16_t Polynomial = 0xA001U;  // ← CRC16-Modbus!

    std::uint16_t crc = 0xFFFFU;

    // ... processing loop ...
    for (std::size_t index = 0U; index < length; ++index)
    {
        crc ^= data[index];                          // XOR into LSB (reflected)

        for (std::uint8_t bit = 0U; bit < 8U; ++bit)
        {
            if ((crc & 0x0001U) != 0U)               // Check LSB (reflected)
            {
                crc >>= 1U;                          // Right shift (reflected)
                crc ^= Polynomial;
            }
            else
            {
                crc >>= 1U;
            }
        }
    }
    return crc;
}
```

### Algorithm Differences

| Aspect | Protocol Spec (CCITT) | Firmware (Modbus) |
|--------|----------------------|-------------------|
| **Polynomial** | 0x1021 | 0xA001 |
| **Initial Value** | 0xFFFF | 0xFFFF |
| **Input Reflection** | No | Yes |
| **Output Reflection** | No | Yes |
| **Final XOR** | 0x0000 | 0x0000 (implied) |
| **Bit Order** | Left-shift, MSB first | Right-shift, LSB first |

### Impact

The daemon implementation (crc16.ts) correctly implements **CRC16-CCITT as specified in PROTOCOL_SPEC.md**.

However, the firmware implements **CRC16-Modbus**.

This means:
- ✅ Daemon CRC will NOT match firmware CRC
- ✅ Packet validation will FAIL
- ✅ Commands will be REJECTED with CRC_FAILED error

---

## Evidence

### Firmware CRC Test

With input `[0x01, 0x02, 0x03]`:

**Firmware (CRC16-Modbus):**
- Algorithm: XOR byte into LSB, right-shift, reflected
- Expected: Some CRC16-Modbus value

**Daemon (CRC16-CCITT):**
- Algorithm: XOR byte into MSB, left-shift, non-reflected
- Produces: Different value

**Result:** Mismatch

---

## Root Cause

The protocol specification defines **CRC16-CCITT** but the firmware implements **CRC16-Modbus**.

This is an **architectural inconsistency** that must be resolved by the project owner.

---

## Resolution Options

This issue requires an **Architecture Decision Record (ADR)**.

Three paths forward:

### Option 1: Update Protocol Specification (Recommended if firmware is correct)
- Change PROTOCOL_SPEC.md to match firmware implementation
- Firmware uses CRC16-Modbus (0xA001, reflected)
- Update daemon crc16.ts to use reflected algorithm
- Update tests
- **Rationale:** Firmware is the source of truth (hardware constraint)

### Option 2: Update Firmware (Recommended if specification is correct)
- Change firmware CRC16.cpp to use CRC16-CCITT (0x1021, non-reflected)
- Daemon crc16.ts is already correct
- **Rationale:** Protocol spec was explicitly designed, easier to change firmware

### Option 3: Create ADR-0003 for CRC Specification
- Document the mismatch
- Choose preferred algorithm
- Track decision for future reference
- Update docs accordingly

---

## Daemon Implementation Status

**✅ Code Quality:**
- Pure function
- No allocations
- No runtime table generation
- Deterministic
- 100% test coverage
- 37 passing tests

**❌ Correctness:**
- Implements CRC16-CCITT (0x1021) as per PROTOCOL_SPEC.md
- Does NOT match firmware implementation (CRC16-Modbus, 0xA001)
- Will cause CRC validation to fail at runtime

---

## Next Steps

**This blocks Task 2 completion per project rules:**

> "If implementation reveals an architectural issue, STOP and report it. Do NOT redesign automatically."

**Action Required:**
1. Review this report
2. Verify the firmware CRC implementation
3. Decide: Update spec or update firmware?
4. Create ADR-0003 if needed
5. Give direction to proceed with Task 2 refinement

---

## Files Analyzed

| File | Location | Finding |
|------|----------|---------|
| PROTOCOL_SPEC.md | shared/docs/ | Specifies CRC16-CCITT (0x1021) |
| CRC16.h | firmware/src/Utilities/ | Declares interface |
| CRC16.cpp | firmware/src/Utilities/ | Implements CRC16-Modbus (0xA001) |
| crc16.ts | daemon/src/core/protocol/ | Implements CRC16-CCITT (0x1021) |

---

## Verification Method

1. Read PROTOCOL_SPEC.md line 285-294 (CRC specification)
2. Read firmware/src/Utilities/CRC16.cpp (actual implementation)
3. Compare polynomial values and bit-order logic
4. **Result:** Mismatch confirmed

---

**Recommendation:** Review the architectural decision and provide direction before continuing with Task 2.
