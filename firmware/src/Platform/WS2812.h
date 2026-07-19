#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : WS2812.h
 *
 * Description:
 * Platform abstraction for WS2812 LED strip.
 *
 ******************************************************************************/

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
NAS::Core::Result Initialize() noexcept;

[[nodiscard]]
NAS::Core::Result SetPixel(
    std::uint16_t index,
    const Color& color) noexcept;

[[nodiscard]]
NAS::Core::Result Fill(
    const Color& color) noexcept;

[[nodiscard]]
NAS::Core::Result Clear() noexcept;

[[nodiscard]]
NAS::Core::Result SetBrightness(
    std::uint8_t brightness) noexcept;

[[nodiscard]]
NAS::Core::Result Show() noexcept;

[[nodiscard]]
bool IsInitialized() noexcept;

[[nodiscard]]
std::uint16_t GetLedCount() noexcept;

} // namespace NAS::Platform::WS2812