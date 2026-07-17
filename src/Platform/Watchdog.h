#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : Watchdog.h
 *
 * Description:
 * Hardware abstraction layer for ESP32 watchdog control.
 *
 ******************************************************************************/

#include "../Core/Result.h"

namespace NAS::Platform::Watchdog
{

[[nodiscard]]
NAS::Core::Result Initialize() noexcept;

[[nodiscard]]
NAS::Core::Result Feed() noexcept;

} // namespace NAS::Platform::Watchdog
