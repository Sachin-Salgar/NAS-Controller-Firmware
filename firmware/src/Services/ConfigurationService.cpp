/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : ConfigurationService.cpp
 *
 ******************************************************************************/

#include "ConfigurationService.h"

using namespace NAS::Core;

namespace NAS::Services
{

NAS::Objects::Configuration
ConfigurationService::configuration_;

bool ConfigurationService::initialized_ = false;

Result ConfigurationService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    auto result = configuration_.Initialize();

    if (!result.IsSuccess())
    {
        return result;
    }

    result = configuration_.Load();

    if (!result.IsSuccess())
    {
        configuration_.ResetToDefaults();
    }

    initialized_ = true;

    return Result::Ok();
}

Result ConfigurationService::Load() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return configuration_.Load();
}

Result ConfigurationService::Save() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return configuration_.Save();
}

Result ConfigurationService::ResetToDefaults() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return configuration_.ResetToDefaults();
}

NAS::Objects::Configuration&
ConfigurationService::GetConfiguration() noexcept
{
    return configuration_;
}

} // namespace NAS::Services