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
    auto result = NAS::Drivers::RelayDriver::Initialize();
    if (!result)
    {
        return result;
    }

    auto state = NAS::Drivers::RelayDriver::GetState(0);
    if (state != NAS::Drivers::RelayState::Off)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

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
    auto result = NAS::Drivers::TemperatureDriver::Initialize(5);
    if (!result)
    {
        return result;
    }

    auto sensorCount = NAS::Drivers::TemperatureDriver::SensorCount();
    if (sensorCount > 3U)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

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
    auto result = NAS::Drivers::AddressableLedDriver::Initialize();
    if (!result)
    {
        return result;
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
NAS::Core::Result TestDrivers() noexcept
{
    NAS::Core::Logger logger;
    logger.Initialize();

    logger.Info("[TEST] Drivers");

    auto result = TestRelayDriver();
    if (!result)
    {
        logger.Error("RelayDriver FAIL");
        return result;
    }
    logger.Info("RelayDriver PASS");

    result = TestPwmFanDriver();
    if (!result)
    {
        logger.Error("PWMFanDriver FAIL");
        return result;
    }
    logger.Info("PWMFanDriver PASS");

    result = TestTemperatureDriver();
    if (!result)
    {
        logger.Error("TemperatureDriver FAIL");
        return result;
    }
    logger.Info("TemperatureDriver PASS");

    result = TestUsbDriver();
    if (!result)
    {
        logger.Error("UsbDriver FAIL");
        return result;
    }
    logger.Info("UsbDriver PASS");

    result = TestStorageDriver();
    if (!result)
    {
        logger.Error("StorageDriver FAIL");
        return result;
    }
    logger.Info("StorageDriver PASS");

    result = TestAddressableLedDriver();
    if (!result)
    {
        logger.Error("AddressableLedDriver FAIL");
        return result;
    }
    logger.Info("AddressableLedDriver PASS");

    result = TestWatchdogDriver();
    if (!result)
    {
        logger.Error("WatchdogDriver FAIL");
        return result;
    }
    logger.Info("WatchdogDriver PASS");

    logger.Info("[PASS] Drivers");
    return NAS::Core::Result::Ok();
}

} // namespace NAS::Tests
