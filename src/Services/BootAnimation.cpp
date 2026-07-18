/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : BootAnimation.cpp
 *
 ******************************************************************************/

#include "BootAnimation.h"

#include "../Config/LedMap.h"
#include "../Drivers/LEDDriver.h"

using namespace NAS::Core;

namespace NAS::Services
{

BootAnimation::BootAnimation() = default;

Result BootAnimation::Update(std::uint32_t deltaMs) noexcept
{
    if (phase_ == Phase::Complete)
    {
        return Result::Ok();
    }

    elapsedMs_ += deltaMs;

    switch (phase_)
    {
        case Phase::Fill:
        {
            if (elapsedMs_ >= kFillDurationMs)
            {
                phase_ = Phase::Hold;
                elapsedMs_ -= kFillDurationMs;
            }

            float progress = static_cast<float>(elapsedMs_) / 
                            static_cast<float>(kFillDurationMs);

            uint16_t ledsToFill = static_cast<uint16_t>(
                progress * NAS::Config::LedMap::AnimationRegionCount);

            auto result = NAS::Drivers::LEDDriver::Clear();
            if (!result)
            {
                return result;
            }

            for (uint16_t i = 0U; i < ledsToFill; ++i)
            {
                uint16_t ledIndex = NAS::Config::LedMap::AnimationRegionStart + i;
                result = NAS::Drivers::LEDDriver::SetPixel(
                    ledIndex,
                    NAS::Config::LedColors::AnimationGreen);

                if (!result)
                {
                    return result;
                }
            }

            return NAS::Drivers::LEDDriver::Show();
        }

        case Phase::Hold:
        {
            if (elapsedMs_ >= kHoldDurationMs)
            {
                phase_ = Phase::Complete;
                return NAS::Drivers::LEDDriver::Clear();
            }

            return Result::Ok();
        }

        case Phase::Complete:
        default:
            return Result::Ok();
    }
}

bool BootAnimation::IsComplete() const noexcept
{
    return phase_ == Phase::Complete;
}

Result BootAnimation::Stop() noexcept
{
    phase_ = Phase::Complete;
    elapsedMs_ = 0U;

    return NAS::Drivers::LEDDriver::Clear();
}

void BootAnimation::Reset() noexcept
{
    phase_ = Phase::Fill;
    elapsedMs_ = 0U;
}

} // namespace NAS::Services
