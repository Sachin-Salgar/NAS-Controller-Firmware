#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : IdleAnimation.h
 *
 * Description:
 * Idle state animation showing system is ready and waiting.
 * Implements breathing/pulse effect on animation region.
 * Runs indefinitely until stopped.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Config/LedColors.h"
#include "../Core/Result.h"
#include "LedAnimator.h"

namespace NAS::Services
{

class IdleAnimation : public LedAnimator
{
public:

    IdleAnimation();

    [[nodiscard]]
    NAS::Core::Result Update(
        std::uint32_t deltaMs) noexcept override;

    [[nodiscard]]
    bool IsComplete() const noexcept override;

    [[nodiscard]]
    NAS::Core::Result Stop() noexcept override;

    [[nodiscard]]
    void Reset() noexcept override;

private:

    std::uint32_t elapsedMs_{0U};

    bool stopped_{false};

    static constexpr std::uint32_t kBreatheCyclems = 2000U;

    [[nodiscard]]
    std::uint8_t CalculateBrightness(
        std::uint32_t cycleMs) const noexcept;

};

} // namespace NAS::Services
