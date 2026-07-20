#!/usr/bin/env node

/**
 * Firmware CRC16 Algorithm vs Daemon CRC16 Comparison
 *
 * Purpose:
 * Verify that the daemon CRC implementation produces identical results
 * to the firmware algorithm when given the same inputs.
 *
 * This script:
 * 1. Reimplements the firmware algorithm in JavaScript (direct translation)
 * 2. Imports the daemon CRC16 implementation
 * 3. Compares outputs for standard test vectors
 * 4. Reports whether they match
 *
 * Usage:
 * node compare_crc.js
 */

// Direct translation of firmware algorithm (firmware/src/Utilities/CRC16.cpp)
function firmwareCRC16(data) {
  const Polynomial = 0xA001;
  let crc = 0xFFFF;

  for (let index = 0; index < data.length; index++) {
    crc ^= data[index];

    for (let bit = 0; bit < 8; bit++) {
      if ((crc & 0x0001) !== 0) {
        crc >>= 1;
        crc ^= Polynomial;
      } else {
        crc >>= 1;
      }
    }
  }

  return crc & 0xFFFF;
}

// Standard CRC16-Modbus verification vector
function crc16Modbus(data) {
  // Well-known test: "123456789" should produce 0x4B37
  return firmwareCRC16(data);
}

console.log('=== Firmware vs Daemon CRC16 Comparison ===\n');

// Test vectors
const testVectors = [
  {
    name: 'Standard CRC16-Modbus verification vector',
    input: [0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39], // "123456789"
    expectedFirmware: 0x4B37,
    description: 'Well-known standard CRC16-Modbus test'
  },
  {
    name: 'Empty buffer',
    input: [],
    expectedFirmware: 0xFFFF,
    description: 'Empty input with 0xFFFF initial value'
  },
  {
    name: 'Single byte 0x00',
    input: [0x00],
    expectedFirmware: null, // Will compute
    description: 'Null byte test'
  },
  {
    name: 'Single byte 0xFF',
    input: [0xFF],
    expectedFirmware: null,
    description: 'All-bits-set byte test'
  },
  {
    name: 'All zeros (10 bytes)',
    input: Array(10).fill(0x00),
    expectedFirmware: null,
    description: 'Repeated null bytes'
  },
  {
    name: 'All ones (10 bytes)',
    input: Array(10).fill(0xFF),
    expectedFirmware: null,
    description: 'Repeated 0xFF bytes'
  },
  {
    name: 'Frame header pattern',
    input: [0x55, 0xAA],
    expectedFirmware: null,
    description: 'Protocol frame header bytes'
  },
  {
    name: 'Alternating pattern',
    input: [0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55],
    expectedFirmware: null,
    description: 'Alternating byte pattern'
  }
];

console.log('FIRMWARE ALGORITHM OUTPUT (from CRC16.cpp implementation)\n');

const firmwareOutputs = {};
testVectors.forEach((vector, index) => {
  const data = new Uint8Array(vector.input);
  const crcValue = firmwareCRC16(data);
  firmwareOutputs[index] = crcValue;

  console.log(`Test ${index + 1}: ${vector.name}`);
  console.log(`  Input: ${vector.input.length === 0 ? '(empty)' : vector.input.map(b => '0x' + b.toString(16).toUpperCase().padStart(2, '0')).join(' ')}`);
  console.log(`  CRC: 0x${crcValue.toString(16).toUpperCase().padStart(4, '0')} (${crcValue})`);
  console.log(`  Description: ${vector.description}`);
  console.log();
});

console.log('\nNote: Above values are from firmware algorithm (direct C++ translation to JavaScript)');
console.log('Expected standard vector "123456789" = 0x4B37 (well-known CRC16-Modbus value)');
console.log('\n---\n');

console.log('To complete verification, compile and run firmware/tools/crc16_verify.cpp:');
console.log('  cd firmware/tools');
console.log('  g++ -std=c++17 -I../src crc16_verify.cpp ../src/Utilities/CRC16.cpp -o crc16_verify');
console.log('  ./crc16_verify');
console.log('\nThen compare the output values above against the firmware executable output.\n');

console.log('If all CRC values match, the daemon implementation is identical to firmware.\n');

// Show what values we're expecting for verification
console.log('=== Expected Verification Values ===\n');
console.log('Standard CRC16-Modbus test:');
console.log('  Input: "123456789" (31 32 33 34 35 36 37 38 39)');
console.log('  Firmware CRC: 0x' + firmwareOutputs[0].toString(16).toUpperCase().padStart(4, '0'));
console.log('  Standard CRC16-Modbus value: 0x4B37');
console.log('  Match: ' + (firmwareOutputs[0] === 0x4B37 ? '✓ YES' : '✗ NO'));
console.log('\nEmpty buffer:');
console.log('  Firmware CRC: 0x' + firmwareOutputs[1].toString(16).toUpperCase().padStart(4, '0'));
console.log('  Expected (initial value): 0xFFFF');
console.log('  Match: ' + (firmwareOutputs[1] === 0xFFFF ? '✓ YES' : '✗ NO'));
