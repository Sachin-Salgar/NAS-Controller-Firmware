#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : Restart.h
 *
 * Description:
 * Hardware abstraction layer for ESP32 restart operations.
 *
 ******************************************************************************/

#include "../Core/Result.h"

namespace NAS::Platform::Restart
{

[[nodiscard]]
NAS::Core::Result Soft() noexcept;

[[nodiscard]]
NAS::Core::Result Hard() noexcept;

} // namespace NAS::Platform::Restart
