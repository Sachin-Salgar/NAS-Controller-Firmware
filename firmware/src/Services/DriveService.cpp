/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : DriveService.cpp
 *
 ******************************************************************************/

#include "DriveService.h"

using namespace NAS::Core;

namespace NAS::Services
{

NAS::Objects::Drive DriveService::drives_[DriveCount];

bool DriveService::initialized_ = false;

Result DriveService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    for (std::uint8_t i = 0U; i < DriveCount; ++i)
    {
        auto result = drives_[i].Initialize(i + 1U);

        if (!result.IsSuccess())
        {
            return result;
        }
    }

    initialized_ = true;

    return Result::Ok();
}

Result DriveService::SetPresent(
    std::uint8_t driveId,
    bool present) noexcept
{
    if ((driveId == 0U) || (driveId > DriveCount))
    {
        return Result(ResultCode::InvalidArgument);
    }

    return drives_[driveId - 1U].SetPresent(present);
}

Result DriveService::SetState(
    std::uint8_t driveId,
    NAS::Objects::DriveState state) noexcept
{
    if ((driveId == 0U) || (driveId > DriveCount))
    {
        return Result(ResultCode::InvalidArgument);
    }

    return drives_[driveId - 1U].SetState(state);
}

Result DriveService::SetTemperature(
    std::uint8_t driveId,
    float temperature) noexcept
{
    if ((driveId == 0U) || (driveId > DriveCount))
    {
        return Result(ResultCode::InvalidArgument);
    }

    return drives_[driveId - 1U].SetTemperature(temperature);
}

Result DriveService::SetHealth(
    std::uint8_t driveId,
    NAS::Objects::DriveHealth health) noexcept
{
    if ((driveId == 0U) || (driveId > DriveCount))
    {
        return Result(ResultCode::InvalidArgument);
    }

    return drives_[driveId - 1U].SetHealth(health);
}

NAS::Objects::Drive&
DriveService::GetDrive(std::uint8_t driveId) noexcept
{
    return drives_[driveId - 1U];
}

} // namespace NAS::Services