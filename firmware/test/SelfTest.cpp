#include "SelfTest.h"

#include "../Core/Logger.h"

using namespace NAS::Core;

namespace NAS::Tests
{

Result SelfTest::Run()
{
    Logger::Info("===============================");
    Logger::Info("NAS Firmware Self Test");
    Logger::Info("===============================");

    if (!TestCore())      return Result(ResultCode::Failed);
    if (!TestPlatform())  return Result(ResultCode::Failed);
    if (!TestDrivers())   return Result(ResultCode::Failed);
    if (!TestObjects())   return Result(ResultCode::Failed);
    if (!TestServices())  return Result(ResultCode::Failed);
    if (!TestProtocol())  return Result(ResultCode::Failed);
    if (!TestSystem())    return Result(ResultCode::Failed);

    Logger::Info("===============================");
    Logger::Info("ALL TESTS PASSED");
    Logger::Info("===============================");

    return Result::Ok();
}

}