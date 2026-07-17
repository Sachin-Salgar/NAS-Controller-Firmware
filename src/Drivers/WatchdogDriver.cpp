/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : WatchdogDriver.cpp
 *
 ******************************************************************************/

#include "WatchdogDriver.h"

#include "../Platform/Watchdog.h"

using namespace NAS::Core;

namespace NAS::Drivers
{

bool WatchdogDriver::initialized_ = false;

Result WatchdogDriver::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    auto result = NAS::Platform::Watchdog::Initialize();

    if (!result.IsSuccess())
    {
        return result;
    }

    initialized_ = true;

    return Result::Ok();
}

Result WatchdogDriver::Feed() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Platform::Watchdog::Feed();
}

} // namespace NAS::Drivers
