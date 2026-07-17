/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : TemperatureDriver.cpp
 *
 ******************************************************************************/

#include "TemperatureDriver.h"

#include "../Platform/OneWire.h"

using namespace NAS::Core;

namespace NAS::Drivers
{

TemperatureDriver::Sensor TemperatureDriver::sensors_[MaxSensors] = {};

std::uint8_t TemperatureDriver::sensorCount_ = 0U;

bool TemperatureDriver::initialized_ = false;

Result TemperatureDriver::Initialize(std::uint8_t oneWirePin) noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    auto result = NAS::Platform::OneWire::Initialize(oneWirePin);

    if (!result.IsSuccess())
    {
        return result;
    }

    sensorCount_ = 0U;

    for (auto& sensor : sensors_)
    {
        sensor.present = false;
        sensor.temperature = 0.0F;
    }

    initialized_ = true;

    return Result::Ok();
}

Result TemperatureDriver::DiscoverSensors() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    /*
     * DS18B20 ROM search will be implemented here.
     */

    sensorCount_ = 0U;

    return Result::Ok();
}

std::uint8_t TemperatureDriver::SensorCount() noexcept
{
    return sensorCount_;
}

Result TemperatureDriver::StartConversion() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    /*
     * Broadcast Convert-T command.
     */

    return Result::Ok();
}

Result TemperatureDriver::ReadTemperature(std::uint8_t sensorId,
                                          float& temperature) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (sensorId >= sensorCount_)
    {
        return Result(ResultCode::InvalidArgument);
    }

    /*
     * Scratchpad reading and CRC verification
     * will be implemented later.
     */

    temperature = sensors_[sensorId].temperature;

    return Result::Ok();
}

}