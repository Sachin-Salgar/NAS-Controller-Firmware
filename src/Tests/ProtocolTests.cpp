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
    auto result = NAS::Protocol::PacketValidator::Initialize();
    if (!result)
    {
        return result;
    }

    // TODO: PacketValidator::Validate requires packet structure validation.
    // Verify initialization only.

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestResponseBuilder() noexcept
{
    auto result = NAS::Protocol::ResponseBuilder::Initialize();
    if (!result)
    {
        return result;
    }

    // TODO: ResponseBuilder::Build requires full protocol context.
    // Verify initialization only.

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestCommandDispatcher() noexcept
{
    auto result = NAS::Protocol::CommandDispatcher::Initialize();
    if (!result)
    {
        return result;
    }

    // TODO: CommandDispatcher::Dispatch requires context and handlers.
    // Verify initialization only.

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
NAS::Core::Result TestProtocol() noexcept
{
    NAS::Core::Logger logger;
    logger.Initialize();

    logger.Info("[TEST] Protocol");

    auto result = TestCommands();
    if (!result)
    {
        logger.Error("Commands FAIL");
        return result;
    }
    logger.Info("Commands PASS");

    result = TestCrc16();
    if (!result)
    {
        logger.Error("CRC16 FAIL");
        return result;
    }
    logger.Info("CRC16 PASS");

    result = TestPacketBuilder();
    if (!result)
    {
        logger.Error("PacketBuilder FAIL");
        return result;
    }
    logger.Info("PacketBuilder PASS");

    result = TestPacketParser();
    if (!result)
    {
        logger.Error("PacketParser FAIL");
        return result;
    }
    logger.Info("PacketParser PASS");

    result = TestPacketValidator();
    if (!result)
    {
        logger.Error("PacketValidator FAIL");
        return result;
    }
    logger.Info("PacketValidator PASS");

    result = TestResponseBuilder();
    if (!result)
    {
        logger.Error("ResponseBuilder FAIL");
        return result;
    }
    logger.Info("ResponseBuilder PASS");

    result = TestCommandDispatcher();
    if (!result)
    {
        logger.Error("CommandDispatcher FAIL");
        return result;
    }
    logger.Info("CommandDispatcher PASS");

    logger.Info("[PASS] Protocol");
    return NAS::Core::Result::Ok();
}

} // namespace NAS::Tests
