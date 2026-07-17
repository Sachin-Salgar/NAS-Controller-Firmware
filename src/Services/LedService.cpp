/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : LedService.cpp
 *
 ******************************************************************************/

#include "LedService.h"

using namespace NAS::Core;

namespace NAS::Services
{

NAS::Objects::Led LedService::leds_[LedCount];

bool LedService::initialized_ = false;

Result LedService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    for (std::uint16_t i = 0U; i < LedCount; ++i)
    {
        auto result = leds_[i].Initialize(i);

        if (!result.IsSuccess())
        {
            return result;
        }
    }

    initialized_ = true;

    return Result::Ok();
}

Result LedService::SetColor(
    std::uint16_t ledId,
    std::uint8_t red,
    std::uint8_t green,
    std::uint8_t blue) noexcept
{
    if (ledId >= LedCount)
    {
        return Result(ResultCode::InvalidArgument);
    }

    return leds_[ledId].SetColor(
        red,
        green,
        blue);
}

Result LedService::SetMode(
    std::uint16_t ledId,
    NAS::Objects::LedMode mode) noexcept
{
    if (ledId >= LedCount)
    {
        return Result(ResultCode::InvalidArgument);
    }

    return leds_[ledId].SetMode(mode);
}

Result LedService::TurnOff(
    std::uint16_t ledId) noexcept
{
    if (ledId >= LedCount)
    {
        return Result(ResultCode::InvalidArgument);
    }

    return leds_[ledId].TurnOff();
}

NAS::Objects::Led&
LedService::GetLed(std::uint16_t ledId) noexcept
{
    return leds_[ledId];
}

} // namespace NAS::Services