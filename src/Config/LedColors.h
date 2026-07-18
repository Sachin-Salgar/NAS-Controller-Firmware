#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Config
 * File    : LedColors.h
 *
 * Description:
 * Centralized LED color definitions for all system states.
 * Single source of truth for all RGB color values used in LED system.
 *
 ******************************************************************************/

#include <cstdint>

namespace NAS::Config::LedColors
{

// Color struct for RGB values
struct Color
{
    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;

    constexpr bool operator==(const Color& other) const noexcept
    {
        return red == other.red && green == other.green && blue == other.blue;
    }

    constexpr bool operator!=(const Color& other) const noexcept
    {
        return !(*this == other);
    }
};

// Power LED States (LED0)
inline constexpr Color PowerOff{0, 0, 0};              // Black (off)
inline constexpr Color PowerBooting{255, 165, 0};      // Orange
inline constexpr Color PowerReady{0, 255, 0};          // Green
inline constexpr Color PowerShutdown{255, 165, 0};     // Orange
inline constexpr Color PowerError{255, 0, 0};          // Red

// Health LED States (LED1)
inline constexpr Color HealthHealthy{0, 255, 0};       // Green
inline constexpr Color HealthWarning{255, 165, 0};     // Orange
inline constexpr Color HealthCritical{255, 0, 0};      // Red

// USB LED States (LED2)
inline constexpr Color UsbDisconnected{0, 0, 0};       // Black (off)
inline constexpr Color UsbConnected{0, 0, 255};        // Blue
inline constexpr Color UsbTransferring{0, 255, 255};   // Cyan

// Temperature LED States (LED3)
inline constexpr Color TemperatureNormal{0, 255, 0};   // Green
inline constexpr Color TemperatureWarm{255, 255, 0};   // Yellow
inline constexpr Color TemperatureHot{255, 165, 0};    // Orange
inline constexpr Color TemperatureCritical{255, 0, 0}; // Red

// Drive LED States
inline constexpr Color DriveOff{0, 0, 0};              // Black (off)
inline constexpr Color DriveIdle{0, 0, 255};           // Blue
inline constexpr Color DriveReading{0, 255, 0};        // Green
inline constexpr Color DriveWriting{255, 255, 0};      // Yellow
inline constexpr Color DriveStandby{128, 128, 128};    // Gray
inline constexpr Color DriveError{255, 0, 0};          // Red
inline constexpr Color DriveMissing{128, 0, 128};      // Purple
inline constexpr Color DriveRebuilding{0, 255, 255};   // Cyan

// Animation / Effect Colors
inline constexpr Color AnimationRed{255, 0, 0};        // Red
inline constexpr Color AnimationGreen{0, 255, 0};      // Green
inline constexpr Color AnimationBlue{0, 0, 255};       // Blue
inline constexpr Color AnimationWhite{255, 255, 255}; // White
inline constexpr Color AnimationCyan{0, 255, 255};     // Cyan
inline constexpr Color AnimationMagenta{255, 0, 255}; // Magenta
inline constexpr Color AnimationYellow{255, 255, 0};   // Yellow

// Special Colors
inline constexpr Color Off{0, 0, 0};                   // Black (off)
inline constexpr Color Dim{32, 32, 32};                // Very dim white
inline constexpr Color Half{128, 128, 128};            // Half brightness white

} // namespace NAS::Config::LedColors
