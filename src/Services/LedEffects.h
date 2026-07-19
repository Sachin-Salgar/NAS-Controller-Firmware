#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : LedEffects.h
 *
 * Description:
 * Coordinator for LED animation effects.
 * Manages current active animation and provides interface for
 * starting/stopping animations.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"
#include "BootAnimation.h"
#include "ShutdownAnimation.h"
#include "IdleAnimation.h"
#include "ErrorAnimation.h"

namespace NAS::Services
{

class LedAnimator;

class LedEffects
{
public:

    enum class AnimationType : std::uint8_t
    {
        None = 0,
        Boot,
        Shutdown,
        Idle,
        Error
    };

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result PlayAnimation(
        AnimationType type) noexcept;

    [[nodiscard]]
    static NAS::Core::Result StopAnimation() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Update(
        std::uint32_t deltaMs) noexcept;

    [[nodiscard]]
    static AnimationType GetCurrentAnimation() noexcept;

    [[nodiscard]]
    static bool IsAnimationRunning() noexcept;

private:

    static AnimationType currentAnimation_;

    static LedAnimator* currentAnimator_;

    static BootAnimation bootAnimator_;

    static ShutdownAnimation shutdownAnimator_;

    static IdleAnimation idleAnimator_;

    static ErrorAnimation errorAnimator_;

    static bool initialized_;

};

} // namespace NAS::Services
