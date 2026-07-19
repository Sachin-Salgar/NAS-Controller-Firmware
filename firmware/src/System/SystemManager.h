#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : System
 * File    : SystemManager.h
 *
 ******************************************************************************/

#include "../Core/Result.h"

namespace NAS::System
{

class SystemManager
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Run() noexcept;

private:

    SystemManager() = delete;
};

} // namespace NAS::System