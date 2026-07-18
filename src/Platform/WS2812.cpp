#include "WS2812.H"

#include <Adafruit_NeoPixel.h>

namespace NAS::Platform::WS2812
{

namespace
{

bool initialized = false;

std::uint8_t gpio = 0;

std::uint16_t leds = 0;

std::uint8_t currentBrightness = 102;

Adafruit_NeoPixel strip;

}

Core::Result Initialize(
    std::uint8_t gpioPin,
    std::uint16_t ledCount) noexcept
{
    if (initialized)
    {
        return Core::Result(Core::ResultCode::AlreadyInitialized);
    }

    gpio = gpioPin;
    leds = ledCount;

    strip.updateLength(ledCount);
    strip.setPin(gpioPin);

    strip.begin();

    strip.setBrightness(currentBrightness);

    strip.clear();

    strip.show();

    initialized = true;

    return Core::Result::Ok();
}

Core::Result SetPixel(
    std::uint16_t index,
    const Color& color) noexcept
{
    if (!initialized)
    {
        return Core::Result(Core::ResultCode::NotInitialized);
    }

    if (index >= leds)
    {
        return Core::Result(Core::ResultCode::InvalidArgument);
    }

    strip.setPixelColor(
        index,
        strip.Color(
            color.red,
            color.green,
            color.blue));

    return Core::Result::Ok();
}

Core::Result Fill(
    const Color& color) noexcept
{
    if (!initialized)
    {
        return Core::Result(Core::ResultCode::NotInitialized);
    }

    for (std::uint16_t i = 0; i < leds; ++i)
    {
        strip.setPixelColor(
            i,
            strip.Color(
                color.red,
                color.green,
                color.blue));
    }

    return Core::Result::Ok();
}

Core::Result Clear() noexcept
{
    if (!initialized)
    {
        return Core::Result(Core::ResultCode::NotInitialized);
    }

    strip.clear();

    return Core::Result::Ok();
}

Core::Result SetBrightness(
    std::uint8_t brightness) noexcept
{
    if (!initialized)
    {
        return Core::Result(Core::ResultCode::NotInitialized);
    }

    currentBrightness = brightness;

    strip.setBrightness(brightness);

    return Core::Result::Ok();
}

Core::Result Show() noexcept
{
    if (!initialized)
    {
        return Core::Result(Core::ResultCode::NotInitialized);
    }

    strip.show();

    return Core::Result::Ok();
}

bool IsInitialized() noexcept
{
    return initialized;
}

std::uint16_t GetLedCount() noexcept
{
    return leds;
}

}