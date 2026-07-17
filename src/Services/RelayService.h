#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : RelayService.h
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"
#include "../Objects/Relay.h"
#include "../Config/HardwareConfig.h"

namespace NAS::Services
{

class RelayService
{
public:

    static constexpr std::uint8_t RelayCount = NAS::Config::Hardware::RelayCount;

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result TurnOn(std::uint8_t relayId) noexcept;

    [[nodiscard]]
    static NAS::Core::Result TurnOff(std::uint8_t relayId) noexcept;

    [[nodiscard]]
    static NAS::Core::Result Toggle(std::uint8_t relayId) noexcept;

    [[nodiscard]]
    static NAS::Objects::Relay& GetRelay(std::uint8_t relayId) noexcept;

private:

    static NAS::Objects::Relay relays_[RelayCount];

    static bool initialized_;
};

}
