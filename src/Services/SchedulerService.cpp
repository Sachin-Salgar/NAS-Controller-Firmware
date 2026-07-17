/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : SchedulerService.cpp
 *
 ******************************************************************************/

#include "SchedulerService.h"

#include "../Core/Scheduler.h"

#include "ProtocolService.h"
#include "TemperatureService.h"
#include "USBService.h"
#include "HealthService.h"

using namespace NAS::Core;

namespace NAS::Services
{

bool SchedulerService::initialized_ = false;

bool SchedulerService::running_ = false;

Result SchedulerService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    auto result = Scheduler::Initialize();

    if (!result.IsSuccess())
    {
        return result;
    }

    Scheduler::AddTask(
        USBService::Process,
        5U);

    Scheduler::AddTask(
        ProtocolService::Process,
        5U);

    Scheduler::AddTask(
        TemperatureService::UpdateAll,
        1000U);

    Scheduler::AddTask(
        HealthService::Update,
        1000U);

    initialized_ = true;

    return Result::Ok();
}

Result SchedulerService::Start() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    running_ = true;

    return Result::Ok();
}

Result SchedulerService::Stop() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    running_ = false;

    return Result::Ok();
}

Result SchedulerService::Execute() noexcept
{
    if (!running_)
    {
        return Result::Ok();
    }

    return Scheduler::Execute();
}

bool SchedulerService::IsRunning() noexcept
{
    return running_;
}

} // namespace NAS::Services