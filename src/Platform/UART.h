#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : UART.h
 *
 * Description:
 * Hardware abstraction layer for ESP32 UART peripherals.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Platform::UART
{

[[nodiscard]]
NAS::Core::Result Initialize(std::uint8_t txPin,
                             std::uint8_t rxPin,
                             std::uint32_t baudRate = 115200U) noexcept;

[[nodiscard]]
std::size_t Available() noexcept;

[[nodiscard]]
NAS::Core::Result Write(const std::uint8_t* buffer,
                        std::size_t length) noexcept;

[[nodiscard]]
NAS::Core::Result Read(std::uint8_t* buffer,
                       std::size_t length) noexcept;

} // namespace NAS::Platform::UART
