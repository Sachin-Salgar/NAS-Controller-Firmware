#pragma once

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Platform::WS2812
{

struct Color
{
    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;
};

[[nodiscard]]
Core::Result Initialize(
    std::uint8_t gpioPin,
    std::uint16_t ledCount) noexcept;

[[nodiscard]]
Core::Result SetPixel(
    std::uint16_t index,
    const Color& color) noexcept;

[[nodiscard]]
Core::Result Fill(
    const Color& color) noexcept;

[[nodiscard]]
Core::Result Clear() noexcept;

[[nodiscard]]
Core::Result SetBrightness(
    std::uint8_t brightness) noexcept;

[[nodiscard]]
Core::Result Show() noexcept;

[[nodiscard]]
bool IsInitialized() noexcept;

[[nodiscard]]
std::uint16_t GetLedCount() noexcept;

}