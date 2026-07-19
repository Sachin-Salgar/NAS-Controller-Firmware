/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Core
 * File    : Scheduler.cpp
 *
 ******************************************************************************/

#include "Scheduler.h"

#include <Arduino.h>

namespace NAS::Core
{

Scheduler::Task Scheduler::tasks_[Scheduler::MaxTasks] = {};

std::uint32_t Scheduler::taskCount_ = 0;

bool Scheduler::initialized_ = false;

Result Scheduler::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    taskCount_ = 0;

    for (std::uint32_t i = 0; i < MaxTasks; ++i)
    {
        tasks_[i].lastExecutionMs = 0;
    }

    initialized_ = true;

    return Result::Ok();
}

Result Scheduler::AddTask(
    TaskFunction task,
    std::uint32_t intervalMs) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (!task)
    {
        return Result(ResultCode::NullPointer);
    }

    if (taskCount_ >= MaxTasks)
    {
        return Result(ResultCode::BufferFull);
    }

    tasks_[taskCount_].function = task;
    tasks_[taskCount_].intervalMs = intervalMs;
    tasks_[taskCount_].lastExecutionMs = millis();
    ++taskCount_;

    return Result::Ok();
}

Result Scheduler::Execute() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    std::uint32_t currentTimeMs = millis();

    for (std::uint32_t i = 0; i < taskCount_; ++i)
    {
        std::uint32_t elapsedMs = currentTimeMs - tasks_[i].lastExecutionMs;

        if (elapsedMs >= tasks_[i].intervalMs)
        {
            tasks_[i].function();
            tasks_[i].lastExecutionMs = currentTimeMs;
        }
    }

    return Result::Ok();
}

} // namespace NAS::Core
