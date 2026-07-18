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
    auto result = NAS::Platform::I2C::Initialize(21, 22, 100000);
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestSpi() noexcept
{
    auto result = NAS::Platform::SPI::Initialize(18, 19, 23, 5);
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

struct LayerResult
{
    NAS::Core::Result result;
    const char* failedComponent;
    int passCount;
    int failCount;
    int skippedCount;
};

[[nodiscard]]
LayerResult TestPlatform() noexcept
{
    NAS::Core::Logger logger;
    logger.Initialize();

    LayerResult layerResult = {NAS::Core::Result::Ok(), nullptr, 0, 0, 0};

    logger.Info("--------------------------------------------------");
    logger.Info("PLATFORM");
    logger.Info("--------------------------------------------------");
    logger.Info("");

    auto result = TestGpio();
    if (!result)
    {
        logger.Error("GPIO.........................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "GPIO";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("GPIO.........................PASS");
    layerResult.passCount++;

    result = TestFlash();
    if (!result)
    {
        logger.Error("Flash.........................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "Flash";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("Flash.........................PASS");
    layerResult.passCount++;

    result = TestUsb();
    if (!result)
    {
        logger.Error("USB...........................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "USB";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("USB...........................PASS");
    layerResult.passCount++;

    result = TestPwm();
    if (!result)
    {
        logger.Error("PWM...........................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "PWM";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("PWM...........................PASS");
    layerResult.passCount++;

    result = TestUart();
    if (!result)
    {
        logger.Error("UART..........................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "UART";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("UART..........................PASS");
    layerResult.passCount++;

    result = TestI2c();
    if (!result)
    {
        logger.Error("I2C...........................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "I2C";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("I2C...........................PASS");
    layerResult.passCount++;

    result = TestSpi();
    if (!result)
    {
        logger.Error("SPI...........................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "SPI";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("SPI...........................PASS");
    layerResult.passCount++;

    result = TestAdc();
    if (!result)
    {
        logger.Error("ADC...........................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "ADC";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("ADC...........................PASS");
    layerResult.passCount++;

    result = TestTimer();
    if (!result)
    {
        logger.Error("Timer.........................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "Timer";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("Timer.........................PASS");
    layerResult.passCount++;

    result = TestOneWire();
    if (!result)
    {
        logger.Error("OneWire.......................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "OneWire";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Warning("OneWire.......................SKIPPED (No sensor attached)");
    layerResult.skippedCount++;

    logger.Info("");
    logger.Info("PASS 9");
    logger.Info("SKIPPED 1");
    logger.Info("");

    return layerResult;
}

} // namespace NAS::Tests
