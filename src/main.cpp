#include <Arduino.h>

#include "Core/Version.h"
#include "System/SystemManager.h"


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

    NAS::System::SystemManager::Initialize();
    NAS::System::SystemManager::Start();
}

void loop()
{
    NAS::System::SystemManager::Run();
}
