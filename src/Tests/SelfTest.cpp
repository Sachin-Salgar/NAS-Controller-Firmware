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

static void PrintResultCode(NAS::Core::ResultCode code) noexcept
{
    NAS::Core::Logger logger;
    (void)logger.Initialize();

    const char* codeStr = "UnknownError";

    if (code == NAS::Core::ResultCode::Success)
        codeStr = "Success";
    else if (code == NAS::Core::ResultCode::Failed)
        codeStr = "Failed";
    else if (code == NAS::Core::ResultCode::InvalidArgument)
        codeStr = "InvalidArgument";
    else if (code == NAS::Core::ResultCode::InvalidState)
        codeStr = "InvalidState";
    else if (code == NAS::Core::ResultCode::NotInitialized)
        codeStr = "NotInitialized";
    else if (code == NAS::Core::ResultCode::AlreadyInitialized)
        codeStr = "AlreadyInitialized";
    else if (code == NAS::Core::ResultCode::Timeout)
        codeStr = "Timeout";
    else if (code == NAS::Core::ResultCode::Busy)
        codeStr = "Busy";
    else if (code == NAS::Core::ResultCode::Unsupported)
        codeStr = "Unsupported";
    else if (code == NAS::Core::ResultCode::NotSupported)
        codeStr = "NotSupported";
    else if (code == NAS::Core::ResultCode::AccessDenied)
        codeStr = "AccessDenied";
    else if (code == NAS::Core::ResultCode::OutOfMemory)
        codeStr = "OutOfMemory";
    else if (code == NAS::Core::ResultCode::NullPointer)
        codeStr = "NullPointer";
    else if (code == NAS::Core::ResultCode::BufferTooSmall)
        codeStr = "BufferTooSmall";
    else if (code == NAS::Core::ResultCode::BufferFull)
        codeStr = "BufferFull";
    else if (code == NAS::Core::ResultCode::CommunicationError)
        codeStr = "CommunicationError";
    else if (code == NAS::Core::ResultCode::ChecksumError)
        codeStr = "ChecksumError";
    else if (code == NAS::Core::ResultCode::CrcError)
        codeStr = "CrcError";
    else if (code == NAS::Core::ResultCode::InvalidHeader)
        codeStr = "InvalidHeader";
    else if (code == NAS::Core::ResultCode::InvalidLength)
        codeStr = "InvalidLength";
    else if (code == NAS::Core::ResultCode::NotFound)
        codeStr = "NotFound";
    else if (code == NAS::Core::ResultCode::HardwareError)
        codeStr = "HardwareError";
    else if (code == NAS::Core::ResultCode::DeviceNotFound)
        codeStr = "DeviceNotFound";
    else if (code == NAS::Core::ResultCode::InvalidConfiguration)
        codeStr = "InvalidConfiguration";
    else if (code == NAS::Core::ResultCode::InternalError)
        codeStr = "InternalError";

    (void)logger.Error(codeStr);
}

[[nodiscard]]
NAS::Core::Result SelfTest::Run() noexcept
{
    Serial.println("[CP1] SelfTest::Run() started");
    Serial.flush();

    NAS::Core::Logger logger;

    Serial.println("[CP2] Logger instance created");
    Serial.flush();

    auto initResult = logger.Initialize();
    if (!initResult)
    {
        Serial.println("[CP_ERR] Logger initialization failed");
        Serial.flush();
        return initResult;
    }

    Serial.println("[CP3] Logger initialized");
    Serial.flush();

    (void)logger.Info("");
    (void)logger.Info("Running Boot Self Test...");
    (void)logger.Info("");

    LayerResult layerResults[7];
    FailureRecord failures[7];
    int failureCount = 0;

    Serial.println("[CP4] About to call TestCore()");
    Serial.flush();
    layerResults[0] = TestCore();
    Serial.println("[CP5] TestCore() returned");
    Serial.flush();

    if (!layerResults[0].result && failureCount < 7)
    {
        failures[failureCount].layer = "CORE";
        failures[failureCount].component = layerResults[0].failedComponent;
        failures[failureCount].code = layerResults[0].failureCode;
        failureCount++;
    }

    Serial.println("[CP6] About to call TestPlatform()");
    Serial.flush();
    layerResults[1] = TestPlatform();
    Serial.println("[CP7] TestPlatform() returned");
    Serial.flush();

    if (!layerResults[1].result && failureCount < 7)
    {
        failures[failureCount].layer = "PLATFORM";
        failures[failureCount].component = layerResults[1].failedComponent;
        failures[failureCount].code = layerResults[1].failureCode;
        failureCount++;
    }

    Serial.println("[CP8] About to call TestDrivers()");
    Serial.flush();
    layerResults[2] = TestDrivers();
    Serial.println("[CP9] TestDrivers() returned");
    Serial.flush();

    if (!layerResults[2].result && failureCount < 7)
    {
        failures[failureCount].layer = "DRIVERS";
        failures[failureCount].component = layerResults[2].failedComponent;
        failures[failureCount].code = layerResults[2].failureCode;
        failureCount++;
    }

    Serial.println("[CP10] About to call TestObjects()");
    Serial.flush();
    layerResults[3] = TestObjects();
    Serial.println("[CP11] TestObjects() returned");
    Serial.flush();

    if (!layerResults[3].result && failureCount < 7)
    {
        failures[failureCount].layer = "OBJECTS";
        failures[failureCount].component = layerResults[3].failedComponent;
        failures[failureCount].code = layerResults[3].failureCode;
        failureCount++;
    }

    Serial.println("[CP12] About to call TestServices()");
    Serial.flush();
    layerResults[4] = TestServices();
    Serial.println("[CP13] TestServices() returned");
    Serial.flush();

    if (!layerResults[4].result && failureCount < 7)
    {
        failures[failureCount].layer = "SERVICES";
        failures[failureCount].component = layerResults[4].failedComponent;
        failures[failureCount].code = layerResults[4].failureCode;
        failureCount++;
    }

    Serial.println("[CP14] About to call TestProtocol()");
    Serial.flush();
    layerResults[5] = TestProtocol();
    Serial.println("[CP15] TestProtocol() returned");
    Serial.flush();

    if (!layerResults[5].result && failureCount < 7)
    {
        failures[failureCount].layer = "PROTOCOL";
        failures[failureCount].component = layerResults[5].failedComponent;
        failures[failureCount].code = layerResults[5].failureCode;
        failureCount++;
    }

    Serial.println("[CP16] About to call TestSystem()");
    Serial.flush();
    layerResults[6] = TestSystem();
    Serial.println("[CP17] TestSystem() returned");
    Serial.flush();

    if (!layerResults[6].result && failureCount < 7)
    {
        failures[failureCount].layer = "SYSTEM";
        failures[failureCount].component = layerResults[6].failedComponent;
        failures[failureCount].code = layerResults[6].failureCode;
        failureCount++;
    }

    (void)logger.Info("");
    (void)logger.Info("==================================================");
    if (failureCount == 0)
    {
        (void)logger.Info("BOOT SELF TEST PASSED");
        (void)logger.Info("==================================================");
        (void)logger.Info("");
        return NAS::Core::Result::Ok();
    }

    (void)logger.Info("BOOT SELF TEST FAILED");
    (void)logger.Info("==================================================");
    (void)logger.Info("");

    for (int i = 0; i < failureCount; ++i)
    {
        (void)logger.Info("Failed Test");
        (void)logger.Info("");
        (void)logger.Error(failures[i].layer);
        (void)logger.Error(" -> ");
        (void)logger.Error(failures[i].component);
        (void)logger.Info("");
        (void)logger.Info("Result Code");
        (void)logger.Info("");

        PrintResultCode(failures[i].code);

        if (i < failureCount - 1)
        {
            (void)logger.Info("");
            (void)logger.Info("---");
            (void)logger.Info("");
        }
    }

    (void)logger.Info("");
    return NAS::Core::Result(NAS::Core::ResultCode::Failed);
}

} // namespace NAS::Tests
