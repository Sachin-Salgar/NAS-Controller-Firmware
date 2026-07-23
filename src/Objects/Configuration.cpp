/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Configuration.cpp
 *
 ******************************************************************************/

#include "Configuration.h"

#include <cstring>

#include "../Drivers/StorageDriver.h"
#include "../Utilities/CRC16.h"

using namespace NAS::Core;

namespace NAS::Objects
{

namespace
{

constexpr std::uint32_t ConfigurationMagic = 0x31474643U;
constexpr std::uint16_t ConfigurationVersion = 1U;

struct ConfigurationRecord
{
    std::uint32_t magic;
    std::uint16_t version;
    Configuration::RelayConfiguration relay;
    Configuration::FanConfiguration fan;
    Configuration::TemperatureConfiguration temperature;
    Configuration::LedConfiguration led;
    std::uint16_t crc;
};

} // namespace

Result Configuration::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    initialized_ = true;

    return ResetToDefaults();
}

Result Configuration::Load() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    ConfigurationRecord record{};

    auto result = NAS::Drivers::StorageDriver::Read(
        "config",
        &record,
        sizeof(record));

    if (!result.IsSuccess())
    {
        return result;
    }

    const std::uint16_t crc = record.crc;
    record.crc = 0U;

    if ((record.magic != ConfigurationMagic) ||
        (record.version != ConfigurationVersion) ||
        !NAS::Utilities::CRC16::Verify(
            reinterpret_cast<const std::uint8_t*>(&record),
            sizeof(record),
            crc))
    {
        auto resetResult = ResetToDefaults();

        if (!resetResult.IsSuccess())
        {
            return resetResult;
        }

        return Result(ResultCode::InvalidConfiguration);
    }

    relay_ = record.relay;
    fan_ = record.fan;
    temperature_ = record.temperature;
    led_ = record.led;

    result = Validate();

    if (!result.IsSuccess())
    {
        auto resetResult = ResetToDefaults();

        if (!resetResult.IsSuccess())
        {
            return resetResult;
        }
    }

    return result;
}

Result Configuration::Save() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    auto result = Validate();

    if (!result.IsSuccess())
    {
        return result;
    }

    ConfigurationRecord record{
        ConfigurationMagic,
        ConfigurationVersion,
        relay_,
        fan_,
        temperature_,
        led_,
        0U
    };

    record.crc = NAS::Utilities::CRC16::Calculate(
        reinterpret_cast<const std::uint8_t*>(&record),
        sizeof(record));

    return NAS::Drivers::StorageDriver::Write(
        "config",
        &record,
        sizeof(record));
}

Result Configuration::ResetToDefaults() noexcept
{
    relay_.defaultState = false;
    relay_.enabled = true;

    fan_.minimumSpeed = 20U;
    fan_.maximumSpeed = 100U;
    fan_.automaticMode = true;

    temperature_.warningThreshold = 45.0F;
    temperature_.criticalThreshold = 55.0F;

    led_.brightness = 128U;
    led_.enabled = true;

    return Result::Ok();
}

Result Configuration::Validate() const noexcept
{
    if (fan_.minimumSpeed > fan_.maximumSpeed)
    {
        return Result(ResultCode::InvalidConfiguration);
    }

    if (fan_.maximumSpeed > 100U)
    {
        return Result(ResultCode::InvalidConfiguration);
    }

    if (temperature_.warningThreshold >= temperature_.criticalThreshold)
    {
        return Result(ResultCode::InvalidConfiguration);
    }

    if (led_.brightness > 255U)
    {
        return Result(ResultCode::InvalidConfiguration);
    }

    return Result::Ok();
}

Configuration::RelayConfiguration&
Configuration::Relay() noexcept
{
    return relay_;
}

Configuration::FanConfiguration&
Configuration::Fan() noexcept
{
    return fan_;
}

Configuration::TemperatureConfiguration&
Configuration::Temperature() noexcept
{
    return temperature_;
}

Configuration::LedConfiguration&
Configuration::Led() noexcept
{
    return led_;
}

} // namespace NAS::Objects