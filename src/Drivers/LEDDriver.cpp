/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : LEDDriver.cpp
 *
 ******************************************************************************/

#include "LEDDriver.h"

using namespace NAS::Core;

namespace NAS::Drivers
{

bool LEDDriver::initialized_ = false;

Result LEDDriver::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    auto result = NAS::Platform::WS2812::Initialize();

    if (!result)
    {
        return result;
    }

    initialized_ = true;

    return Result::Ok();
}

Result LEDDriver::SetPixel(
    std::uint16_t index,
    const NAS::Platform::WS2812::Color& color) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Platform::WS2812::SetPixel(index, color);
}

Result LEDDriver::Fill(
    const NAS::Platform::WS2812::Color& color) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Platform::WS2812::Fill(color);
}

Result LEDDriver::Clear() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Platform::WS2812::Clear();
}

Result LEDDriver::SetBrightness(
    std::uint8_t brightness) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Platform::WS2812::SetBrightness(brightness);
}

Result LEDDriver::Show() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Platform::WS2812::Show();
}

} // namespace NAS::Drivers