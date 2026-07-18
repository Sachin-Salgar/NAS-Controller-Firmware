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

using namespace NAS::Core;

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println();
    Serial.println("================================");
    Serial.println("NAS Controller Boot");
    Serial.println("================================");
    Serial.println("Calling SystemManager::Initialize()");

    auto result =
        NAS::System::SystemManager::Initialize();

    if (!result.IsSuccess())
    {
        Serial.println("SystemManager FAILED");

        while (true)
        {
            delay(1000);
        }
    }

    Serial.println("SystemManager SUCCESS");
}

void loop()
{
    static bool started = false;

    if (!started)
    {
        started = true;

        (void)NAS::System::SystemManager::Run();
    }

    delay(1);
}
