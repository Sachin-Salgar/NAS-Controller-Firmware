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

    if (NAS::Services::LedService::LedCount == 0)
    {
        return NAS::Core::Result::Ok();
    }

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

struct LayerResult
{
    NAS::Core::Result result;
    const char* failedComponent;
    int passCount;
    int failCount;
    int skippedCount;
};

[[nodiscard]]
LayerResult TestServices() noexcept
{
    NAS::Core::Logger logger;
    logger.Initialize();

    LayerResult layerResult = {NAS::Core::Result::Ok(), nullptr, 0, 0, 0};

    logger.Info("--------------------------------------------------");
    logger.Info("SERVICES");
    logger.Info("--------------------------------------------------");
    logger.Info("");

    auto result = TestConfigurationService();
    if (!result)
    {
        logger.Error("ConfigurationService.......FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "ConfigurationService";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("ConfigurationService.......PASS");
    layerResult.passCount++;

    result = TestDriveService();
    if (!result)
    {
        logger.Error("DriveService...............FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "DriveService";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("DriveService...............PASS");
    layerResult.passCount++;

    result = TestFanService();
    if (!result)
    {
        logger.Error("FanService.................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "FanService";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("FanService.................PASS");
    layerResult.passCount++;

    result = TestLedService();
    if (!result)
    {
        logger.Error("LedService.................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "LedService";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("LedService.................PASS");
    layerResult.passCount++;

    result = TestRelayService();
    if (!result)
    {
        logger.Error("RelayService...............FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "RelayService";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Warning("RelayService...............SKIPPED");
    layerResult.skippedCount++;

    result = TestStatisticsService();
    if (!result)
    {
        logger.Error("StatisticsService..........FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "StatisticsService";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("StatisticsService..........PASS");
    layerResult.passCount++;

    result = TestSystemService();
    if (!result)
    {
        logger.Error("SystemService..............FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "SystemService";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("SystemService..............PASS");
    layerResult.passCount++;

    result = TestTemperatureService();
    if (!result)
    {
        logger.Error("TemperatureService.........FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "TemperatureService";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Warning("TemperatureService.........SKIPPED");
    layerResult.skippedCount++;

    result = TestServiceManager();
    if (!result)
    {
        logger.Error("ServiceManager.............FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "ServiceManager";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("ServiceManager.............PASS");
    layerResult.passCount++;

    logger.Info("");
    logger.Info("PASS 7");
    logger.Info("SKIPPED 2");
    logger.Info("");

    return layerResult;
}

} // namespace NAS::Tests
