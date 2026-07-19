/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : SystemStatus.cpp
 *
 ******************************************************************************/

#include "SystemStatus.h"

using namespace NAS::Core;

namespace NAS::Objects
{

Result SystemStatus::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    state_ = SystemState::Booting;
    usbConnected_ = false;
    freeHeap_ = 0U;
    cpuTemperature_ = 0.0F;
    uptime_ = 0U;

    initialized_ = true;

    return Result::Ok();
}

Result SystemStatus::SetState(SystemState state) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    state_ = state;

    return Result::Ok();
}

Result SystemStatus::SetUsbConnected(bool connected) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    usbConnected_ = connected;

    return Result::Ok();
}

Result SystemStatus::SetFreeHeap(std::uint32_t bytes) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    freeHeap_ = bytes;

    return Result::Ok();
}

Result SystemStatus::SetCpuTemperature(float temperature) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    cpuTemperature_ = temperature;

    return Result::Ok();
}

Result SystemStatus::SetUptime(std::uint64_t milliseconds) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    uptime_ = milliseconds;

    return Result::Ok();
}

SystemState SystemStatus::GetState() const noexcept
{
    return state_;
}

bool SystemStatus::IsUsbConnected() const noexcept
{
    return usbConnected_;
}

std::uint32_t SystemStatus::GetFreeHeap() const noexcept
{
    return freeHeap_;
}

float SystemStatus::GetCpuTemperature() const noexcept
{
    return cpuTemperature_;
}

std::uint64_t SystemStatus::GetUptime() const noexcept
{
    return uptime_;
}

} // namespace NAS::Objects