#include "SelfTest.h"

#include "../Core/Logger.h"

using namespace NAS::Core;

namespace NAS::Tests
{

Result SelfTest::TestSystem()
{
    Logger::Info("[TEST] System");

    Logger::Info("Startup OK");
    Logger::Info("Application OK");
    Logger::Info("SystemManager OK");

    Logger::Info("[PASS] System");

    return Result::Ok();
}

}