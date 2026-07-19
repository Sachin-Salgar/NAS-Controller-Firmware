#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Utilities
 * File    : StringHelpers.h
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

namespace NAS::Utilities::StringHelpers
{

[[nodiscard]]
std::size_t Length(
    const char* string) noexcept;

[[nodiscard]]
bool IsEmpty(
    const char* string) noexcept;

[[nodiscard]]
bool Equals(
    const char* left,
    const char* right) noexcept;

[[nodiscard]]
bool StartsWith(
    const char* string,
    const char* prefix) noexcept;

[[nodiscard]]
bool EndsWith(
    const char* string,
    const char* suffix) noexcept;

[[nodiscard]]
bool Copy(
    char* destination,
    std::size_t destinationSize,
    const char* source) noexcept;

[[nodiscard]]
bool Append(
    char* destination,
    std::size_t destinationSize,
    const char* source) noexcept;

[[nodiscard]]
bool ToUpper(
    char* string) noexcept;

[[nodiscard]]
bool ToLower(
    char* string) noexcept;

} // namespace NAS::Utilities::StringHelpers