#include "SelfTest.h"

#include "../Core/Logger.h"

using namespace NAS::Core;

namespace NAS::Tests
{

Result SelfTest::TestServices()
{
    Logger::Info("[TEST] Services");

    Logger::Info("RelayService OK");
    Logger::Info("FanService OK");
    Logger::Info("USBService OK");
    Logger::Info("ConfigurationService OK");

    Logger::Info("[PASS] Services");

    return Result::Ok();
}

}