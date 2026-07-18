/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Tests
 * File    : ObjectTests.cpp
 *
 * Description:
 * Objects layer self-tests (Configuration, Drive, Fan, Led, Relay,
 * Statistics, SystemStatus, TemperatureSensor).
 *
 ******************************************************************************/

#include "../Core/Logger.h"
#include "../Core/Result.h"
#include "../Objects/Configuration.h"
#include "../Objects/Drive.h"
#include "../Objects/Fan.h"
#include "../Objects/Led.h"
#include "../Objects/Relay.h"
#include "../Objects/Statistics.h"
#include "../Objects/SystemStatus.h"
#include "../Objects/TemperatureSensor.h"

namespace NAS::Tests
{

[[nodiscard]]
static NAS::Core::Result TestConfiguration() noexcept
{
    NAS::Objects::Configuration config;

    auto result = config.Initialize();
    if (!result)
    {
        return result;
    }

    auto& relayConfig = config.Relay();
    (void)relayConfig;

    auto& fanConfig = config.Fan();
    (void)fanConfig;

    auto& tempConfig = config.Temperature();
    (void)tempConfig;

    auto& ledConfig = config.Led();
    (void)ledConfig;

    // TODO: Configuration::Load and Save require flash storage.
    // Test ResetToDefaults to verify configuration state management.
    result = config.ResetToDefaults();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestDrive() noexcept
{
    NAS::Objects::Drive drive;

    auto result = drive.Initialize(0);
    if (!result)
    {
        return result;
    }

    if (drive.GetId() != 0)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (drive.IsPresent())
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (drive.GetState() != NAS::Objects::DriveState::Offline)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (drive.GetHealth() != NAS::Objects::DriveHealth::Unknown)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = drive.SetPresent(true);
    if (!result)
    {
        return result;
    }

    if (!drive.IsPresent())
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = drive.SetState(NAS::Objects::DriveState::Idle);
    if (!result)
    {
        return result;
    }

    if (drive.GetState() != NAS::Objects::DriveState::Idle)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = drive.SetTemperature(35.5F);
    if (!result)
    {
        return result;
    }

    result = drive.SetHealth(NAS::Objects::DriveHealth::Good);
    if (!result)
    {
        return result;
    }

    if (drive.GetHealth() != NAS::Objects::DriveHealth::Good)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = drive.SetCapacity(1000000000000ULL);
    if (!result)
    {
        return result;
    }

    result = drive.SetSerialNumber("DRIVE-001");
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestFan() noexcept
{
    NAS::Objects::Fan fan;

    auto result = fan.Initialize(0, 12, 5000, 8);
    if (!result)
    {
        return result;
    }

    if (fan.GetChannel() != 0)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (fan.GetSpeed() != 0)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (fan.GetMode() != NAS::Objects::FanMode::Automatic)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = fan.SetSpeed(50);
    if (!result)
    {
        return result;
    }

    if (fan.GetSpeed() != 50)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = fan.SetMode(NAS::Objects::FanMode::Manual);
    if (!result)
    {
        return result;
    }

    if (fan.GetMode() != NAS::Objects::FanMode::Manual)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestLed() noexcept
{
    NAS::Objects::Led led;

    auto result = led.Initialize(0, 13);
    if (!result)
    {
        return result;
    }

    // TODO: Led object may have state getters that can be verified.
    // Inspect implementation to determine available APIs.

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestRelay() noexcept
{
    NAS::Objects::Relay relay;

    auto result = relay.Initialize(0, 14);
    if (!result)
    {
        return result;
    }

    // TODO: Relay object state management depends on implementation.
    // Test what APIs are available in the actual Relay class.

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestStatistics() noexcept
{
    NAS::Objects::Statistics stats;

    auto result = stats.Initialize();
    if (!result)
    {
        return result;
    }

    if (stats.GetBootCount() != 0)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    stats.IncrementBootCount();
    if (stats.GetBootCount() != 1)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    stats.IncrementUsbPacketsReceived();
    if (stats.GetUsbPacketsReceived() != 1)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    stats.IncrementUsbPacketsTransmitted();
    if (stats.GetUsbPacketsTransmitted() != 1)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    stats.IncrementProtocolErrors();
    if (stats.GetProtocolErrors() != 1)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    stats.IncrementRelayCycles(0);
    if (stats.GetRelayCycles(0) != 1)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    stats.AddFanRuntime(100);
    if (stats.GetFanRuntime() != 100)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    stats.AddSystemUptime(200);
    if (stats.GetSystemUptime() != 200)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = stats.Reset();
    if (!result)
    {
        return result;
    }

    if (stats.GetBootCount() != 0)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestSystemStatus() noexcept
{
    NAS::Objects::SystemStatus status;

    auto result = status.Initialize();
    if (!result)
    {
        return result;
    }

    // TODO: SystemStatus behavior depends on implementation.
    // Verify initialization only or test available getters.

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestTemperatureSensor() noexcept
{
    NAS::Objects::TemperatureSensor sensor;

    auto result = sensor.Initialize(0);
    if (!result)
    {
        return result;
    }

    // TODO: TemperatureSensor behavior depends on implementation.
    // Verify initialization and state management.

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
NAS::Core::Result TestObjects() noexcept
{
    NAS::Core::Logger logger;
    logger.Initialize();

    logger.Info("[TEST] Objects");

    auto result = TestConfiguration();
    if (!result)
    {
        logger.Error("Configuration FAIL");
        return result;
    }
    logger.Info("Configuration PASS");

    result = TestDrive();
    if (!result)
    {
        logger.Error("Drive FAIL");
        return result;
    }
    logger.Info("Drive PASS");

    result = TestFan();
    if (!result)
    {
        logger.Error("Fan FAIL");
        return result;
    }
    logger.Info("Fan PASS");

    result = TestLed();
    if (!result)
    {
        logger.Error("Led FAIL");
        return result;
    }
    logger.Info("Led PASS");

    result = TestRelay();
    if (!result)
    {
        logger.Error("Relay FAIL");
        return result;
    }
    logger.Info("Relay PASS");

    result = TestStatistics();
    if (!result)
    {
        logger.Error("Statistics FAIL");
        return result;
    }
    logger.Info("Statistics PASS");

    result = TestSystemStatus();
    if (!result)
    {
        logger.Error("SystemStatus FAIL");
        return result;
    }
    logger.Info("SystemStatus PASS");

    result = TestTemperatureSensor();
    if (!result)
    {
        logger.Error("TemperatureSensor FAIL");
        return result;
    }
    logger.Info("TemperatureSensor PASS");

    logger.Info("[PASS] Objects");
    return NAS::Core::Result::Ok();
}

} // namespace NAS::Tests
