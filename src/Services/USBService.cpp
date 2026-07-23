/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : USBService.cpp
 *
 ******************************************************************************/

#include "USBService.h"

#include "../Platform/USB.h"
#include "StatisticsService.h"
#include "SystemService.h"

using namespace NAS::Core;

namespace NAS::Services
{

bool USBService::initialized_ = false;

Result USBService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    auto result = NAS::Platform::USB::Initialize();

    if (!result.IsSuccess())
    {
        return result;
    }

    initialized_ = true;

    return Result::Ok();
}

Result USBService::Process() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return SystemService::SetUsbConnected(
        NAS::Platform::USB::IsConnected());
}

Result USBService::Send(const std::uint8_t* data,
                        std::size_t length) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    auto result =
        NAS::Platform::USB::Write(
            data,
            length);

    if (result.IsSuccess())
    {
        StatisticsService::IncrementUsbPacketsTransmitted();
    }

    return result;
}

Result USBService::Receive(std::uint8_t* data,
                           std::size_t bufferSize,
                           std::size_t& bytesReceived) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    auto result =
        NAS::Platform::USB::Read(
            data,
            bufferSize,
            bytesReceived);

    if (result.IsSuccess() && (bytesReceived > 0U))
    {
        StatisticsService::IncrementUsbPacketsReceived();
    }

    return result;
}

bool USBService::IsConnected() noexcept
{
    if (!initialized_)
    {
        return false;
    }

    return NAS::Platform::USB::IsConnected();
}

} // namespace NAS::Services