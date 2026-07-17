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


    auto result =
        NAS::System::SystemManager::Initialize();

    if (!result.IsSuccess())
    {
        while (true)
        {
            delay(1000);
        }
    }
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
