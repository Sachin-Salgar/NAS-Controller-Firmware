#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : LedEventSubscriber.h
 *
 * Description:
 * EventBus subscriber for LED system.
 * Automatically responds to system events (startup, shutdown, temperature,
 * USB, drive state changes) by updating LED indicators and animations.
 *
 ******************************************************************************/

#include "../Core/Result.h"

namespace NAS::Services
{

class LedEventSubscriber
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

private:

    static bool initialized_;

};

} // namespace NAS::Services
