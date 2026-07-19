/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Utilities
 * File    : CRC16.cpp
 *
 ******************************************************************************/

#include "CRC16.h"

namespace NAS::Utilities
{

std::uint16_t CRC16::Calculate(
    const std::uint8_t* data,
    std::size_t length) noexcept
{
    constexpr std::uint16_t Polynomial = 0xA001U;

    std::uint16_t crc = 0xFFFFU;

    if (data == nullptr)
    {
        return crc;
    }

    for (std::size_t index = 0U; index < length; ++index)
    {
        crc ^= data[index];

        for (std::uint8_t bit = 0U; bit < 8U; ++bit)
        {
            if ((crc & 0x0001U) != 0U)
            {
                crc >>= 1U;
                crc ^= Polynomial;
            }
            else
            {
                crc >>= 1U;
            }
        }
    }

    return crc;
}

bool CRC16::Verify(
    const std::uint8_t* data,
    std::size_t length,
    std::uint16_t expectedCrc) noexcept
{
    return Calculate(data, length) == expectedCrc;
}

} // namespace NAS::Utilities