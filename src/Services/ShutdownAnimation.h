#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : ShutdownAnimation.h
 *
 * Description:
 * Shutdown animation for system power-down sequence.
 * Shows a progressive fade-out animation.
 * Completes when all LEDs are off.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Config/LedColors.h"
#include "../Core/Result.h"
#include "LedAnimator.h"

namespace NAS::Services
{

class ShutdownAnimation : public LedAnimator
{
public:

    ShutdownAnimation();

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

    bool complete_{false};

    static constexpr std::uint32_t kFadeDurationMs = 1500U;

};

} // namespace NAS::Services
