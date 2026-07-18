#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : LedService.h
 *
 * Description:
 * LED Service
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"
#include "../Objects/Led.h"

namespace NAS::Services
{

class LedService
{
public:

    static constexpr std::uint16_t DriveLedCount = 8U;

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetDriveState(
        std::uint8_t driveId,
        NAS::Objects::DriveLedState state) noexcept;

    [[nodiscard]]
    static NAS::Core::Result Refresh() noexcept;

    [[nodiscard]]
    static NAS::Core::Result ClearAll() noexcept;

private:

    static NAS::Objects::Led driveLeds_[DriveLedCount];

    static bool initialized_;

};

} // namespace NAS::Services