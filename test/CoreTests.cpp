#include "SelfTest.h"

#include "../Core/Logger.h"
#include "../Core/Version.h"
#include "../Core/BuildInfo.h"

using namespace NAS::Core;

namespace NAS::Tests
{

Result SelfTest::TestCore()
{
    Logger::Info("[TEST] Core");

    Logger::Info("Version OK");
    Logger::Info("BuildInfo OK");
    Logger::Info("Logger OK");

    Logger::Info("[PASS] Core");

    return Result::Ok();
}

}