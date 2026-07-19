#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Utilities
 * File    : TimeHelpers.h
 *
 ******************************************************************************/

#include <cstdint>

namespace NAS::Utilities::TimeHelpers
{

[[nodiscard]]
std::uint32_t MillisecondsToSeconds(
    std::uint32_t milliseconds) noexcept;

[[nodiscard]]
std::uint32_t SecondsToMilliseconds(
    std::uint32_t seconds) noexcept;

[[nodiscard]]
std::uint32_t MinutesToMilliseconds(
    std::uint32_t minutes) noexcept;

[[nodiscard]]
std::uint32_t HoursToMilliseconds(
    std::uint32_t hours) noexcept;

[[nodiscard]]
std::uint32_t DaysToMilliseconds(
    std::uint32_t days) noexcept;

[[nodiscard]]
std::uint32_t MillisecondsToMinutes(
    std::uint32_t milliseconds) noexcept;

[[nodiscard]]
std::uint32_t MillisecondsToHours(
    std::uint32_t milliseconds) noexcept;

[[nodiscard]]
std::uint32_t MillisecondsToDays(
    std::uint32_t milliseconds) noexcept;

[[nodiscard]]
bool HasElapsed(
    std::uint32_t currentTime,
    std::uint32_t previousTime,
    std::uint32_t interval) noexcept;

[[nodiscard]]
std::uint32_t ElapsedMilliseconds(
    std::uint32_t currentTime,
    std::uint32_t previousTime) noexcept;

} // namespace NAS::Utilities::TimeHelpers