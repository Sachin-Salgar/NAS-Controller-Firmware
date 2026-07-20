/*
 * Firmware CRC16 Verification Utility
 *
 * Purpose:
 * Execute the actual firmware CRC16::Calculate() function with test inputs
 * and print CRC outputs for comparison against daemon implementation.
 *
 * Compilation:
 * g++ -std=c++17 -I../src crc16_verify.cpp ../src/Utilities/CRC16.cpp -o crc16_verify
 *
 * Usage:
 * ./crc16_verify
 */

#include <cstdint>
#include <cstddef>
#include <iostream>
#include <iomanip>
#include "../src/Utilities/CRC16.h"

namespace NAS::Utilities {
  // Forward declaration (defined in CRC16.cpp)
  extern std::uint16_t CRC16::Calculate(
    const std::uint8_t* data,
    std::size_t length) noexcept;
}

int main() {
  using namespace NAS::Utilities;

  std::cout << "=== Firmware CRC16 Verification ===" << std::endl;
  std::cout << std::endl;

  // Test vector 1: Standard CRC16-Modbus verification vector
  // Input: ASCII "123456789"
  std::cout << "Test 1: Standard CRC16-Modbus '123456789'" << std::endl;
  {
    std::uint8_t data[] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };
    std::uint16_t crc = CRC16::Calculate(data, 9);
    std::cout << "  Input: 31 32 33 34 35 36 37 38 39" << std::endl;
    std::cout << "  CRC: 0x" << std::hex << std::setw(4) << std::setfill('0') << crc << std::endl;
    std::cout << "  Decimal: " << std::dec << crc << std::endl;
  }
  std::cout << std::endl;

  // Test vector 2: Empty buffer
  std::cout << "Test 2: Empty buffer" << std::endl;
  {
    std::uint8_t data[] = {};
    std::uint16_t crc = CRC16::Calculate(data, 0);
    std::cout << "  Input: (empty)" << std::endl;
    std::cout << "  CRC: 0x" << std::hex << std::setw(4) << std::setfill('0') << crc << std::endl;
    std::cout << "  Decimal: " << std::dec << crc << std::endl;
  }
  std::cout << std::endl;

  // Test vector 3: Single byte 0x00
  std::cout << "Test 3: Single byte 0x00" << std::endl;
  {
    std::uint8_t data[] = { 0x00 };
    std::uint16_t crc = CRC16::Calculate(data, 1);
    std::cout << "  Input: 00" << std::endl;
    std::cout << "  CRC: 0x" << std::hex << std::setw(4) << std::setfill('0') << crc << std::endl;
    std::cout << "  Decimal: " << std::dec << crc << std::endl;
  }
  std::cout << std::endl;

  // Test vector 4: Single byte 0xFF
  std::cout << "Test 4: Single byte 0xFF" << std::endl;
  {
    std::uint8_t data[] = { 0xFF };
    std::uint16_t crc = CRC16::Calculate(data, 1);
    std::cout << "  Input: FF" << std::endl;
    std::cout << "  CRC: 0x" << std::hex << std::setw(4) << std::setfill('0') << crc << std::endl;
    std::cout << "  Decimal: " << std::dec << crc << std::endl;
  }
  std::cout << std::endl;

  // Test vector 5: All zeros (10 bytes)
  std::cout << "Test 5: All zeros (10 bytes)" << std::endl;
  {
    std::uint8_t data[10] = { 0x00 };
    std::uint16_t crc = CRC16::Calculate(data, 10);
    std::cout << "  Input: 00 00 00 00 00 00 00 00 00 00" << std::endl;
    std::cout << "  CRC: 0x" << std::hex << std::setw(4) << std::setfill('0') << crc << std::endl;
    std::cout << "  Decimal: " << std::dec << crc << std::endl;
  }
  std::cout << std::endl;

  // Test vector 6: All ones (10 bytes)
  std::cout << "Test 6: All ones (10 bytes)" << std::endl;
  {
    std::uint8_t data[10];
    for (int i = 0; i < 10; i++) {
      data[i] = 0xFF;
    }
    std::uint16_t crc = CRC16::Calculate(data, 10);
    std::cout << "  Input: FF FF FF FF FF FF FF FF FF FF" << std::endl;
    std::cout << "  CRC: 0x" << std::hex << std::setw(4) << std::setfill('0') << crc << std::endl;
    std::cout << "  Decimal: " << std::dec << crc << std::endl;
  }
  std::cout << std::endl;

  // Test vector 7: Incremental calculation
  std::cout << "Test 7: Incremental calculation consistency" << std::endl;
  {
    // Calculate [0x01, 0x02, 0x03, 0x04] as one call
    std::uint8_t allData[] = { 0x01, 0x02, 0x03, 0x04 };
    std::uint16_t crc_all = CRC16::Calculate(allData, 4);

    std::cout << "  All at once [01 02 03 04]: 0x" << std::hex << std::setw(4)
              << std::setfill('0') << crc_all << std::endl;
    std::cout << std::dec;  // Reset to decimal for next output
  }
  std::cout << std::endl;

  std::cout << "=== End of verification ===" << std::endl;

  return 0;
}
