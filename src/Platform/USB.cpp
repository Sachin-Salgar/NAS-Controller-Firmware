/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : USB.cpp
 *
 ******************************************************************************/

#include "USB.h"

#include <Arduino.h>

using namespace NAS::Core;

namespace NAS::Platform::USB
{
namespace
{
bool initialized = false;
}

Result Initialize(std::uint32_t baudRate) noexcept
{
    if (initialized)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    Serial.begin(baudRate);
    initialized = true;
    return Result::Ok();
}

bool IsConnected() noexcept
{
    return static_cast<bool>(Serial);
}

std::size_t Available() noexcept
{
    return static_cast<std::size_t>(Serial.available());
}

Result Read(char& character) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (Serial.available() == 0)
    {
        return Result(ResultCode::Timeout);
    }

    character = static_cast<char>(Serial.read());
    return Result::Ok();
}

Result ReadLine(char* buffer,
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

    std::size_t index = 0U;

    while ((Serial.available() > 0) && (index < (length - 1U)))
    {
        char character = static_cast<char>(Serial.read());

        if (character == '\n')
        {
            break;
        }

        if (character != '\r')
        {
            buffer[index++] = character;
        }
    }

    buffer[index] = '\0';
    return Result::Ok();
}

Result Write(const char* text) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (text == nullptr)
    {
        return Result(ResultCode::InvalidArgument);
    }

    Serial.print(text);
    return Result::Ok();
}

Result WriteLine(const char* text) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (text == nullptr)
    {
        return Result(ResultCode::InvalidArgument);
    }

    Serial.println(text);
    return Result::Ok();
}

Result Flush() noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    Serial.flush();
    return Result::Ok();
}

Result Write(const std::uint8_t* data,
             std::size_t length) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (data == nullptr || length == 0U)
    {
        return Result(ResultCode::InvalidArgument);
    }

    Serial.write(data, length);
    return Result::Ok();
}

Result Read(std::uint8_t* data,
            std::size_t bufferSize,
            std::size_t& bytesReceived) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (data == nullptr || bufferSize == 0U)
    {
        return Result(ResultCode::InvalidArgument);
    }

    bytesReceived = 0U;

    while ((Serial.available() > 0) && (bytesReceived < bufferSize))
    {
        data[bytesReceived++] = static_cast<std::uint8_t>(Serial.read());
    }

    return Result::Ok();
}

} // namespace NAS::Platform::USB
