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

#include "TestResult.h"
#include "TestFormatter.h"
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

[[nodiscard]]
LayerResult TestDrivers() noexcept
{
    NAS::Core::Logger logger;
    (void)logger.Initialize();

    LayerResult layerResult = {NAS::Core::Result::Ok(), nullptr,
        NAS::Core::ResultCode::Success, 0, 0, 0};

    TestFormatter::PrintHeader("DRIVERS");

    auto result = TestAddressableLedDriver();
    if (!result)
    {
        TestFormatter::PrintFail("AddressableLedDriver");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "AddressableLedDriver";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("AddressableLedDriver");
        layerResult.passCount++;
    }

    result = TestPwmFanDriver();
    if (!result)
    {
        TestFormatter::PrintFail("PWMFanDriver");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "PWMFanDriver";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("PWMFanDriver");
        layerResult.passCount++;
    }

    result = TestRelayDriver();
    if (!result)
    {
        TestFormatter::PrintFail("RelayDriver");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "RelayDriver";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintSkipped("RelayDriver", "Relay not installed");
        layerResult.skippedCount++;
    }

    result = TestTemperatureDriver();
    if (!result)
    {
        TestFormatter::PrintFail("TemperatureDriver");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "TemperatureDriver";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintSkipped("TemperatureDriver", "DS18B20 unavailable");
        layerResult.skippedCount++;
    }

    result = TestStorageDriver();
    if (!result)
    {
        TestFormatter::PrintFail("StorageDriver");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "StorageDriver";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("StorageDriver");
        layerResult.passCount++;
    }

    result = TestUsbDriver();
    if (!result)
    {
        TestFormatter::PrintFail("UsbDriver");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "UsbDriver";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("UsbDriver");
        layerResult.passCount++;
    }

    result = TestWatchdogDriver();
    if (!result)
    {
        TestFormatter::PrintFail("WatchdogDriver");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "WatchdogDriver";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("WatchdogDriver");
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
