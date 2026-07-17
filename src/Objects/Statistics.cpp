/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Statistics.cpp
 *
 ******************************************************************************/

#include "Statistics.h"

using namespace NAS::Core;

namespace NAS::Objects
{

Result Statistics::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    Reset();

    initialized_ = true;

    return Result::Ok();
}

void Statistics::IncrementBootCount() noexcept
{
    ++bootCount_;
}

void Statistics::IncrementUsbPacketsReceived() noexcept
{
    ++usbPacketsReceived_;
}

void Statistics::IncrementUsbPacketsTransmitted() noexcept
{
    ++usbPacketsTransmitted_;
}

void Statistics::IncrementProtocolErrors() noexcept
{
    ++protocolErrors_;
}

void Statistics::IncrementRelayCycles(std::uint8_t relayId) noexcept
{
    if ((relayId == 0U) || (relayId > RelayCount))
    {
        return;
    }

    ++relayCycles_[relayId - 1U];
}

void Statistics::AddFanRuntime(std::uint32_t seconds) noexcept
{
    fanRuntimeSeconds_ += seconds;
}

void Statistics::AddSystemUptime(std::uint32_t seconds) noexcept
{
    systemUptimeSeconds_ += seconds;
}

std::uint32_t Statistics::GetBootCount() const noexcept
{
    return bootCount_;
}

std::uint32_t Statistics::GetUsbPacketsReceived() const noexcept
{
    return usbPacketsReceived_;
}

std::uint32_t Statistics::GetUsbPacketsTransmitted() const noexcept
{
    return usbPacketsTransmitted_;
}

std::uint32_t Statistics::GetProtocolErrors() const noexcept
{
    return protocolErrors_;
}

std::uint32_t Statistics::GetRelayCycles(std::uint8_t relayId) const noexcept
{
    if ((relayId == 0U) || (relayId > RelayCount))
    {
        return 0U;
    }

    return relayCycles_[relayId - 1U];
}

std::uint32_t Statistics::GetFanRuntime() const noexcept
{
    return fanRuntimeSeconds_;
}

std::uint32_t Statistics::GetSystemUptime() const noexcept
{
    return systemUptimeSeconds_;
}

Result Statistics::Reset() noexcept
{
    bootCount_ = 0U;
    usbPacketsReceived_ = 0U;
    usbPacketsTransmitted_ = 0U;
    protocolErrors_ = 0U;
    fanRuntimeSeconds_ = 0U;
    systemUptimeSeconds_ = 0U;

    for (auto& relay : relayCycles_)
    {
        relay = 0U;
    }

    return Result::Ok();
}

} // namespace NAS::Objects