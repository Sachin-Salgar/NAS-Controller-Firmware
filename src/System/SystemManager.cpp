/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : System
 * File    : SystemManager.cpp
 *
 ******************************************************************************/

#include "SystemManager.h"

#include "Application.h"
#include "Startup.h"

using namespace NAS::Core;

namespace NAS::System
{

Result SystemManager::Initialize() noexcept
{
    auto result =
        Startup::Initialize();

    if (!result.IsSuccess())
    {
        return result;
    }

    return Application::Initialize();
}

Result SystemManager::Run() noexcept
{
    return Application::Run();
}

} // namespace NAS::System