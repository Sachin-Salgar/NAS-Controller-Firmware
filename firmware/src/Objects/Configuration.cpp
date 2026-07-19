/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Configuration.cpp
 *
 ******************************************************************************/

#include "Configuration.h"

#include "../Drivers/StorageDriver.h"

using namespace NAS::Core;

namespace NAS::Objects
{

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

    return NAS::Drivers::StorageDriver::Read(
        "config",
        this,
        sizeof(Configuration));
}

Result Configuration::Save() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Drivers::StorageDriver::Write(
        "config",
        this,
        sizeof(Configuration));
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