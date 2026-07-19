#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Config
 * File    : BuildConfig.h
 *
 ******************************************************************************/

namespace NAS::Config::Build
{

inline constexpr char ProjectName[] = "NAS Controller Firmware";

inline constexpr char BoardName[] = "ESP32 Dev Module";

inline constexpr char FirmwareName[] = "NAS Controller";

inline constexpr bool DevelopmentBuild = true;

inline constexpr bool ProductionBuild = false;

inline constexpr bool EnableAssertions = true;

inline constexpr bool EnableVerboseLogging = false;

} // namespace NAS::Config::Build