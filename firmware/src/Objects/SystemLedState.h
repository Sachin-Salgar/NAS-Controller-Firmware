#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : SystemLedState.h
 *
 * Description:
 * State enums for system status LEDs.
 * Defines states for Power, Health, USB, and Temperature LEDs.
 *
 ******************************************************************************/

#include <cstdint>

namespace NAS::Objects
{

enum class PowerLedState : std::uint8_t
{
    Off = 0,
    Booting,
    Ready,
    Shutdown,
    Error
};

enum class HealthLedState : std::uint8_t
{
    Healthy = 0,
    Warning,
    Critical
};

enum class UsbLedState : std::uint8_t
{
    Disconnected = 0,
    Connected,
    Transferring
};

enum class TemperatureLedState : std::uint8_t
{
    Normal = 0,
    Warm,
    Hot,
    Critical
};

} // namespace NAS::Objects
