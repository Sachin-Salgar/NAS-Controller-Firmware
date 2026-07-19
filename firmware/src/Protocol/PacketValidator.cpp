/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : PacketValidator.cpp
 *
 ******************************************************************************/

#include "PacketValidator.h"

#include <cstring>

using namespace NAS::Core;

namespace NAS::Protocol
{

Result PacketValidator::Validate(
    const std::uint8_t* packet,
    std::size_t length) noexcept
{
    if (packet == nullptr)
    {
        return Result(ResultCode::NullPointer);
    }

    if (length < MinimumPacketSize)
    {
        return Result(ResultCode::InvalidLength);
    }

    std::uint16_t header = 0U;

    std::memcpy(
        &header,
        packet,
        sizeof(header));

    if (header != Header)
    {
        return Result(ResultCode::InvalidHeader);
    }

    std::uint16_t payloadLength = 0U;

    std::memcpy(
        &payloadLength,
        packet + 6U,
        sizeof(payloadLength));

    const std::size_t expectedLength =
        MinimumPacketSize + payloadLength;

    if (length != expectedLength)
    {
        return Result(ResultCode::InvalidLength);
    }

    std::uint16_t receivedCrc = 0U;

    std::memcpy(
        &receivedCrc,
        packet + (length - sizeof(std::uint16_t)),
        sizeof(receivedCrc));

    const std::uint16_t calculatedCrc =
        CalculateCrc16(
            packet,
            length - sizeof(std::uint16_t));

    if (receivedCrc != calculatedCrc)
    {
        return Result(ResultCode::CrcError);
    }

    return Result::Ok();
}

std::uint16_t PacketValidator::CalculateCrc16(
    const std::uint8_t* data,
    std::size_t length) noexcept
{
    constexpr std::uint16_t Polynomial = 0xA001U;

    std::uint16_t crc = 0xFFFFU;

    for (std::size_t i = 0U; i < length; ++i)
    {
        crc ^= data[i];

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

} // namespace NAS::Protocol