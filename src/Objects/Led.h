#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Led.h
 *
 * Description:
 * Addressable LED Object.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"
#include "../Drivers/AddressableLedDriver.h"

namespace NAS::Objects
{

enum class LedMode : std::uint8_t
{
    Off = 0,
    Solid,
    Blink,
    Pulse
};

class Led
{
public:

    Led() = default;

    [[nodiscard]]
    NAS::Core::Result Initialize(std::uint16_t index) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetColor(std::uint8_t red,
                               std::uint8_t green,
                               std::uint8_t blue) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetMode(LedMode mode) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetBrightness(std::uint8_t brightness) noexcept;

    [[nodiscard]]
    NAS::Core::Result TurnOff() noexcept;

    [[nodiscard]]
    std::uint16_t GetIndex() const noexcept;

    [[nodiscard]]
    LedMode GetMode() const noexcept;

private:

    std::uint16_t index_{0U};

    std::uint8_t red_{0U};
    std::uint8_t green_{0U};
    std::uint8_t blue_{0U};

    std::uint8_t brightness_{255U};

    LedMode mode_{LedMode::Off};

    bool initialized_{false};
};

} // namespace NAS::Objects