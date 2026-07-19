/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Utilities
 * File    : BitHelpers.cpp
 *
 ******************************************************************************/

#include "BitHelpers.h"

namespace NAS::Utilities::BitHelpers
{

bool IsBitSet(
    std::uint32_t value,
    std::uint8_t bit) noexcept
{
    if (bit >= 32U)
    {
        return false;
    }

    return (value & (1UL << bit)) != 0U;
}

void SetBit(
    std::uint32_t& value,
    std::uint8_t bit) noexcept
{
    if (bit >= 32U)
    {
        return;
    }

    value |= (1UL << bit);
}

void ClearBit(
    std::uint32_t& value,
    std::uint8_t bit) noexcept
{
    if (bit >= 32U)
    {
        return;
    }

    value &= ~(1UL << bit);
}

void ToggleBit(
    std::uint32_t& value,
    std::uint8_t bit) noexcept
{
    if (bit >= 32U)
    {
        return;
    }

    value ^= (1UL << bit);
}

std::uint32_t CreateMask(
    std::uint8_t bit) noexcept
{
    if (bit >= 32U)
    {
        return 0U;
    }

    return (1UL << bit);
}

std::uint32_t CreateMask(
    std::uint8_t firstBit,
    std::uint8_t lastBit) noexcept
{
    if ((firstBit > lastBit) || (lastBit >= 32U))
    {
        return 0U;
    }

    std::uint32_t mask = 0U;

    for (std::uint8_t bit = firstBit; bit <= lastBit; ++bit)
    {
        mask |= (1UL << bit);
    }

    return mask;
}

} // namespace NAS::Utilities::BitHelpers