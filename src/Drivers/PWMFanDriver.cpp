/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : PWMFanDriver.cpp
 *
 ******************************************************************************/

#include "PWMFanDriver.h"

#include "../Platform/PWM.h"

using namespace NAS::Core;

namespace NAS::Drivers
{

PWMFanDriver::Fan PWMFanDriver::fans_[MaxChannels] = {};

bool PWMFanDriver::initialized_ = false;

Result PWMFanDriver::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    for (auto& fan : fans_)
    {
        fan.speedPercent = 0U;
        fan.configured = false;
    }

    initialized_ = true;

    return Result::Ok();
}

Result PWMFanDriver::Configure(std::uint8_t channel,
                               std::uint8_t pin,
                               std::uint32_t frequency,
                               std::uint8_t resolution) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if ((channel == 0U) || (channel > MaxChannels))
    {
        return Result(ResultCode::InvalidArgument);
    }

    fans_[channel - 1U].configured = true;
    fans_[channel - 1U].speedPercent = 0U;

    return NAS::Platform::PWM::Configure(
        channel - 1U,
        pin,
        frequency,
        resolution);
}

Result PWMFanDriver::SetSpeed(std::uint8_t channel,
                              std::uint8_t percent) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if ((channel == 0U) || (channel > MaxChannels))
    {
        return Result(ResultCode::InvalidArgument);
    }

    if (percent > 100U)
    {
        return Result(ResultCode::InvalidArgument);
    }

    auto& fan = fans_[channel - 1U];

    if (!fan.configured)
    {
        return Result(ResultCode::NotInitialized);
    }

    fan.speedPercent = percent;

    constexpr std::uint32_t MaxDuty = 255U;

    const std::uint32_t duty =
        (static_cast<std::uint32_t>(percent) * MaxDuty) / 100U;

    return NAS::Platform::PWM::SetDutyCycle(
        channel - 1U,
        duty);
}

std::uint8_t PWMFanDriver::GetSpeed(std::uint8_t channel) noexcept
{
    if ((channel == 0U) || (channel > MaxChannels))
    {
        return 0U;
    }

    return fans_[channel - 1U].speedPercent;
}

}