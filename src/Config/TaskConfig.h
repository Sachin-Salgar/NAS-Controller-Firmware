#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Config
 * File    : TaskConfig.h
 *
 ******************************************************************************/

#include <cstdint>

namespace NAS::Config::Tasks
{

inline constexpr std::uint32_t UsbTaskIntervalMs = 5U;

inline constexpr std::uint32_t ProtocolTaskIntervalMs = 5U;


inline constexpr std::uint32_t TemperatureTaskIntervalMs = 1000U;


inline constexpr std::uint32_t HealthTaskIntervalMs = 1000U;

inline constexpr std::uint32_t StatisticsTaskIntervalMs = 1000U;

} // namespace NAS::Config::Tasks