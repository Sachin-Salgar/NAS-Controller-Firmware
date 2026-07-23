/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : SchedulerService.cpp
 *
 ******************************************************************************/

#include "SchedulerService.h"

#include "../Core/Scheduler.h"
#include "../Config/TaskConfig.h"

#include "ProtocolService.h"
#include "TemperatureService.h"
#include "USBService.h"
#include "HealthService.h"
#include "StatisticsService.h"

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

    result = Scheduler::AddTask(
        USBService::Process,
        NAS::Config::Tasks::UsbTaskIntervalMs);

    if (!result.IsSuccess())
    {
        return result;
    }

    result = Scheduler::AddTask(
        ProtocolService::Process,
        NAS::Config::Tasks::ProtocolTaskIntervalMs);

    if (!result.IsSuccess())
    {
        return result;
    }

    result = Scheduler::AddTask(
        TemperatureService::UpdateAll,
        NAS::Config::Tasks::TemperatureTaskIntervalMs);

    if (!result.IsSuccess())
    {
        return result;
    }

    result = Scheduler::AddTask(
        HealthService::Update,
        NAS::Config::Tasks::HealthTaskIntervalMs);

    if (!result.IsSuccess())
    {
        return result;
    }

    result = Scheduler::AddTask(
        StatisticsService::Update,
        NAS::Config::Tasks::StatisticsTaskIntervalMs);

    if (!result.IsSuccess())
    {
        return result;
    }

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
