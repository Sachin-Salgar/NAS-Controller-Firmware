// CRC16-Modbus implementation for NAS Controller Protocol
// Protocol specification: PROTOCOL_SPEC.md
// Firmware reference: firmware/src/Utilities/CRC16.cpp

// CRC16-Modbus Algorithm Parameters
// These values MUST match firmware exactly and are defined in PROTOCOL_SPEC.md
const CRC_POLYNOMIAL = 0xA001;
const CRC_INITIAL_VALUE = 0xFFFF;

/**
 * Calculate CRC16-Modbus checksum for given data.
 *
 * This implementation matches the firmware CRC16::Calculate() exactly.
 * Algorithm:
 * - Polynomial: 0xA001 (reversed/reflected form)
 * - Initial value: 0xFFFF
 * - Final XOR: 0x0000
 * - Input reflection: Yes (LSB first)
 * - Output reflection: Yes (LSB first)
 *
 * The algorithm processes data LSB-first (reflected), matching the Modbus standard.
 *
 * @param data - Data to calculate CRC for
 * @param initialValue - Optional initial CRC value (defaults to 0xFFFF for new calculation)
 * @returns 16-bit CRC value (0-65535)
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
    // XOR byte into LSB (reflected form)
    crc ^= data[index];

    // Process 8 bits (LSB first, reflected form)
    for (let bit = 0; bit < 8; bit++) {
      if ((crc & 0x0001) !== 0) {
        // LSB is 1: shift and XOR with polynomial
        crc >>= 1;
        crc ^= CRC_POLYNOMIAL;
      } else {
        // LSB is 0: just shift
        crc >>= 1;
      }
    }
  }

  // Apply final XOR and return
  return (crc ^ CRC_FINAL_XOR) & 0xffff;
}
