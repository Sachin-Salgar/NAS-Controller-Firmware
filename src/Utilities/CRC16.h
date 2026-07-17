#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Utilities
 * File    : CRC16.h
 *
 * Description:
 * CRC-16 (Modbus) utility.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

namespace NAS::Utilities
{

class CRC16
{
public:

    [[nodiscard]]
    static std::uint16_t Calculate(
        const std::uint8_t* data,
        std::size_t length) noexcept;

    [[nodiscard]]
    static bool Verify(
        const std::uint8_t* data,
        std::size_t length,
        std::uint16_t expectedCrc) noexcept;

private:

    CRC16() = delete;
};

} // namespace NAS::Utilities