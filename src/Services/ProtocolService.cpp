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
#include "../Protocol/CommandDispatcher.h"
#include "../Protocol/ResponseBuilder.h"
#include "../Protocol/Commands.h"
#include "../Config/ProtocolConfig.h"

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

    std::uint8_t responsePayload[NAS::Config::Protocol::MaximumPayloadSize]{};
    std::uint16_t responsePayloadLength = 0U;

    result = NAS::Protocol::CommandDispatcher::Dispatch(
        parser.GetCommand(),
        parser.GetPayload(),
        parser.GetPayloadLength(),
        responsePayload,
        sizeof(responsePayload),
        responsePayloadLength);

    std::uint8_t responsePacket[NAS::Config::Protocol::MaximumPacketSize]{};
    std::size_t responsePacketLength = 0U;

    auto buildResult = result.IsSuccess()
        ? NAS::Protocol::ResponseBuilder::BuildSuccess(
            parser.GetSequence(),
            parser.GetCommand(),
            responsePayload,
            responsePayloadLength,
            responsePacket,
            sizeof(responsePacket),
            responsePacketLength)
        : NAS::Protocol::ResponseBuilder::BuildError(
            parser.GetSequence(),
            parser.GetCommand(),
            result.Code(),
            responsePacket,
            sizeof(responsePacket),
            responsePacketLength);

    if (!buildResult.IsSuccess())
    {
        StatisticsService::IncrementProtocolErrors();
        return buildResult;
    }

    auto sendResult = USBService::Send(
        responsePacket,
        responsePacketLength);

    if (!sendResult.IsSuccess())
    {
        return sendResult;
    }

    if (!result.IsSuccess())
    {
        StatisticsService::IncrementProtocolErrors();
    }

    return result;
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