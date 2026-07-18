/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : IdleAnimation.cpp
 *
 ******************************************************************************/

#include "IdleAnimation.h"

#include <cmath>

#include "../Config/LedMap.h"
#include "../Drivers/LEDDriver.h"

using namespace NAS::Core;

namespace NAS::Services
{

IdleAnimation::IdleAnimation() = default;

Result IdleAnimation::Update(std::uint32_t deltaMs) noexcept
{
    if (stopped_)
    {
        return Result::Ok();
    }

    elapsedMs_ += deltaMs;

    if (elapsedMs_ >= kBreatheCyclems)
    {
        elapsedMs_ -= kBreatheCyclems;
    }

    uint8_t brightness = CalculateBrightness(elapsedMs_);

    auto result = NAS::Drivers::LEDDriver::SetBrightness(brightness);
    if (!result)
    {
        return result;
    }

    for (uint16_t i = NAS::Config::LedMap::AnimationRegionStart;
         i <= NAS::Config::LedMap::AnimationRegionEnd; ++i)
    {
        result = NAS::Drivers::LEDDriver::SetPixel(
            i,
            NAS::Config::LedColors::AnimationBlue);

        if (!result)
        {
            return result;
        }
    }

    return NAS::Drivers::LEDDriver::Show();
}

bool IdleAnimation::IsComplete() const noexcept
{
    return false;
}

Result IdleAnimation::Stop() noexcept
{
    stopped_ = true;
    elapsedMs_ = 0U;

    return NAS::Drivers::LEDDriver::Clear();
}

void IdleAnimation::Reset() noexcept
{
    stopped_ = false;
    elapsedMs_ = 0U;
}

std::uint8_t IdleAnimation::CalculateBrightness(
    std::uint32_t cycleMs) const noexcept
{
    float normalized = static_cast<float>(cycleMs) / 
                      static_cast<float>(kBreatheCyclems);
    
    float sine = std::sin(normalized * 2.0f * 3.14159f);
    
    float breathe = (sine + 1.0f) / 2.0f;
    
    uint8_t minBrightness = 50U;
    uint8_t maxBrightness = 200U;
    uint8_t range = maxBrightness - minBrightness;

    return minBrightness + static_cast<uint8_t>(breathe * range);
}

} // namespace NAS::Services
