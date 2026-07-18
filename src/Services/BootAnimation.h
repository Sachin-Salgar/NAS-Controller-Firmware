#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : BootAnimation.h
 *
 * Description:
 * Boot animation for system startup sequence.
 * Shows a progressive fill animation followed by ready state.
 * Maximum 3 seconds total.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Config/LedColors.h"
#include "../Core/Result.h"
#include "LedAnimator.h"

namespace NAS::Services
{

class BootAnimation : public LedAnimator
{
public:

    BootAnimation();

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

    enum class Phase : std::uint8_t
    {
        Fill,
        Hold,
        Complete
    };

    Phase phase_{Phase::Fill};

    std::uint32_t elapsedMs_{0U};

    static constexpr std::uint32_t kFillDurationMs = 2000U;
    static constexpr std::uint32_t kHoldDurationMs = 1000U;
    static constexpr std::uint32_t kTotalDurationMs = kFillDurationMs + kHoldDurationMs;

};

} // namespace NAS::Services
