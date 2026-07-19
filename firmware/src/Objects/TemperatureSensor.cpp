/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : TemperatureSensor.cpp
 *
 ******************************************************************************/

#include "TemperatureSensor.h"

#include "../Drivers/TemperatureDriver.h"

using namespace NAS::Core;

namespace NAS::Objects
{

Result TemperatureSensor::Initialize(std::uint8_t sensorId) noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    sensorId_ = sensorId;
    initialized_ = true;

    return Result::Ok();
}

Result TemperatureSensor::Update() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    float temperature = 0.0F;

    auto result =
        NAS::Drivers::TemperatureDriver::ReadTemperature(
            sensorId_,
            temperature);

    if (!result.IsSuccess())
    {
        return result;
    }

    currentTemperature_ = temperature;

    if (temperature < minimumTemperature_)
    {
        minimumTemperature_ = temperature;
    }

    if (temperature > maximumTemperature_)
    {
        maximumTemperature_ = temperature;
    }

    alarmActive_ = (temperature >= alarmThreshold_);

    return Result::Ok();
}

float TemperatureSensor::GetTemperature() const noexcept
{
    return currentTemperature_;
}

float TemperatureSensor::GetMinimumTemperature() const noexcept
{
    return minimumTemperature_;
}

float TemperatureSensor::GetMaximumTemperature() const noexcept
{
    return maximumTemperature_;
}

Result TemperatureSensor::SetAlarmThreshold(float temperature) noexcept
{
    alarmThreshold_ = temperature;

    return Result::Ok();
}

float TemperatureSensor::GetAlarmThreshold() const noexcept
{
    return alarmThreshold_;
}

bool TemperatureSensor::IsAlarmActive() const noexcept
{
    return alarmActive_;
}

std::uint8_t TemperatureSensor::GetId() const noexcept
{
    return sensorId_;
}

} // namespace NAS::Objects