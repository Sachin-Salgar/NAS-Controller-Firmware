/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Led.cpp
 *
 ******************************************************************************/

#include "Led.h"

using namespace NAS::Core;

namespace NAS::Objects
{

Result Led::Initialize(std::uint16_t index) noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    index_ = index;
    initialized_ = true;

    return Result::Ok();
}

Result Led::SetColor(std::uint8_t red,
                     std::uint8_t green,
                     std::uint8_t blue) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    red_ = red;
    green_ = green;
    blue_ = blue;

    return NAS::Drivers::AddressableLedDriver::SetColor(
        index_,
        {red_, green_, blue_});
}

Result Led::SetMode(LedMode mode) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    mode_ = mode;

    return Result::Ok();
}

Result Led::SetBrightness(std::uint8_t brightness) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    brightness_ = brightness;

    return NAS::Drivers::AddressableLedDriver::SetBrightness(
        brightness_);
}

Result Led::TurnOff() noexcept
{
    mode_ = LedMode::Off;

    return SetColor(0U, 0U, 0U);
}

std::uint16_t Led::GetIndex() const noexcept
{
    return index_;
}

LedMode Led::GetMode() const noexcept
{
    return mode_;
}

} // namespace NAS::Objects