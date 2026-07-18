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

#include "TestResult.h"
#include "TestFormatter.h"
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

[[nodiscard]]
LayerResult TestSystem() noexcept
{
    NAS::Core::Logger logger;
    logger.Initialize();

    LayerResult layerResult = {NAS::Core::Result::Ok(), nullptr,
        NAS::Core::ResultCode::Success, 0, 0, 0};

    TestFormatter::PrintHeader("SYSTEM");

    auto result = TestStartup();
    if (!result)
    {
        TestFormatter::PrintFail("Startup");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Startup";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Startup");
        layerResult.passCount++;
    }

    result = TestApplication();
    if (!result)
    {
        TestFormatter::PrintFail("Application");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Application";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Application");
        layerResult.passCount++;
    }

    result = TestSystemManager();
    if (!result)
    {
        TestFormatter::PrintFail("SystemManager");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "SystemManager";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("SystemManager");
        layerResult.passCount++;
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
