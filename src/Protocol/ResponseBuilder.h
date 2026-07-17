#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : ResponseBuilder.h
 *
 * Description:
 * Builds protocol response packets.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Protocol
{

class ResponseBuilder
{
public:

    [[nodiscard]]
    static NAS::Core::Result BuildSuccess(
        std::uint16_t sequence,
        std::uint16_t command,
        const std::uint8_t* payload,
        std::uint16_t payloadLength,
        std::uint8_t* response,
        std::size_t responseBufferSize,
        std::size_t& responseLength) noexcept;

    [[nodiscard]]
    static NAS::Core::Result BuildError(
        std::uint16_t sequence,
        std::uint16_t command,
        NAS::Core::ResultCode errorCode,
        std::uint8_t* response,
        std::size_t responseBufferSize,
        std::size_t& responseLength) noexcept;

private:

    ResponseBuilder() = delete;
};

} // namespace NAS::Protocol