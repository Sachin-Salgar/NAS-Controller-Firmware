#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Config
 * File    : HardwareConfig.h
 *
 ******************************************************************************/

#include <cstdint>

namespace NAS::Config::Hardware
{

inline constexpr std::uint8_t RelayCount = 2U;

inline constexpr std::uint8_t FanCount = 5U;

inline constexpr std::uint8_t TemperatureSensorCount = 5U;

inline constexpr std::uint8_t LedCount = 60U;

inline constexpr std::uint8_t DriveCount = 10U;

inline constexpr std::uint32_t FanPwmFrequency = 25000U;

inline constexpr std::uint8_t FanPwmResolution = 8U;

} // namespace NAS::Config::Hardware