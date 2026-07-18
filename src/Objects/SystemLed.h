#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : SystemLed.h
 *
 * Description:
 * Logical System Status LED Object.
 * Represents individual system status LEDs (Power, Health, USB, Temperature).
 *
 ******************************************************************************/

#include <cstdint>

#include "../Config/LedColors.h"
#include "../Core/Result.h"

namespace NAS::Objects
{

class SystemLed
{
public:

    SystemLed() = default;

    [[nodiscard]]
    NAS::Core::Result Initialize(
        std::uint16_t ledIndex) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetColor(
        const NAS::Config::LedColors::Color& color) noexcept;

    [[nodiscard]]
    NAS::Core::Result Refresh() noexcept;

    [[nodiscard]]
    std::uint16_t GetIndex() const noexcept;

    [[nodiscard]]
    NAS::Config::LedColors::Color GetColor() const noexcept;

private:

    using Color = NAS::Config::LedColors::Color;

    bool initialized_{false};

    std::uint16_t ledIndex_{0U};

    Color currentColor_{0, 0, 0};

};

} // namespace NAS::Objects
