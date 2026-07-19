#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : OneWire.h
 *
 * Description:
 * OneWire bus abstraction layer for DS18B20 sensors.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Platform::OneWire
{

[[nodiscard]]
NAS::Core::Result Initialize(std::uint8_t pin) noexcept;

[[nodiscard]]
NAS::Core::Result Reset() noexcept;

[[nodiscard]]
NAS::Core::Result WriteByte(std::uint8_t data) noexcept;

[[nodiscard]]
NAS::Core::Result ReadByte(std::uint8_t& data) noexcept;

[[nodiscard]]
NAS::Core::Result WriteBuffer(const std::uint8_t* buffer,
                              std::size_t length) noexcept;

[[nodiscard]]
NAS::Core::Result ReadBuffer(std::uint8_t* buffer,
                             std::size_t length) noexcept;

} // namespace NAS::Platform::OneWire