/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Tests
 * File    : SelfTest.cpp
 *
 * Description:
 * Production-quality diagnostic framework orchestrator.
 * Runs all layer tests and reports detailed results with failure diagnostics.
 *
 ******************************************************************************/

#include "SelfTest.h"
#include "../Core/Logger.h"
#include "../Core/Result.h"

namespace NAS::Tests
{

// Forward declarations from test modules
struct LayerResult
{
    NAS::Core::Result result;
    const char* failedComponent;
    int passCount;
    int failCount;
    int skippedCount;
};

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

[[nodiscard]]
NAS::Core::Result SelfTest::Run() noexcept
{
    NAS::Core::Logger logger;

    auto initResult = logger.Initialize();
    if (!initResult)
    {
        return initResult;
    }

    logger.Info("");
    logger.Info("Running Boot Self Test...");
    logger.Info("");

    LayerResult layerResults[7];
    int totalPass = 0;
    int totalFail = 0;
    int totalSkipped = 0;
    const char* firstFailedComponent = nullptr;
    const char* firstFailedLayer = nullptr;

    // Test Core layer
    layerResults[0] = TestCore();
    if (!layerResults[0].result)
    {
        totalFail += layerResults[0].failCount;
        totalPass += layerResults[0].passCount;
        totalSkipped += layerResults[0].skippedCount;
        firstFailedComponent = layerResults[0].failedComponent;
        firstFailedLayer = "CORE";
    } else {
        totalPass += layerResults[0].passCount;
        totalSkipped += layerResults[0].skippedCount;
    }

    // Test Platform layer
    if (!firstFailedComponent)
    {
        layerResults[1] = TestPlatform();
        if (!layerResults[1].result)
        {
            totalFail += layerResults[1].failCount;
            totalPass += layerResults[1].passCount;
            totalSkipped += layerResults[1].skippedCount;
            firstFailedComponent = layerResults[1].failedComponent;
            firstFailedLayer = "PLATFORM";
        } else {
            totalPass += layerResults[1].passCount;
            totalSkipped += layerResults[1].skippedCount;
        }
    }

    // Test Drivers layer
    if (!firstFailedComponent)
    {
        layerResults[2] = TestDrivers();
        if (!layerResults[2].result)
        {
            totalFail += layerResults[2].failCount;
            totalPass += layerResults[2].passCount;
            totalSkipped += layerResults[2].skippedCount;
            firstFailedComponent = layerResults[2].failedComponent;
            firstFailedLayer = "DRIVERS";
        } else {
            totalPass += layerResults[2].passCount;
            totalSkipped += layerResults[2].skippedCount;
        }
    }

    // Test Objects layer
    if (!firstFailedComponent)
    {
        layerResults[3] = TestObjects();
        if (!layerResults[3].result)
        {
            totalFail += layerResults[3].failCount;
            totalPass += layerResults[3].passCount;
            totalSkipped += layerResults[3].skippedCount;
            firstFailedComponent = layerResults[3].failedComponent;
            firstFailedLayer = "OBJECTS";
        } else {
            totalPass += layerResults[3].passCount;
            totalSkipped += layerResults[3].skippedCount;
        }
    }

    // Test Services layer
    if (!firstFailedComponent)
    {
        layerResults[4] = TestServices();
        if (!layerResults[4].result)
        {
            totalFail += layerResults[4].failCount;
            totalPass += layerResults[4].passCount;
            totalSkipped += layerResults[4].skippedCount;
            firstFailedComponent = layerResults[4].failedComponent;
            firstFailedLayer = "SERVICES";
        } else {
            totalPass += layerResults[4].passCount;
            totalSkipped += layerResults[4].skippedCount;
        }
    }

    // Test Protocol layer
    if (!firstFailedComponent)
    {
        layerResults[5] = TestProtocol();
        if (!layerResults[5].result)
        {
            totalFail += layerResults[5].failCount;
            totalPass += layerResults[5].passCount;
            totalSkipped += layerResults[5].skippedCount;
            firstFailedComponent = layerResults[5].failedComponent;
            firstFailedLayer = "PROTOCOL";
        } else {
            totalPass += layerResults[5].passCount;
            totalSkipped += layerResults[5].skippedCount;
        }
    }

    // Test System layer
    if (!firstFailedComponent)
    {
        layerResults[6] = TestSystem();
        if (!layerResults[6].result)
        {
            totalFail += layerResults[6].failCount;
            totalPass += layerResults[6].passCount;
            totalSkipped += layerResults[6].skippedCount;
            firstFailedComponent = layerResults[6].failedComponent;
            firstFailedLayer = "SYSTEM";
        } else {
            totalPass += layerResults[6].passCount;
            totalSkipped += layerResults[6].skippedCount;
        }
    }

    logger.Info("");
    logger.Info("==================================================");
    if (firstFailedComponent)
    {
        logger.Info("BOOT SELF TEST FAILED");
        logger.Info("==================================================");
        logger.Info("");
        logger.Info("Failed Test");
        logger.Info("");
        logger.Error(firstFailedLayer);
        logger.Error(" -> ");
        logger.Error(firstFailedComponent);
        logger.Info("");
        logger.Info("Result Code");
        logger.Info("");
        logger.Error("Check logs above for details");
        logger.Info("");
        return NAS::Core::Result(NAS::Core::ResultCode::Failed);
    } else {
        logger.Info("BOOT SELF TEST PASSED");
        logger.Info("==================================================");
        logger.Info("");
        return NAS::Core::Result::Ok();
    }
}

} // namespace NAS::Tests
