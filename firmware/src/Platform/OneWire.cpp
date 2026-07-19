/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Platform
 * File    : OneWire.cpp
 *
 ******************************************************************************/

#include "OneWire.h"

#include <OneWire.h>

using namespace NAS::Core;

namespace NAS::Platform::OneWire
{
namespace
{
::OneWire* bus = nullptr;
std::uint8_t pin = 0U;
bool initialized = false;
}

Result Initialize(std::uint8_t pinNumber) noexcept
{
    if (initialized)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    pin = pinNumber;
    bus = new ::OneWire(pin);

    if (bus == nullptr)
    {
        return Result(ResultCode::OutOfMemory);
    }

    initialized = true;
    return Result::Ok();
}

Result Reset() noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    return bus->reset() ? Result::Ok() : Result(ResultCode::CommunicationError);
}

Result WriteByte(std::uint8_t data) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    bus->write(data);
    return Result::Ok();
}

Result ReadByte(std::uint8_t& data) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    data = bus->read();
    return Result::Ok();
}

Result WriteBuffer(const std::uint8_t* buffer,
                   std::size_t length) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (buffer == nullptr)
    {
        return Result(ResultCode::InvalidArgument);
    }

    for (std::size_t index = 0U; index < length; ++index)
    {
        bus->write(buffer[index]);
    }

    return Result::Ok();
}

Result ReadBuffer(std::uint8_t* buffer,
                  std::size_t length) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (buffer == nullptr)
    {
        return Result(ResultCode::InvalidArgument);
    }

    for (std::size_t index = 0U; index < length; ++index)
    {
        buffer[index] = bus->read();
    }

    return Result::Ok();
}

} // namespace NAS::Platform::OneWire