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

#include "TestResult.h"
#include "TestFormatter.h"
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
    if (relayConfig.enabled == false)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    auto& fanConfig = config.Fan();
    if (fanConfig.minimumSpeed > fanConfig.maximumSpeed)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    auto& tempConfig = config.Temperature();
    if (tempConfig.warningThreshold == 0.0F)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    auto& ledConfig = config.Led();
    if (ledConfig.brightness == 0)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

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

    auto result = led.Initialize(0);
    if (!result)
    {
        return result;
    }

    if (led.GetIndex() != 0)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (led.GetMode() != NAS::Objects::LedMode::Off)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = led.SetColor(255, 128, 64);
    if (!result)
    {
        return result;
    }

    result = led.SetMode(NAS::Objects::LedMode::Solid);
    if (!result)
    {
        return result;
    }

    if (led.GetMode() != NAS::Objects::LedMode::Solid)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = led.TurnOff();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestRelay() noexcept
{
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

    if (status.GetState() != NAS::Objects::SystemState::Booting)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = status.SetState(NAS::Objects::SystemState::Ready);
    if (!result)
    {
        return result;
    }

    if (status.GetState() != NAS::Objects::SystemState::Ready)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = status.SetFreeHeap(100000);
    if (!result)
    {
        return result;
    }

    if (status.GetFreeHeap() != 100000)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestTemperatureSensor() noexcept
{
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
LayerResult TestObjects() noexcept
{
    NAS::Core::Logger logger;
    (void)logger.Initialize();

    LayerResult layerResult = {NAS::Core::Result::Ok(), nullptr,
        NAS::Core::ResultCode::Success, 0, 0, 0};

    TestFormatter::PrintHeader("OBJECTS");

    auto result = TestConfiguration();
    if (!result)
    {
        TestFormatter::PrintFail("Configuration");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Configuration";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Configuration");
        layerResult.passCount++;
    }

    result = TestDrive();
    if (!result)
    {
        TestFormatter::PrintFail("Drive");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Drive";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Drive");
        layerResult.passCount++;
    }

    result = TestFan();
    if (!result)
    {
        TestFormatter::PrintFail("Fan");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Fan";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Fan");
        layerResult.passCount++;
    }

    result = TestLed();
    if (!result)
    {
        TestFormatter::PrintFail("Led");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Led";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Led");
        layerResult.passCount++;
    }

    result = TestRelay();
    if (!result)
    {
        TestFormatter::PrintFail("Relay");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Relay";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintSkipped("Relay");
        layerResult.skippedCount++;
    }

    result = TestStatistics();
    if (!result)
    {
        TestFormatter::PrintFail("Statistics");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Statistics";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Statistics");
        layerResult.passCount++;
    }

    result = TestSystemStatus();
    if (!result)
    {
        TestFormatter::PrintFail("SystemStatus");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "SystemStatus";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("SystemStatus");
        layerResult.passCount++;
    }

    result = TestTemperatureSensor();
    if (!result)
    {
        TestFormatter::PrintFail("TemperatureSensor");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "TemperatureSensor";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintSkipped("TemperatureSensor");
        layerResult.skippedCount++;
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
