/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : PacketValidator.cpp
 *
 ******************************************************************************/

#include "PacketValidator.h"

#include <cstring>

#include "../Config/ProtocolConfig.h"
#include "../Utilities/CRC16.h"

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

    if ((length < MinimumPacketSize) ||
        (length > NAS::Config::Protocol::MaximumPacketSize))
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

    if (payloadLength > NAS::Config::Protocol::MaximumPayloadSize)
    {
        return Result(ResultCode::InvalidLength);
    }

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
    return NAS::Utilities::CRC16::Calculate(data, length);
}

} // namespace NAS::Protocol