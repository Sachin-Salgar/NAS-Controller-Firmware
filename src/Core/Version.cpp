/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Core
 * File    : Version.cpp
 *
 ******************************************************************************/

#include "Version.h"

#include "BuildInfo.h"

namespace NAS::Core::Version
{

//=============================================================================
// Firmware Information
//=============================================================================

const char* FirmwareName() noexcept
{
    return BuildInfo::FirmwareName;
}

std::uint16_t Major() noexcept
{
    return BuildInfo::FirmwareMajor;
}

std::uint16_t Minor() noexcept
{
    return BuildInfo::FirmwareMinor;
}

std::uint16_t Patch() noexcept
{
    return BuildInfo::FirmwarePatch;
}

//=============================================================================
// Build Information
//=============================================================================

const char* BuildDate() noexcept
{
    return BuildInfo::BuildDate;
}

const char* BuildTime() noexcept
{
    return BuildInfo::BuildTime;
}

const char* BuildConfiguration() noexcept
{
    return BuildInfo::BuildConfiguration;
}

const char* Compiler() noexcept
{
    return BuildInfo::Compiler;
}

const char* TargetPlatform() noexcept
{
    return BuildInfo::TargetPlatform;
}

const char* TargetBoard() noexcept
{
    return BuildInfo::TargetBoard;
}

const char* GitCommit() noexcept
{
    return BuildInfo::GitCommit;
}

} // namespace NAS::Core::Version