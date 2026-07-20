// Packet encoder for NAS Controller Protocol
// Protocol specification: shared/docs/PROTOCOL_SPEC.md
// Firmware reference: firmware/src/Protocol/PacketBuilder.cpp

import { crc16 } from "./crc16";

// Protocol constants from PROTOCOL_SPEC.md
const HEADER = 0x55aa;
const FOOTER = 0xaa;
const MIN_PAYLOAD_LENGTH = 0;
const MAX_PAYLOAD_LENGTH = 256;

export interface PacketEncodeResult {
  packet: Uint8Array;
  length: number;
}

export interface PacketEncodeError {
  code: string;
  message: string;
}

/**
 * Encode a protocol packet according to PROTOCOL_SPEC.md.
 *
 * Packet structure:
 * [Header(2)] [Seq(1)] [Cmd(1)] [Len(2)] [Payload(0-256)] [CRC16(2)] [Footer(1)]
 *
 * Header: 0x55AA (big-endian)
 * Seq: 0x00-0xFF (sequence number, rolls over)
 * Cmd: Command byte
 * Len: Payload length (big-endian)
 * Payload: Command-specific data (0-256 bytes)
 * CRC16: Covers Header through Payload (NOT CRC or Footer)
 * Footer: 0xAA
 *
 * @param sequence - Sequence number (0-255)
 * @param command - Command byte
 * @param payload - Command payload (optional, max 256 bytes)
 * @returns Encoded packet with length, or error
 *
 * @throws {Error} if parameters are invalid
 *
 * @example
 * const result = encodePacket(1, 0x10, new Uint8Array([0x01, 0x01]));
 * if (!("code" in result)) {
 *   console.log(`Packet length: ${result.length}`);
 * }
 */
export function encodePacket(
  sequence: number,
  command: number,
  payload?: Uint8Array,
): PacketEncodeResult | PacketEncodeError {
  // Validate sequence
  if (!Number.isInteger(sequence) || sequence < 0 || sequence > 0xff) {
    return {
      code: "INVALID_SEQUENCE",
      message: "sequence must be an integer between 0 and 255",
    };
  }

  // Validate command
  if (!Number.isInteger(command) || command < 0 || command > 0xff) {
    return {
      code: "INVALID_COMMAND",
      message: "command must be an integer between 0 and 255",
    };
  }

  // Determine payload length
  const payloadLength = payload ? payload.length : 0;

  // Validate payload length
  if (payloadLength < MIN_PAYLOAD_LENGTH || payloadLength > MAX_PAYLOAD_LENGTH) {
    return {
      code: "INVALID_PAYLOAD_LENGTH",
      message: `payload length must be between ${MIN_PAYLOAD_LENGTH} and ${MAX_PAYLOAD_LENGTH}`,
    };
  }

  // Validate payload is Uint8Array if provided
  if (payload && !(payload instanceof Uint8Array)) {
    return {
      code: "INVALID_PAYLOAD",
      message: "payload must be a Uint8Array",
    };
  }

  // Calculate total packet size
  // Header(2) + Seq(1) + Cmd(1) + Len(2) + Payload + CRC(2) + Footer(1)
  const totalSize = 2 + 1 + 1 + 2 + payloadLength + 2 + 1;

  // Create packet buffer
  const packet = new Uint8Array(totalSize);
  let offset = 0;

  // Write header (0x55AA, big-endian)
  packet[offset] = (HEADER >> 8) & 0xff;
  offset++;
  packet[offset] = HEADER & 0xff;
  offset++;

  // Write sequence number (1 byte)
  packet[offset] = sequence & 0xff;
  offset++;

  // Write command (1 byte)
  packet[offset] = command & 0xff;
  offset++;

  // Write payload length (2 bytes, big-endian)
  packet[offset] = (payloadLength >> 8) & 0xff;
  offset++;
  packet[offset] = payloadLength & 0xff;
  offset++;

  // Write payload if present
  if (payloadLength > 0 && payload) {
    packet.set(payload, offset);
    offset += payloadLength;
  }

  // Calculate CRC16 over Header through Payload (excluding CRC and Footer)
  const crcData = packet.slice(0, offset);
  const crcValue = crc16(crcData);

  // Write CRC16 (2 bytes, big-endian)
  packet[offset] = (crcValue >> 8) & 0xff;
  offset++;
  packet[offset] = crcValue & 0xff;
  offset++;

  // Write footer (1 byte)
  packet[offset] = FOOTER & 0xff;
  offset++;

  return {
    packet,
    length: offset,
  };
}

/**
 * Check if a result is an error.
 *
 * Utility function for type narrowing.
 *
 * @param result - Result from encodePacket
 * @returns true if result is an error
 *
 * @example
 * const result = encodePacket(1, 0x10, payload);
 * if (isPacketEncodeError(result)) {
 *   console.error(result.message);
 * } else {
 *   console.log(result.packet);
 * }
 */
export function isPacketEncodeError(
  result: PacketEncodeResult | PacketEncodeError,
): result is PacketEncodeError {
  return "code" in result;
}
