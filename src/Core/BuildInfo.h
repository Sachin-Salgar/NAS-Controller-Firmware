#pragma once

#include <cstdint>

namespace NAS::Core::BuildInfo
{

//--------------------------------------------------------------------------
// Firmware Information
//--------------------------------------------------------------------------

inline constexpr char FirmwareName[] = "NAS Controller Firmware";

inline constexpr std::uint16_t FirmwareMajor = 1;
inline constexpr std::uint16_t FirmwareMinor = 0;
inline constexpr std::uint16_t FirmwarePatch = 0;

//--------------------------------------------------------------------------
// Build Information
//--------------------------------------------------------------------------

inline constexpr char BuildDate[] = __DATE__;
inline constexpr char BuildTime[] = __TIME__;

inline constexpr char BuildConfiguration[] =
#ifdef NDEBUG
    "Release";
#else
    "Debug";
#endif

inline constexpr char Compiler[] =
#if defined(__clang__)
    "Clang";
#elif defined(__GNUC__)
    "GCC";
#elif defined(_MSC_VER)
    "MSVC";
#else
    "Unknown";
#endif

inline constexpr char TargetPlatform[] = "ESP32";
inline constexpr char TargetBoard[] = "ESP32-WROOM-32";

//--------------------------------------------------------------------------
// Git Information
//--------------------------------------------------------------------------
//
// Define GIT_COMMIT_HASH from the build system if available.
//
// Example:
// -DGIT_COMMIT_HASH=\"abc1234\"
//
//--------------------------------------------------------------------------

#ifdef GIT_COMMIT_HASH
inline constexpr char GitCommit[] = GIT_COMMIT_HASH;
#else
inline constexpr char GitCommit[] = "UNKNOWN";
#endif

} // namespace NAS::Core::BuildInfo