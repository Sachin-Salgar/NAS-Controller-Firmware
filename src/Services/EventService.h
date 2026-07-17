#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : EventService.h
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Services
{

enum class EventType : std::uint16_t
{
    None = 0,

    SystemStarted,
    SystemReady,
    SystemShutdown,

    RelayChanged,
    FanChanged,

    TemperatureUpdated,
    TemperatureAlarm,

    DriveInserted,
    DriveRemoved,
    DriveStateChanged,

    UsbConnected,
    UsbDisconnected,

    ConfigurationChanged,

    Warning,
    Error
};

struct Event
{
    EventType type{EventType::None};

    std::uint32_t parameter1{0U};

    std::uint32_t parameter2{0U};
};

class EventService
{
public:

    static constexpr std::size_t QueueSize = 32U;

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Publish(
        const Event& event) noexcept;

    [[nodiscard]]
    static NAS::Core::Result GetNextEvent(
        Event& event) noexcept;

    [[nodiscard]]
    static bool HasPendingEvents() noexcept;

private:

    static Event queue_[QueueSize];

    static std::size_t head_;

    static std::size_t tail_;

    static std::size_t count_;

    static bool initialized_;
};

} // namespace NAS::Services