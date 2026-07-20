// CRC16-CCITT implementation for NAS Controller Protocol
// Protocol specification: PROTOCOL_SPEC.md

// CRC16-CCITT Algorithm Parameters
// These values are defined in the protocol specification and must match firmware
const CRC_POLYNOMIAL = 0x1021;
const CRC_INITIAL_VALUE = 0xffff;
const CRC_FINAL_XOR = 0x0000;

// Bit mask for checking the most significant bit (MSB)
const CRC_MSB_MASK = 0x8000;

/**
 * Calculate CRC16-CCITT checksum for given data.
 *
 * This implementation matches the firmware CRC16-CCITT calculation exactly.
 * Algorithm:
 * - Polynomial: 0x1021
 * - Initial value: 0xFFFF
 * - Final XOR: 0x0000
 * - Input reflection: No
 * - Output reflection: No
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

  for (let i = 0; i < data.length; i++) {
    const byte = data[i];

    // XOR byte into MSB of CRC
    crc ^= byte << 8;

    // Process 8 bits
    for (let j = 0; j < 8; j++) {
      // Check if MSB is set
      if ((crc & CRC_MSB_MASK) !== 0) {
        // Shift left and XOR with polynomial
        crc = ((crc << 1) ^ CRC_POLYNOMIAL) & 0xffff;
      } else {
        // Shift left only
        crc = (crc << 1) & 0xffff;
      }
    }
  }

  // Apply final XOR and return
  return (crc ^ CRC_FINAL_XOR) & 0xffff;
}
