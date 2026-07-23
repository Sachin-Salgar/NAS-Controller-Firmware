/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : StatisticsService.cpp
 *
 ******************************************************************************/

#include "StatisticsService.h"

#include "FanService.h"
#include "SystemService.h"
#include "../Config/TaskConfig.h"

using namespace NAS::Core;

namespace NAS::Services
{

NAS::Objects::Statistics
StatisticsService::statistics_;

bool StatisticsService::initialized_ = false;

Result StatisticsService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    auto result = statistics_.Initialize();

    if (!result.IsSuccess())
    {
        return result;
    }

    initialized_ = true;

    return Result::Ok();
}

void StatisticsService::IncrementBootCount() noexcept
{
    statistics_.IncrementBootCount();
}

void StatisticsService::IncrementUsbPacketsReceived() noexcept
{
    statistics_.IncrementUsbPacketsReceived();
}

void StatisticsService::IncrementUsbPacketsTransmitted() noexcept
{
    statistics_.IncrementUsbPacketsTransmitted();
}

void StatisticsService::IncrementProtocolErrors() noexcept
{
    statistics_.IncrementProtocolErrors();
}

void StatisticsService::IncrementRelayCycles(std::uint8_t relayId) noexcept
{
    statistics_.IncrementRelayCycles(relayId);
}

void StatisticsService::AddFanRuntime(std::uint32_t seconds) noexcept
{
    statistics_.AddFanRuntime(seconds);
}

void StatisticsService::AddSystemUptime(std::uint32_t seconds) noexcept
{
    statistics_.AddSystemUptime(seconds);
}

Result StatisticsService::Update() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    constexpr std::uint32_t IntervalSeconds =
        NAS::Config::Tasks::StatisticsTaskIntervalMs / 1000U;

    statistics_.AddSystemUptime(IntervalSeconds);

    auto statusResult = SystemService::SetUptime(
        static_cast<std::uint64_t>(statistics_.GetSystemUptime()) * 1000ULL);

    if (!statusResult.IsSuccess())
    {
        return statusResult;
    }

    for (std::uint8_t fanId = 1U;
         fanId <= FanService::FanCount;
         ++fanId)
    {
        if (FanService::GetFan(fanId).GetSpeed() > 0U)
        {
            statistics_.AddFanRuntime(IntervalSeconds);
            break;
        }
    }

    return Result::Ok();
}

NAS::Objects::Statistics&
StatisticsService::GetStatistics() noexcept
{
    return statistics_;
}

} // namespace NAS::Services