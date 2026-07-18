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

bool LEDDriver::configured_ = false;

std::uint8_t LEDDriver::gpioPin_ = 0U;

std::uint16_t LEDDriver::ledCount_ = 0U;

Result LEDDriver::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    configured_ = false;
    gpioPin_ = 0U;
    ledCount_ = 0U;

    initialized_ = true;

    return Result::Ok();
}

Result LEDDriver::Configure(
    std::uint8_t gpioPin,
    std::uint16_t ledCount) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (ledCount == 0U)
    {
        return Result(ResultCode::InvalidArgument);
    }

    gpioPin_ = gpioPin;
    ledCount_ = ledCount;

    auto result =
        NAS::Platform::WS2812::Initialize(
            gpioPin,
            ledCount);

    if (!result)
    {
        return result;
    }

    configured_ = true;

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

    if (!configured_)
    {
        return Result(ResultCode::InvalidState);
    }

    return NAS::Platform::WS2812::SetPixel(
        index,
        color);
}

Result LEDDriver::Fill(
    const NAS::Platform::WS2812::Color& color) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (!configured_)
    {
        return Result(ResultCode::InvalidState);
    }

    return NAS::Platform::WS2812::Fill(color);
}

Result LEDDriver::Clear() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (!configured_)
    {
        return Result(ResultCode::InvalidState);
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

    if (!configured_)
    {
        return Result(ResultCode::InvalidState);
    }

    return NAS::Platform::WS2812::SetBrightness(
        brightness);
}

Result LEDDriver::Refresh() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (!configured_)
    {
        return Result(ResultCode::InvalidState);
    }

    return NAS::Platform::WS2812::Show();
}

}