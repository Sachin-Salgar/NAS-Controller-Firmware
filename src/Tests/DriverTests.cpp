/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Tests
 * File    : DriverTests.cpp
 *
 * Description:
 * Driver layer self-tests (RelayDriver, PWMFanDriver, TemperatureDriver,
 * UsbDriver, StorageDriver, AddressableLedDriver, WatchdogDriver).
 *
 ******************************************************************************/

#include "../Core/Logger.h"
#include "../Core/Result.h"
#include "../Drivers/RelayDriver.h"
#include "../Drivers/PWMFanDriver.h"
#include "../Drivers/TemperatureDriver.h"
#include "../Drivers/UsbDriver.h"
#include "../Drivers/StorageDriver.h"
#include "../Drivers/AddressableLedDriver.h"
#include "../Drivers/WatchdogDriver.h"

namespace NAS::Tests
{

[[nodiscard]]
static NAS::Core::Result TestRelayDriver() noexcept
{
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestPwmFanDriver() noexcept
{
    auto result = NAS::Drivers::PWMFanDriver::Initialize();
    if (!result)
    {
        return result;
    }

    result = NAS::Drivers::PWMFanDriver::Configure(0, 12, 5000, 8);
    if (!result)
    {
        return result;
    }

    auto speed = NAS::Drivers::PWMFanDriver::GetSpeed(0);
    if (speed > 100)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestTemperatureDriver() noexcept
{
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestUsbDriver() noexcept
{
    auto result = NAS::Drivers::UsbDriver::Initialize();
    if (!result)
    {
        return result;
    }

    bool connected = NAS::Drivers::UsbDriver::IsConnected();
    (void)connected;

    auto available = NAS::Drivers::UsbDriver::Available();
    (void)available;

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestStorageDriver() noexcept
{
    auto result = NAS::Drivers::StorageDriver::Initialize();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestAddressableLedDriver() noexcept
{
    auto result = NAS::Drivers::AddressableLedDriver::Initialize(15, 25);
    if (!result)
    {
        return result;
    }

    auto ledCount = NAS::Drivers::AddressableLedDriver::LedCount();
    if (ledCount != 25)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestWatchdogDriver() noexcept
{
    auto result = NAS::Drivers::WatchdogDriver::Initialize();
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
LayerResult TestDrivers() noexcept
{
    NAS::Core::Logger logger;
    logger.Initialize();

    LayerResult layerResult = {NAS::Core::Result::Ok(), nullptr, 0, 0, 0};

    logger.Info("--------------------------------------------------");
    logger.Info("DRIVERS");
    logger.Info("--------------------------------------------------");
    logger.Info("");

    auto result = TestAddressableLedDriver();
    if (!result)
    {
        logger.Error("AddressableLedDriver.......FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "AddressableLedDriver";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("AddressableLedDriver.......PASS");
    layerResult.passCount++;

    result = TestPwmFanDriver();
    if (!result)
    {
        logger.Error("PWMFanDriver...............FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "PWMFanDriver";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("PWMFanDriver...............PASS");
    layerResult.passCount++;

    result = TestRelayDriver();
    if (!result)
    {
        logger.Error("RelayDriver................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "RelayDriver";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Warning("RelayDriver................SKIPPED (Relay not installed)");
    layerResult.skippedCount++;

    result = TestTemperatureDriver();
    if (!result)
    {
        logger.Error("TemperatureDriver..........FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "TemperatureDriver";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Warning("TemperatureDriver..........SKIPPED (DS18B20 unavailable)");
    layerResult.skippedCount++;

    result = TestStorageDriver();
    if (!result)
    {
        logger.Error("StorageDriver..............FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "StorageDriver";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("StorageDriver..............PASS");
    layerResult.passCount++;

    result = TestUsbDriver();
    if (!result)
    {
        logger.Error("UsbDriver..................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "UsbDriver";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("UsbDriver..................PASS");
    layerResult.passCount++;

    result = TestWatchdogDriver();
    if (!result)
    {
        logger.Error("WatchdogDriver.............FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "WatchdogDriver";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("WatchdogDriver.............PASS");
    layerResult.passCount++;

    logger.Info("");
    logger.Info("PASS 5");
    logger.Info("SKIPPED 2");
    logger.Info("");

    return layerResult;
}

} // namespace NAS::Tests
