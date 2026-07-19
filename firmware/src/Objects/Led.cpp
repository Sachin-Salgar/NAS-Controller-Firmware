/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Led.cpp
 *
 ******************************************************************************/

#include "Led.h"

#include "../Config/LedColors.h"
#include "../Drivers/LEDDriver.h"

using namespace NAS::Core;

namespace NAS::Objects
{

Result Led::Initialize(std::uint16_t ledIndex) noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    ledIndex_ = ledIndex;
    state_ = DriveLedState::Off;

    initialized_ = true;

    return Result::Ok();
}

Result Led::SetState(DriveLedState state) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    state_ = state;

    const Color color = StateToColor(state_);

    return NAS::Drivers::LEDDriver::SetPixel(
        ledIndex_,
        {
            color.red,
            color.green,
            color.blue
        });
}

Result Led::Refresh() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Drivers::LEDDriver::Show();
}

std::uint16_t Led::GetIndex() const noexcept
{
    return ledIndex_;
}

DriveLedState Led::GetState() const noexcept
{
    return state_;
}

Led::Color Led::StateToColor(DriveLedState state) noexcept
{
    using namespace NAS::Config::LedColors;

    switch (state)
    {
        case DriveLedState::Off:
            return DriveOff;

        case DriveLedState::Idle:
            return DriveIdle;

        case DriveLedState::Reading:
            return DriveReading;

        case DriveLedState::Writing:
            return DriveWriting;

        case DriveLedState::Standby:
            return DriveStandby;

        case DriveLedState::Error:
            return DriveError;

        case DriveLedState::Missing:
            return DriveMissing;

        case DriveLedState::Rebuilding:
            return DriveRebuilding;

        default:
            return DriveOff;
    }
}

} // namespace NAS::Objects
