#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Core
 * File    : Event.h
 *
 * Description:
 * Defines the firmware event types and event object used throughout the
 * firmware. Events are exchanged between modules through EventBus.
 *
 ******************************************************************************/

#include <cstdint>

namespace NAS::Core
{

//=============================================================================
// Event Types
//=============================================================================

enum class EventType : std::uint16_t
{
    None = 0,

    //=========================================================================
    // System
    //=========================================================================

    SystemStartup,
    SystemReady,
    SystemShutdown,
    SystemReset,

    //=========================================================================
    // Relay
    //=========================================================================

    RelayOn,
    RelayOff,
    RelayStateChanged,

    //=========================================================================
    // Fan
    //=========================================================================

    FanSpeedChanged,
    FanModeChanged,

    //=========================================================================
    // Temperature
    //=========================================================================

    TemperatureUpdated,
    TemperatureWarning,
    TemperatureCritical,

    //=========================================================================
    // Drive
    //=========================================================================

    DriveConnected,
    DriveDisconnected,
    DriveStateChanged,

    //=========================================================================
    // LED
    //=========================================================================

    LedStateChanged,

    //=========================================================================
    // USB
    //=========================================================================

    UsbConnected,
    UsbDisconnected,
    UsbCommandReceived,

    //=========================================================================
    // Configuration
    //=========================================================================

    ConfigurationLoaded,
    ConfigurationSaved,

    //=========================================================================
    // Error
    //=========================================================================

    Warning,
    Error
};

//=============================================================================
// Event
//=============================================================================

class Event
{
public:

    constexpr Event() noexcept = default;

    constexpr Event(EventType type,
                    std::uint16_t objectId = 0U,
                    std::uint32_t value = 0U) noexcept
        : type_(type),
          objectId_(objectId),
          value_(value)
    {
    }

    [[nodiscard]]
    constexpr EventType Type() const noexcept
    {
        return type_;
    }

    [[nodiscard]]
    constexpr std::uint16_t ObjectId() const noexcept
    {
        return objectId_;
    }

    [[nodiscard]]
    constexpr std::uint32_t Value() const noexcept
    {
        return value_;
    }

private:

    EventType type_ { EventType::None };

    std::uint16_t objectId_ { 0U };

    std::uint32_t value_ { 0U };
};

} // namespace NAS::Core