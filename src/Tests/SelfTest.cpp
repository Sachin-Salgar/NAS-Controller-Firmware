/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Tests
 * File    : SelfTest.cpp
 *
 * Description:
 * Production-quality diagnostic framework orchestrator.
 * Runs all layer tests independently, collects all failures,
 * and reports detailed diagnostics with ResultCode.
 *
 ******************************************************************************/

#include "SelfTest.h"
#include "TestResult.h"
#include "../Core/Logger.h"
#include "../Core/Result.h"
#include "../Core/ResultCodeHelper.h"
#include <Arduino.h>

namespace NAS::Tests
{

[[nodiscard]]
LayerResult TestCore() noexcept;

[[nodiscard]]
LayerResult TestPlatform() noexcept;

[[nodiscard]]
LayerResult TestDrivers() noexcept;

[[nodiscard]]
LayerResult TestObjects() noexcept;

[[nodiscard]]
LayerResult TestServices() noexcept;

[[nodiscard]]
LayerResult TestProtocol() noexcept;

[[nodiscard]]
LayerResult TestSystem() noexcept;

struct FailureRecord
{
    const char* layer;
    const char* component;
    NAS::Core::ResultCode code;
};

static void PrintFailureDetail(const char* layer, const char* component,
                               NAS::Core::ResultCode code) noexcept
{
    NAS::Core::Logger logger;
    (void)logger.Initialize();

    Serial.println();
    Serial.print("  ");
    Serial.print(layer);
    Serial.print(" -> ");
    Serial.println(component);
    Serial.print("    Result  : ");
    Serial.println(NAS::Core::ResultCodeToString(code));
    Serial.print("    Details : ");
    Serial.println(NAS::Core::ResultCodeDescription(code));
    Serial.flush();
}

[[nodiscard]]
NAS::Core::Result SelfTest::Run() noexcept
{
    Serial.println();
    Serial.println(">> START SelfTest::Run");
    Serial.flush();

    NAS::Core::Logger logger;

    auto initResult = logger.Initialize();
    if (!initResult)
    {
        Serial.println("!! Logger initialization failed");
        Serial.flush();
        return initResult;
    }

    (void)logger.Info("");
    (void)logger.Info("Running Boot Self Test...");
    (void)logger.Info("");

    LayerResult layerResults[7];
    FailureRecord failures[7];
    int failureCount = 0;

    Serial.println(">> TestCore");
    Serial.flush();
    layerResults[0] = TestCore();
    Serial.println("<< TestCore");
    Serial.flush();

    if (!layerResults[0].result && failureCount < 7)
    {
        failures[failureCount].layer = "CORE";
        failures[failureCount].component = layerResults[0].failedComponent;
        failures[failureCount].code = layerResults[0].failureCode;
        failureCount++;
    }

    Serial.println(">> TestPlatform");
    Serial.flush();
    layerResults[1] = TestPlatform();
    Serial.println("<< TestPlatform");
    Serial.flush();

    if (!layerResults[1].result && failureCount < 7)
    {
        failures[failureCount].layer = "PLATFORM";
        failures[failureCount].component = layerResults[1].failedComponent;
        failures[failureCount].code = layerResults[1].failureCode;
        failureCount++;
    }

    Serial.println(">> TestDrivers");
    Serial.flush();
    layerResults[2] = TestDrivers();
    Serial.println("<< TestDrivers");
    Serial.flush();

    if (!layerResults[2].result && failureCount < 7)
    {
        failures[failureCount].layer = "DRIVERS";
        failures[failureCount].component = layerResults[2].failedComponent;
        failures[failureCount].code = layerResults[2].failureCode;
        failureCount++;
    }

    Serial.println(">> TestObjects");
    Serial.flush();
    layerResults[3] = TestObjects();
    Serial.println("<< TestObjects");
    Serial.flush();

    if (!layerResults[3].result && failureCount < 7)
    {
        failures[failureCount].layer = "OBJECTS";
        failures[failureCount].component = layerResults[3].failedComponent;
        failures[failureCount].code = layerResults[3].failureCode;
        failureCount++;
    }

    Serial.println(">> TestServices");
    Serial.flush();
    layerResults[4] = TestServices();
    Serial.println("<< TestServices");
    Serial.flush();

    if (!layerResults[4].result && failureCount < 7)
    {
        failures[failureCount].layer = "SERVICES";
        failures[failureCount].component = layerResults[4].failedComponent;
        failures[failureCount].code = layerResults[4].failureCode;
        failureCount++;
    }

    Serial.println(">> TestProtocol");
    Serial.flush();
    layerResults[5] = TestProtocol();
    Serial.println("<< TestProtocol");
    Serial.flush();

    if (!layerResults[5].result && failureCount < 7)
    {
        failures[failureCount].layer = "PROTOCOL";
        failures[failureCount].component = layerResults[5].failedComponent;
        failures[failureCount].code = layerResults[5].failureCode;
        failureCount++;
    }

    Serial.println(">> TestSystem");
    Serial.flush();
    layerResults[6] = TestSystem();
    Serial.println("<< TestSystem");
    Serial.flush();

    if (!layerResults[6].result && failureCount < 7)
    {
        failures[failureCount].layer = "SYSTEM";
        failures[failureCount].component = layerResults[6].failedComponent;
        failures[failureCount].code = layerResults[6].failureCode;
        failureCount++;
    }

    (void)logger.Info("");

    Serial.println("====================================");
    Serial.println("BOOT SELF TEST SUMMARY");
    Serial.println("====================================");
    Serial.flush();

    if (failureCount == 0)
    {
        Serial.println("Core ............... PASS");
        Serial.println("Platform ........... PASS");
        Serial.println("Drivers ............ PASS");
        Serial.println("Objects ............ PASS");
        Serial.println("Services ........... PASS");
        Serial.println("Protocol ........... PASS");
        Serial.println("System ............. PASS");
        Serial.println("====================================");
        Serial.println("Passed Layers : 7");
        Serial.println("Failed Layers : 0");
        Serial.println("====================================");
        Serial.flush();
        Serial.println();
        Serial.println("<< SelfTest::Run PASSED");
        Serial.flush();
        return NAS::Core::Result::Ok();
    }

    Serial.print("Core ............... ");
    Serial.println(layerResults[0].result ? "PASS" : "FAIL");
    Serial.print("Platform ........... ");
    Serial.println(layerResults[1].result ? "PASS" : "FAIL");
    if (!layerResults[1].result && layerResults[1].failedComponent)
    {
        Serial.print("  ");
        Serial.print(layerResults[1].failedComponent);
        Serial.print(" ......... ");
        Serial.println(NAS::Core::ResultCodeToString(layerResults[1].failureCode));
    }
    Serial.print("Drivers ............ ");
    Serial.println(layerResults[2].result ? "PASS" : "FAIL");
    if (!layerResults[2].result && layerResults[2].failedComponent)
    {
        Serial.print("  ");
        Serial.print(layerResults[2].failedComponent);
        Serial.print(" ......... ");
        Serial.println(NAS::Core::ResultCodeToString(layerResults[2].failureCode));
    }
    Serial.print("Objects ............ ");
    Serial.println(layerResults[3].result ? "PASS" : "FAIL");
    if (!layerResults[3].result && layerResults[3].failedComponent)
    {
        Serial.print("  ");
        Serial.print(layerResults[3].failedComponent);
        Serial.print(" ......... ");
        Serial.println(NAS::Core::ResultCodeToString(layerResults[3].failureCode));
    }
    Serial.print("Services ........... ");
    Serial.println(layerResults[4].result ? "PASS" : "FAIL");
    if (!layerResults[4].result && layerResults[4].failedComponent)
    {
        Serial.print("  ");
        Serial.print(layerResults[4].failedComponent);
        Serial.print(" ......... ");
        Serial.println(NAS::Core::ResultCodeToString(layerResults[4].failureCode));
    }
    Serial.print("Protocol ........... ");
    Serial.println(layerResults[5].result ? "PASS" : "FAIL");
    if (!layerResults[5].result && layerResults[5].failedComponent)
    {
        Serial.print("  ");
        Serial.print(layerResults[5].failedComponent);
        Serial.print(" ......... ");
        Serial.println(NAS::Core::ResultCodeToString(layerResults[5].failureCode));
    }
    Serial.print("System ............. ");
    Serial.println(layerResults[6].result ? "PASS" : "FAIL");
    if (!layerResults[6].result && layerResults[6].failedComponent)
    {
        Serial.print("  ");
        Serial.print(layerResults[6].failedComponent);
        Serial.print(" ......... ");
        Serial.println(NAS::Core::ResultCodeToString(layerResults[6].failureCode));
    }
    Serial.println("====================================");
    Serial.print("Passed Layers : ");
    Serial.println(7 - failureCount);
    Serial.print("Failed Layers : ");
    Serial.println(failureCount);
    Serial.println("====================================");
    Serial.flush();

    for (int i = 0; i < failureCount; ++i)
    {
        PrintFailureDetail(failures[i].layer, failures[i].component,
                          failures[i].code);
    }

    Serial.println();
    Serial.println("<< SelfTest::Run FAILED");
    Serial.flush();
    return NAS::Core::Result(NAS::Core::ResultCode::Failed);
}

} // namespace NAS::Tests
