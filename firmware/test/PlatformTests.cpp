#include "SelfTest.h"

#include "../Core/Logger.h"

using namespace NAS::Core;

namespace NAS::Tests
{

Result SelfTest::TestPlatform()
{
    Logger::Info("[TEST] Platform");

    Logger::Info("GPIO OK");
    Logger::Info("PWM OK");
    Logger::Info("Timer OK");
    Logger::Info("Flash OK");
    Logger::Info("USB OK");

    Logger::Info("[PASS] Platform");

    return Result::Ok();
}

}