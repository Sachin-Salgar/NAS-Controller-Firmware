#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : ErrorAnimation.h
 *
 * Description:
 * Error state animation - red flash pattern.
 * Indicates critical system error requiring attention.
 * Runs indefinitely until stopped.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Config/LedColors.h"
#include "../Core/Result.h"
#include "LedAnimator.h"

namespace NAS::Services
{

class ErrorAnimation : public LedAnimator
{
public:

    ErrorAnimation();

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

    static constexpr std::uint32_t kFlashPeriodMs = 500U;
    static constexpr std::uint32_t kFlashOnMs = 250U;

};

} // namespace NAS::Services
