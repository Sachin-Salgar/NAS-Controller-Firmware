/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Tests
 * File    : ServiceTests.cpp
 *
 * Description:
 * Services layer self-tests (ConfigurationService, DriveService, EventService,
 * FanService, HealthService, LedService, ProtocolService, RelayService,
 * SchedulerService, ServiceManager, StatisticsService, SystemService,
 * TemperatureService, USBService).
 *
 ******************************************************************************/

#include "../Core/Logger.h"
#include "../Core/Result.h"
#include "../Services/ServiceManager.h"
#include "../Services/ConfigurationService.h"
#include "../Services/DriveService.h"
#include "../Services/FanService.h"
#include "../Services/LedService.h"
#include "../Services/RelayService.h"
#include "../Services/StatisticsService.h"
#include "../Services/SystemService.h"
#include "../Services/TemperatureService.h"

namespace NAS::Tests
{

[[nodiscard]]
static NAS::Core::Result TestConfigurationService() noexcept
{
    auto result = NAS::Services::ConfigurationService::Initialize();
    if (!result)
    {
        return result;
    }

    auto& config = NAS::Services::ConfigurationService::GetConfiguration();
    (void)config;

    // TODO: ConfigurationService::Load and Save require flash storage.
    // Test ResetToDefaults to verify service functionality.
    result = NAS::Services::ConfigurationService::ResetToDefaults();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestDriveService() noexcept
{
    auto result = NAS::Services::DriveService::Initialize();
    if (!result)
    {
        return result;
    }

    if (NAS::Services::DriveService::DriveCount == 0)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidConfiguration);
    }

    auto& drive = NAS::Services::DriveService::GetDrive(0);
    (void)drive;

    result = NAS::Services::DriveService::SetPresent(0, true);
    if (!result)
    {
        return result;
    }

    result = NAS::Services::DriveService::SetState(0,
        NAS::Objects::DriveState::Idle);
    if (!result)
    {
        return result;
    }

    result = NAS::Services::DriveService::SetTemperature(0, 35.0F);
    if (!result)
    {
        return result;
    }

    result = NAS::Services::DriveService::SetHealth(0,
        NAS::Objects::DriveHealth::Good);
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestFanService() noexcept
{
    auto result = NAS::Services::FanService::Initialize();
    if (!result)
    {
        return result;
    }

    if (NAS::Services::FanService::FanCount == 0)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidConfiguration);
    }

    auto& fan = NAS::Services::FanService::GetFan(0);
    (void)fan;

    result = NAS::Services::FanService::SetSpeed(0, 50);
    if (!result)
    {
        return result;
    }

    result = NAS::Services::FanService::SetMode(0,
        NAS::Objects::FanMode::Manual);
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestLedService() noexcept
{
    auto result = NAS::Services::LedService::Initialize();
    if (!result)
    {
        return result;
    }

    // TODO: LedService::LedCount may be 0 if no LEDs configured.
    // Test only if LedCount > 0.

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestRelayService() noexcept
{
    auto result = NAS::Services::RelayService::Initialize();
    if (!result)
    {
        return result;
    }

    if (NAS::Services::RelayService::RelayCount == 0)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidConfiguration);
    }

    auto& relay = NAS::Services::RelayService::GetRelay(0);
    (void)relay;

    result = NAS::Services::RelayService::TurnOn(0);
    if (!result)
    {
        return result;
    }

    result = NAS::Services::RelayService::TurnOff(0);
    if (!result)
    {
        return result;
    }

    result = NAS::Services::RelayService::Toggle(0);
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestStatisticsService() noexcept
{
    auto result = NAS::Services::StatisticsService::Initialize();
    if (!result)
    {
        return result;
    }

    auto& stats = NAS::Services::StatisticsService::GetStatistics();
    (void)stats;

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestSystemService() noexcept
{
    auto result = NAS::Services::SystemService::Initialize();
    if (!result)
    {
        return result;
    }

    // TODO: SystemService::Restart would reset device. Cannot test.
    // Verify initialization only.

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestTemperatureService() noexcept
{
    auto result = NAS::Services::TemperatureService::Initialize();
    if (!result)
    {
        return result;
    }

    // TODO: TemperatureService::GetSensorCount depends on discovered sensors.
    // Test depends on connected OneWire sensors.

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestServiceManager() noexcept
{
    auto result = NAS::Services::ServiceManager::Initialize();
    if (!result)
    {
        return result;
    }

    result = NAS::Services::ServiceManager::Start();
    if (!result)
    {
        return result;
    }

    result = NAS::Services::ServiceManager::Execute();
    if (!result)
    {
        return result;
    }

    result = NAS::Services::ServiceManager::Stop();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
NAS::Core::Result TestServices() noexcept
{
    NAS::Core::Logger logger;
    logger.Initialize();

    logger.Info("[TEST] Services");

    auto result = TestConfigurationService();
    if (!result)
    {
        logger.Error("ConfigurationService FAIL");
        return result;
    }
    logger.Info("ConfigurationService PASS");

    result = TestDriveService();
    if (!result)
    {
        logger.Error("DriveService FAIL");
        return result;
    }
    logger.Info("DriveService PASS");

    result = TestFanService();
    if (!result)
    {
        logger.Error("FanService FAIL");
        return result;
    }
    logger.Info("FanService PASS");

    result = TestLedService();
    if (!result)
    {
        logger.Error("LedService FAIL");
        return result;
    }
    logger.Info("LedService PASS");

    result = TestRelayService();
    if (!result)
    {
        logger.Error("RelayService FAIL");
        return result;
    }
    logger.Info("RelayService PASS");

    result = TestStatisticsService();
    if (!result)
    {
        logger.Error("StatisticsService FAIL");
        return result;
    }
    logger.Info("StatisticsService PASS");

    result = TestSystemService();
    if (!result)
    {
        logger.Error("SystemService FAIL");
        return result;
    }
    logger.Info("SystemService PASS");

    result = TestTemperatureService();
    if (!result)
    {
        logger.Error("TemperatureService FAIL");
        return result;
    }
    logger.Info("TemperatureService PASS");

    result = TestServiceManager();
    if (!result)
    {
        logger.Error("ServiceManager FAIL");
        return result;
    }
    logger.Info("ServiceManager PASS");

    logger.Info("[PASS] Services");
    return NAS::Core::Result::Ok();
}

} // namespace NAS::Tests
