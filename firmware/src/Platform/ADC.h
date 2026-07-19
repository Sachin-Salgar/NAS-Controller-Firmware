#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : ADC.h
 *
 * Description:
 * Hardware abstraction layer for ESP32 ADC inputs.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Platform::ADC
{

[[nodiscard]]
NAS::Core::Result Initialize() noexcept;

[[nodiscard]]
NAS::Core::Result Configure(std::uint8_t pin) noexcept;

[[nodiscard]]
std::uint16_t ReadRaw(std::uint8_t pin) noexcept;

[[nodiscard]]
std::uint16_t ReadMillivolts(std::uint8_t pin) noexcept;

} // namespace NAS::Platform::ADC
