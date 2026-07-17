#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : LedService.h
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"
#include "../Objects/Led.h"
#include "../Config/HardwareConfig.h"

namespace NAS::Services
{

class LedService
{
public:

    static constexpr std::uint16_t LedCount = NAS::Config::Hardware::LedCount;

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetColor(
        std::uint16_t ledId,
        std::uint8_t red,
        std::uint8_t green,
        std::uint8_t blue) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetMode(
        std::uint16_t ledId,
        NAS::Objects::LedMode mode) noexcept;

    [[nodiscard]]
    static NAS::Core::Result TurnOff(
        std::uint16_t ledId) noexcept;

    [[nodiscard]]
    static NAS::Objects::Led&
    GetLed(std::uint16_t ledId) noexcept;

private:

    static NAS::Objects::Led leds_[LedCount];

    static bool initialized_;
};

} // namespace NAS::Services
