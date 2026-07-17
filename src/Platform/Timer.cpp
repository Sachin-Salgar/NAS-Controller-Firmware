/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : Timer.cpp
 *
 ******************************************************************************/

#include "Timer.h"

#include <Arduino.h>

using namespace NAS::Core;

namespace NAS::Platform::Timer
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

std::uint32_t Milliseconds() noexcept
{
    return millis();
}

std::uint64_t Microseconds() noexcept
{
    return micros();
}

void DelayMilliseconds(std::uint32_t milliseconds) noexcept
{
    delay(milliseconds);
}

void DelayMicroseconds(std::uint32_t microseconds) noexcept
{
    delayMicroseconds(microseconds);
}

} // namespace NAS::Platform::Timer