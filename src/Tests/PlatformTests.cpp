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
#include "../Core/ResultCodeHelper.h"
#include <Arduino.h>
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
    Serial.println("  >> TestGpio");
    Serial.flush();
    auto result = NAS::Platform::GPIO::Initialize();
    if (!result)
    {
        Serial.println("  << TestGpio FAIL");
        Serial.flush();
        return result;
    }
    Serial.println("  << TestGpio PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestFlash() noexcept
{
    Serial.println("  >> TestFlash");
    Serial.flush();
    auto result = NAS::Platform::Flash::Initialize();
    if (!result)
    {
        Serial.println("  << TestFlash FAIL");
        Serial.flush();
        return result;
    }
    Serial.println("  << TestFlash PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestPwm() noexcept
{
    Serial.println("  >> TestPwm");
    Serial.flush();
    auto result = NAS::Platform::PWM::Initialize();
    if (!result)
    {
        Serial.println("  << TestPwm FAIL");
        Serial.flush();
        return result;
    }
    Serial.println("  << TestPwm PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestUart() noexcept
{
    Serial.println("  >> TestUart");
    Serial.flush();
    auto result = NAS::Platform::UART::Initialize(16, 17, 115200);
    if (!result)
    {
        Serial.println("  << TestUart FAIL");
        Serial.flush();
        return result;
    }
    Serial.println("  << TestUart PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestTimer() noexcept
{
    Serial.println("  >> TestTimer");
    Serial.flush();
    auto result = NAS::Platform::Timer::Initialize();
    if (!result)
    {
        Serial.println("  << TestTimer FAIL");
        Serial.flush();
        return result;
    }
    Serial.println("  << TestTimer PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestI2c() noexcept
{
    Serial.println("  >> TestI2c");
    Serial.flush();
    auto result = NAS::Platform::I2C::Initialize(21, 22, 100000);
    if (!result)
    {
        Serial.println("  << TestI2c FAIL");
        Serial.flush();
        return result;
    }
    Serial.println("  << TestI2c PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestSpi() noexcept
{
    Serial.println("  >> TestSpi");
    Serial.flush();
    auto result = NAS::Platform::SPI::Initialize(18, 19, 23, 5);
    if (!result)
    {
        Serial.println("  << TestSpi FAIL");
        Serial.flush();
        return result;
    }
    Serial.println("  << TestSpi PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestUsb() noexcept
{
    Serial.println("  >> TestUsb");
    Serial.flush();
    auto result = NAS::Platform::USB::Initialize();
    if (!result)
    {
        Serial.print("  << TestUsb FAIL - Result: ");
        Serial.print(NAS::Core::ResultCodeToString(result.Code()));
        Serial.print(", Description: ");
        Serial.println(NAS::Core::ResultCodeDescription(result.Code()));
        Serial.flush();
        return result;
    }
    Serial.println("  << TestUsb PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestWatchdog() noexcept
{
    Serial.println("  >> TestWatchdog");
    Serial.flush();
    auto result = NAS::Platform::Watchdog::Initialize();
    if (!result)
    {
        Serial.println("  << TestWatchdog FAIL");
        Serial.flush();
        return result;
    }
    Serial.println("  << TestWatchdog PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestAdc() noexcept
{
    Serial.println("  >> TestAdc");
    Serial.flush();
    auto result = NAS::Platform::ADC::Initialize();
    if (!result)
    {
        Serial.println("  << TestAdc FAIL");
        Serial.flush();
        return result;
    }
    Serial.println("  << TestAdc PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestOneWire() noexcept
{
    Serial.println("  >> TestOneWire");
    Serial.flush();
    auto result = NAS::Platform::OneWire::Initialize(13);
    if (!result)
    {
        Serial.println("  << TestOneWire FAIL");
        Serial.flush();
        return result;
    }
    Serial.println("  << TestOneWire PASS");
    Serial.flush();
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
        Serial.print("    Result: ");
        Serial.print(NAS::Core::ResultCodeToString(result.Code()));
        Serial.println();
        Serial.flush();
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
        Serial.print("    Result: ");
        Serial.print(NAS::Core::ResultCodeToString(result.Code()));
        Serial.println();
        Serial.flush();
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
        Serial.print("    Result: ");
        Serial.print(NAS::Core::ResultCodeToString(result.Code()));
        Serial.println();
        Serial.flush();
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
        Serial.print("    Result: ");
        Serial.print(NAS::Core::ResultCodeToString(result.Code()));
        Serial.println();
        Serial.flush();
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
        Serial.print("    Result: ");
        Serial.print(NAS::Core::ResultCodeToString(result.Code()));
        Serial.println();
        Serial.flush();
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
        Serial.print("    Result: ");
        Serial.print(NAS::Core::ResultCodeToString(result.Code()));
        Serial.println();
        Serial.flush();
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
        Serial.print("    Result: ");
        Serial.print(NAS::Core::ResultCodeToString(result.Code()));
        Serial.println();
        Serial.flush();
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
        Serial.print("    Result: ");
        Serial.print(NAS::Core::ResultCodeToString(result.Code()));
        Serial.println();
        Serial.flush();
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
        Serial.print("    Result: ");
        Serial.print(NAS::Core::ResultCodeToString(result.Code()));
        Serial.println();
        Serial.flush();
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
        Serial.print("    Result: ");
        Serial.print(NAS::Core::ResultCodeToString(result.Code()));
        Serial.println();
        Serial.flush();
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

    Serial.println("TestPlatform: END");
    Serial.flush();
    return layerResult;
}

} // namespace NAS::Tests
