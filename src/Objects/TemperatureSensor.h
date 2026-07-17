#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : TemperatureSensor.h
 *
 * Description:
 * Temperature Sensor object.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Objects
{

class TemperatureSensor
{
public:

    TemperatureSensor() = default;

    [[nodiscard]]
    NAS::Core::Result Initialize(std::uint8_t sensorId) noexcept;

    [[nodiscard]]
    NAS::Core::Result Update() noexcept;

    [[nodiscard]]
    float GetTemperature() const noexcept;

    [[nodiscard]]
    float GetMinimumTemperature() const noexcept;

    [[nodiscard]]
    float GetMaximumTemperature() const noexcept;

    [[nodiscard]]
    NAS::Core::Result SetAlarmThreshold(float temperature) noexcept;

    [[nodiscard]]
    float GetAlarmThreshold() const noexcept;

    [[nodiscard]]
    bool IsAlarmActive() const noexcept;

    [[nodiscard]]
    std::uint8_t GetId() const noexcept;

private:

    std::uint8_t sensorId_{0U};

    float currentTemperature_{0.0F};

    float minimumTemperature_{1000.0F};

    float maximumTemperature_{-1000.0F};

    float alarmThreshold_{60.0F};

    bool alarmActive_{false};

    bool initialized_{false};
};

} // namespace NAS::Objects