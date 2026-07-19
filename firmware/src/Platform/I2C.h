#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : I2C.h
 *
 * Description:
 * Hardware abstraction layer for ESP32 I2C peripherals.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Platform::I2C
{

[[nodiscard]]
NAS::Core::Result Initialize(std::uint8_t sdaPin,
                             std::uint8_t sclPin,
                             std::uint32_t clockSpeed = 100000U) noexcept;

[[nodiscard]]
NAS::Core::Result WriteByte(std::uint8_t deviceAddress,
                            std::uint8_t registerAddress,
                            std::uint8_t value) noexcept;

[[nodiscard]]
NAS::Core::Result ReadByte(std::uint8_t deviceAddress,
                           std::uint8_t registerAddress,
                           std::uint8_t& value) noexcept;

[[nodiscard]]
NAS::Core::Result WriteBuffer(std::uint8_t deviceAddress,
                              const std::uint8_t* buffer,
                              std::size_t length) noexcept;

[[nodiscard]]
NAS::Core::Result ReadBuffer(std::uint8_t deviceAddress,
                             std::uint8_t* buffer,
                             std::size_t length) noexcept;

} // namespace NAS::Platform::I2C
