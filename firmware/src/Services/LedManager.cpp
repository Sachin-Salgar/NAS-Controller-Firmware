/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : LedManager.cpp
 *
 ******************************************************************************/

#include "LedManager.h"

#include "../Config/LedColors.h"
#include "../Config/LedMap.h"
#include "LedService.h"

using namespace NAS::Core;
using namespace NAS::Config::LedColors;
using namespace NAS::Config::LedMap;

namespace NAS::Services
{

NAS::Objects::SystemLed LedManager::powerLed_{};
NAS::Objects::SystemLed LedManager::healthLed_{};
NAS::Objects::SystemLed LedManager::usbLed_{};
NAS::Objects::SystemLed LedManager::temperatureLed_{};

NAS::Objects::PowerLedState LedManager::currentPowerState_ = 
    NAS::Objects::PowerLedState::Off;
NAS::Objects::HealthLedState LedManager::currentHealthState_ = 
    NAS::Objects::HealthLedState::Healthy;
NAS::Objects::UsbLedState LedManager::currentUsbState_ = 
    NAS::Objects::UsbLedState::Disconnected;
NAS::Objects::TemperatureLedState LedManager::currentTempState_ = 
    NAS::Objects::TemperatureLedState::Normal;

bool LedManager::initialized_ = false;

Result LedManager::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    auto result = LedService::Initialize();
    if (!result)
    {
        return result;
    }

    result = LedEffects::Initialize();
    if (!result)
    {
        return result;
    }

    result = powerLed_.Initialize(PowerLedIndex);
    if (!result)
    {
        return result;
    }

    result = healthLed_.Initialize(HealthLedIndex);
    if (!result)
    {
        return result;
    }

    result = usbLed_.Initialize(UsbLedIndex);
    if (!result)
    {
        return result;
    }

    result = temperatureLed_.Initialize(TemperatureLedIndex);
    if (!result)
    {
        return result;
    }

    initialized_ = true;
    return Result::Ok();
}

Result LedManager::Update(std::uint32_t deltaMs) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return LedEffects::Update(deltaMs);
}

Result LedManager::SetPowerLed(
    NAS::Objects::PowerLedState state) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (currentPowerState_ == state)
    {
        return Result::Ok();
    }

    currentPowerState_ = state;
    auto color = PowerStateToColor(state);

    return powerLed_.SetColor(color);
}

Result LedManager::SetHealthLed(
    NAS::Objects::HealthLedState state) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (currentHealthState_ == state)
    {
        return Result::Ok();
    }

    currentHealthState_ = state;
    auto color = HealthStateToColor(state);

    return healthLed_.SetColor(color);
}

Result LedManager::SetUsbLed(
    NAS::Objects::UsbLedState state) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (currentUsbState_ == state)
    {
        return Result::Ok();
    }

    currentUsbState_ = state;
    auto color = UsbStateToColor(state);

    return usbLed_.SetColor(color);
}

Result LedManager::SetTemperatureLed(
    NAS::Objects::TemperatureLedState state) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (currentTempState_ == state)
    {
        return Result::Ok();
    }

    currentTempState_ = state;
    auto color = TemperatureStateToColor(state);

    return temperatureLed_.SetColor(color);
}

Result LedManager::SetDriveLed(
    std::uint8_t driveId,
    NAS::Objects::DriveLedState state) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return LedService::SetDriveState(driveId, state);
}

Result LedManager::PlayAnimation(
    LedEffects::AnimationType animationType) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return LedEffects::PlayAnimation(animationType);
}

Result LedManager::StopAnimation() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return LedEffects::StopAnimation();
}

Result LedManager::RefreshAll() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return LedService::Refresh();
}

NAS::Config::LedColors::Color LedManager::PowerStateToColor(
    NAS::Objects::PowerLedState state) noexcept
{
    switch (state)
    {
        case NAS::Objects::PowerLedState::Off:
            return PowerOff;

        case NAS::Objects::PowerLedState::Booting:
            return PowerBooting;

        case NAS::Objects::PowerLedState::Ready:
            return PowerReady;

        case NAS::Objects::PowerLedState::Shutdown:
            return PowerShutdown;

        case NAS::Objects::PowerLedState::Error:
            return PowerError;

        default:
            return PowerOff;
    }
}

NAS::Config::LedColors::Color LedManager::HealthStateToColor(
    NAS::Objects::HealthLedState state) noexcept
{
    switch (state)
    {
        case NAS::Objects::HealthLedState::Healthy:
            return HealthHealthy;

        case NAS::Objects::HealthLedState::Warning:
            return HealthWarning;

        case NAS::Objects::HealthLedState::Critical:
            return HealthCritical;

        default:
            return HealthHealthy;
    }
}

NAS::Config::LedColors::Color LedManager::UsbStateToColor(
    NAS::Objects::UsbLedState state) noexcept
{
    switch (state)
    {
        case NAS::Objects::UsbLedState::Disconnected:
            return UsbDisconnected;

        case NAS::Objects::UsbLedState::Connected:
            return UsbConnected;

        case NAS::Objects::UsbLedState::Transferring:
            return UsbTransferring;

        default:
            return UsbDisconnected;
    }
}

NAS::Config::LedColors::Color LedManager::TemperatureStateToColor(
    NAS::Objects::TemperatureLedState state) noexcept
{
    switch (state)
    {
        case NAS::Objects::TemperatureLedState::Normal:
            return TemperatureNormal;

        case NAS::Objects::TemperatureLedState::Warm:
            return TemperatureWarm;

        case NAS::Objects::TemperatureLedState::Hot:
            return TemperatureHot;

        case NAS::Objects::TemperatureLedState::Critical:
            return TemperatureCritical;

        default:
            return TemperatureNormal;
    }
}

} // namespace NAS::Services
