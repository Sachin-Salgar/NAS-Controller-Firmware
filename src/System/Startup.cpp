/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : System
 * File    : Startup.cpp
 *
 ******************************************************************************/

#include "Startup.h"

#include "../Services/ServiceManager.h"

using namespace NAS::Core;

namespace NAS::System
{

Result Startup::Initialize() noexcept
{
    Result result;

    result = NAS::Services::ServiceManager::Initialize();
    if (!result.IsSuccess())
    {
        return result;
    }

    return Result::Ok();
}

} // namespace NAS::System
