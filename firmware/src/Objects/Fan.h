#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Fan.h
 *
 * Description:
 * Fan object.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Objects
{

enum class FanMode : std::uint8_t
{
    Manual = 0,
    Automatic
};

class Fan
{
public:

    Fan() = default;

    [[nodiscard]]
    NAS::Core::Result Initialize(std::uint8_t channel,
                                 std::uint8_t pin,
                                 std::uint32_t frequency,
                                 std::uint8_t resolution) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetSpeed(std::uint8_t percent) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetMode(FanMode mode) noexcept;

    [[nodiscard]]
    std::uint8_t GetSpeed() const noexcept;

    [[nodiscard]]
    FanMode GetMode() const noexcept;

    [[nodiscard]]
    std::uint8_t GetChannel() const noexcept;

private:

    std::uint8_t channel_{0U};

    std::uint8_t speed_{0U};

    FanMode mode_{FanMode::Automatic};

    bool initialized_{false};
};

} // namespace NAS::Objects