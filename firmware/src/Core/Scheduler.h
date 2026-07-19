#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Core
 * File    : Scheduler.h
 *
 ******************************************************************************/

#include <cstdint>
#include <functional>

#include "Result.h"

namespace NAS::Core
{

class Scheduler
{
public:

    using TaskFunction = std::function<void()>;

    [[nodiscard]]
    static Result Initialize() noexcept;

    [[nodiscard]]
    static Result AddTask(
        TaskFunction task,
        std::uint32_t intervalMs) noexcept;

    [[nodiscard]]
    static Result Execute() noexcept;

private:

    static constexpr std::uint32_t MaxTasks = 16;

    struct Task
    {
        TaskFunction function;
        std::uint32_t intervalMs;
        std::uint32_t lastExecutionMs;
    };

    static Task tasks_[MaxTasks];
    static std::uint32_t taskCount_;
    static bool initialized_;
};

} // namespace NAS::Core
