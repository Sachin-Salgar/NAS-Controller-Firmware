#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Relay.h
 *
 * Description:
 * Relay object.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"
#include "../Drivers/RelayDriver.h"

namespace NAS::Objects
{

class Relay
{
public:

    Relay() = default;

    [[nodiscard]]
    NAS::Core::Result Initialize(std::uint8_t id,
                                 std::uint8_t gpioPin,
                                 bool activeLow = true,
                                 bool defaultState = false) noexcept;

    [[nodiscard]]
    NAS::Core::Result TurnOn() noexcept;

    [[nodiscard]]
    NAS::Core::Result TurnOff() noexcept;

    [[nodiscard]]
    NAS::Core::Result Toggle() noexcept;

    [[nodiscard]]
    NAS::Core::Result SetEnabled(bool enabled) noexcept;

    [[nodiscard]]
    bool IsOn() const noexcept;

    [[nodiscard]]
    bool IsEnabled() const noexcept;

    [[nodiscard]]
    std::uint8_t GetId() const noexcept;

private:

    std::uint8_t id_ {0U};

    bool enabled_ {true};

    bool state_ {false};

    bool defaultState_ {false};

    bool initialized_ {false};
};

} // namespace NAS::Objects