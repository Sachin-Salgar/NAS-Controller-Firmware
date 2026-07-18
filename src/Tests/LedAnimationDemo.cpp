#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

namespace NAS::Tests
{

constexpr uint8_t LED_PIN = 18;
constexpr uint16_t LED_COUNT = 60;

Adafruit_NeoPixel strip(
    LED_COUNT,
    LED_PIN,
    NEO_GRB + NEO_KHZ800);

void RunLedAnimationDemo()
{
    strip.begin();
    strip.setBrightness(100);
    strip.show();

    while (true)
    {
        // Rainbow
        for (long hue = 0; hue < 65536; hue += 256)
        {
            for (int i = 0; i < LED_COUNT; i++)
            {
                strip.setPixelColor(
                    i,
                    strip.gamma32(
                        strip.ColorHSV(hue + i * 65536L / LED_COUNT)));
            }

            strip.show();
            delay(8);
        }

        // Theater Chase
        for (int j = 0; j < 20; j++)
        {
            for (int q = 0; q < 3; q++)
            {
                strip.clear();

                for (int i = q; i < LED_COUNT; i += 3)
                {
                    strip.setPixelColor(i, strip.Color(255,255,255));
                }

                strip.show();
                delay(80);
            }
        }

        // Sparkle
        for (int i = 0; i < 300; i++)
        {
            strip.clear();

            int p = random(LED_COUNT);

            strip.setPixelColor(
                p,
                strip.Color(
                    random(256),
                    random(256),
                    random(256)));

            strip.show();
            delay(15);
        }

        // Breathing White
        for (int b = 10; b <= 255; b += 5)
        {
            strip.setBrightness(b);

            strip.fill(strip.Color(255,255,255));
            strip.show();
            delay(15);
        }

        for (int b = 255; b >= 10; b -= 5)
        {
            strip.setBrightness(b);

            strip.fill(strip.Color(255,255,255));
            strip.show();
            delay(15);
        }

        strip.setBrightness(100);
    }
}

}