#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : SPI.h
 *
 * Description:
 * Hardware abstraction layer for ESP32 SPI peripherals.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Platform::SPI
{

[[nodiscard]]
NAS::Core::Result Initialize(std::uint8_t sckPin,
                             std::uint8_t misoPin,
                             std::uint8_t mosiPin,
                             std::uint8_t csPin) noexcept;

[[nodiscard]]
NAS::Core::Result Transfer(const std::uint8_t* txBuffer,
                           std::uint8_t* rxBuffer,
                           std::size_t length) noexcept;

} // namespace NAS::Platform::SPI
