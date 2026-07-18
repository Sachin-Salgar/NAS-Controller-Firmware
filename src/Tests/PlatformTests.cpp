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

#include "TestResult.h"
#include "TestFormatter.h"
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

[[nodiscard]]
LayerResult TestPlatform() noexcept
{
    NAS::Core::Logger logger;
    (void)logger.Initialize();

    LayerResult layerResult = {NAS::Core::Result::Ok(), nullptr,
        NAS::Core::ResultCode::Success, 0, 0, 0};

    TestFormatter::PrintHeader("PLATFORM");

    auto result = TestGpio();
    if (!result)
    {
        TestFormatter::PrintFail("GPIO");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "GPIO";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("GPIO");
        layerResult.passCount++;
    }

    result = TestFlash();
    if (!result)
    {
        TestFormatter::PrintFail("Flash");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Flash";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Flash");
        layerResult.passCount++;
    }

    result = TestUsb();
    if (!result)
    {
        TestFormatter::PrintFail("USB");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "USB";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("USB");
        layerResult.passCount++;
    }

    result = TestPwm();
    if (!result)
    {
        TestFormatter::PrintFail("PWM");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "PWM";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("PWM");
        layerResult.passCount++;
    }

    result = TestUart();
    if (!result)
    {
        TestFormatter::PrintFail("UART");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "UART";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("UART");
        layerResult.passCount++;
    }

    result = TestI2c();
    if (!result)
    {
        TestFormatter::PrintFail("I2C");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "I2C";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("I2C");
        layerResult.passCount++;
    }

    result = TestSpi();
    if (!result)
    {
        TestFormatter::PrintFail("SPI");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "SPI";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("SPI");
        layerResult.passCount++;
    }

    result = TestAdc();
    if (!result)
    {
        TestFormatter::PrintFail("ADC");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "ADC";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("ADC");
        layerResult.passCount++;
    }

    result = TestTimer();
    if (!result)
    {
        TestFormatter::PrintFail("Timer");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Timer";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Timer");
        layerResult.passCount++;
    }

    result = TestOneWire();
    if (!result)
    {
        TestFormatter::PrintFail("OneWire");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "OneWire";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintSkipped("OneWire", "No sensor attached");
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
