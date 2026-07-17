/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Relay.cpp
 *
 ******************************************************************************/

#include "Relay.h"

using namespace NAS::Core;

namespace NAS::Objects
{

Result Relay::Initialize(std::uint8_t id,
                         std::uint8_t gpioPin,
                         bool activeLow,
                         bool defaultState) noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    id_ = id;
    defaultState_ = defaultState;
    state_ = defaultState;
    enabled_ = true;

    auto result = NAS::Drivers::RelayDriver::Configure(
        id_,
        gpioPin,
        activeLow);

    if (!result.IsSuccess())
    {
        return result;
    }

    result = defaultState
             ? NAS::Drivers::RelayDriver::SetState(
                   id_,
                   NAS::Drivers::RelayState::On)
             : NAS::Drivers::RelayDriver::SetState(
                   id_,
                   NAS::Drivers::RelayState::Off);

    if (!result.IsSuccess())
    {
        return result;
    }

    initialized_ = true;

    return Result::Ok();
}

Result Relay::TurnOn() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (!enabled_)
    {
        return Result(ResultCode::AccessDenied);
    }

    auto result = NAS::Drivers::RelayDriver::SetState(
        id_,
        NAS::Drivers::RelayState::On);

    if (result.IsSuccess())
    {
        state_ = true;
    }

    return result;
}

Result Relay::TurnOff() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (!enabled_)
    {
        return Result(ResultCode::AccessDenied);
    }

    auto result = NAS::Drivers::RelayDriver::SetState(
        id_,
        NAS::Drivers::RelayState::Off);

    if (result.IsSuccess())
    {
        state_ = false;
    }

    return result;
}

Result Relay::Toggle() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (!enabled_)
    {
        return Result(ResultCode::AccessDenied);
    }

    auto result = NAS::Drivers::RelayDriver::Toggle(id_);

    if (result.IsSuccess())
    {
        state_ = !state_;
    }

    return result;
}

Result Relay::SetEnabled(bool enabled) noexcept
{
    enabled_ = enabled;

    return Result::Ok();
}

bool Relay::IsOn() const noexcept
{
    return state_;
}

bool Relay::IsEnabled() const noexcept
{
    return enabled_;
}

std::uint8_t Relay::GetId() const noexcept
{
    return id_;
}

} // namespace NAS::Objects