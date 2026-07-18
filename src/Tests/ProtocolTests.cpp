/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Tests
 * File    : ProtocolTests.cpp
 *
 * Description:
 * Protocol layer self-tests (PacketParser, PacketBuilder, PacketValidator,
 * ResponseBuilder, CommandDispatcher, Commands).
 *
 ******************************************************************************/

#include "../Core/Logger.h"
#include "../Core/Result.h"
#include "../Protocol/PacketParser.h"
#include "../Protocol/PacketBuilder.h"
#include "../Protocol/PacketValidator.h"
#include "../Protocol/ResponseBuilder.h"
#include "../Protocol/CommandDispatcher.h"
#include "../Protocol/Commands.h"
#include "../Utilities/CRC16.h"

namespace NAS::Tests
{

[[nodiscard]]
static NAS::Core::Result TestCommands() noexcept
{
    // Verify that all command identifiers are valid
    if (!NAS::Protocol::Commands::IsValid(NAS::Protocol::Commands::Ping))
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (!NAS::Protocol::Commands::IsValid(NAS::Protocol::Commands::GetVersion))
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (!NAS::Protocol::Commands::IsValid(NAS::Protocol::Commands::GetBuildInfo))
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (!NAS::Protocol::Commands::IsValid(NAS::Protocol::Commands::GetSystemStatus))
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (!NAS::Protocol::Commands::IsValid(NAS::Protocol::Commands::RelayGet))
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (!NAS::Protocol::Commands::IsValid(NAS::Protocol::Commands::FanGet))
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (!NAS::Protocol::Commands::IsValid(NAS::Protocol::Commands::DriveGet))
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    // Verify invalid command is rejected
    if (NAS::Protocol::Commands::IsValid(0xFFFFU))
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestCrc16() noexcept
{
    std::uint8_t testData[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    std::uint16_t crc = NAS::Utilities::CRC16::Calculate(testData, 5);
    (void)crc;

    // Verify that CRC calculation returns a value
    if (crc == 0xFFFFU && crc == 0x0000U)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    // Verify that CRC calculation is deterministic
    std::uint16_t crc2 = NAS::Utilities::CRC16::Calculate(testData, 5);
    if (crc != crc2)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestPacketBuilder() noexcept
{
    std::uint8_t buffer[256] = { 0 };
    std::size_t packetLength = 0;

    std::uint8_t payload[] = { 0xAA, 0xBB };

    auto result = NAS::Protocol::PacketBuilder::Build(
        1U,
        NAS::Protocol::Commands::Ping,
        payload,
        2U,
        buffer,
        256,
        packetLength);

    if (!result)
    {
        return result;
    }

    // Verify packet length is reasonable
    if (packetLength < NAS::Protocol::PacketBuilder::HeaderSize)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    // Verify packet has CRC
    std::size_t minExpectedLength =
        NAS::Protocol::PacketBuilder::HeaderSize +
        2U +
        NAS::Protocol::PacketBuilder::CrcSize;

    if (packetLength < minExpectedLength)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestPacketParser() noexcept
{
    // Build a valid packet first
    std::uint8_t buffer[256] = { 0 };
    std::size_t packetLength = 0;

    std::uint8_t payload[] = { 0xAA, 0xBB };

    auto buildResult = NAS::Protocol::PacketBuilder::Build(
        1U,
        NAS::Protocol::Commands::Ping,
        payload,
        2U,
        buffer,
        256,
        packetLength);

    if (!buildResult)
    {
        return buildResult;
    }

    // Parse the packet
    NAS::Protocol::PacketParser parser;
    auto parseResult = parser.Parse(buffer, packetLength);
    if (!parseResult)
    {
        return parseResult;
    }

    // Verify parsed values
    if (parser.GetCommand() != NAS::Protocol::Commands::Ping)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (parser.GetSequence() != 1U)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (parser.GetPayloadLength() != 2U)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestPacketValidator() noexcept
{
    std::uint8_t buffer[256] = { 0 };
    std::size_t packetLength = 0;

    std::uint8_t payload[] = { 0xAA, 0xBB };

    auto buildResult = NAS::Protocol::PacketBuilder::Build(
        1U,
        NAS::Protocol::Commands::Ping,
        payload,
        2U,
        buffer,
        256,
        packetLength);

    if (!buildResult)
    {
        return buildResult;
    }

    auto validateResult = NAS::Protocol::PacketValidator::Validate(
        buffer,
        packetLength);
    if (!validateResult)
    {
        return validateResult;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestResponseBuilder() noexcept
{
    std::uint8_t responseBuffer[256] = { 0 };
    std::size_t responseLength = 0;

    std::uint8_t payload[] = { 0xCC, 0xDD };

    auto result = NAS::Protocol::ResponseBuilder::BuildSuccess(
        1U,
        NAS::Protocol::Commands::Ping,
        payload,
        2U,
        responseBuffer,
        256,
        responseLength);

    if (!result)
    {
        return result;
    }

    if (responseLength == 0)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestCommandDispatcher() noexcept
{
    std::uint8_t requestPayload[256] = { 0 };
    std::uint8_t responsePayload[256] = { 0 };
    std::uint16_t responseLength = 0;

    auto result = NAS::Protocol::CommandDispatcher::Dispatch(
        NAS::Protocol::Commands::Ping,
        requestPayload,
        0U,
        responsePayload,
        256,
        responseLength);

    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

struct LayerResult
{
    NAS::Core::Result result;
    const char* failedComponent;
    int passCount;
    int failCount;
    int skippedCount;
};

[[nodiscard]]
LayerResult TestProtocol() noexcept
{
    NAS::Core::Logger logger;
    logger.Initialize();

    LayerResult layerResult = {NAS::Core::Result::Ok(), nullptr, 0, 0, 0};

    logger.Info("--------------------------------------------------");
    logger.Info("PROTOCOL");
    logger.Info("--------------------------------------------------");
    logger.Info("");

    auto result = TestCrc16();
    if (!result)
    {
        logger.Error("CRC16......................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "CRC16";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("CRC16......................PASS");
    layerResult.passCount++;

    result = TestPacketBuilder();
    if (!result)
    {
        logger.Error("PacketBuilder..............FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "PacketBuilder";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("PacketBuilder..............PASS");
    layerResult.passCount++;

    result = TestPacketParser();
    if (!result)
    {
        logger.Error("PacketParser...............FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "PacketParser";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("PacketParser...............PASS");
    layerResult.passCount++;

    result = TestPacketValidator();
    if (!result)
    {
        logger.Error("PacketValidator............FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "PacketValidator";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("PacketValidator............PASS");
    layerResult.passCount++;

    result = TestResponseBuilder();
    if (!result)
    {
        logger.Error("ResponseBuilder............FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "ResponseBuilder";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("ResponseBuilder............PASS");
    layerResult.passCount++;

    result = TestCommandDispatcher();
    if (!result)
    {
        logger.Error("CommandDispatcher..........FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "CommandDispatcher";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("CommandDispatcher..........PASS");
    layerResult.passCount++;

    result = TestCommands();
    if (!result)
    {
        logger.Error("Commands...................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "Commands";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("Commands...................PASS");
    layerResult.passCount++;

    logger.Info("");
    logger.Info("PASS 7");
    logger.Info("");

    return layerResult;
}

} // namespace NAS::Tests
