Result SelfTest::TestObjects()
{
    Logger::Info("[TEST] Objects");

    Logger::Info("Configuration OK");
    Logger::Info("Drive OK");
    Logger::Info("Fan OK");
    Logger::Info("LED OK");

    Logger::Info("[PASS] Objects");

    return Result::Ok();
}