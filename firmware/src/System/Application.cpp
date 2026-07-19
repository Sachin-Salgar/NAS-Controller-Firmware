/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : System
 * File    : Application.cpp
 *
 ******************************************************************************/

#include "Application.h"

#include "../Services/ServiceManager.h"

using namespace NAS::Core;

namespace NAS::System
{

Result Application::Initialize() noexcept
{
    return NAS::Services::ServiceManager::Start();
}

Result Application::Run() noexcept
{
    while (true)
    {
        auto result =
            NAS::Services::ServiceManager::Execute();

        if (!result.IsSuccess())
        {
            return result;
        }
    }
}

} // namespace NAS::System