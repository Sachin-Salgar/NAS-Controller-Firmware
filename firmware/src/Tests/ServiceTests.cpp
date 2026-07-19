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

#include "TestResult.h"
#include "TestFormatter.h"
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

[[nodiscard]]
LayerResult TestServices() noexcept
{
    NAS::Core::Logger logger;
    (void)logger.Initialize();

    LayerResult layerResult = {NAS::Core::Result::Ok(), nullptr,
        NAS::Core::ResultCode::Success, 0, 0, 0};

    TestFormatter::PrintHeader("SERVICES");

    auto result = TestConfigurationService();
    if (!result)
    {
        TestFormatter::PrintFail("ConfigurationService");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "ConfigurationService";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("ConfigurationService");
        layerResult.passCount++;
    }

    result = TestDriveService();
    if (!result)
    {
        TestFormatter::PrintFail("DriveService");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "DriveService";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("DriveService");
        layerResult.passCount++;
    }

    result = TestFanService();
    if (!result)
    {
        TestFormatter::PrintFail("FanService");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "FanService";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("FanService");
        layerResult.passCount++;
    }

    result = TestLedService();
    if (!result)
    {
        TestFormatter::PrintFail("LedService");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "LedService";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("LedService");
        layerResult.passCount++;
    }

    result = TestRelayService();
    if (!result)
    {
        TestFormatter::PrintFail("RelayService");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "RelayService";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintSkipped("RelayService");
        layerResult.skippedCount++;
    }

    result = TestStatisticsService();
    if (!result)
    {
        TestFormatter::PrintFail("StatisticsService");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "StatisticsService";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("StatisticsService");
        layerResult.passCount++;
    }

    result = TestSystemService();
    if (!result)
    {
        TestFormatter::PrintFail("SystemService");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "SystemService";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("SystemService");
        layerResult.passCount++;
    }

    result = TestTemperatureService();
    if (!result)
    {
        TestFormatter::PrintFail("TemperatureService");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "TemperatureService";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintSkipped("TemperatureService");
        layerResult.skippedCount++;
    }

    result = TestServiceManager();
    if (!result)
    {
        TestFormatter::PrintFail("ServiceManager");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "ServiceManager";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("ServiceManager");
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
