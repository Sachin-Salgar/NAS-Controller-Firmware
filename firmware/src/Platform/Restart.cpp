#include "Restart.h"

#include <Arduino.h>

using namespace NAS::Core;

namespace NAS::Platform::Restart
{

Result Soft() noexcept
{
    ESP.restart();
    return Result::Ok();
}

Result Hard() noexcept
{
    ESP.restart();
    return Result::Ok();
}

} // namespace NAS::Platform::Restart
