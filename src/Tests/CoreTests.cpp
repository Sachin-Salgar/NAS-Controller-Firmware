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

#include "TestResult.h"
#include "TestFormatter.h"
#include "../Core/Logger.h"
#include "../Core/Result.h"
#include "../Core/Version.h"
#include "../Core/BuildInfo.h"
#include "../Core/Event.h"
#include "../Core/EventBus.h"
#include "../Core/Scheduler.h"
#include <Arduino.h>

namespace NAS::Tests
{

[[nodiscard]]
static NAS::Core::Result TestLogger() noexcept
{
    Serial.println("  >> TestLogger");
    Serial.flush();

    NAS::Core::Logger logger;

    auto result = logger.Initialize();
    if (!result)
    {
        (void)logger.Critical("[FAIL] Logger initialization failed");
        Serial.println("  << TestLogger FAIL");
        Serial.flush();
        return result;
    }

    result = logger.SetLevel(NAS::Core::LogLevel::Debug);
    if (!result)
    {
        Serial.println("  << TestLogger FAIL (SetLevel)");
        Serial.flush();
        return result;
    }

    if (logger.Level() != NAS::Core::LogLevel::Debug)
    {
        Serial.println("  << TestLogger FAIL (Level check)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    result = logger.Info("[TEST] Logger");
    if (!result)
    {
        Serial.println("  << TestLogger FAIL (Info)");
        Serial.flush();
        return result;
    }

    Serial.println("  << TestLogger PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestResult() noexcept
{
    Serial.println("  >> TestResult");
    Serial.flush();

    NAS::Core::Result success = NAS::Core::Result::Ok();
    if (!success.IsSuccess())
    {
        Serial.println("  << TestResult FAIL (success check)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    NAS::Core::Result failure(NAS::Core::ResultCode::Failed);
    if (failure.IsSuccess())
    {
        Serial.println("  << TestResult FAIL (failure check)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (!failure.IsFailure())
    {
        Serial.println("  << TestResult FAIL (IsFailure check)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (success.Code() != NAS::Core::ResultCode::Success)
    {
        Serial.println("  << TestResult FAIL (Code check)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (!static_cast<bool>(success))
    {
        Serial.println("  << TestResult FAIL (bool cast)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    Serial.println("  << TestResult PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestVersion() noexcept
{
    Serial.println("  >> TestVersion");
    Serial.flush();

    const char* firmwareName = NAS::Core::Version::FirmwareName();
    if (firmwareName == nullptr)
    {
        Serial.println("  << TestVersion FAIL (FirmwareName)");
        Serial.flush();
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
        Serial.println("  << TestVersion FAIL (BuildDate)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* buildTime = NAS::Core::Version::BuildTime();
    if (buildTime == nullptr)
    {
        Serial.println("  << TestVersion FAIL (BuildTime)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* buildConfiguration = NAS::Core::Version::BuildConfiguration();
    if (buildConfiguration == nullptr)
    {
        Serial.println("  << TestVersion FAIL (BuildConfiguration)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* compiler = NAS::Core::Version::Compiler();
    if (compiler == nullptr)
    {
        Serial.println("  << TestVersion FAIL (Compiler)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* targetPlatform = NAS::Core::Version::TargetPlatform();
    if (targetPlatform == nullptr)
    {
        Serial.println("  << TestVersion FAIL (TargetPlatform)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* targetBoard = NAS::Core::Version::TargetBoard();
    if (targetBoard == nullptr)
    {
        Serial.println("  << TestVersion FAIL (TargetBoard)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* gitCommit = NAS::Core::Version::GitCommit();
    if (gitCommit == nullptr)
    {
        Serial.println("  << TestVersion FAIL (GitCommit)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    Serial.println("  << TestVersion PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestBuildInfo() noexcept
{
    Serial.println("  >> TestBuildInfo");
    Serial.flush();

    const char* firmwareName = NAS::Core::BuildInfo::FirmwareName;
    if (firmwareName == nullptr)
    {
        Serial.println("  << TestBuildInfo FAIL (FirmwareName)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    if (NAS::Core::BuildInfo::FirmwareMajor == 0xFFFFU)
    {
        Serial.println("  << TestBuildInfo FAIL (FirmwareMajor)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    const char* buildDate = NAS::Core::BuildInfo::BuildDate;
    if (buildDate == nullptr)
    {
        Serial.println("  << TestBuildInfo FAIL (BuildDate)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* buildTime = NAS::Core::BuildInfo::BuildTime;
    if (buildTime == nullptr)
    {
        Serial.println("  << TestBuildInfo FAIL (BuildTime)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* buildConfiguration = NAS::Core::BuildInfo::BuildConfiguration;
    if (buildConfiguration == nullptr)
    {
        Serial.println("  << TestBuildInfo FAIL (BuildConfiguration)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* compiler = NAS::Core::BuildInfo::Compiler;
    if (compiler == nullptr)
    {
        Serial.println("  << TestBuildInfo FAIL (Compiler)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* targetPlatform = NAS::Core::BuildInfo::TargetPlatform;
    if (targetPlatform == nullptr)
    {
        Serial.println("  << TestBuildInfo FAIL (TargetPlatform)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* targetBoard = NAS::Core::BuildInfo::TargetBoard;
    if (targetBoard == nullptr)
    {
        Serial.println("  << TestBuildInfo FAIL (TargetBoard)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    const char* gitCommit = NAS::Core::BuildInfo::GitCommit;
    if (gitCommit == nullptr)
    {
        Serial.println("  << TestBuildInfo FAIL (GitCommit)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::NullPointer);
    }

    Serial.println("  << TestBuildInfo PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestEvent() noexcept
{
    Serial.println("  >> TestEvent");
    Serial.flush();

    NAS::Core::Event event1;
    if (event1.Type() != NAS::Core::EventType::None)
    {
        Serial.println("  << TestEvent FAIL (event1 type)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    NAS::Core::Event event2(NAS::Core::EventType::SystemStartup, 1U, 42U);
    if (event2.Type() != NAS::Core::EventType::SystemStartup)
    {
        Serial.println("  << TestEvent FAIL (event2 type)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (event2.ObjectId() != 1U)
    {
        Serial.println("  << TestEvent FAIL (event2 ObjectId)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    if (event2.Value() != 42U)
    {
        Serial.println("  << TestEvent FAIL (event2 Value)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    Serial.println("  << TestEvent PASS");
    Serial.flush();
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
    Serial.println("  >> TestEventBus");
    Serial.flush();

    NAS::Core::EventBus eventBus;

    Serial.println("  > EventBus::Initialize");
    Serial.flush();
    auto result = eventBus.Initialize();
    if (!result)
    {
        Serial.println("  << TestEventBus FAIL (Initialize)");
        Serial.flush();
        return result;
    }
    Serial.println("  < EventBus::Initialize OK");
    Serial.flush();

    if (eventBus.SubscriberCount() != 0)
    {
        Serial.println("  << TestEventBus FAIL (SubscriberCount check)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    Serial.println("  > EventBus::Subscribe");
    Serial.flush();
    result = eventBus.Subscribe(TestEventHandler);
    if (!result)
    {
        Serial.println("  << TestEventBus FAIL (Subscribe)");
        Serial.flush();
        return result;
    }
    Serial.println("  < EventBus::Subscribe OK");
    Serial.flush();

    if (eventBus.SubscriberCount() != 1)
    {
        Serial.println("  << TestEventBus FAIL (SubscriberCount check 2)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    eventHandlerCalled = false;

    Serial.println("  > EventBus::Publish");
    Serial.flush();
    NAS::Core::Event event(NAS::Core::EventType::SystemReady);
    result = eventBus.Publish(event);
    Serial.println("  < EventBus::Publish OK");
    Serial.flush();

    if (!result)
    {
        Serial.println("  << TestEventBus FAIL (Publish)");
        Serial.flush();
        return result;
    }

    if (!eventHandlerCalled)
    {
        Serial.println("  << TestEventBus FAIL (Handler not called)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    Serial.println("  > EventBus::Unsubscribe");
    Serial.flush();
    result = eventBus.Unsubscribe(TestEventHandler);
    Serial.println("  < EventBus::Unsubscribe OK");
    Serial.flush();

    if (!result)
    {
        Serial.println("  << TestEventBus FAIL (Unsubscribe)");
        Serial.flush();
        return result;
    }

    if (eventBus.SubscriberCount() != 0)
    {
        Serial.println("  << TestEventBus FAIL (SubscriberCount final)");
        Serial.flush();
        return NAS::Core::Result(NAS::Core::ResultCode::InvalidState);
    }

    Serial.println("  << TestEventBus PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
static NAS::Core::Result TestScheduler() noexcept
{
    Serial.println("  >> TestScheduler");
    Serial.flush();

    auto result = NAS::Core::Scheduler::Initialize();
    if (!result)
    {
        if (result.Code() != NAS::Core::ResultCode::AlreadyInitialized)
        {
            Serial.println("  << TestScheduler FAIL (Initialize)");
            Serial.flush();
            return result;
        }
        Serial.println("  - Scheduler already initialized (expected)");
        Serial.flush();
    }

    Serial.println("  > Scheduler::AddTask");
    Serial.flush();
    static int taskExecutionCount = 0;
    auto task = [](){ taskExecutionCount++; };

    result = NAS::Core::Scheduler::AddTask(task, 100U);
    if (!result)
    {
        Serial.println("  << TestScheduler FAIL (AddTask)");
        Serial.flush();
        return result;
    }
    Serial.println("  < Scheduler::AddTask OK");
    Serial.flush();

    Serial.println("  > Scheduler::Execute");
    Serial.flush();
    result = NAS::Core::Scheduler::Execute();
    Serial.println("  < Scheduler::Execute OK");
    Serial.flush();

    if (!result)
    {
        Serial.println("  << TestScheduler FAIL (Execute)");
        Serial.flush();
        return result;
    }

    Serial.println("  << TestScheduler PASS");
    Serial.flush();
    return NAS::Core::Result::Ok();
}

[[nodiscard]]
LayerResult TestCore() noexcept
{
    Serial.println("TestCore: START");
    Serial.flush();

    NAS::Core::Logger logger;
    (void)logger.Initialize();

    LayerResult layerResult = {NAS::Core::Result::Ok(), nullptr,
        NAS::Core::ResultCode::Success, 0, 0, 0};

    TestFormatter::PrintHeader("CORE");

    auto result = TestLogger();
    if (!result)
    {
        TestFormatter::PrintFail("Logger");
        layerResult.result = result;
        layerResult.failedComponent = "Logger";
        layerResult.failureCode = result.Code();
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Logger");
        layerResult.passCount++;
    }

    result = TestResult();
    if (!result)
    {
        TestFormatter::PrintFail("Result");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Result";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Result");
        layerResult.passCount++;
    }

    result = TestVersion();
    if (!result)
    {
        TestFormatter::PrintFail("Version");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Version";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Version");
        layerResult.passCount++;
    }

    result = TestBuildInfo();
    if (!result)
    {
        TestFormatter::PrintFail("BuildInfo");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "BuildInfo";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("BuildInfo");
        layerResult.passCount++;
    }

    result = TestEvent();
    if (!result)
    {
        TestFormatter::PrintFail("Event");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Event";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Event");
        layerResult.passCount++;
    }

    result = TestEventBus();
    if (!result)
    {
        TestFormatter::PrintFail("EventBus");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "EventBus";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("EventBus");
        layerResult.passCount++;
    }

    result = TestScheduler();
    if (!result)
    {
        TestFormatter::PrintFail("Scheduler");
        if (!layerResult.result)
        {
            layerResult.failedComponent = "Scheduler";
            layerResult.failureCode = result.Code();
        }
        layerResult.failCount++;
    } else {
        TestFormatter::PrintPass("Scheduler");
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

    Serial.println("TestCore: END");
    Serial.flush();
    return layerResult;
}

} // namespace NAS::Tests
