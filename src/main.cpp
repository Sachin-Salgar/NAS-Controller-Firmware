/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Target  : ESP32
 * File    : main.cpp
 *
 ******************************************************************************/

#include <Arduino.h>

#include "Core/BuildInfo.h"
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
}

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println();
    Serial.println("================================");
    Serial.println("NAS Controller Firmware");
    Serial.println("================================");

    Serial.print("Version : ");
    Serial.println(Version::GetString());

    Serial.print("Build   : ");
    Serial.println(BuildInfo::BuildDateTime());

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
    Serial.println("System Ready");
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