#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : DriveService.h
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"
#include "../Objects/Drive.h"
#include "../Config/HardwareConfig.h"

namespace NAS::Services
{

class DriveService
{
public:

    static constexpr std::uint8_t DriveCount = NAS::Config::Hardware::DriveCount;

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetPresent(
        std::uint8_t driveId,
        bool present) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetState(
        std::uint8_t driveId,
        NAS::Objects::DriveState state) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetTemperature(
        std::uint8_t driveId,
        float temperature) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetHealth(
        std::uint8_t driveId,
        NAS::Objects::DriveHealth health) noexcept;

    [[nodiscard]]
    static NAS::Objects::Drive&
    GetDrive(std::uint8_t driveId) noexcept;

private:

    static NAS::Objects::Drive drives_[DriveCount];

    static bool initialized_;
};

} // namespace NAS::Services
