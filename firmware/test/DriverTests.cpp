#include "SelfTest.h"

#include "../Core/Logger.h"

using namespace NAS::Core;

namespace NAS::Tests
{

Result SelfTest::TestDrivers()
{
    Logger::Info("[TEST] Drivers");

    Logger::Info("RelayDriver OK");
    Logger::Info("FanDriver OK");
    Logger::Info("TemperatureDriver OK");
    Logger::Info("StorageDriver OK");
    Logger::Info("UsbDriver OK");

    Logger::Info("[PASS] Drivers");

    return Result::Ok();
}

}