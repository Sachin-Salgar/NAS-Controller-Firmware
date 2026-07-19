/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : SystemLed.cpp
 *
 ******************************************************************************/

#include "SystemLed.h"

#include "../Drivers/LEDDriver.h"

using namespace NAS::Core;

namespace NAS::Objects
{

Result SystemLed::Initialize(std::uint16_t ledIndex) noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    ledIndex_ = ledIndex;
    currentColor_ = {0, 0, 0};

    initialized_ = true;

    return Result::Ok();
}

Result SystemLed::SetColor(
    const NAS::Config::LedColors::Color& color) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    currentColor_ = color;

    return NAS::Drivers::LEDDriver::SetPixel(ledIndex_, color);
}

Result SystemLed::Refresh() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Drivers::LEDDriver::Show();
}

std::uint16_t SystemLed::GetIndex() const noexcept
{
    return ledIndex_;
}

NAS::Config::LedColors::Color SystemLed::GetColor() const noexcept
{
    return currentColor_;
}

} // namespace NAS::Objects
