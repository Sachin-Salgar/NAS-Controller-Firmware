#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : PWMFanDriver.h
 *
 * Description:
 * PWM Fan Driver.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Drivers
{

class PWMFanDriver
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Configure(std::uint8_t channel,
                                       std::uint8_t pin,
                                       std::uint32_t frequency,
                                       std::uint8_t resolution) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetSpeed(std::uint8_t channel,
                                      std::uint8_t percent) noexcept;

    [[nodiscard]]
    static std::uint8_t GetSpeed(std::uint8_t channel) noexcept;

private:

    struct Fan
    {
        std::uint8_t speedPercent;
        bool configured;
    };

    static constexpr std::uint8_t MaxChannels = 2U;

    static Fan fans_[MaxChannels];

    static bool initialized_;
};

}