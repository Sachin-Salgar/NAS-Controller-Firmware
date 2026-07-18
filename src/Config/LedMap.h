#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Config
 * File    : LedMap.h
 *
 * Description:
 * Centralized LED index mapping for 60 WS2812B LED strip.
 * All LED indices defined here to avoid magic numbers throughout codebase.
 *
 * LED Layout (60 total):
 * - Status Region (LED0-3):   System status indicators
 * - Drive Region (LED4-13):   Drive status (supports up to 10 drives)
 * - Animation Region (LED14-59): Available for animations and effects
 *
 ******************************************************************************/

#include <cstdint>

#include "HardwareConfig.h"

namespace NAS::Config::LedMap
{

// Status Region - System Status Indicators
inline constexpr std::uint16_t PowerLedIndex = 0U;
inline constexpr std::uint16_t HealthLedIndex = 1U;
inline constexpr std::uint16_t UsbLedIndex = 2U;
inline constexpr std::uint16_t TemperatureLedIndex = 3U;

inline constexpr std::uint16_t StatusRegionStart = 0U;
inline constexpr std::uint16_t StatusRegionEnd = 3U;
inline constexpr std::uint16_t StatusRegionCount = 4U;

// Drive Region - Drive Status LEDs
inline constexpr std::uint16_t DriveRegionStart = 4U;
inline constexpr std::uint16_t MaxDriveCount = Hardware::DriveCount;

inline constexpr std::uint16_t DriveRegionEnd = 
    DriveRegionStart + MaxDriveCount - 1U;

inline constexpr std::uint16_t DriveRegionCount = MaxDriveCount;

// Helper to get LED index for a specific drive
inline constexpr std::uint16_t GetDriveLedIndex(std::uint8_t driveId) noexcept
{
    return DriveRegionStart + driveId;
}

// Animation Region - Available for effects and animations
inline constexpr std::uint16_t AnimationRegionStart = 
    DriveRegionEnd + 1U;

inline constexpr std::uint16_t AnimationRegionEnd = 59U;

inline constexpr std::uint16_t AnimationRegionCount = 
    AnimationRegionEnd - AnimationRegionStart + 1U;

// Total LED strip
inline constexpr std::uint16_t TotalLedCount = Hardware::LedCount;

// Validation
static_assert(TotalLedCount == 60U, 
    "LED map assumes 60-LED strip");
static_assert(AnimationRegionEnd < TotalLedCount,
    "Animation region exceeds LED strip capacity");
static_assert(DriveRegionEnd < AnimationRegionStart,
    "Drive region overlaps with animation region");

} // namespace NAS::Config::LedMap
