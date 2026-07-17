#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Core
 * File    : Version.h
 *
 * Description:
 * Public interface for retrieving firmware version and build information.
 *
 ******************************************************************************/

#include <cstdint>

namespace NAS::Core::Version
{

//=============================================================================
// Firmware Information
//=============================================================================

/**
 * @brief Returns the firmware name.
 */
[[nodiscard]]
const char* FirmwareName() noexcept;

/**
 * @brief Returns the firmware major version.
 */
[[nodiscard]]
std::uint16_t Major() noexcept;

/**
 * @brief Returns the firmware minor version.
 */
[[nodiscard]]
std::uint16_t Minor() noexcept;

/**
 * @brief Returns the firmware patch version.
 */
[[nodiscard]]
std::uint16_t Patch() noexcept;

//=============================================================================
// Build Information
//=============================================================================

/**
 * @brief Returns the firmware build date.
 */
[[nodiscard]]
const char* BuildDate() noexcept;

/**
 * @brief Returns the firmware build time.
 */
[[nodiscard]]
const char* BuildTime() noexcept;

/**
 * @brief Returns the build configuration.
 *
 * Possible values:
 * - Debug
 * - Release
 */
[[nodiscard]]
const char* BuildConfiguration() noexcept;

/**
 * @brief Returns the compiler name.
 */
[[nodiscard]]
const char* Compiler() noexcept;

/**
 * @brief Returns the target platform.
 */
[[nodiscard]]
const char* TargetPlatform() noexcept;

/**
 * @brief Returns the target board.
 */
[[nodiscard]]
const char* TargetBoard() noexcept;

/**
 * @brief Returns the Git commit hash.
 */
[[nodiscard]]
const char* GitCommit() noexcept;

} // namespace NAS::Core::Version