/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : PacketParser.cpp
 *
 ******************************************************************************/

#include "PacketParser.h"

#include <cstring>

#include "Commands.h"
#include "PacketValidator.h"

using namespace NAS::Core;

namespace NAS::Protocol
{

Result PacketParser::Parse(
    const std::uint8_t* packet,
    std::size_t length) noexcept
{
    auto result =
        PacketValidator::Validate(
            packet,
            length);

    if (!result.IsSuccess())
    {
        return result;
    }

    // Packet Layout
    //
    // Offset  Size
    // ------  ----
    // 0       Header (2)
    // 2       Sequence (2)
    // 4       Command (2)
    // 6       Payload Length (2)
    // 8       Payload (...)
    // End-2   CRC16

    std::memcpy(
        &sequence_,
        packet + 2U,
        sizeof(sequence_));

    std::memcpy(
        &command_,
        packet + 4U,
        sizeof(command_));

    std::memcpy(
        &payloadLength_,
        packet + 6U,
        sizeof(payloadLength_));

    if (!Commands::IsValid(command_))
    {
        return Result(ResultCode::NotSupported);
    }

    if (payloadLength_ > 0U)
    {
        payload_ = packet + 8U;
    }
    else
    {
        payload_ = nullptr;
    }

    return Result::Ok();
}

std::uint16_t PacketParser::GetCommand() const noexcept
{
    return command_;
}

std::uint16_t PacketParser::GetSequence() const noexcept
{
    return sequence_;
}

std::uint16_t PacketParser::GetPayloadLength() const noexcept
{
    return payloadLength_;
}

const std::uint8_t* PacketParser::GetPayload() const noexcept
{
    return payload_;
}

} // namespace NAS::Protocol