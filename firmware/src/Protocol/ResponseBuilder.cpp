/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : ResponseBuilder.cpp
 *
 ******************************************************************************/

#include "ResponseBuilder.h"

#include <cstring>

#include "PacketBuilder.h"

using namespace NAS::Core;

namespace NAS::Protocol
{

Result ResponseBuilder::BuildSuccess(
    std::uint16_t sequence,
    std::uint16_t command,
    const std::uint8_t* payload,
    std::uint16_t payloadLength,
    std::uint8_t* response,
    std::size_t responseBufferSize,
    std::size_t& responseLength) noexcept
{
    return PacketBuilder::Build(
        sequence,
        command,
        payload,
        payloadLength,
        response,
        responseBufferSize,
        responseLength);
}

Result ResponseBuilder::BuildError(
    std::uint16_t sequence,
    std::uint16_t command,
    ResultCode errorCode,
    std::uint8_t* response,
    std::size_t responseBufferSize,
    std::size_t& responseLength) noexcept
{
    std::uint16_t error =
        static_cast<std::uint16_t>(errorCode);

    return PacketBuilder::Build(
        sequence,
        command,
        reinterpret_cast<const std::uint8_t*>(&error),
        sizeof(error),
        response,
        responseBufferSize,
        responseLength);
}

} // namespace NAS::Protocol