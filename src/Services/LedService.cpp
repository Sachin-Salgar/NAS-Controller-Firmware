/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : LedService.cpp
 *
 ******************************************************************************/

#include "LedService.h"
#include "../Drivers/LEDDriver.h"

using namespace NAS::Core;

namespace NAS::Services
{

bool LedService::initialized_ = false;

NAS::Objects::Led LedService::driveLeds_[DriveLedCount];

Result LedService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    for (std::uint8_t i = 0U; i < DriveLedCount; ++i)
    {
        auto result = driveLeds_[i].Initialize(i);

        if (!result)
        {
            return result;
        }
    }

    initialized_ = true;

    return Result::Ok();
}

Result LedService::SetDriveState(
    std::uint8_t driveId,
    NAS::Objects::DriveLedState state) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (driveId >= DriveLedCount)
    {
        return Result(ResultCode::InvalidArgument);
    }

    return driveLeds_[driveId].SetState(state);
}

Result LedService::Refresh() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Drivers::LEDDriver::Show();
}

Result LedService::ClearAll() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    for (std::uint8_t i = 0U; i < DriveLedCount; ++i)
    {
        auto result = driveLeds_[i].SetState(
            NAS::Objects::DriveLedState::Off);

        if (!result)
        {
            return result;
        }
    }

    return NAS::Drivers::LEDDriver::Show();
}

} // namespace NAS::Services