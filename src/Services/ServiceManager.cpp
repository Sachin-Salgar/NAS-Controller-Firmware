/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : ServiceManager.cpp
 *
 ******************************************************************************/

#include "ServiceManager.h"

#include "ConfigurationService.h"
#include "StatisticsService.h"
#include "SystemService.h"
#include "RelayService.h"
#include "FanService.h"
#include "TemperatureService.h"
#include "LedService.h"
#include "DriveService.h"
#include "USBService.h"
#include "ProtocolService.h"
#include "HealthService.h"
#include "EventService.h"
#include "SchedulerService.h"

using namespace NAS::Core;

namespace NAS::Services
{

bool ServiceManager::initialized_ = false;

Result ServiceManager::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    Result result;

    result = ConfigurationService::Initialize();
    if (!result.IsSuccess()) return result;

    result = StatisticsService::Initialize();
    if (!result.IsSuccess()) return result;

    result = SystemService::Initialize();
    if (!result.IsSuccess()) return result;

    result = RelayService::Initialize();
    if (!result.IsSuccess()) return result;

    result = FanService::Initialize();
    if (!result.IsSuccess()) return result;

    result = TemperatureService::Initialize();
    if (!result.IsSuccess()) return result;

    result = LedService::Initialize();
    if (!result.IsSuccess()) return result;

    result = DriveService::Initialize();
    if (!result.IsSuccess()) return result;

    result = USBService::Initialize();
    if (!result.IsSuccess()) return result;

    result = ProtocolService::Initialize();
    if (!result.IsSuccess()) return result;

    result = HealthService::Initialize();
    if (!result.IsSuccess()) return result;

    result = EventService::Initialize();
    if (!result.IsSuccess()) return result;

    result = SchedulerService::Initialize();
    if (!result.IsSuccess()) return result;

    initialized_ = true;

    return Result::Ok();
}

Result ServiceManager::Start() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    StatisticsService::IncrementBootCount();

    auto result = SystemService::SetState(
        NAS::Objects::SystemState::Ready);

    if (!result.IsSuccess())
    {
        return result;
    }

    result = EventService::Publish(
        {
            EventType::SystemStarted,
            0U,
            0U
        });

    if (!result.IsSuccess())
    {
        return result;
    }

    return SchedulerService::Start();
}

Result ServiceManager::Stop() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    auto result = EventService::Publish(
        {
            EventType::SystemShutdown,
            0U,
            0U
        });

    if (!result.IsSuccess())
    {
        return result;
    }

    result = SystemService::SetState(
        NAS::Objects::SystemState::Shutdown);

    if (!result.IsSuccess())
    {
        return result;
    }

    return SchedulerService::Stop();
}

Result ServiceManager::Execute() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return SchedulerService::Execute();
}

} // namespace NAS::Services