/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : System
 * File    : Startup.cpp
 *
 ******************************************************************************/

#include "Startup.h"

#include "../Platform/PlatformManager.h"
#include "../Drivers/DriverManager.h"
#include "../Objects/ObjectManager.h"
#include "../Services/ServiceManager.h"

using namespace NAS::Core;

namespace NAS::System
{

Result Startup::Initialize() noexcept
{
    Result result;

    result = NAS::Platform::PlatformManager::Initialize();
    if (!result.IsSuccess())
    {
        return result;
    }

    result = NAS::Drivers::DriverManager::Initialize();
    if (!result.IsSuccess())
    {
        return result;
    }

    result = NAS::Objects::ObjectManager::Initialize();
    if (!result.IsSuccess())
    {
        return result;
    }

    result = NAS::Services::ServiceManager::Initialize();
    if (!result.IsSuccess())
    {
        return result;
    }

    return Result::Ok();
}

} // namespace NAS::System