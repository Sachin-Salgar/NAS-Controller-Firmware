#include "Flash.h"

#include <Arduino.h>
#include <Preferences.h>

#include <cstdio>

using namespace NAS::Core;

namespace NAS::Platform::Flash
{
namespace
{
bool initialized = false;
Preferences preferences;

void BuildKey(std::uint32_t address, char* buffer, std::size_t length) noexcept
{
    std::snprintf(buffer, length, "%u", static_cast<unsigned int>(address));
}
}

Result Initialize() noexcept
{
    if (initialized)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    preferences.begin("nascfg", false);
    initialized = true;
    return Result::Ok();
}

Result Read(std::uint32_t address,
            std::uint8_t* buffer,
            std::size_t length) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if ((buffer == nullptr) || (length == 0U))
    {
        return Result(ResultCode::InvalidArgument);
    }

    for (std::size_t index = 0U; index < length; ++index)
    {
        char key[16]{};
        BuildKey(address + static_cast<std::uint32_t>(index), key, sizeof(key));
        buffer[index] = static_cast<std::uint8_t>(preferences.getUChar(key, 0U));
    }

    return Result::Ok();
}

Result Write(std::uint32_t address,
             const std::uint8_t* buffer,
             std::size_t length) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if ((buffer == nullptr) || (length == 0U))
    {
        return Result(ResultCode::InvalidArgument);
    }

    for (std::size_t index = 0U; index < length; ++index)
    {
        char key[16]{};
        BuildKey(address + static_cast<std::uint32_t>(index), key, sizeof(key));
        preferences.putUChar(key, buffer[index]);
    }

    return Result::Ok();
}

Result Erase(std::uint32_t address,
             std::size_t length) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    for (std::size_t index = 0U; index < length; ++index)
    {
        char key[16]{};
        BuildKey(address + static_cast<std::uint32_t>(index), key, sizeof(key));
        preferences.remove(key);
    }

    return Result::Ok();
}

} // namespace NAS::Platform::Flash
