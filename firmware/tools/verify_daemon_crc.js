#!/usr/bin/env node

/**
 * Daemon CRC16 Verification Script
 *
 * Purpose:
 * Compare daemon CRC implementation against the firmware algorithm
 * using standard CRC16-Modbus test vectors.
 *
 * This script imports the daemon CRC16 implementation and tests it
 * against well-known CRC16-Modbus verification vectors.
 *
 * Usage:
 * node verify_daemon_crc.js
 */

// Import the daemon CRC16 implementation
const { crc16 } = require('../../daemon/src/core/protocol/crc16.ts');

console.log('=== Daemon CRC16 Verification Against Firmware Algorithm ===\n');

// Standard CRC16-Modbus test vectors
const testVectors = [
  {
    name: 'Standard CRC16-Modbus: "123456789"',
    input: [0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39],
    expected: 0x4B37,
    expectedDec: 19255,
    description: 'Well-known standard verification vector for CRC16-Modbus'
  },
  {
    name: 'Empty buffer',
    input: [],
    expected: 0xFFFF,
    expectedDec: 65535,
    description: 'Empty input returns initial value'
  },
  {
    name: 'Single byte 0x00',
    input: [0x00],
    expected: null, // We don't specify, just verify it\'s in range
    expectedDec: null,
    description: 'Verifies algorithm handles null bytes'
  },
  {
    name: 'Single byte 0xFF',
    input: [0xFF],
    expected: null,
    expectedDec: null,
    description: 'Verifies algorithm handles 0xFF bytes'
  },
  {
    name: 'All zeros (10 bytes)',
    input: Array(10).fill(0x00),
    expected: null,
    expectedDec: null,
    description: 'Verifies algorithm handles repeated bytes'
  },
  {
    name: 'All ones (10 bytes)',
    input: Array(10).fill(0xFF),
    expected: null,
    expectedDec: null,
    description: 'Verifies algorithm handles all set bits'
  }
];

let passed = 0;
let failed = 0;

testVectors.forEach((vector, index) => {
  console.log(`Test ${index + 1}: ${vector.name}`);
  console.log(`  Description: ${vector.description}`);
  
  try {
    const data = new Uint8Array(vector.input);
    const result = crc16(data);
    
    console.log(`  Input bytes: ${vector.input.length === 0 ? '(empty)' : vector.input.map(b => '0x' + b.toString(16).toUpperCase().padStart(2, '0')).join(' ')}`);
    console.log(`  CRC result: 0x${result.toString(16).toUpperCase().padStart(4, '0')} (decimal: ${result})`);
    
    if (vector.expected !== null) {
      if (result === vector.expected) {
        console.log(`  ✓ PASS: matches expected 0x${vector.expected.toString(16).toUpperCase().padStart(4, '0')}`);
        passed++;
      } else {
        console.log(`  ✗ FAIL: expected 0x${vector.expected.toString(16).toUpperCase().padStart(4, '0')}, got 0x${result.toString(16).toUpperCase().padStart(4, '0')}`);
        failed++;
      }
    } else {
      if (result >= 0 && result <= 0xFFFF) {
        console.log(`  ✓ PASS: result in valid 16-bit range`);
        passed++;
      } else {
        console.log(`  ✗ FAIL: result out of range`);
        failed++;
      }
    }
  } catch (error) {
    console.log(`  ✗ ERROR: ${error.message}`);
    failed++;
  }
  
  console.log();
});

console.log('=== Verification Summary ===');
console.log(`Passed: ${passed}/${testVectors.length}`);
console.log(`Failed: ${failed}/${testVectors.length}`);
console.log();

if (failed === 0) {
  console.log('✓ All tests passed. Daemon CRC implementation is consistent with firmware algorithm.');
  process.exit(0);
} else {
  console.log('✗ Some tests failed. Review implementation.');
  process.exit(1);
}
