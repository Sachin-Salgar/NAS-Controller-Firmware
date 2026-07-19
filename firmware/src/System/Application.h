#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : System
 * File    : Application.h
 *
 ******************************************************************************/

#include "../Core/Result.h"

namespace NAS::System
{

class Application
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Run() noexcept;

private:

    Application() = delete;
};

} // namespace NAS::System