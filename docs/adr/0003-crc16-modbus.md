# ADR-0003: CRC16-Modbus for Protocol Error Detection

**Date:** July 2026  
**Status:** DECIDED ✓  
**Scope:** Protocol v1.0.0 (frozen)

---

## Context

The NAS Controller Protocol requires a mechanism to detect transmission errors and data corruption.

Options considered:
1. CRC16-CCITT (0x1021 polynomial)
2. CRC16-Modbus (0xA001 polynomial) ← **Selected**
3. CRC32 (overkill for this application)
4. Simple checksum (inadequate error detection)

---

## Decision

**Use CRC16-Modbus for all protocol packets.**

### Parameters

| Parameter | Value |
|-----------|-------|
| Algorithm | CRC16-Modbus (Reflected) |
| Polynomial | 0xA001 |
| Initial Value | 0xFFFF |
| Input Reflection | Yes (right-shift) |
| Output Reflection | Yes (reflected) |
| Final XOR | 0x0000 (none) |
| Byte Order | Little-endian (LSB first) |

---

## Rationale

### Why CRC16-Modbus?

1. **Firmware Efficiency**
   - No lookup tables required
   - Bit-shifting algorithm fits embedded constraints
   - Minimal CPU overhead

2. **Wide Compatibility**
   - Industry standard for industrial control
   - Widely implemented in other systems
   - Existing reference implementations available

3. **Adequate Error Detection**
   - Detects single bit errors
   - Detects burst errors up to 16 bits
   - Suitable for control protocol (not safety-critical)

4. **Little-Endian Alignment**
   - Matches ESP32 native byte order
   - No endianness conversion overhead
   - Aligns with rest of protocol (little-endian)

5. **Simplicity**
   - Single algorithm for entire protocol
   - Deterministic, no ambiguity
   - Easy to verify and test

### Why NOT Other Options?

**CRC16-CCITT (0x1021):**
- Common in network protocols, but Modbus is more suitable for embedded control
- No significant advantage over Modbus variant
- Different byte order handling would add complexity

**CRC32:**
- Excessive for this protocol
- Doubles CRC overhead (2 → 4 bytes per packet)
- Unnecessary complexity for non-safety application
- Firmware burden without benefit

**Checksum:**
- Inadequate error detection
- Cannot detect byte swaps or transpositions
- Does not meet reliability requirements

---

## Implementation

### Firmware Reference

Located in `src/Utilities/CRC16.cpp` and `src/Utilities/CRC16.h`

```cpp
static std::uint16_t Calculate(
    const std::uint8_t* data,
    std::size_t length) noexcept
{
    constexpr std::uint16_t Polynomial = 0xA001U;
    std::uint16_t crc = 0xFFFFU;
    
    if (data == nullptr)
        return crc;
    
    for (std::size_t index = 0U; index < length; ++index)
    {
        crc ^= data[index];
        
        for (std::uint8_t bit = 0U; bit < 8U; ++bit)
        {
            if ((crc & 0x0001U) != 0U)
            {
                crc >>= 1U;
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

### Packet Coverage

CRC is calculated over:
- Header (2 bytes)
- Sequence (2 bytes)
- Command (2 bytes)
- Length (2 bytes)
- Payload (0-500 bytes)

**NOT included:**
- The CRC field itself (of course)

### Transmission

CRC16 value (2 bytes) appended to packet in little-endian order:
- Byte 0: CRC LSB (low byte first)
- Byte 1: CRC MSB (high byte second)

---

## Verification

### Firmware Verification ✓

The firmware implementation has been verified:

1. **Algorithm Trace** — Correct polynomial and initial value
2. **Reflection** — Correct right-shift implementation
3. **Test Vectors** — Generated and documented in `shared/docs/protocol/test_vectors/`

### Test Vectors

Five canonical test vectors verify CRC implementation:

| Vector | Purpose | Status |
|--------|---------|--------|
| vector_001 | Minimum packet (Ping) | ✓ Verified |
| vector_002 | Typical command (Relay Set) | ✓ Verified |
| vector_003 | Maximum payload (500 bytes) | ✓ Verified |
| vector_004 | Random payload (50 bytes) | ✓ Verified |
| vector_005 | Invalid CRC (error detection) | ✓ Verified |

### Compatibility

Any independent implementation must:
1. Use identical CRC16-Modbus algorithm
2. Calculate over identical byte range (header through payload)
3. Produce identical CRC values for each test vector
4. Transmit in little-endian byte order

---

## Consequences

### Positive

1. ✓ Simple, efficient implementation
2. ✓ No lookup tables or complex logic
3. ✓ Aligns with industrial control standards
4. ✓ Good error detection for this application
5. ✓ Easy to verify and test
6. ✓ Well-documented, widely understood

### Negative

1. ⚠ Modbus-specific (less common in modern networking)
2. ⚠ Not cryptographically secure (but not required)
3. ⚠ Requires bit-shifting (minor CPU cost)

---

## Freezing

This decision is **FROZEN** for Protocol v1.0.0.

CRC16-Modbus parameters are immutable within v1.x. Any change requires:
1. New ADR for Protocol v2.0.0
2. Updated specification
3. New canonical test vectors
4. All implementations rewritten

---

## References

### Implementation
- `src/Utilities/CRC16.h`
- `src/Utilities/CRC16.cpp`
- `src/Protocol/PacketBuilder.cpp`
- `src/Protocol/PacketValidator.cpp`

### Specification
- `shared/docs/PROTOCOL_SPEC.md` — Complete protocol specification (Section: CRC16-Modbus Algorithm)
- `shared/docs/PROTOCOL_REGISTRY.md` — Protocol constants

### Verification
- `shared/docs/protocol/test_vectors/` — Canonical test vectors
- `shared/docs/protocol/PROTOCOL_FREEZE_CHECKLIST.md` — Verification checklist
- `PROJECT_RULES.md` (Rule 14) — Verification chain

---

## Related Decisions

- **ADR-0001:** Firmware Architecture (layer structure)
- **ADR-0007:** Protocol Layer (packet handling)

---

## Sign-Off

**Decision Date:** July 2026  
**Technical Authority:** Sachin Salgar  
**Status:** ✓ DECIDED (Implemented and Verified)

This ADR is complete and the decision is locked for v1.x.
