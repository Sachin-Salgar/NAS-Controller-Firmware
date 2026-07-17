/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Utilities
 * File    : StringHelpers.cpp
 *
 ******************************************************************************/

#include "StringHelpers.h"

#include <cctype>
#include <cstring>

namespace NAS::Utilities::StringHelpers
{

std::size_t Length(
    const char* string) noexcept
{
    return (string == nullptr) ? 0U : std::strlen(string);
}

bool IsEmpty(
    const char* string) noexcept
{
    return (string == nullptr) || (string[0] == '\0');
}

bool Equals(
    const char* left,
    const char* right) noexcept
{
    if ((left == nullptr) || (right == nullptr))
    {
        return false;
    }

    return std::strcmp(left, right) == 0;
}

bool StartsWith(
    const char* string,
    const char* prefix) noexcept
{
    if ((string == nullptr) || (prefix == nullptr))
    {
        return false;
    }

    while (*prefix != '\0')
    {
        if (*string++ != *prefix++)
        {
            return false;
        }
    }

    return true;
}

bool EndsWith(
    const char* string,
    const char* suffix) noexcept
{
    if ((string == nullptr) || (suffix == nullptr))
    {
        return false;
    }

    const std::size_t stringLength = std::strlen(string);
    const std::size_t suffixLength = std::strlen(suffix);

    if (suffixLength > stringLength)
    {
        return false;
    }

    return std::strcmp(
        string + (stringLength - suffixLength),
        suffix) == 0;
}

bool Copy(
    char* destination,
    std::size_t destinationSize,
    const char* source) noexcept
{
    if ((destination == nullptr) ||
        (source == nullptr) ||
        (destinationSize == 0U))
    {
        return false;
    }

    std::strncpy(
        destination,
        source,
        destinationSize - 1U);

    destination[destinationSize - 1U] = '\0';

    return true;
}

bool Append(
    char* destination,
    std::size_t destinationSize,
    const char* source) noexcept
{
    if ((destination == nullptr) ||
        (source == nullptr))
    {
        return false;
    }

    const std::size_t length = std::strlen(destination);

    if (length >= destinationSize)
    {
        return false;
    }

    std::strncpy(
        destination + length,
        source,
        destinationSize - length - 1U);

    destination[destinationSize - 1U] = '\0';

    return true;
}

bool ToUpper(
    char* string) noexcept
{
    if (string == nullptr)
    {
        return false;
    }

    while (*string != '\0')
    {
        *string = static_cast<char>(
            std::toupper(
                static_cast<unsigned char>(*string)));

        ++string;
    }

    return true;
}

bool ToLower(
    char* string) noexcept
{
    if (string == nullptr)
    {
        return false;
    }

    while (*string != '\0')
    {
        *string = static_cast<char>(
            std::tolower(
                static_cast<unsigned char>(*string)));

        ++string;
    }

    return true;
}

} // namespace NAS::Utilities::StringHelpers