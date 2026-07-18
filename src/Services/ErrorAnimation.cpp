/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : ErrorAnimation.cpp
 *
 ******************************************************************************/

#include "ErrorAnimation.h"

#include "../Config/LedMap.h"
#include "../Drivers/LEDDriver.h"

using namespace NAS::Core;

namespace NAS::Services
{

ErrorAnimation::ErrorAnimation() = default;

Result ErrorAnimation::Update(std::uint32_t deltaMs) noexcept
{
    if (stopped_)
    {
        return Result::Ok();
    }

    elapsedMs_ += deltaMs;

    if (elapsedMs_ >= kFlashPeriodMs)
    {
        elapsedMs_ -= kFlashPeriodMs;
    }

    bool isFlashing = elapsedMs_ < kFlashOnMs;

    auto result = NAS::Drivers::LEDDriver::Clear();
    if (!result)
    {
        return result;
    }

    if (isFlashing)
    {
        for (uint16_t i = NAS::Config::LedMap::AnimationRegionStart;
             i <= NAS::Config::LedMap::AnimationRegionEnd; ++i)
        {
            result = NAS::Drivers::LEDDriver::SetPixel(
                i,
                NAS::Config::LedColors::AnimationRed);

            if (!result)
            {
                return result;
            }
        }
    }

    return NAS::Drivers::LEDDriver::Show();
}

bool ErrorAnimation::IsComplete() const noexcept
{
    return false;
}

Result ErrorAnimation::Stop() noexcept
{
    stopped_ = true;
    elapsedMs_ = 0U;

    return NAS::Drivers::LEDDriver::Clear();
}

void ErrorAnimation::Reset() noexcept
{
    stopped_ = false;
    elapsedMs_ = 0U;
}

} // namespace NAS::Services
