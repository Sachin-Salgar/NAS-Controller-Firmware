#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : ProtocolService.h
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Services
{

class ProtocolService
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Process() noexcept;

    [[nodiscard]]
    static NAS::Core::Result ProcessPacket(
        const std::uint8_t* packet,
        std::size_t length) noexcept;

private:

    static bool initialized_;
};

} // namespace NAS::Services