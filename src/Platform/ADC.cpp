#include "ADC.h"

#include <Arduino.h>

using namespace NAS::Core;

namespace NAS::Platform::ADC
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

Result Configure(std::uint8_t pin) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    analogReadResolution(12);
    analogSetPinAttenuation(pin, ADC_11db);
    return Result::Ok();
}

std::uint16_t ReadRaw(std::uint8_t pin) noexcept
{
    return static_cast<std::uint16_t>(analogRead(pin));
}

std::uint16_t ReadMillivolts(std::uint8_t pin) noexcept
{
    return static_cast<std::uint16_t>(analogRead(pin));
}

} // namespace NAS::Platform::ADC
