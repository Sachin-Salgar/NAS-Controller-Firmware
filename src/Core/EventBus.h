#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Core
 * File    : EventBus.h
 *
 * Description:
 * Lightweight publish-subscribe event bus.
 *
 ******************************************************************************/

#include <cstddef>

#include "Event.h"
#include "Result.h"

namespace NAS::Core
{

class EventBus
{
public:

    using EventHandler = void (*)(const Event&);

    static constexpr std::size_t MaxSubscribers = 16U;

    EventBus() noexcept;

    [[nodiscard]]
    Result Initialize() noexcept;

    [[nodiscard]]
    Result Subscribe(EventHandler handler) noexcept;

    [[nodiscard]]
    Result Unsubscribe(EventHandler handler) noexcept;

    [[nodiscard]]
    Result Publish(const Event& event) const noexcept;

    [[nodiscard]]
    std::size_t SubscriberCount() const noexcept;

private:

    EventHandler subscribers_[MaxSubscribers];

    std::size_t subscriberCount_;

    bool initialized_;
};

} // namespace NAS::Core