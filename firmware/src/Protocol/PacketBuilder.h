#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : PacketBuilder.h
 *
 * Description:
 * Builds protocol packets.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Protocol
{

class PacketBuilder
{
public:

    static constexpr std::size_t HeaderSize = 8U;

    static constexpr std::size_t CrcSize = 2U;

    [[nodiscard]]
    static NAS::Core::Result Build(
        std::uint16_t sequence,
        std::uint16_t command,
        const std::uint8_t* payload,
        std::uint16_t payloadLength,
        std::uint8_t* packet,
        std::size_t bufferSize,
        std::size_t& packetLength) noexcept;

private:

    PacketBuilder() = delete;
};

} // namespace NAS::Protocol