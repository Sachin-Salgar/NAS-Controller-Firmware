#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : PacketParser.h
 *
 * Description:
 * Parses a validated protocol packet.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Protocol
{

class PacketParser
{
public:

    PacketParser() = default;

    [[nodiscard]]
    NAS::Core::Result Parse(
        const std::uint8_t* packet,
        std::size_t length) noexcept;

    [[nodiscard]]
    std::uint16_t GetCommand() const noexcept;

    [[nodiscard]]
    std::uint16_t GetSequence() const noexcept;

    [[nodiscard]]
    std::uint16_t GetPayloadLength() const noexcept;

    [[nodiscard]]
    const std::uint8_t* GetPayload() const noexcept;

private:

    std::uint16_t command_{0U};

    std::uint16_t sequence_{0U};

    std::uint16_t payloadLength_{0U};

    const std::uint8_t* payload_{nullptr};
};

} // namespace NAS::Protocol