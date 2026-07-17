/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Core
 * File    : EventBus.cpp
 *
 ******************************************************************************/

#include "EventBus.h"

namespace NAS::Core
{

EventBus::EventBus() noexcept
    : subscribers_{},
      subscriberCount_(0U),
      initialized_(false)
{
}

Result EventBus::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    subscriberCount_ = 0U;

    for (auto& subscriber : subscribers_)
    {
        subscriber = nullptr;
    }

    initialized_ = true;

    return Result::Ok();
}

Result EventBus::Subscribe(EventHandler handler) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (handler == nullptr)
    {
        return Result(ResultCode::InvalidArgument);
    }

    if (subscriberCount_ >= MaxSubscribers)
    {
        return Result(ResultCode::OutOfMemory);
    }

    for (std::size_t index = 0U; index < subscriberCount_; ++index)
    {
        if (subscribers_[index] == handler)
        {
            return Result(ResultCode::AlreadyInitialized);
        }
    }

    subscribers_[subscriberCount_++] = handler;

    return Result::Ok();
}

Result EventBus::Unsubscribe(EventHandler handler) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (handler == nullptr)
    {
        return Result(ResultCode::InvalidArgument);
    }

    for (std::size_t index = 0U; index < subscriberCount_; ++index)
    {
        if (subscribers_[index] == handler)
        {
            for (std::size_t move = index; move < (subscriberCount_ - 1U); ++move)
            {
                subscribers_[move] = subscribers_[move + 1U];
            }

            subscribers_[subscriberCount_ - 1U] = nullptr;
            --subscriberCount_;

            return Result::Ok();
        }
    }

    return Result(ResultCode::DeviceNotFound);
}

Result EventBus::Publish(const Event& event) const noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    for (std::size_t index = 0U; index < subscriberCount_; ++index)
    {
        if (subscribers_[index] != nullptr)
        {
            subscribers_[index](event);
        }
    }

    return Result::Ok();
}

std::size_t EventBus::SubscriberCount() const noexcept
{
    return subscriberCount_;
}

} // namespace NAS::Core