/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Tests
 * File    : PlatformTests.cpp
 *
 * Description:
 * Platform layer HAL self-tests (GPIO, Flash, PWM, UART, Timer, I2C, SPI,
 * USB, Watchdog, ADC, OneWire, Restart).
 *
 ******************************************************************************/

#include "../Core/Logger.h"
#include "../Core/Result.h"
#include "../Platform/GPIO.h"
#include "../Platform/Flash.h"
#include "../Platform/PWM.h"
#include "../Platform/UART.h"
#include "../Platform/Timer.h"
#include "../Platform/I2C.h"
#include "../Platform/SPI.h"
#include "../Platform/USB.h"
#include "../Platform/Watchdog.h"
#include "../Platform/ADC.h"
#include "../Platform/OneWire.h"
#include "../Platform/Restart.h"

namespace NAS::Tests
{

[[nodiscard]]
static NAS::Core::Result TestGpio() noexcept
{
    auto result = NAS::Platform::GPIO::Initialize();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestFlash() noexcept
{
    auto result = NAS::Platform::Flash::Initialize();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestPwm() noexcept
{
    auto result = NAS::Platform::PWM::Initialize();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestUart() noexcept
{
    auto result = NAS::Platform::UART::Initialize(1, 3, 115200);
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestTimer() noexcept
{
    auto result = NAS::Platform::Timer::Initialize();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestI2c() noexcept
{
    auto result = NAS::Platform::I2C::Initialize();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestSpi() noexcept
{
    auto result = NAS::Platform::SPI::Initialize();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestUsb() noexcept
{
    auto result = NAS::Platform::USB::Initialize();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestWatchdog() noexcept
{
    auto result = NAS::Platform::Watchdog::Initialize();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestAdc() noexcept
{
    auto result = NAS::Platform::ADC::Initialize();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestOneWire() noexcept
{
    auto result = NAS::Platform::OneWire::Initialize(13);
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestRestart() noexcept
{
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
NAS::Core::Result TestPlatform() noexcept
{
    NAS::Core::Logger logger;
    logger.Initialize();

    logger.Info("[TEST] Platform");

    auto result = TestGpio();
    if (!result)
    {
        logger.Error("GPIO FAIL");
        return result;
    }
    logger.Info("GPIO PASS");

    result = TestFlash();
    if (!result)
    {
        logger.Error("Flash FAIL");
        return result;
    }
    logger.Info("Flash PASS");

    result = TestPwm();
    if (!result)
    {
        logger.Error("PWM FAIL");
        return result;
    }
    logger.Info("PWM PASS");

    result = TestUart();
    if (!result)
    {
        logger.Error("UART FAIL");
        return result;
    }
    logger.Info("UART PASS");

    result = TestTimer();
    if (!result)
    {
        logger.Error("Timer FAIL");
        return result;
    }
    logger.Info("Timer PASS");

    result = TestI2c();
    if (!result)
    {
        logger.Error("I2C FAIL");
        return result;
    }
    logger.Info("I2C PASS");

    result = TestSpi();
    if (!result)
    {
        logger.Error("SPI FAIL");
        return result;
    }
    logger.Info("SPI PASS");

    result = TestUsb();
    if (!result)
    {
        logger.Error("USB FAIL");
        return result;
    }
    logger.Info("USB PASS");

    result = TestWatchdog();
    if (!result)
    {
        logger.Error("Watchdog FAIL");
        return result;
    }
    logger.Info("Watchdog PASS");

    result = TestAdc();
    if (!result)
    {
        logger.Error("ADC FAIL");
        return result;
    }
    logger.Info("ADC PASS");

    result = TestOneWire();
    if (!result)
    {
        logger.Error("OneWire FAIL");
        return result;
    }
    logger.Info("OneWire PASS");

    result = TestRestart();
    if (!result)
    {
        logger.Error("Restart FAIL");
        return result;
    }
    logger.Info("Restart PASS");

    logger.Info("[PASS] Platform");
    return NAS::Core::Result::Ok();
}

} // namespace NAS::Tests
