#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Config
 * File    : Features.h
 *
 ******************************************************************************/

namespace NAS::Config::Features
{

inline constexpr bool EnableUsb = true;

inline constexpr bool EnableTemperatureMonitoring = true;

inline constexpr bool EnableRelayControl = true;

inline constexpr bool EnableFanControl = true;

inline constexpr bool EnableLedControl = true;

inline constexpr bool EnableDriveMonitoring = true;

inline constexpr bool EnableStatistics = true;

inline constexpr bool EnablePersistentConfiguration = true;

inline constexpr bool EnableWatchdog = true;

inline constexpr bool EnableDebugOutput = false;

} // namespace NAS::Config::Features