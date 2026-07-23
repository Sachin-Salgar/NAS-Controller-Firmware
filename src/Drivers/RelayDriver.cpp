/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : RelayDriver.cpp
 *
 ******************************************************************************/

#include "RelayDriver.h"

#include "../Platform/GPIO.h"

using namespace NAS::Core;

namespace NAS::Drivers
{

RelayDriver::Relay RelayDriver::relays_[MaxRelays] = {};

bool RelayDriver::initialized_ = false;

Result RelayDriver::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    for (auto& relay : relays_)
    {
        relay.pin = 0U;
        relay.activeLow = true;
        relay.state = RelayState::Off;
        relay.configured = false;
    }

    initialized_ = true;

    return Result::Ok();
}

Result RelayDriver::Configure(std::uint8_t relayId,
                              std::uint8_t gpioPin,
                              bool activeLow) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if ((relayId == 0U) || (relayId > MaxRelays))
    {
        return Result(ResultCode::InvalidArgument);
    }

    auto result = Platform::GPIO::Configure(
        gpioPin,
        Platform::GPIO::Mode::Output);

    if (!result.IsSuccess())
    {
        return result;
    }

    result = Platform::GPIO::Write(
        gpioPin,
        activeLow ? Platform::GPIO::Level::High
                  : Platform::GPIO::Level::Low);

    if (!result.IsSuccess())
    {
        return result;
    }

    auto& relay = relays_[relayId - 1U];

    relay.pin = gpioPin;
    relay.activeLow = activeLow;
    relay.state = RelayState::Off;
    relay.configured = true;

    return Result::Ok();
}

Result RelayDriver::SetState(std::uint8_t relayId,
                             RelayState state) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if ((relayId == 0U) || (relayId > MaxRelays))
    {
        return Result(ResultCode::InvalidArgument);
    }

    auto& relay = relays_[relayId - 1U];

    if (!relay.configured)
    {
        return Result(ResultCode::NotInitialized);
    }

    relay.state = state;

    Platform::GPIO::Level level;

    if (relay.activeLow)
    {
        level = (state == RelayState::On)
                    ? Platform::GPIO::Level::Low
                   : Platform::GPIO::Level::High;
    }
    else
    {
        level = (state == RelayState::On)
                    ? Platform::GPIO::Level::High
                    : Platform::GPIO::Level::Low;
    }

    return Platform::GPIO::Write(relay.pin, level);
}

Result RelayDriver::Toggle(std::uint8_t relayId) noexcept
{
    if ((relayId == 0U) || (relayId > MaxRelays))
    {
        return Result(ResultCode::InvalidArgument);
    }

    auto& relay = relays_[relayId - 1U];

    return SetState(
        relayId,
        relay.state == RelayState::On
            ? RelayState::Off
            : RelayState::On);
}

RelayState RelayDriver::GetState(std::uint8_t relayId) noexcept
{
    if ((relayId == 0U) || (relayId > MaxRelays))
    {
        return RelayState::Off;
    }

    return relays_[relayId - 1U].state;
}

} // namespace NAS::Drivers
