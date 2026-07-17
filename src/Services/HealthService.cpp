/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : HealthService.cpp
 *
 ******************************************************************************/

#include "HealthService.h"

#include "../Platform/Watchdog.h"

#include "SystemService.h"
#include "TemperatureService.h"

using namespace NAS::Core;

namespace NAS::Services
{

bool HealthService::initialized_ = false;

bool HealthService::healthy_ = true;

Result HealthService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    auto result = NAS::Platform::Watchdog::Initialize();

    if (!result.IsSuccess())
    {
        return result;
    }

    initialized_ = true;

    healthy_ = true;

    return Result::Ok();
}

Result HealthService::Update() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    healthy_ = true;

    for (std::uint8_t i = 0U;
         i < TemperatureService::SensorCount;
         ++i)
    {
        if (TemperatureService::IsAlarmActive(i))
        {
            healthy_ = false;
            break;
        }
    }

    if (healthy_)
    {
        SystemService::SetState(
            NAS::Objects::SystemState::Ready);
    }
    else
    {
        SystemService::SetState(
            NAS::Objects::SystemState::Warning);
    }

    NAS::Platform::Watchdog::Feed();

    return Result::Ok();
}

bool HealthService::IsHealthy() noexcept
{
    return healthy_;
}

} // namespace NAS::Services