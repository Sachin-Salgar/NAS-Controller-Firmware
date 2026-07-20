// CRC16-Modbus implementation matching firmware exactly
const CRC_POLYNOMIAL = 0xA001;
const CRC_INITIAL_VALUE = 0xFFFF;

function crc16(data, initialValue = CRC_INITIAL_VALUE) {
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
  
  // Firmware returns CRC without final XOR
  return crc & 0xffff;
}

// Test vectors
const vectors = [
  {
    id: 1,
    name: "Minimum Valid Packet",
    data: [0x55, 0xAA, 0x01, 0x10, 0x00, 0x00],
    expected: "0x4E1F"
  },
  {
    id: 2,
    name: "Typical Relay Set Command",
    data: [0x55, 0xAA, 0x02, 0x10, 0x00, 0x02, 0x00, 0x01],
    expected: "0x6E3D"
  },
  {
    id: 4,
    name: "Random/Complex Payload",
    data: [0x55, 0xAA, 0x04, 0x60, 0x00, 0x10,
            0x55, 0xAA, 0x55, 0xAA, 0xAA, 0x55,
            0x44, 0x01, 0xFF, 0x80, 0x00, 0xCC, 0x33],
    expected: "0x29C2"
  },
  {
    id: 5,
    name: "CRC Failure Case",
    data: [0x55, 0xAA, 0x05, 0x10, 0x00, 0x02, 0x00, 0x01],
    expected: "0x5E1D"
  }
];

console.log("CRC16-Modbus Verification\n");

vectors.forEach(v => {
  const buf = Buffer.from(v.data);
  const result = crc16(new Uint8Array(v.data));
  const resultHex = "0x" + result.toString(16).toUpperCase().padStart(4, '0');
  const match = resultHex === v.expected;
  
  console.log(`Vector ${v.id}: ${v.name}`);
  console.log(`  Data: ${buf.toString('hex').toUpperCase()}`);
  console.log(`  Calculated: ${resultHex}`);
  console.log(`  Expected:   ${v.expected}`);
  console.log(`  Match: ${match ? "✓" : "✗"}`);
  console.log();
});

// Test Vector 3 (max payload)
console.log("Vector 3: Maximum Payload Packet (256 bytes)");
const v3Data = new Uint8Array(262);
v3Data[0] = 0x55;
v3Data[1] = 0xAA;
v3Data[2] = 0x03;
v3Data[3] = 0x20;
v3Data[4] = 0x01;
v3Data[5] = 0x00;
for (let i = 0; i < 256; i++) {
  v3Data[6 + i] = i & 0xFF;
}
const result3 = crc16(v3Data);
const result3Hex = "0x" + result3.toString(16).toUpperCase().padStart(4, '0');
console.log(`  Calculated: ${result3Hex}`);
console.log(`  Expected:   0x4B85`);
console.log(`  Match: ${result3Hex === "0x4B85" ? "✓" : "✗"}`);
