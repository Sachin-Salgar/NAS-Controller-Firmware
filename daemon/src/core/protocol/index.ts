// Protocol layer exports
// Includes CRC16, packet encoding (decoding is a future task)

export { crc16 } from "./crc16";
export { encodePacket, isPacketEncodeError } from "./packet-encoder";
export type { PacketEncodeResult, PacketEncodeError } from "./packet-encoder";
