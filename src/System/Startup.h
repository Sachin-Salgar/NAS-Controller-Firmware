#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : System
 * File    : Startup.h
 *
 ******************************************************************************/

#include "../Core/Result.h"

namespace NAS::System
{

class Startup
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

private:

    Startup() = delete;
};

} // namespace NAS::System