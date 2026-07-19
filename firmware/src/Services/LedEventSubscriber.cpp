/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : LedEventSubscriber.cpp
 *
 ******************************************************************************/

#include "LedEventSubscriber.h"

#include "../Core/Event.h"
#include "../Core/EventBus.h"
#include "../Objects/SystemLedState.h"
#include "LedManager.h"

using namespace NAS::Core;

namespace NAS::Services
{

bool LedEventSubscriber::initialized_ = false;

Result LedEventSubscriber::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    EventBus::Subscribe(EventType::SystemStartup, [](const Event&) {
        LedManager::SetPowerLed(NAS::Objects::PowerLedState::Booting);
        LedManager::PlayAnimation(LedEffects::AnimationType::Boot);
    });

    EventBus::Subscribe(EventType::SystemReady, [](const Event&) {
        LedManager::SetPowerLed(NAS::Objects::PowerLedState::Ready);
        LedManager::StopAnimation();
    });

    EventBus::Subscribe(EventType::SystemShutdown, [](const Event&) {
        LedManager::SetPowerLed(NAS::Objects::PowerLedState::Shutdown);
        LedManager::PlayAnimation(LedEffects::AnimationType::Shutdown);
    });

    EventBus::Subscribe(EventType::DriveStateChanged, [](const Event& e) {
        auto driveId = e.ObjectId();
        auto driveState = static_cast<NAS::Objects::DriveLedState>(e.Value());
        LedManager::SetDriveLed(static_cast<uint8_t>(driveId), driveState);
    });

    EventBus::Subscribe(EventType::TemperatureWarning, [](const Event&) {
        LedManager::SetTemperatureLed(NAS::Objects::TemperatureLedState::Warm);
    });

    EventBus::Subscribe(EventType::TemperatureCritical, [](const Event&) {
        LedManager::SetTemperatureLed(NAS::Objects::TemperatureLedState::Critical);
        LedManager::PlayAnimation(LedEffects::AnimationType::Error);
    });

    EventBus::Subscribe(EventType::UsbConnected, [](const Event&) {
        LedManager::SetUsbLed(NAS::Objects::UsbLedState::Connected);
    });

    EventBus::Subscribe(EventType::UsbDisconnected, [](const Event&) {
        LedManager::SetUsbLed(NAS::Objects::UsbLedState::Disconnected);
    });

    EventBus::Subscribe(EventType::Error, [](const Event&) {
        LedManager::SetHealthLed(NAS::Objects::HealthLedState::Critical);
        LedManager::PlayAnimation(LedEffects::AnimationType::Error);
    });

    EventBus::Subscribe(EventType::Warning, [](const Event&) {
        LedManager::SetHealthLed(NAS::Objects::HealthLedState::Warning);
    });

    initialized_ = true;
    return Result::Ok();
}

} // namespace NAS::Services
