#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : HealthService.h
 *
 ******************************************************************************/

#include "../Core/Result.h"

namespace NAS::Services
{

class HealthService
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Update() noexcept;

    [[nodiscard]]
    static bool IsHealthy() noexcept;

private:

    static bool initialized_;

    static bool healthy_;
};

} // namespace NAS::Services