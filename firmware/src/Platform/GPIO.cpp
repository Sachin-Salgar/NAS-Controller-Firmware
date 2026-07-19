/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : GPIO.cpp
 *
 ******************************************************************************/

#include "GPIO.h"

#include <Arduino.h>

using namespace NAS::Core;

namespace NAS::Platform::GPIO
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

Result Configure(std::uint8_t pin,
                 Mode mode) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    switch (mode)
    {
        case Mode::Input:
            pinMode(pin, INPUT);
            break;

        case Mode::InputPullUp:
            pinMode(pin, INPUT_PULLUP);
            break;

        case Mode::InputPullDown:
            pinMode(pin, INPUT_PULLDOWN);
            break;

        case Mode::Output:
            pinMode(pin, OUTPUT);
            break;

        case Mode::OutputOpenDrain:
            pinMode(pin, OUTPUT_OPEN_DRAIN);
            break;

        default:
            return Result(ResultCode::InvalidArgument);
    }

    return Result::Ok();
}

Result Write(std::uint8_t pin,
             Level level) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    digitalWrite(pin, level == Level::High ? HIGH : LOW);
    return Result::Ok();
}

Result Toggle(std::uint8_t pin) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    digitalWrite(pin, !digitalRead(pin));
    return Result::Ok();
}

Result Read(std::uint8_t pin,
            Level& level) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    level = (digitalRead(pin) == HIGH) ? Level::High : Level::Low;
    return Result::Ok();
}

} // namespace NAS::Platform::GPIO