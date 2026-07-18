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
LayerResult TestSystem() noexcept
{
    NAS::Core::Logger logger;
    logger.Initialize();

    LayerResult layerResult = {NAS::Core::Result::Ok(), nullptr, 0, 0, 0};

    logger.Info("--------------------------------------------------");
    logger.Info("SYSTEM");
    logger.Info("--------------------------------------------------");
    logger.Info("");

    auto result = TestStartup();
    if (!result)
    {
        logger.Error("Startup....................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "Startup";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("Startup....................PASS");
    layerResult.passCount++;

    result = TestApplication();
    if (!result)
    {
        logger.Error("Application................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "Application";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("Application................PASS");
    layerResult.passCount++;

    result = TestSystemManager();
    if (!result)
    {
        logger.Error("SystemManager..............FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "SystemManager";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("SystemManager..............PASS");
    layerResult.passCount++;

    logger.Info("");
    logger.Info("PASS 3");
    logger.Info("");

    return layerResult;
}

} // namespace NAS::Tests
