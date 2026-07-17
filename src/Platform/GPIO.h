#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : GPIO.h
 *
 * Description:
 * Hardware abstraction layer for ESP32 GPIO.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Platform::GPIO
{

enum class Mode : std::uint8_t
{
    Input,
    InputPullUp,
    InputPullDown,
    Output,
    OutputOpenDrain
};

enum class Level : std::uint8_t
{
    Low = 0,
    High = 1
};

[[nodiscard]]
NAS::Core::Result Initialize() noexcept;

[[nodiscard]]
NAS::Core::Result Configure(std::uint8_t pin,
                            Mode mode) noexcept;

[[nodiscard]]
NAS::Core::Result Write(std::uint8_t pin,
                        Level level) noexcept;

[[nodiscard]]
NAS::Core::Result Toggle(std::uint8_t pin) noexcept;

[[nodiscard]]
NAS::Core::Result Read(std::uint8_t pin,
                       Level& level) noexcept;

} // namespace NAS::Platform::GPIO