/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Tests
 * File    : SelfTest.cpp
 *
 * Description:
 * Production-quality firmware self-test framework that validates every layer.
 * Entry point: SelfTest::Run()
 *
 ******************************************************************************/

#include "SelfTest.h"
#include "../Core/Logger.h"
#include "../Core/Result.h"

namespace NAS::Tests
{

// Forward declarations from test modules
[[nodiscard]]
NAS::Core::Result TestCore() noexcept;

[[nodiscard]]
NAS::Core::Result TestPlatform() noexcept;

[[nodiscard]]
NAS::Core::Result TestDrivers() noexcept;

[[nodiscard]]
NAS::Core::Result TestObjects() noexcept;

[[nodiscard]]
NAS::Core::Result TestServices() noexcept;

[[nodiscard]]
NAS::Core::Result TestProtocol() noexcept;

[[nodiscard]]
NAS::Core::Result TestSystem() noexcept;

[[nodiscard]]
NAS::Core::Result SelfTest::Run() noexcept
{
    NAS::Core::Logger logger;

    auto initResult = logger.Initialize();
    if (!initResult)
    {
        return initResult;
    }

    logger.Info("=================================================");
    logger.Info("NAS Controller Firmware Self-Test");
    logger.Info("=================================================");

    // Test Core layer
    auto result = TestCore();
    if (!result)
    {
        logger.Error("SELF-TEST FAILED: Core layer");
        return result;
    }

    // Test Platform layer
    result = TestPlatform();
    if (!result)
    {
        logger.Error("SELF-TEST FAILED: Platform layer");
        return result;
    }

    // Test Drivers layer
    result = TestDrivers();
    if (!result)
    {
        logger.Error("SELF-TEST FAILED: Drivers layer");
        return result;
    }

    // Test Objects layer
    result = TestObjects();
    if (!result)
    {
        logger.Error("SELF-TEST FAILED: Objects layer");
        return result;
    }

    // Test Services layer
    result = TestServices();
    if (!result)
    {
        logger.Error("SELF-TEST FAILED: Services layer");
        return result;
    }

    // Test Protocol layer
    result = TestProtocol();
    if (!result)
    {
        logger.Error("SELF-TEST FAILED: Protocol layer");
        return result;
    }

    // Test System layer
    result = TestSystem();
    if (!result)
    {
        logger.Error("SELF-TEST FAILED: System layer");
        return result;
    }

    logger.Info("=================================================");
    logger.Info("SELF-TEST PASSED");
    logger.Info("=================================================");

    return NAS::Core::Result::Ok();
}

} // namespace NAS::Tests
