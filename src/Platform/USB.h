#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : USB.h
 *
 * Description:
 * USB CDC abstraction layer.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Platform::USB
{

[[nodiscard]]
NAS::Core::Result Initialize(std::uint32_t baudRate = 115200U) noexcept;

[[nodiscard]]
bool IsConnected() noexcept;

[[nodiscard]]
std::size_t Available() noexcept;

[[nodiscard]]
NAS::Core::Result Read(char& character) noexcept;

[[nodiscard]]
NAS::Core::Result ReadLine(char* buffer,
                           std::size_t length) noexcept;

[[nodiscard]]
NAS::Core::Result Write(const char* text) noexcept;

[[nodiscard]]
NAS::Core::Result WriteLine(const char* text) noexcept;

[[nodiscard]]
NAS::Core::Result Flush() noexcept;

} // namespace NAS::Platform::USB