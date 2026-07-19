/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Fan.cpp
 *
 ******************************************************************************/

#include "Fan.h"

#include "../Drivers/PWMFanDriver.h"

using namespace NAS::Core;

namespace NAS::Objects
{

Result Fan::Initialize(std::uint8_t channel,
                       std::uint8_t pin,
                       std::uint32_t frequency,
                       std::uint8_t resolution) noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    channel_ = channel;
    speed_ = 0U;
    mode_ = FanMode::Automatic;

    auto result =
        NAS::Drivers::PWMFanDriver::Configure(
            channel_,
            pin,
            frequency,
            resolution);

    if (!result.IsSuccess())
    {
        return result;
    }

    initialized_ = true;

    return Result::Ok();
}

Result Fan::SetSpeed(std::uint8_t percent) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    auto result =
        NAS::Drivers::PWMFanDriver::SetSpeed(
            channel_,
            percent);

    if (result.IsSuccess())
    {
        speed_ = percent;
    }

    return result;
}

Result Fan::SetMode(FanMode mode) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    mode_ = mode;

    return Result::Ok();
}

std::uint8_t Fan::GetSpeed() const noexcept
{
    return speed_;
}

FanMode Fan::GetMode() const noexcept
{
    return mode_;
}

std::uint8_t Fan::GetChannel() const noexcept
{
    return channel_;
}

} // namespace NAS::Objects