#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : RelayDriver.h
 *
 * Description:
 * Relay hardware driver.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Drivers
{

enum class RelayState : std::uint8_t
{
    Off = 0,
    On
};

class RelayDriver
{
public:

    RelayDriver() = default;

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Configure(std::uint8_t relayId,
                                       std::uint8_t gpioPin,
                                       bool activeLow = true) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetState(std::uint8_t relayId,
                                      RelayState state) noexcept;

    [[nodiscard]]
    static NAS::Core::Result Toggle(std::uint8_t relayId) noexcept;

    [[nodiscard]]
    static RelayState GetState(std::uint8_t relayId) noexcept;

private:

    struct Relay
    {
        std::uint8_t pin;
        bool activeLow;
        RelayState state;
        bool configured;
    };

    static constexpr std::uint8_t MaxRelays = 2U;

    static Relay relays_[MaxRelays];

    static bool initialized_;
};

} // namespace NAS::Drivers