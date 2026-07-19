/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Utilities
 * File    : TimeHelpers.cpp
 *
 ******************************************************************************/

#include "TimeHelpers.h"

namespace NAS::Utilities::TimeHelpers
{

namespace
{

constexpr std::uint32_t MillisecondsPerSecond = 1000U;

constexpr std::uint32_t SecondsPerMinute = 60U;

constexpr std::uint32_t MinutesPerHour = 60U;

constexpr std::uint32_t HoursPerDay = 24U;

} // namespace

std::uint32_t MillisecondsToSeconds(
    std::uint32_t milliseconds) noexcept
{
    return milliseconds / MillisecondsPerSecond;
}

std::uint32_t SecondsToMilliseconds(
    std::uint32_t seconds) noexcept
{
    return seconds * MillisecondsPerSecond;
}

std::uint32_t MinutesToMilliseconds(
    std::uint32_t minutes) noexcept
{
    return minutes *
           SecondsPerMinute *
           MillisecondsPerSecond;
}

std::uint32_t HoursToMilliseconds(
    std::uint32_t hours) noexcept
{
    return hours *
           MinutesPerHour *
           SecondsPerMinute *
           MillisecondsPerSecond;
}

std::uint32_t DaysToMilliseconds(
    std::uint32_t days) noexcept
{
    return days *
           HoursPerDay *
           MinutesPerHour *
           SecondsPerMinute *
           MillisecondsPerSecond;
}

std::uint32_t MillisecondsToMinutes(
    std::uint32_t milliseconds) noexcept
{
    return milliseconds /
           (MillisecondsPerSecond * SecondsPerMinute);
}

std::uint32_t MillisecondsToHours(
    std::uint32_t milliseconds) noexcept
{
    return milliseconds /
           (MillisecondsPerSecond *
            SecondsPerMinute *
            MinutesPerHour);
}

std::uint32_t MillisecondsToDays(
    std::uint32_t milliseconds) noexcept
{
    return milliseconds /
           (MillisecondsPerSecond *
            SecondsPerMinute *
            MinutesPerHour *
            HoursPerDay);
}

bool HasElapsed(
    std::uint32_t currentTime,
    std::uint32_t previousTime,
    std::uint32_t interval) noexcept
{
    return ElapsedMilliseconds(
               currentTime,
               previousTime) >= interval;
}

std::uint32_t ElapsedMilliseconds(
    std::uint32_t currentTime,
    std::uint32_t previousTime) noexcept
{
    return currentTime - previousTime;
}

} // namespace NAS::Utilities::TimeHelpers