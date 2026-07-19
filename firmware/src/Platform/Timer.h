#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : Timer.h
 *
 * Description:
 * Platform timing abstraction.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Platform::Timer
{

[[nodiscard]]
NAS::Core::Result Initialize() noexcept;

[[nodiscard]]
std::uint32_t Milliseconds() noexcept;

[[nodiscard]]
std::uint64_t Microseconds() noexcept;

void DelayMilliseconds(std::uint32_t milliseconds) noexcept;

void DelayMicroseconds(std::uint32_t microseconds) noexcept;

} // namespace NAS::Platform::Timer