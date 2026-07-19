#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : LedManager.h
 *
 * Description:
 * Centralized LED manager coordinating system status LEDs, drive LEDs,
 * and animation effects. Single interface for all LED control.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Objects/SystemLed.h"
#include "../Objects/SystemLedState.h"
#include "../Objects/Led.h"
#include "../Core/Result.h"
#include "LedEffects.h"

namespace NAS::Services
{

class LedManager
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Update(
        std::uint32_t deltaMs) noexcept;

    // System Status LED Control
    [[nodiscard]]
    static NAS::Core::Result SetPowerLed(
        NAS::Objects::PowerLedState state) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetHealthLed(
        NAS::Objects::HealthLedState state) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetUsbLed(
        NAS::Objects::UsbLedState state) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetTemperatureLed(
        NAS::Objects::TemperatureLedState state) noexcept;

    // Drive LED Control
    [[nodiscard]]
    static NAS::Core::Result SetDriveLed(
        std::uint8_t driveId,
        NAS::Objects::DriveLedState state) noexcept;

    // Animation Control
    [[nodiscard]]
    static NAS::Core::Result PlayAnimation(
        LedEffects::AnimationType animationType) noexcept;

    [[nodiscard]]
    static NAS::Core::Result StopAnimation() noexcept;

    // Utility
    [[nodiscard]]
    static NAS::Core::Result RefreshAll() noexcept;

private:

    static NAS::Objects::SystemLed powerLed_;
    static NAS::Objects::SystemLed healthLed_;
    static NAS::Objects::SystemLed usbLed_;
    static NAS::Objects::SystemLed temperatureLed_;

    static NAS::Objects::PowerLedState currentPowerState_;
    static NAS::Objects::HealthLedState currentHealthState_;
    static NAS::Objects::UsbLedState currentUsbState_;
    static NAS::Objects::TemperatureLedState currentTempState_;

    static bool initialized_;

    [[nodiscard]]
    static NAS::Config::LedColors::Color PowerStateToColor(
        NAS::Objects::PowerLedState state) noexcept;

    [[nodiscard]]
    static NAS::Config::LedColors::Color HealthStateToColor(
        NAS::Objects::HealthLedState state) noexcept;

    [[nodiscard]]
    static NAS::Config::LedColors::Color UsbStateToColor(
        NAS::Objects::UsbLedState state) noexcept;

    [[nodiscard]]
    static NAS::Config::LedColors::Color TemperatureStateToColor(
        NAS::Objects::TemperatureLedState state) noexcept;

};

} // namespace NAS::Services
