#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Drive.h
 *
 * Description:
 * NAS Drive Object.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Objects
{

enum class DriveState : std::uint8_t
{
    Offline = 0,
    Standby,
    Idle,
    Reading,
    Writing,
    Error
};

enum class DriveHealth : std::uint8_t
{
    Unknown = 0,
    Good,
    Warning,
    Critical
};

class Drive
{
public:

    Drive() = default;

    [[nodiscard]]
    NAS::Core::Result Initialize(std::uint8_t id) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetPresent(bool present) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetState(DriveState state) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetTemperature(float temperature) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetHealth(DriveHealth health) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetCapacity(std::uint64_t bytes) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetSerialNumber(const char* serial) noexcept;

    [[nodiscard]]
    std::uint8_t GetId() const noexcept;

    [[nodiscard]]
    bool IsPresent() const noexcept;

    [[nodiscard]]
    DriveState GetState() const noexcept;

    [[nodiscard]]
    DriveHealth GetHealth() const noexcept;

    [[nodiscard]]
    float GetTemperature() const noexcept;

    [[nodiscard]]
    std::uint64_t GetCapacity() const noexcept;

    [[nodiscard]]
    const char* GetSerialNumber() const noexcept;

private:

    static constexpr std::size_t SerialLength = 32U;

    std::uint8_t id_{0U};

    bool present_{false};

    DriveState state_{DriveState::Offline};

    DriveHealth health_{DriveHealth::Unknown};

    float temperature_{0.0F};

    std::uint64_t capacity_{0U};

    char serial_[SerialLength]{};

    bool initialized_{false};
};

} // namespace NAS::Objects