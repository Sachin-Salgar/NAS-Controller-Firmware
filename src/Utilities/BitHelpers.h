#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Utilities
 * File    : BitHelpers.h
 *
 ******************************************************************************/

#include <cstdint>

namespace NAS::Utilities::BitHelpers
{

[[nodiscard]]
bool IsBitSet(
    std::uint32_t value,
    std::uint8_t bit) noexcept;

void SetBit(
    std::uint32_t& value,
    std::uint8_t bit) noexcept;

void ClearBit(
    std::uint32_t& value,
    std::uint8_t bit) noexcept;

void ToggleBit(
    std::uint32_t& value,
    std::uint8_t bit) noexcept;

[[nodiscard]]
std::uint32_t CreateMask(
    std::uint8_t bit) noexcept;

[[nodiscard]]
std::uint32_t CreateMask(
    std::uint8_t firstBit,
    std::uint8_t lastBit) noexcept;

} // namespace NAS::Utilities::BitHelpers