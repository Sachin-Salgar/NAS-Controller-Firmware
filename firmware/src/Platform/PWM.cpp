/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : PWM.cpp
 *
 ******************************************************************************/

#include "PWM.h"

#include <Arduino.h>

#include <array>

using namespace NAS::Core;

namespace NAS::Platform::PWM
{
namespace
{
constexpr std::uint8_t MaxChannels = 16U;
std::array<std::uint32_t, MaxChannels> dutyCycle{};
bool initialized = false;
}

Result Initialize() noexcept
{
    if (initialized)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    dutyCycle.fill(0U);
    initialized = true;
    return Result::Ok();
}

Result Configure(std::uint8_t channel,
                 std::uint8_t pin,
                 std::uint32_t frequency,
                 std::uint8_t resolution) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (channel >= MaxChannels)
    {
        return Result(ResultCode::InvalidArgument);
    }

    ledcSetup(channel, frequency, resolution);
    ledcAttachPin(pin, channel);
    dutyCycle[channel] = 0U;

    return Result::Ok();
}

Result SetDutyCycle(std::uint8_t channel,
                    std::uint32_t duty) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (channel >= MaxChannels)
    {
        return Result(ResultCode::InvalidArgument);
    }

    dutyCycle[channel] = duty;
    ledcWrite(channel, duty);

    return Result::Ok();
}

std::uint32_t GetDutyCycle(std::uint8_t channel) noexcept
{
    if (channel >= MaxChannels)
    {
        return 0U;
    }

    return dutyCycle[channel];
}

} // namespace NAS::Platform::PWM