/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Target  : ESP32
 * File    : main.cpp
 *
 ******************************************************************************/

#include <Arduino.h>

#include "Core/Version.h"
#include "System/SystemManager.h"
#include "Tests/SelfTest.h"

using namespace NAS::Core;

namespace
{

void Halt(const char* message)
{
    Serial.println();
    Serial.println("================================");
    Serial.println("FATAL ERROR");
    Serial.println("================================");
    Serial.println(message);

    while (true)
    {
        delay(1000);
    }
}

} // anonymous namespace

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println();
    Serial.println("================================");
    Serial.println(Version::FirmwareName());
    Serial.println("================================");

    Serial.print("Version : ");
    Serial.print(Version::Major());
    Serial.print(".");
    Serial.print(Version::Minor());
    Serial.print(".");
    Serial.println(Version::Patch());

    Serial.print("Build Date : ");
    Serial.print(Version::BuildDate());
    Serial.print(" ");
    Serial.println(Version::BuildTime());

    Serial.print("Configuration : ");
    Serial.println(Version::BuildConfiguration());

    Serial.print("Compiler : ");
    Serial.println(Version::Compiler());

    Serial.print("Platform : ");
    Serial.println(Version::TargetPlatform());

    Serial.print("Board : ");
    Serial.println(Version::TargetBoard());

    Serial.print("Git Commit : ");
    Serial.println(Version::GitCommit());

    Serial.println();

    Serial.println("[1/3] Initializing System...");

    auto result = NAS::System::SystemManager::Initialize();

    if (!result.IsSuccess())
    {
        Halt("SystemManager initialization FAILED.");
    }

    Serial.println("PASS");
    Serial.println();

    Serial.println("[2/3] Running Boot Self Test...");

    result = NAS::Tests::SelfTest::Run();

    if (!result.IsSuccess())
    {
        Halt("Boot Self Test FAILED.");
    }

    Serial.println("PASS");
    Serial.println();

    Serial.println("[3/3] Starting Firmware...");
    Serial.println();

    Serial.println("================================");
    Serial.println("SYSTEM READY");
    Serial.println("================================");
}

void loop()
{
    static bool started = false;

    if (!started)
    {
        started = true;

        auto result = NAS::System::SystemManager::Run();

        if (!result.IsSuccess())
        {
            Halt("SystemManager runtime FAILED.");
        }
    }

    delay(1);
}