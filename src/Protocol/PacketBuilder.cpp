/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : PacketBuilder.cpp
 *
 ******************************************************************************/

#include "PacketBuilder.h"

#include <cstring>

#include "PacketValidator.h"

using namespace NAS::Core;

namespace NAS::Protocol
{

Result PacketBuilder::Build(
    std::uint16_t sequence,
    std::uint16_t command,
    const std::uint8_t* payload,
    std::uint16_t payloadLength,
    std::uint8_t* packet,
    std::size_t bufferSize,
    std::size_t& packetLength) noexcept
{
    if (packet == nullptr)
    {
        return Result(ResultCode::NullPointer);
    }

    packetLength =
        HeaderSize +
        payloadLength +
        CrcSize;

    if (bufferSize < packetLength)
    {
        return Result(ResultCode::BufferTooSmall);
    }

    std::size_t offset = 0U;

    const std::uint16_t header =
        PacketValidator::Header;

    std::memcpy(
        packet + offset,
        &header,
        sizeof(header));

    offset += sizeof(header);

    std::memcpy(
        packet + offset,
        &sequence,
        sizeof(sequence));

    offset += sizeof(sequence);

    std::memcpy(
        packet + offset,
        &command,
        sizeof(command));

    offset += sizeof(command);

    std::memcpy(
        packet + offset,
        &payloadLength,
        sizeof(payloadLength));

    offset += sizeof(payloadLength);

    if ((payloadLength > 0U) &&
        (payload != nullptr))
    {
        std::memcpy(
            packet + offset,
            payload,
            payloadLength);

        offset += payloadLength;
    }

    const std::uint16_t crc =
        PacketValidator::CalculateCrc16(
            packet,
            offset);

    std::memcpy(
        packet + offset,
        &crc,
        sizeof(crc));

    return Result::Ok();
}

} // namespace NAS::Protocol