/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : SystemService.cpp
 *
 ******************************************************************************/

#include "SystemService.h"

using namespace NAS::Core;

namespace NAS::Services
{

NAS::Objects::SystemStatus
SystemService::status_;

bool SystemService::initialized_ = false;

Result SystemService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    auto result = status_.Initialize();

    if (!result.IsSuccess())
    {
        return result;
    }

    initialized_ = true;

    return Result::Ok();
}

Result SystemService::SetState(
    NAS::Objects::SystemState state) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return status_.SetState(state);
}

Result SystemService::SetUsbConnected(
    bool connected) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return status_.SetUsbConnected(connected);
}

Result SystemService::SetFreeHeap(
    std::uint32_t bytes) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return status_.SetFreeHeap(bytes);
}

Result SystemService::SetCpuTemperature(
    float temperature) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return status_.SetCpuTemperature(temperature);
}

Result SystemService::SetUptime(
    std::uint64_t milliseconds) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return status_.SetUptime(milliseconds);
}

NAS::Objects::SystemStatus&
SystemService::GetStatus() noexcept
{
    return status_;
}

} // namespace NAS::Services