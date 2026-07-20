// CRC16-Modbus implementation for NAS Controller Protocol
// Protocol specification: shared/docs/PROTOCOL_SPEC.md
// Firmware reference: firmware/src/Utilities/CRC16.cpp

// CRC16-Modbus Algorithm Parameters
// These values MUST match firmware exactly and are defined in PROTOCOL_SPEC.md
const CRC_POLYNOMIAL = 0xA001;
const CRC_INITIAL_VALUE = 0xFFFF;
const CRC_FINAL_XOR = 0x0000;

/**
 * Calculate CRC16-Modbus checksum for given data.
 *
 * This implementation matches the firmware CRC16::Calculate() exactly.
 *
 * Algorithm Parameters (from PROTOCOL_SPEC.md):
 * - Polynomial: 0xA001 (reflected form of 0x8005)
 * - Initial Value: 0xFFFF
 * - Input Reflection: Yes (LSB-first processing)
 * - Output Reflection: Yes (reflected algorithm)
 * - Final XOR: 0x0000
 *
 * The algorithm is a reflected CRC-16-Modbus variant where:
 * - Each byte is XORed into the LSB
 * - The register is shifted right (LSB first)
 * - The polynomial is applied when the LSB is 1
 *
 * @param data - Byte sequence to calculate CRC for
 * @param initialValue - Optional initial CRC value (defaults to 0xFFFF)
 * @returns 16-bit unsigned CRC value (0-65535)
 *
 * @throws {TypeError} if data is not a Uint8Array
 * @throws {RangeError} if initialValue is outside [0, 65535]
 *
 * @example
 * const crc = crc16(new Uint8Array([0x01, 0x02, 0x03]));
 *
 * @example Incremental calculation
 * let crc = crc16(new Uint8Array([0x01, 0x02]));
 * crc = crc16(new Uint8Array([0x03, 0x04]), crc);
 */
export function crc16(
  data: Uint8Array,
  initialValue: number = CRC_INITIAL_VALUE,
): number {
  if (data === null || data === undefined) {
    throw new TypeError("data must be a Uint8Array");
  }

  if (!(data instanceof Uint8Array)) {
    throw new TypeError("data must be a Uint8Array");
  }

  if (typeof initialValue !== "number" || initialValue < 0 || initialValue > 0xffff) {
    throw new RangeError("initialValue must be a number between 0 and 65535");
  }

  let crc = initialValue;

  for (let index = 0; index < data.length; index++) {
    crc ^= data[index];

    for (let bit = 0; bit < 8; bit++) {
      if ((crc & 0x0001) !== 0) {
        crc >>= 1;
        crc ^= CRC_POLYNOMIAL;
      } else {
        crc >>= 1;
      }
    }
  }

  return (crc ^ CRC_FINAL_XOR) & 0xffff;
}
