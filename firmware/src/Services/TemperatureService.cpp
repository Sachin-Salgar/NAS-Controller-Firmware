/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : TemperatureService.cpp
 *
 ******************************************************************************/

#include "TemperatureService.h"

using namespace NAS::Core;

namespace NAS::Services
{

NAS::Objects::TemperatureSensor
TemperatureService::sensors_[SensorCount];

bool TemperatureService::initialized_ = false;

Result TemperatureService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    for (std::uint8_t i = 0U; i < SensorCount; ++i)
    {
        auto result = sensors_[i].Initialize(i);

        if (!result.IsSuccess())
        {
            return result;
        }
    }

    initialized_ = true;

    return Result::Ok();
}

Result TemperatureService::UpdateAll() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    for (auto& sensor : sensors_)
    {
        auto result = sensor.Update();

        if (!result.IsSuccess())
        {
            return result;
        }
    }

    return Result::Ok();
}

float TemperatureService::GetTemperature(std::uint8_t sensorId) noexcept
{
    if ((sensorId >= SensorCount))
    {
        return 0.0F;
    }

    return sensors_[sensorId].GetTemperature();
}

bool TemperatureService::IsAlarmActive(std::uint8_t sensorId) noexcept
{
    if ((sensorId >= SensorCount))
    {
        return false;
    }

    return sensors_[sensorId].IsAlarmActive();
}

NAS::Objects::TemperatureSensor&
TemperatureService::GetSensor(std::uint8_t sensorId) noexcept
{
    return sensors_[sensorId];
}

} // namespace NAS::Services