#include "LedHardwareTest.h"

#include <Adafruit_NeoPixel.h>

namespace NAS::Tests
{

void RunLedHardwareTest()
{
    Adafruit_NeoPixel strip(60, 18, NEO_GRB + NEO_KHZ800);

    strip.begin();
    strip.setBrightness(100);

    for (int i = 0; i < 60; i++)
    {
        strip.setPixelColor(i, strip.Color(255, 0, 0));
    }

    strip.show();

    while (true)
    {
        delay(1000);
    }
}

}