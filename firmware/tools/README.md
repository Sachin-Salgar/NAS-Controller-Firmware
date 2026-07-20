# Firmware Verification Tools

Utility programs for verifying firmware implementation correctness.

## crc16_verify

Standalone utility for verifying CRC16-Modbus implementation.

### Purpose

Execute the actual firmware `CRC16::Calculate()` function with test inputs and print CRC outputs. This enables:

1. **Algorithm verification** - Confirm firmware algorithm correctness
2. **Cross-platform comparison** - Compare firmware CRC outputs against daemon implementation
3. **Protocol vector generation** - Generate authentic CRC values for protocol test vectors (requires firmware execution)

### Compilation

```bash
cd firmware/tools
g++ -std=c++17 -I../src crc16_verify.cpp ../src/Utilities/CRC16.cpp -o crc16_verify
```

### Usage

```bash
./crc16_verify
```

Output format:
```
Test N: [description]
  Input: [hex bytes]
  CRC: 0x[hex value]
  Decimal: [decimal value]
```

### Test Vectors

The utility runs standard CRC16-Modbus test vectors including:
- Standard verification string ("123456789")
- Empty buffer
- Single bytes
- All zeros / all ones patterns

### Next Steps

1. Compile and run this utility on the development machine
2. Record firmware CRC outputs
3. Compare against daemon implementation outputs
4. If identical, protocol CRC implementation is verified
5. Use outputs to populate canonical test vectors in PROTOCOL_SPEC.md (only after verification)

### Notes

- This is a host-side verification tool (not firmware code)
- Links actual firmware CRC16 implementation
- Produces deterministic output for comparison
- Can be extended with additional test vectors as needed
