#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Configuration.h
 *
 * Description:
 * Persistent firmware configuration object.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Objects
{

class Configuration
{
public:

    struct RelayConfiguration
    {
        bool defaultState;
        bool enabled;
    };

    struct FanConfiguration
    {
        std::uint8_t minimumSpeed;
        std::uint8_t maximumSpeed;
        bool automaticMode;
    };

    struct TemperatureConfiguration
    {
        float warningThreshold;
        float criticalThreshold;
    };

    struct LedConfiguration
    {
        std::uint8_t brightness;
        bool enabled;
    };

    [[nodiscard]]
    NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    NAS::Core::Result Load() noexcept;

    [[nodiscard]]
    NAS::Core::Result Save() noexcept;

    [[nodiscard]]
    NAS::Core::Result ResetToDefaults() noexcept;

    [[nodiscard]]
    NAS::Core::Result Validate() const noexcept;

    [[nodiscard]]
    RelayConfiguration& Relay() noexcept;

    [[nodiscard]]
    FanConfiguration& Fan() noexcept;

    [[nodiscard]]
    TemperatureConfiguration& Temperature() noexcept;

    [[nodiscard]]
    LedConfiguration& Led() noexcept;

private:

    RelayConfiguration relay_;

    FanConfiguration fan_;

    TemperatureConfiguration temperature_;

    LedConfiguration led_;

    bool initialized_{false};
};

} // namespace NAS::Objects