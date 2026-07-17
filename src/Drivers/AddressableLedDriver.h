#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : AddressableLedDriver.h
 *
 * Description:
 * WS2812B Addressable LED Driver.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Drivers
{

class AddressableLedDriver
{
public:

    struct Color
    {
        std::uint8_t red;
        std::uint8_t green;
        std::uint8_t blue;
    };

    [[nodiscard]]
    static NAS::Core::Result Initialize(std::uint8_t gpioPin,
                                        std::uint16_t ledCount) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetColor(std::uint16_t index,
                                      Color color) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetBrightness(std::uint8_t brightness) noexcept;

    [[nodiscard]]
    static NAS::Core::Result Clear() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Show() noexcept;

    [[nodiscard]]
    static std::uint16_t LedCount() noexcept;

private:

    static constexpr std::uint16_t MaxLeds = 25U;

    static Color leds_[MaxLeds];

    static std::uint16_t ledCount_;

    static std::uint8_t brightness_;

    static bool initialized_;
};

} // namespace NAS::Drivers