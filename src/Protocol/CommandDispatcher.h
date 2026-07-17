#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : CommandDispatcher.h
 *
 * Description:
 * Dispatches validated protocol commands to Services.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Protocol
{

class CommandDispatcher
{
public:

    [[nodiscard]]
    static NAS::Core::Result Dispatch(
        std::uint16_t command,
        const std::uint8_t* requestPayload,
        std::uint16_t requestLength,
        std::uint8_t* responsePayload,
        std::uint16_t responseBufferSize,
        std::uint16_t& responseLength) noexcept;

private:

    CommandDispatcher() = delete;

    [[nodiscard]]
    static NAS::Core::Result DispatchSystemCommand(
        std::uint16_t command,
        const std::uint8_t* requestPayload,
        std::uint16_t requestLength,
        std::uint8_t* responsePayload,
        std::uint16_t responseBufferSize,
        std::uint16_t& responseLength) noexcept;

    [[nodiscard]]
    static NAS::Core::Result DispatchRelayCommand(
        std::uint16_t command,
        const std::uint8_t* requestPayload,
        std::uint16_t requestLength,
        std::uint8_t* responsePayload,
        std::uint16_t responseBufferSize,
        std::uint16_t& responseLength) noexcept;

    [[nodiscard]]
    static NAS::Core::Result DispatchFanCommand(
        std::uint16_t command,
        const std::uint8_t* requestPayload,
        std::uint16_t requestLength,
        std::uint8_t* responsePayload,
        std::uint16_t responseBufferSize,
        std::uint16_t& responseLength) noexcept;

    [[nodiscard]]
    static NAS::Core::Result DispatchTemperatureCommand(
        std::uint16_t command,
        const std::uint8_t* requestPayload,
        std::uint16_t requestLength,
        std::uint8_t* responsePayload,
        std::uint16_t responseBufferSize,
        std::uint16_t& responseLength) noexcept;

    [[nodiscard]]
    static NAS::Core::Result DispatchLedCommand(
        std::uint16_t command,
        const std::uint8_t* requestPayload,
        std::uint16_t requestLength,
        std::uint8_t* responsePayload,
        std::uint16_t responseBufferSize,
        std::uint16_t& responseLength) noexcept;

    [[nodiscard]]
    static NAS::Core::Result DispatchDriveCommand(
        std::uint16_t command,
        const std::uint8_t* requestPayload,
        std::uint16_t requestLength,
        std::uint8_t* responsePayload,
        std::uint16_t responseBufferSize,
        std::uint16_t& responseLength) noexcept;
};

} // namespace NAS::Protocol