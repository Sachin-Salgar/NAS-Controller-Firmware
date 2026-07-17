#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : TemperatureDriver.h
 *
 * Description:
 * DS18B20 Temperature Driver.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Drivers
{

class TemperatureDriver
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize(std::uint8_t oneWirePin) noexcept;

    [[nodiscard]]
    static NAS::Core::Result DiscoverSensors() noexcept;

    [[nodiscard]]
    static std::uint8_t SensorCount() noexcept;

    [[nodiscard]]
    static NAS::Core::Result StartConversion() noexcept;

    [[nodiscard]]
    static NAS::Core::Result ReadTemperature(std::uint8_t sensorId,
                                             float& temperature) noexcept;

private:

    struct Sensor
    {
        std::uint8_t address[8];
        float temperature;
        bool present;
    };

    static constexpr std::uint8_t MaxSensors = 3U;

    static Sensor sensors_[MaxSensors];

    static std::uint8_t sensorCount_;

    static bool initialized_;
};

}