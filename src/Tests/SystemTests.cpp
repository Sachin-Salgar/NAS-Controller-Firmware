/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Tests
 * File    : SystemTests.cpp
 *
 * Description:
 * System layer self-tests (Application, Startup, SystemManager).
 *
 ******************************************************************************/

#include "../Core/Logger.h"
#include "../Core/Result.h"
#include "../System/Application.h"
#include "../System/Startup.h"
#include "../System/SystemManager.h"

namespace NAS::Tests
{

[[nodiscard]]
static NAS::Core::Result TestStartup() noexcept
{
    auto result = NAS::System::Startup::Initialize();
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestApplication() noexcept
{
    auto result = NAS::System::Application::Initialize();
    if (!result)
    {
        return result;
    }

    // TODO: Application::Run starts the main loop and does not return.
    // Cannot test Run() in test context. Verify initialization only.

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestSystemManager() noexcept
{
    auto result = NAS::System::SystemManager::Initialize();
    if (!result)
    {
        return result;
    }

    // TODO: SystemManager functionality depends on full system initialization.
    // Verify initialization only.

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
NAS::Core::Result TestSystem() noexcept
{
    NAS::Core::Logger logger;
    logger.Initialize();

    logger.Info("[TEST] System");

    auto result = TestStartup();
    if (!result)
    {
        logger.Error("Startup FAIL");
        return result;
    }
    logger.Info("Startup PASS");

    result = TestApplication();
    if (!result)
    {
        logger.Error("Application FAIL");
        return result;
    }
    logger.Info("Application PASS");

    result = TestSystemManager();
    if (!result)
    {
        logger.Error("SystemManager FAIL");
        return result;
    }
    logger.Info("SystemManager PASS");

    logger.Info("[PASS] System");
    return NAS::Core::Result::Ok();
}

} // namespace NAS::Tests
