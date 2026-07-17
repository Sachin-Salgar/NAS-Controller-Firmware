/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : ProtocolService.cpp
 *
 ******************************************************************************/

#include "ProtocolService.h"

#include "../Protocol/PacketParser.h"
#include "../Protocol/PacketBuilder.h"
#include "../Protocol/Commands.h"

#include "USBService.h"
#include "StatisticsService.h"

using namespace NAS::Core;

namespace NAS::Services
{

bool ProtocolService::initialized_ = false;

Result ProtocolService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    initialized_ = true;

    return Result::Ok();
}

Result ProtocolService::Process() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    std::uint8_t buffer[512]{};

    std::size_t bytesReceived = 0U;

    auto result =
        USBService::Receive(
            buffer,
            sizeof(buffer),
            bytesReceived);

    if (!result.IsSuccess())
    {
        return result;
    }

    if (bytesReceived == 0U)
    {
        return Result::Ok();
    }

    return ProcessPacket(
        buffer,
        bytesReceived);
}

Result ProtocolService::ProcessPacket(
    const std::uint8_t* packet,
    std::size_t length) noexcept
{
    if ((packet == nullptr) || (length == 0U))
    {
        return Result(ResultCode::InvalidArgument);
    }

    NAS::Protocol::PacketParser parser;

    auto result =
        parser.Parse(
            packet,
            length);

    if (!result.IsSuccess())
    {
        StatisticsService::IncrementProtocolErrors();

        return result;
    }

    return ExecuteCommand(
        parser.GetCommand(),
        parser.GetPayload(),
        parser.GetPayloadLength());
}

Result ProtocolService::ExecuteCommand(
    std::uint16_t command,
    const std::uint8_t* payload,
    std::size_t payloadLength) noexcept
{
    (void)payload;
    (void)payloadLength;

    switch (command)
    {
        case NAS::Protocol::Commands::Ping:
        {
            return Result::Ok();
        }

        default:
        {
            StatisticsService::IncrementProtocolErrors();

            return Result(ResultCode::NotSupported);
        }
    }
}

} // namespace NAS::Services