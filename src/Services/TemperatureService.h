#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : TemperatureService.h
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"
#include "../Objects/TemperatureSensor.h"

namespace NAS::Services
{

class TemperatureService
{
public:

    static constexpr std::uint8_t SensorCount = 3U;

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result UpdateAll() noexcept;

    [[nodiscard]]
    static float GetTemperature(std::uint8_t sensorId) noexcept;

    [[nodiscard]]
    static bool IsAlarmActive(std::uint8_t sensorId) noexcept;

    [[nodiscard]]
    static NAS::Objects::TemperatureSensor&
    GetSensor(std::uint8_t sensorId) noexcept;

private:

    static NAS::Objects::TemperatureSensor sensors_[SensorCount];

    static bool initialized_;
};

} // namespace NAS::Services