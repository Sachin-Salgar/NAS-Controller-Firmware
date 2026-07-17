#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : ServiceManager.h
 *
 ******************************************************************************/

#include "../Core/Result.h"

namespace NAS::Services
{

class ServiceManager
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

private:

    static bool initialized_;
};

} // namespace NAS::Services