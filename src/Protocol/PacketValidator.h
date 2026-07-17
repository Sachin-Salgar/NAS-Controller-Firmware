#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : PacketValidator.h
 *
 * Description:
 * Protocol packet validation.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Protocol
{

class PacketValidator
{
public:

    static constexpr std::uint16_t Header = 0x55AA;

    static constexpr std::size_t MinimumPacketSize = 12U;

    [[nodiscard]]
    static NAS::Core::Result Validate(
        const std::uint8_t* packet,
        std::size_t length) noexcept;

    [[nodiscard]]
    static std::uint16_t CalculateCrc16(
        const std::uint8_t* data,
        std::size_t length) noexcept;

private:

    PacketValidator() = delete;
};

} // namespace NAS::Protocol