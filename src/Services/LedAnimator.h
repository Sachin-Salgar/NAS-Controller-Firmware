#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : LedAnimator.h
 *
 * Description:
 * Base class for LED animations using frame-based timing.
 * All animations inherit from this class and implement Update(deltaMs).
 * No blocking delays - designed for integration with Scheduler.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Services
{

class LedAnimator
{
public:

    virtual ~LedAnimator() = default;

    [[nodiscard]]
    virtual NAS::Core::Result Update(
        std::uint32_t deltaMs) noexcept = 0;

    [[nodiscard]]
    virtual bool IsComplete() const noexcept = 0;

    [[nodiscard]]
    virtual NAS::Core::Result Stop() noexcept = 0;

    [[nodiscard]]
    virtual void Reset() noexcept = 0;

protected:

    LedAnimator() = default;

};

} // namespace NAS::Services
