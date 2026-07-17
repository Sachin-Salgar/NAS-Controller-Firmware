#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : ConfigurationService.h
 *
 ******************************************************************************/

#include "../Core/Result.h"
#include "../Objects/Configuration.h"

namespace NAS::Services
{

class ConfigurationService
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Load() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Save() noexcept;

    [[nodiscard]]
    static NAS::Core::Result ResetToDefaults() noexcept;

    [[nodiscard]]
    static NAS::Objects::Configuration&
    GetConfiguration() noexcept;

private:

    static NAS::Objects::Configuration configuration_;

    static bool initialized_;
};

} // namespace NAS::Services