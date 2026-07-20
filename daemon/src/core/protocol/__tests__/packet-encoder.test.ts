import { encodePacket, isPacketEncodeError } from "../packet-encoder";

describe("encodePacket - Packet Encoder", () => {
  describe("basic packet structure", () => {
    it("should encode minimal packet (no payload)", () => {
      const result = encodePacket(0x01, 0x10);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      // Expected structure: Header(2) + Seq(1) + Cmd(1) + Len(2) + Payload(0) + CRC(2) + Footer(1) = 9 bytes
      expect(result.length).toBe(9);
      expect(result.packet).toBeInstanceOf(Uint8Array);
      expect(result.packet.length).toBe(9);
    });

    it("should encode packet with small payload", () => {
      const payload = new Uint8Array([0x01, 0x01]);
      const result = encodePacket(0x01, 0x10, payload);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      // Expected: Header(2) + Seq(1) + Cmd(1) + Len(2) + Payload(2) + CRC(2) + Footer(1) = 11 bytes
      expect(result.length).toBe(11);
    });

    it("should encode packet with maximum payload", () => {
      const payload = new Uint8Array(256).fill(0xAA);
      const result = encodePacket(0xFF, 0x20, payload);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      // Expected: Header(2) + Seq(1) + Cmd(1) + Len(2) + Payload(256) + CRC(2) + Footer(1) = 265 bytes
      expect(result.length).toBe(265);
    });
  });

  describe("header encoding", () => {
    it("should encode header as 0x55AA (big-endian)", () => {
      const result = encodePacket(0x01, 0x10);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      expect(result.packet[0]).toBe(0x55);
      expect(result.packet[1]).toBe(0xaa);
    });

    it("should be consistent across multiple packets", () => {
      const result1 = encodePacket(0x01, 0x10);
      const result2 = encodePacket(0x02, 0x20);

      if (isPacketEncodeError(result1) || isPacketEncodeError(result2)) {
        fail("Encoding failed");
      }

      expect(result1.packet[0]).toBe(result2.packet[0]);
      expect(result1.packet[1]).toBe(result2.packet[1]);
    });
  });

  describe("sequence number encoding", () => {
    it("should encode sequence at offset 2", () => {
      const result = encodePacket(0x05, 0x10);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      expect(result.packet[2]).toBe(0x05);
    });

    it("should handle sequence rollover at 0xFF", () => {
      const result = encodePacket(0xFF, 0x10);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      expect(result.packet[2]).toBe(0xFF);
    });

    it("should handle sequence 0x00", () => {
      const result = encodePacket(0x00, 0x10);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      expect(result.packet[2]).toBe(0x00);
    });
  });

  describe("command encoding", () => {
    it("should encode command at offset 3", () => {
      const result = encodePacket(0x01, 0x10);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      expect(result.packet[3]).toBe(0x10);
    });

    it("should handle various command bytes", () => {
      const commands = [0x01, 0x10, 0x20, 0x30, 0x40, 0xFF];

      commands.forEach((cmd) => {
        const result = encodePacket(0x01, cmd);

        if (isPacketEncodeError(result)) {
          fail(`Encoding failed for command 0x${cmd.toString(16)}`);
        }

        expect(result.packet[3]).toBe(cmd);
      });
    });
  });

  describe("payload length encoding", () => {
    it("should encode length as big-endian at offset 4-5", () => {
      const payload = new Uint8Array(256).fill(0xAA);
      const result = encodePacket(0x01, 0x10, payload);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      // Length 256 = 0x0100 (big-endian)
      expect(result.packet[4]).toBe(0x01);
      expect(result.packet[5]).toBe(0x00);
    });

    it("should encode small lengths correctly", () => {
      const payload = new Uint8Array([0x01, 0x02]);
      const result = encodePacket(0x01, 0x10, payload);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      // Length 2 = 0x0002 (big-endian)
      expect(result.packet[4]).toBe(0x00);
      expect(result.packet[5]).toBe(0x02);
    });

    it("should encode zero length for packets without payload", () => {
      const result = encodePacket(0x01, 0x10);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      expect(result.packet[4]).toBe(0x00);
      expect(result.packet[5]).toBe(0x00);
    });
  });

  describe("payload encoding", () => {
    it("should copy payload to correct offset", () => {
      const payload = new Uint8Array([0xAA, 0xBB, 0xCC]);
      const result = encodePacket(0x01, 0x10, payload);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      // Payload starts at offset 6 (after header, seq, cmd, len)
      expect(result.packet[6]).toBe(0xAA);
      expect(result.packet[7]).toBe(0xBB);
      expect(result.packet[8]).toBe(0xCC);
    });

    it("should preserve payload data exactly", () => {
      const payload = new Uint8Array([
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
        0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5,
        0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
      ]);
      const result = encodePacket(0x01, 0x10, payload);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      const extractedPayload = result.packet.slice(6, 6 + payload.length);
      expect(extractedPayload).toEqual(payload);
    });

    it("should handle empty payload gracefully", () => {
      const result = encodePacket(0x01, 0x10, new Uint8Array(0));

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      // Should be same as no payload
      expect(result.length).toBe(9);
    });
  });

  describe("CRC16 encoding", () => {
    it("should include CRC16 in packet", () => {
      const result = encodePacket(0x01, 0x10);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      // CRC is at offset (total_length - 3) for a 9-byte packet
      // offset 6 and 7 (counting from 0)
      expect(result.packet.length).toBeGreaterThan(8);
    });

    it("should calculate CRC over header through payload only", () => {
      // Two packets with same content but different values - CRC should differ
      const result1 = encodePacket(0x01, 0x10, new Uint8Array([0x01, 0x01]));
      const result2 = encodePacket(0x02, 0x10, new Uint8Array([0x01, 0x01]));

      if (isPacketEncodeError(result1) || isPacketEncodeError(result2)) {
        fail("Encoding failed");
      }

      // CRC should be different due to different sequence numbers
      // CRC is at offset (length - 3) for both packets
      const crc1Offset = result1.length - 3;
      const crc2Offset = result2.length - 3;
      expect(result1.packet[crc1Offset]).not.toBe(result2.packet[crc2Offset]);
    });

    it("should be deterministic - same input produces same CRC", () => {
      const payload = new Uint8Array([0x01, 0x02, 0x03]);
      const result1 = encodePacket(0x01, 0x10, payload);
      const result2 = encodePacket(0x01, 0x10, payload);

      if (isPacketEncodeError(result1) || isPacketEncodeError(result2)) {
        fail("Encoding failed");
      }

      // CRC bytes should be identical
      expect(result1.packet[result1.length - 3]).toBe(result2.packet[result2.length - 3]);
      expect(result1.packet[result1.length - 2]).toBe(result2.packet[result2.length - 2]);
    });
  });

  describe("footer encoding", () => {
    it("should append footer as 0xAA", () => {
      const result = encodePacket(0x01, 0x10);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      expect(result.packet[result.length - 1]).toBe(0xAA);
    });

    it("should append footer for all packet sizes", () => {
      const payloads = [
        undefined,
        new Uint8Array([0x01]),
        new Uint8Array([0x01, 0x02, 0x03]),
        new Uint8Array(256).fill(0xFF),
      ];

      payloads.forEach((payload) => {
        const result = encodePacket(0x01, 0x10, payload);

        if (isPacketEncodeError(result)) {
          fail("Encoding failed");
        }

        expect(result.packet[result.length - 1]).toBe(0xAA);
      });
    });
  });

  describe("example command packets", () => {
    it("should encode RELAY_SET command (turn on relay 1)", () => {
      const payload = new Uint8Array([0x01, 0x01]); // Relay ID: 1, State: ON
      const result = encodePacket(0x01, 0x10, payload);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      expect(result.packet[0]).toBe(0x55); // Header MSB
      expect(result.packet[1]).toBe(0xaa); // Header LSB
      expect(result.packet[2]).toBe(0x01); // Sequence
      expect(result.packet[3]).toBe(0x10); // Command (RELAY_SET)
      expect(result.packet[4]).toBe(0x00); // Payload length MSB
      expect(result.packet[5]).toBe(0x02); // Payload length LSB
      expect(result.packet[6]).toBe(0x01); // Relay ID
      expect(result.packet[7]).toBe(0x01); // State (ON)
      expect(result.packet[result.length - 1]).toBe(0xaa); // Footer
    });

    it("should encode PING command (connectivity check)", () => {
      const result = encodePacket(0x01, 0x01); // PING command

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      expect(result.packet[3]).toBe(0x01); // Command (PING)
      expect(result.packet[4]).toBe(0x00); // Payload length MSB
      expect(result.packet[5]).toBe(0x00); // Payload length LSB
      expect(result.packet[result.length - 1]).toBe(0xaa); // Footer
    });

    it("should encode FAN_SET_SPEED command", () => {
      const payload = new Uint8Array([0x00, 0x50]); // Fan ID: 0, Speed: 80%
      const result = encodePacket(0x02, 0x30, payload);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      expect(result.packet[2]).toBe(0x02); // Sequence
      expect(result.packet[3]).toBe(0x30); // Command (FAN_SET_SPEED)
      expect(result.packet[6]).toBe(0x00); // Fan ID
      expect(result.packet[7]).toBe(0x50); // Speed (80%)
    });
  });

  describe("input validation", () => {
    it("should reject invalid sequence (negative)", () => {
      const result = encodePacket(-1, 0x10);

      if (!isPacketEncodeError(result)) {
        fail("Should have rejected negative sequence");
      }

      expect(result.code).toBe("INVALID_SEQUENCE");
    });

    it("should reject invalid sequence (too large)", () => {
      const result = encodePacket(0x100, 0x10);

      if (!isPacketEncodeError(result)) {
        fail("Should have rejected sequence > 255");
      }

      expect(result.code).toBe("INVALID_SEQUENCE");
    });

    it("should reject non-integer sequence", () => {
      const result = encodePacket(1.5 as any, 0x10);

      if (!isPacketEncodeError(result)) {
        fail("Should have rejected non-integer sequence");
      }

      expect(result.code).toBe("INVALID_SEQUENCE");
    });

    it("should reject invalid command (negative)", () => {
      const result = encodePacket(0x01, -1);

      if (!isPacketEncodeError(result)) {
        fail("Should have rejected negative command");
      }

      expect(result.code).toBe("INVALID_COMMAND");
    });

    it("should reject invalid command (too large)", () => {
      const result = encodePacket(0x01, 0x100);

      if (!isPacketEncodeError(result)) {
        fail("Should have rejected command > 255");
      }

      expect(result.code).toBe("INVALID_COMMAND");
    });

    it("should reject non-integer command", () => {
      const result = encodePacket(0x01, 1.5 as any);

      if (!isPacketEncodeError(result)) {
        fail("Should have rejected non-integer command");
      }

      expect(result.code).toBe("INVALID_COMMAND");
    });

    it("should reject payload that is not Uint8Array", () => {
      const result = encodePacket(0x01, 0x10, [0x01, 0x02] as any);

      if (!isPacketEncodeError(result)) {
        fail("Should have rejected non-Uint8Array payload");
      }

      expect(result.code).toBe("INVALID_PAYLOAD");
    });

    it("should reject payload longer than 256 bytes", () => {
      const payload = new Uint8Array(257).fill(0xAA);
      const result = encodePacket(0x01, 0x10, payload);

      if (!isPacketEncodeError(result)) {
        fail("Should have rejected payload > 256 bytes");
      }

      expect(result.code).toBe("INVALID_PAYLOAD_LENGTH");
    });
  });

  describe("error handling with type narrowing", () => {
    it("isPacketEncodeError should identify errors correctly", () => {
      const validResult = encodePacket(0x01, 0x10);
      const invalidResult = encodePacket(-1, 0x10);

      expect(isPacketEncodeError(validResult)).toBe(false);
      expect(isPacketEncodeError(invalidResult)).toBe(true);
    });

    it("should allow conditional logic based on error type", () => {
      const result = encodePacket(0x01, 0x10, new Uint8Array([0x01]));

      if (!isPacketEncodeError(result)) {
        // Type should be narrowed to PacketEncodeResult
        expect(result.packet).toBeInstanceOf(Uint8Array);
        expect(result.length).toBeGreaterThan(0);
      }
    });
  });

  describe("edge cases", () => {
    it("should handle all byte values in payload", () => {
      const payload = new Uint8Array(256);
      for (let i = 0; i < 256; i++) {
        payload[i] = i;
      }

      const result = encodePacket(0x01, 0x10, payload);

      if (isPacketEncodeError(result)) {
        fail(`Encoding failed: ${result.message}`);
      }

      expect(result.length).toBe(265); // 2+1+1+2+256+2+1
    });

    it("should handle sequence wraparound at boundary", () => {
      const result1 = encodePacket(0xFE, 0x10);
      const result2 = encodePacket(0xFF, 0x10);
      const result3 = encodePacket(0x00, 0x10);

      if (
        isPacketEncodeError(result1) ||
        isPacketEncodeError(result2) ||
        isPacketEncodeError(result3)
      ) {
        fail("Encoding failed");
      }

      expect(result1.packet[2]).toBe(0xFE);
      expect(result2.packet[2]).toBe(0xFF);
      expect(result3.packet[2]).toBe(0x00);
    });

    it("should be consistent with no payload vs empty payload", () => {
      const result1 = encodePacket(0x01, 0x10);
      const result2 = encodePacket(0x01, 0x10, new Uint8Array(0));

      if (isPacketEncodeError(result1) || isPacketEncodeError(result2)) {
        fail("Encoding failed");
      }

      expect(result1.length).toBe(result2.length);
      expect(result1.packet).toEqual(result2.packet);
    });
  });

  describe("packet roundtrip consistency", () => {
    it("should produce consistent packets across multiple calls", () => {
      const payload = new Uint8Array([0x01, 0x02, 0x03]);
      const packets = [
        encodePacket(0x05, 0x15, payload),
        encodePacket(0x05, 0x15, payload),
        encodePacket(0x05, 0x15, payload),
      ];

      packets.forEach((result, index) => {
        if (isPacketEncodeError(result)) {
          fail(`Encoding failed at call ${index}`);
        }
      });

      // All three packets should be identical
      expect(
        (packets[0] as any).packet.every(
          (byte: number, i: number) => byte === (packets[1] as any).packet[i],
        ),
      ).toBe(true);
      expect(
        (packets[1] as any).packet.every(
          (byte: number, i: number) => byte === (packets[2] as any).packet[i],
        ),
      ).toBe(true);
    });
  });
});
