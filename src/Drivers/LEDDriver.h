#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : LEDDriver.h
 *
 * Description:
 * WS2812 LED Driver.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"
#include "../Platform/WS2812.h"

namespace NAS::Drivers
{

class LEDDriver
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetPixel(
        std::uint16_t index,
        const NAS::Platform::WS2812::Color& color) noexcept;

    [[nodiscard]]
    static NAS::Core::Result Fill(
        const NAS::Platform::WS2812::Color& color) noexcept;

    [[nodiscard]]
    static NAS::Core::Result Clear() noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetBrightness(
        std::uint8_t brightness) noexcept;

    [[nodiscard]]
    static NAS::Core::Result Show() noexcept;

private:

    static bool initialized_;

};

} // namespace NAS::Drivers