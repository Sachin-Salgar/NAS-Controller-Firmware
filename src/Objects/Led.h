#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Led.h
 *
 * Description:
 * Logical Drive LED Object.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Objects
{

enum class DriveLedState : std::uint8_t
{
    Off = 0,

    Idle,

    Reading,

    Writing,

    Error,

    Missing,

    Rebuilding
};

class Led
{
public:

    Led() = default;

    [[nodiscard]]
    NAS::Core::Result Initialize(
        std::uint16_t ledIndex) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetState(
        DriveLedState state) noexcept;

    [[nodiscard]]
    NAS::Core::Result Refresh() noexcept;

    [[nodiscard]]
    std::uint16_t GetIndex() const noexcept;

    [[nodiscard]]
    DriveLedState GetState() const noexcept;

private:

    struct Color
    {
        std::uint8_t red;
        std::uint8_t green;
        std::uint8_t blue;
    };

    [[nodiscard]]
    static Color StateToColor(
        DriveLedState state) noexcept;

private:

    bool initialized_{false};

    std::uint16_t ledIndex_{0U};

    DriveLedState state_{DriveLedState::Off};

};

} // namespace NAS::Objects