#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : Flash.h
 *
 * Description:
 * Hardware abstraction layer for ESP32 flash access.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Platform::Flash
{

[[nodiscard]]
NAS::Core::Result Initialize() noexcept;

[[nodiscard]]
NAS::Core::Result Read(std::uint32_t address,
                       std::uint8_t* buffer,
                       std::size_t length) noexcept;

[[nodiscard]]
NAS::Core::Result Write(std::uint32_t address,
                        const std::uint8_t* buffer,
                        std::size_t length) noexcept;

[[nodiscard]]
NAS::Core::Result Erase(std::uint32_t address,
                        std::size_t length) noexcept;

} // namespace NAS::Platform::Flash
