/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Tests
 * File    : CoreTests.cpp
 *
 * Description:
 * Core layer self-tests (Logger, Result, Version, BuildInfo, Event,
 * EventBus, Scheduler).
 *
 ******************************************************************************/

#include "../Core/Logger.h"
#include "../Core/Result.h"
#include "../Core/Version.h"
#include "../Core/BuildInfo.h"
#include "../Core/Event.h"
#include "../Core/EventBus.h"
#include "../Core/Scheduler.h"

namespace NAS::Tests
{

[[nodiscard]]
static NAS::Core::Result TestLogger() noexcept
{
    NAS::Core::Logger logger;

    auto result = logger.Initialize();
    if (!result)
    {
        logger.Critical("[FAIL] Logger initialization failed");
        return result;
    }

    result = logger.SetLevel(NAS::Core::LogLevel::Debug);
    if (!result)
    {
        return result;
    }

    if (logger.Level() != NAS::Core::LogLevel::Debug)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = logger.Info("[TEST] Logger");
    if (!result)
    {
        return result;
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestResult() noexcept
{
    NAS::Core::Result success = NAS::Core::Result::Ok();
    if (!success.IsSuccess())
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    NAS::Core::Result failure(NAS::Core::ResultCode::Failed);
    if (failure.IsSuccess())
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (!failure.IsFailure())
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (success.Code() != NAS::Core::ResultCode::Success)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (!static_cast<bool>(success))
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestVersion() noexcept
{
    const char* firmwareName = NAS::Core::Version::FirmwareName();
    if (firmwareName == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    std::uint16_t major = NAS::Core::Version::Major();
    std::uint16_t minor = NAS::Core::Version::Minor();
    std::uint16_t patch = NAS::Core::Version::Patch();

    (void)major;
    (void)minor;
    (void)patch;

    const char* buildDate = NAS::Core::Version::BuildDate();
    if (buildDate == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* buildTime = NAS::Core::Version::BuildTime();
    if (buildTime == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* buildConfiguration = NAS::Core::Version::BuildConfiguration();
    if (buildConfiguration == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* compiler = NAS::Core::Version::Compiler();
    if (compiler == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* targetPlatform = NAS::Core::Version::TargetPlatform();
    if (targetPlatform == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* targetBoard = NAS::Core::Version::TargetBoard();
    if (targetBoard == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* gitCommit = NAS::Core::Version::GitCommit();
    if (gitCommit == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestBuildInfo() noexcept
{
    const char* firmwareName = NAS::Core::BuildInfo::FirmwareName;
    if (firmwareName == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    if (NAS::Core::BuildInfo::FirmwareMajor == 0xFFFFU)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    const char* buildDate = NAS::Core::BuildInfo::BuildDate;
    if (buildDate == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* buildTime = NAS::Core::BuildInfo::BuildTime;
    if (buildTime == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* buildConfiguration = NAS::Core::BuildInfo::BuildConfiguration;
    if (buildConfiguration == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* compiler = NAS::Core::BuildInfo::Compiler;
    if (compiler == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* targetPlatform = NAS::Core::BuildInfo::TargetPlatform;
    if (targetPlatform == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* targetBoard = NAS::Core::BuildInfo::TargetBoard;
    if (targetBoard == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* gitCommit = NAS::Core::BuildInfo::GitCommit;
    if (gitCommit == nullptr)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestEvent() noexcept
{
    NAS::Core::Event event1;
    if (event1.Type() != NAS::Core::EventType::None)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    NAS::Core::Event event2(NAS::Core::EventType::SystemStartup, 1U, 42U);
    if (event2.Type() != NAS::Core::EventType::SystemStartup)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (event2.ObjectId() != 1U)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (event2.Value() != 42U)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    return NAS::Core::Result::Ok();
}

static bool eventHandlerCalled = false;

static void TestEventHandler(const NAS::Core::Event& event)
{
    if (event.Type() == NAS::Core::EventType::SystemReady)
    {
        eventHandlerCalled = true;
    }
}

[[nodiscard]]
static NAS::Core::Result TestEventBus() noexcept
{
    NAS::Core::EventBus eventBus;

    auto result = eventBus.Initialize();
    if (!result)
    {
        return result;
    }

    if (eventBus.SubscriberCount() != 0)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = eventBus.Subscribe(TestEventHandler);
    if (!result)
    {
        return result;
    }

    if (eventBus.SubscriberCount() != 1)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    eventHandlerCalled = false;

    NAS::Core::Event event(NAS::Core::EventType::SystemReady);
    result = eventBus.Publish(event);
    if (!result)
    {
        return result;
    }

    if (!eventHandlerCalled)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = eventBus.Unsubscribe(TestEventHandler);
    if (!result)
    {
        return result;
    }

    if (eventBus.SubscriberCount() != 0)
    {
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestScheduler() noexcept
{
    auto result = NAS::Core::Scheduler::Initialize();
    if (!result)
    {
        return result;
    }

    static int taskExecutionCount = 0;
    auto task = [](){ taskExecutionCount++; };

    result = NAS::Core::Scheduler::AddTask(task, 100U);
    if (!result)
    {
        return result;
    }

    result = NAS::Core::Scheduler::Execute();
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
LayerResult TestCore() noexcept
{
    NAS::Core::Logger logger;
    logger.Initialize();

    LayerResult layerResult = {NAS::Core::Result::Ok(), nullptr, 0, 0, 0};

    logger.Info("--------------------------------------------------");
    logger.Info("CORE");
    logger.Info("--------------------------------------------------");
    logger.Info("");

    auto result = TestLogger();
    if (!result)
    {
        logger.Error("Logger.......................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "Logger";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("Logger.......................PASS");
    layerResult.passCount++;

    result = TestResult();
    if (!result)
    {
        logger.Error("Result.......................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "Result";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("Result.......................PASS");
    layerResult.passCount++;

    result = TestVersion();
    if (!result)
    {
        logger.Error("Version......................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "Version";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("Version......................PASS");
    layerResult.passCount++;

    result = TestBuildInfo();
    if (!result)
    {
        logger.Error("BuildInfo....................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "BuildInfo";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("BuildInfo....................PASS");
    layerResult.passCount++;

    result = TestEvent();
    if (!result)
    {
        logger.Error("Event.......................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "Event";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("Event.......................PASS");
    layerResult.passCount++;

    result = TestEventBus();
    if (!result)
    {
        logger.Error("EventBus.....................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "EventBus";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("EventBus.....................PASS");
    layerResult.passCount++;

    result = TestScheduler();
    if (!result)
    {
        logger.Error("Scheduler....................FAIL");
        layerResult.result = result;
        layerResult.failedComponent = "Scheduler";
        layerResult.failCount = 1;
        return layerResult;
    }
    logger.Info("Scheduler....................PASS");
    layerResult.passCount++;

    logger.Info("");
    logger.Info("PASS 7");
    logger.Info("");

    return layerResult;
}

} // namespace NAS::Tests
