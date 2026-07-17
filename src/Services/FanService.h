#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : FanService.h
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"
#include "../Objects/Fan.h"

namespace NAS::Services
{

class FanService
{
public:

    static constexpr std::uint8_t FanCount = 2U;

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetSpeed(std::uint8_t fanId,
                                      std::uint8_t speed) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetMode(std::uint8_t fanId,
                                     NAS::Objects::FanMode mode) noexcept;

    [[nodiscard]]
    static NAS::Objects::Fan& GetFan(std::uint8_t fanId) noexcept;

private:

    static NAS::Objects::Fan fans_[FanCount];

    static bool initialized_;
};

} // namespace NAS::Services