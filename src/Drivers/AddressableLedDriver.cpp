/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : AddressableLedDriver.cpp
 *
 ******************************************************************************/

#include "AddressableLedDriver.h"

using namespace NAS::Core;

namespace NAS::Drivers
{

AddressableLedDriver::Color
AddressableLedDriver::leds_[MaxLeds] = {};

std::uint16_t AddressableLedDriver::ledCount_ = 0U;

std::uint8_t AddressableLedDriver::brightness_ = 255U;

bool AddressableLedDriver::initialized_ = false;

Result AddressableLedDriver::Initialize(std::uint8_t,
                                        std::uint16_t ledCount) noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    if ((ledCount == 0U) || (ledCount > MaxLeds))
    {
        return Result(ResultCode::InvalidArgument);
    }

    ledCount_ = ledCount;
    brightness_ = 255U;

    (void)Clear();

    initialized_ = true;

    return Result::Ok();
}

Result AddressableLedDriver::SetColor(std::uint16_t index,
                                      Color color) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (index >= ledCount_)
    {
        return Result(ResultCode::InvalidArgument);
    }

    leds_[index] = color;

    return Result::Ok();
}

Result AddressableLedDriver::SetBrightness(std::uint8_t brightness) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    brightness_ = brightness;

    return Result::Ok();
}

Result AddressableLedDriver::Clear() noexcept
{
    for (std::uint16_t i = 0U; i < MaxLeds; ++i)
    {
        leds_[i] = {0U, 0U, 0U};
    }

    return Result::Ok();
}

Result AddressableLedDriver::Show() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    /*
     * Physical WS2812 transmission will be
     * implemented during hardware bring-up.
     */

    return Result::Ok();
}

std::uint16_t AddressableLedDriver::LedCount() noexcept
{
    return ledCount_;
}

} // namespace NAS::Drivers
