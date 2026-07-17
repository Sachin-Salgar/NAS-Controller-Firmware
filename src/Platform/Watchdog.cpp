#include "Watchdog.h"

#include <Arduino.h>
#include <esp_task_wdt.h>

using namespace NAS::Core;

namespace NAS::Platform::Watchdog
{
namespace
{
bool initialized = false;
}

Result Initialize() noexcept
{
    if (initialized)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    initialized = true;
    return Result::Ok();
}

Result Feed() noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    esp_task_wdt_reset();
    return Result::Ok();
}

} // namespace NAS::Platform::Watchdog
