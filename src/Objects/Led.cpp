/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Led.cpp
 *
 ******************************************************************************/

#include "Led.h"

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
    switch (state)
    {
        case DriveLedState::Off:
            return {0U, 0U, 0U};

        case DriveLedState::Idle:
            // Blue
            return {0U, 0U, 255U};

        case DriveLedState::Reading:
            // Green
            return {0U, 255U, 0U};

        case DriveLedState::Writing:
            // Yellow
            return {255U, 255U, 0U};

        case DriveLedState::Error:
            // Red
            return {255U, 0U, 0U};

        case DriveLedState::Missing:
            // Purple
            return {128U, 0U, 128U};

        case DriveLedState::Rebuilding:
            // Cyan
            return {0U, 255U, 255U};

        default:
            return {0U, 0U, 0U};
    }
}

} // namespace NAS::Objects