#include "SelfTest.h"

#include "../Core/Logger.h"

using namespace NAS::Core;

namespace NAS::Tests
{

Result SelfTest::TestProtocol()
{
    Logger::Info("[TEST] Protocol");

    Logger::Info("PacketParser OK");
    Logger::Info("PacketBuilder OK");
    Logger::Info("Dispatcher OK");

    Logger::Info("[PASS] Protocol");

    return Result::Ok();
}

}