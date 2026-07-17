#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : WatchdogDriver.h
 *
 * Description:
 * Watchdog driver - abstraction for system watchdog.
 *
 ******************************************************************************/

#include "../Core/Result.h"

namespace NAS::Drivers
{

class WatchdogDriver
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Feed() noexcept;

private:

    static bool initialized_;
};

} // namespace NAS::Drivers
