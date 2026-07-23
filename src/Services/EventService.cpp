/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : EventService.cpp
 *
 ******************************************************************************/

#include "EventService.h"

using namespace NAS::Core;

namespace NAS::Services
{

Event EventService::queue_[QueueSize];

std::size_t EventService::head_ = 0U;

std::size_t EventService::tail_ = 0U;

std::size_t EventService::count_ = 0U;

bool EventService::initialized_ = false;

Result EventService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    head_ = 0U;
    tail_ = 0U;
    count_ = 0U;

    initialized_ = true;

    return Result::Ok();
}

Result EventService::Publish(
    const Event& event) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (count_ >= QueueSize)
    {
        return Result(ResultCode::BufferFull);
    }

    queue_[tail_] = event;

    tail_ = (tail_ + 1U) % QueueSize;

    ++count_;

    return Result::Ok();
}

Result EventService::GetNextEvent(
    Event& event) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (count_ == 0U)
    {
        return Result(ResultCode::NotFound);
    }

    event = queue_[head_];

    head_ = (head_ + 1U) % QueueSize;

    --count_;

    return Result::Ok();
}

bool EventService::HasPendingEvents() noexcept
{
    return count_ != 0U;
}

Result EventService::Clear() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    head_ = 0U;
    tail_ = 0U;
    count_ = 0U;

    return Result::Ok();
}

std::size_t EventService::GetPendingCount() noexcept
{
    return count_;
}

} // namespace NAS::Services