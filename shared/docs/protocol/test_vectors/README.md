# Protocol Test Vector Templates Repository

**Purpose:** Templates for test vectors that will become canonical after firmware verification

**Status:** ⏳ TEMPLATES PREPARED - Awaiting firmware execution to generate canonical vectors

**Important Distinction:**
- **Vector Templates** (current state): Input bytes specified, CRC values are NULL, awaiting firmware execution
- **Canonical Vectors** (after verification): CRC values captured from firmware, packet bytes frozen, ready for all implementations to use
- Templates are NOT evidence. Canonical vectors are evidence only after firmware generates them.

---

## Overview

This directory contains official test vectors for the NAS Controller binary protocol. Every implementation (firmware, daemon, test tools, future clients) must produce identical packet bytes for all vectors.

---

## Vector Format

Each vector is stored as `vector_NNN.json` with the following schema:

```json
{
  "id": "001",
  "name": "Empty Payload Packet",
  "description": "Minimum valid packet with empty payload",
  "purpose": "Boundary condition: verify algorithm with minimal input",
  "firmware_version": "1.0.0",
  "protocol_version": "1.0.0",
  "crc_algorithm": {
    "scheme": "CRC-16-Modbus",
    "polynomial": "0xA001",
    "initial_value": "0xFFFF",
    "input_reflection": true,
    "output_reflection": true,
    "final_xor": "0x0000"
  },
  "crc_coverage": {
    "description": "Header through Length field (no payload)",
    "start_byte": 0,
    "end_byte": 5,
    "includes": ["Header", "Sequence", "Command", "Length"],
    "excludes": ["Payload", "CRC", "Footer"]
  },
  "input": {
    "packet_bytes": "55AA01100000",
    "description": "Header=0x55AA, Seq=0x01, Cmd=0x10, Len=0x0000"
  },
  "crc_output": {
    "calculated_value": "0x____",
    "transmitted_bytes": ["0x__", "0x__"],
    "byte_order": "MSB-first or LSB-first",
    "position_in_packet": "After payload, before footer"
  },
  "verification": {
    "generated_by": "firmware/src/Utilities/CRC16.cpp::Calculate()",
    "method": "Firmware harness execution and packet capture",
    "date": "2026-07-20",
    "status": "PENDING_VERIFICATION"
  }
}
```

---

## Vector Templates

The following **five vector templates** have been prepared. They will become canonical after firmware execution:

### Vector 1: Minimum Packet (Empty Payload)
- **Purpose:** Boundary condition (smallest valid packet)
- **Input:** Header + Seq + Cmd + Len (0 bytes payload)
- **Example:** `55 AA 01 10 00 00`
- **Tests:** Algorithm correctness with minimal input

### Vector 2: Typical Command (Relay Set)
- **Purpose:** Common operation (relay control)
- **Input:** Header + Seq + Cmd + Len + Payload (2 bytes)
- **Example:** `55 AA 01 10 00 02 01 01`
- **Tests:** Algorithm with small payload

### Vector 3: Maximum Payload Packet
- **Purpose:** Boundary condition (maximum valid packet)
- **Input:** Header + Seq + Cmd + Len + Payload (256 bytes)
- **Tests:** Algorithm correctness with maximum input

### Vector 4: Random/Complex Payload
- **Purpose:** General correctness verification
- **Input:** Packet with arbitrary payload pattern
- **Example:** Mix of 0x00, 0xFF, 0xAA, 0x55 bytes
- **Tests:** Algorithm robustness across all byte values

### Vector 5: CRC Verification Failure Case
- **Purpose:** Verify error detection
- **Input:** Valid packet with intentionally corrupted CRC
- **Expected:** Verification should fail
- **Tests:** CRC validation logic

---

## How to Use

### For Firmware Implementation
```cpp
#include "vector_001.json"  // Load test vector
uint8_t input[] = {0x55, 0xAA, 0x01, 0x10, 0x00, 0x00};
uint16_t crc = CRC16::Calculate(input, 6);
assert(crc == 0x____);  // Expected value from vector
```

### For Daemon Implementation
```typescript
import testVectors from './vector_001.json';
const input = Buffer.from('55AA01100000', 'hex');
const crc = calculateCrc16(input);
assert(crc === parseInt(testVectors.crc_output.calculated_value, 16));
```

### For Test Harness
```python
import json
with open('vector_001.json') as f:
    vector = json.load(f)
input_bytes = bytes.fromhex(vector['input']['packet_bytes'])
crc = calculate_crc16(input_bytes)
assert crc == int(vector['crc_output']['calculated_value'], 0)
print(f"✓ Vector {vector['id']} passed")
```

---

## Verification Status

| Vector | Name | Status |
|--------|------|--------|
| 001 | Minimum Packet | ⏳ PENDING |
| 002 | Relay Set Command | ⏳ PENDING |
| 003 | Maximum Payload | ⏳ PENDING |
| 004 | Random Payload | ⏳ PENDING |
| 005 | CRC Failure Case | ⏳ PENDING |

---

## Interoperability Requirement

> Any independent implementation of the protocol shall produce **identical packet bytes** for all canonical test vectors.

This applies to:
- ✅ Firmware reference implementation
- ✅ Daemon (TypeScript/Node.js)
- ✅ Test tools (Python, Go, etc.)
- ✅ Future clients (Web, Mobile, CLI)

**Not identical CRC values alone.** Identical complete packets.

This ensures protocol correctness is verified at the system integration level, not just the algorithm level.

---

## Adding New Vectors

If a bug is found that vectors didn't catch:
1. Create new vector that reproduces the bug
2. Add to repository with new ID
3. Update PROTOCOL_SPEC.md to include it
4. Re-run all implementations against new set
5. Update version number in PROTOCOL_SPEC.md

---

## Frozen Status

Once all vectors are generated and verified:
- **Protocol Version:** 1.0.0 (frozen)
- **Last Updated:** [Date]
- **Next Change Requires:** New ADR + Protocol Version bump

---

**Repository Status:** Ready for test vector generation during final verification session
