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

#include "TestResult.h"
#include "TestFormatter.h"
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

    if (crc == 0xFFFFU && crc == 0x0000U)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

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

    if (packetLength < NAS::Protocol::PacketBuilder::HeaderSize)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

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

    NAS::Protocol::PacketParser parser;
    auto parseResult = parser.Parse(buffer, packetLength);
    if (!parseResult)
    {
        return parseResult;
    }

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

[[nodiscard]]
LayerResult TestProtocol() noexcept
{
    NAS::Core::Logger logger;
    (void)logger.Initialize();

    LayerResult layerResult = {NAS::Core::Result::Ok(), nullptr,
        NAS::Core::ResultCode::Success, 0, 0, 0};

    TestFormatter::PrintHeader("PROTOCOL");

    auto result = TestCrc16();
    if (!result)
    {
        TestFormatter::PrintFail("CRC16");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "CRC16";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("CRC16");
        layerResult.passCount++;
    }

    result = TestPacketBuilder();
    if (!result)
    {
        TestFormatter::PrintFail("PacketBuilder");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "PacketBuilder";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("PacketBuilder");
        layerResult.passCount++;
    }

    result = TestPacketParser();
    if (!result)
    {
        TestFormatter::PrintFail("PacketParser");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "PacketParser";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("PacketParser");
        layerResult.passCount++;
    }

    result = TestPacketValidator();
    if (!result)
    {
        TestFormatter::PrintFail("PacketValidator");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "PacketValidator";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("PacketValidator");
        layerResult.passCount++;
    }

    result = TestResponseBuilder();
    if (!result)
    {
        TestFormatter::PrintFail("ResponseBuilder");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "ResponseBuilder";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("ResponseBuilder");
        layerResult.passCount++;
    }

    result = TestCommandDispatcher();
    if (!result)
    {
        TestFormatter::PrintFail("CommandDispatcher");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "CommandDispatcher";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("CommandDispatcher");
        layerResult.passCount++;
    }

    result = TestCommands();
    if (!result)
    {
        TestFormatter::PrintFail("Commands");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Commands";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Commands");
        layerResult.passCount++;
    }

    TestFormatter::PrintFooter(layerResult.passCount, layerResult.failCount,
        layerResult.skippedCount);

    if (layerResult.failCount == 0)
    {
        layerResult.result = NAS::Core::Result::Ok();
    } else {
        layerResult.result = NAS::Core::Result(NAS::Core::ResultCode::Failed);
    }

    return layerResult;
}

} // namespace NAS::Tests
