/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : ShutdownAnimation.cpp
 *
 ******************************************************************************/

#include "ShutdownAnimation.h"

#include "../Config/LedMap.h"
#include "../Drivers/LEDDriver.h"

using namespace NAS::Core;

namespace NAS::Services
{

ShutdownAnimation::ShutdownAnimation() = default;

Result ShutdownAnimation::Update(std::uint32_t deltaMs) noexcept
{
    if (complete_)
    {
        return Result::Ok();
    }

    elapsedMs_ += deltaMs;

    if (elapsedMs_ >= kFadeDurationMs)
    {
        complete_ = true;
        return NAS::Drivers::LEDDriver::Clear();
    }

    float progress = static_cast<float>(elapsedMs_) / 
                    static_cast<float>(kFadeDurationMs);

    uint8_t brightness = static_cast<uint8_t>(
        (1.0f - progress) * 255.0f);

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
            NAS::Config::LedColors::AnimationOrange);

        if (!result)
        {
            return result;
        }
    }

    return NAS::Drivers::LEDDriver::Show();
}

bool ShutdownAnimation::IsComplete() const noexcept
{
    return complete_;
}

Result ShutdownAnimation::Stop() noexcept
{
    complete_ = true;
    elapsedMs_ = 0U;

    return NAS::Drivers::LEDDriver::Clear();
}

void ShutdownAnimation::Reset() noexcept
{
    complete_ = false;
    elapsedMs_ = 0U;
}

} // namespace NAS::Services
