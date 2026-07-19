/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : LedEffects.cpp
 *
 ******************************************************************************/

#include "LedEffects.h"

#include "BootAnimation.h"
#include "ErrorAnimation.h"
#include "IdleAnimation.h"
#include "LedAnimator.h"
#include "ShutdownAnimation.h"

using namespace NAS::Core;

namespace NAS::Services
{

LedEffects::AnimationType LedEffects::currentAnimation_ = LedEffects::AnimationType::None;
LedAnimator* LedEffects::currentAnimator_ = nullptr;
BootAnimation LedEffects::bootAnimator_{};
ShutdownAnimation LedEffects::shutdownAnimator_{};
IdleAnimation LedEffects::idleAnimator_{};
ErrorAnimation LedEffects::errorAnimator_{};
bool LedEffects::initialized_ = false;

Result LedEffects::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    initialized_ = true;
    return Result::Ok();
}

Result LedEffects::PlayAnimation(AnimationType type) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (type == AnimationType::None)
    {
        return StopAnimation();
    }

    if (currentAnimation_ == type && currentAnimator_ != nullptr)
    {
        return Result::Ok();
    }

    LedAnimator* animator = nullptr;

    switch (type)
    {
        case AnimationType::Boot:
            animator = &bootAnimator_;
            break;

        case AnimationType::Shutdown:
            animator = &shutdownAnimator_;
            break;

        case AnimationType::Idle:
            animator = &idleAnimator_;
            break;

        case AnimationType::Error:
            animator = &errorAnimator_;
            break;

        case AnimationType::None:
        default:
            return StopAnimation();
    }

    if (currentAnimator_ != nullptr)
    {
        auto stopResult = currentAnimator_->Stop();
        if (!stopResult)
        {
            return stopResult;
        }
    }

    if (animator != nullptr)
    {
        animator->Reset();
    }

    currentAnimation_ = type;
    currentAnimator_ = animator;

    return Result::Ok();
}

Result LedEffects::StopAnimation() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (currentAnimator_ != nullptr)
    {
        auto result = currentAnimator_->Stop();
        if (!result)
        {
            return result;
        }
    }

    currentAnimation_ = AnimationType::None;
    currentAnimator_ = nullptr;

    return Result::Ok();
}

Result LedEffects::Update(std::uint32_t deltaMs) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (currentAnimator_ == nullptr)
    {
        return Result::Ok();
    }

    auto result = currentAnimator_->Update(deltaMs);

    if (currentAnimator_->IsComplete())
    {
        currentAnimation_ = AnimationType::None;
        currentAnimator_ = nullptr;
    }

    return result;
}

LedEffects::AnimationType LedEffects::GetCurrentAnimation() noexcept
{
    return currentAnimation_;
}

bool LedEffects::IsAnimationRunning() noexcept
{
    return currentAnimator_ != nullptr;
}

} // namespace NAS::Services
