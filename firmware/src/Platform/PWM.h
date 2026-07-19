#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : PWM.h
 *
 * Description:
 * Hardware abstraction layer for ESP32 LEDC PWM.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Platform::PWM
{

[[nodiscard]]
NAS::Core::Result Initialize() noexcept;

[[nodiscard]]
NAS::Core::Result Configure(std::uint8_t channel,
                            std::uint8_t pin,
                            std::uint32_t frequency,
                            std::uint8_t resolution) noexcept;

[[nodiscard]]
NAS::Core::Result SetDutyCycle(std::uint8_t channel,
                               std::uint32_t dutyCycle) noexcept;

[[nodiscard]]
std::uint32_t GetDutyCycle(std::uint8_t channel) noexcept;

} // namespace NAS::Platform::PWM