/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : CommandDispatcher.cpp
 *
 ******************************************************************************/

#include "CommandDispatcher.h"

#include "Commands.h"

using namespace NAS::Core;

namespace NAS::Protocol
{

Result CommandDispatcher::Dispatch(
    std::uint16_t command,
    const std::uint8_t* requestPayload,
    std::uint16_t requestLength,
    std::uint8_t* responsePayload,
    std::uint16_t responseBufferSize,
    std::uint16_t& responseLength) noexcept
{
    switch (command & 0xFF00U)
    {
        case 0x0000U:
            return DispatchSystemCommand(
                command,
                requestPayload,
                requestLength,
                responsePayload,
                responseBufferSize,
                responseLength);

        case 0x1000U:
            return DispatchRelayCommand(
                command,
                requestPayload,
                requestLength,
                responsePayload,
                responseBufferSize,
                responseLength);

        case 0x1100U:
            return DispatchFanCommand(
                command,
                requestPayload,
                requestLength,
                responsePayload,
                responseBufferSize,
                responseLength);

        case 0x1200U:
            return DispatchTemperatureCommand(
                command,
                requestPayload,
                requestLength,
                responsePayload,
                responseBufferSize,
                responseLength);

        case 0x1300U:
            return DispatchLedCommand(
                command,
                requestPayload,
                requestLength,
                responsePayload,
                responseBufferSize,
                responseLength);

        case 0x1400U:
            return DispatchDriveCommand(
                command,
                requestPayload,
                requestLength,
                responsePayload,
                responseBufferSize,
                responseLength);

        default:
            return Result(ResultCode::NotSupported);
    }
}

Result CommandDispatcher::DispatchSystemCommand(
    std::uint16_t,
    const std::uint8_t*,
    std::uint16_t,
    std::uint8_t*,
    std::uint16_t,
    std::uint16_t& responseLength) noexcept
{
    responseLength = 0U;
    return Result::Ok();
}

Result CommandDispatcher::DispatchRelayCommand(
    std::uint16_t,
    const std::uint8_t*,
    std::uint16_t,
    std::uint8_t*,
    std::uint16_t,
    std::uint16_t& responseLength) noexcept
{
    responseLength = 0U;
    return Result::Ok();
}

Result CommandDispatcher::DispatchFanCommand(
    std::uint16_t,
    const std::uint8_t*,
    std::uint16_t,
    std::uint8_t*,
    std::uint16_t,
    std::uint16_t& responseLength) noexcept
{
    responseLength = 0U;
    return Result::Ok();
}

Result CommandDispatcher::DispatchTemperatureCommand(
    std::uint16_t,
    const std::uint8_t*,
    std::uint16_t,
    std::uint8_t*,
    std::uint16_t,
    std::uint16_t& responseLength) noexcept
{
    responseLength = 0U;
    return Result::Ok();
}

Result CommandDispatcher::DispatchLedCommand(
    std::uint16_t,
    const std::uint8_t*,
    std::uint16_t,
    std::uint8_t*,
    std::uint16_t,
    std::uint16_t& responseLength) noexcept
{
    responseLength = 0U;
    return Result::Ok();
}

Result CommandDispatcher::DispatchDriveCommand(
    std::uint16_t,
    const std::uint8_t*,
    std::uint16_t,
    std::uint8_t*,
    std::uint16_t,
    std::uint16_t& responseLength) noexcept
{
    responseLength = 0U;
    return Result::Ok();
}

} // namespace NAS::Protocol