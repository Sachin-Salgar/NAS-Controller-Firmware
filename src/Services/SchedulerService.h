#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : SchedulerService.h
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Services
{

class SchedulerService
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Start() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Stop() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Execute() noexcept;

    [[nodiscard]]
    static bool IsRunning() noexcept;

private:

    static bool initialized_;

    static bool running_;
};

} // namespace NAS::Services