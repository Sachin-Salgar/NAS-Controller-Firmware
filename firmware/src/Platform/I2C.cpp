#include "I2C.h"

#include <Arduino.h>
#include <Wire.h>

using namespace NAS::Core;

namespace NAS::Platform::I2C
{
namespace
{
bool initialized = false;
}

Result Initialize(std::uint8_t sdaPin,
                  std::uint8_t sclPin,
                  std::uint32_t clockSpeed) noexcept
{
    if (initialized)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    Wire.begin(sdaPin, sclPin);
    initialized = true;
    return Result::Ok();
}

Result WriteByte(std::uint8_t deviceAddress,
                 std::uint8_t registerAddress,
                 std::uint8_t value) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.write(value);
    return Wire.endTransmission() == 0 ? Result::Ok() : Result(ResultCode::CommunicationError);
}

Result ReadByte(std::uint8_t deviceAddress,
                std::uint8_t registerAddress,
                std::uint8_t& value) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    if (Wire.endTransmission() != 0)
    {
        return Result(ResultCode::CommunicationError);
    }

    if (Wire.requestFrom(deviceAddress, 1U) != 1U)
    {
        return Result(ResultCode::CommunicationError);
    }

    value = static_cast<std::uint8_t>(Wire.read());
    return Result::Ok();
}

Result WriteBuffer(std::uint8_t deviceAddress,
                   const std::uint8_t* buffer,
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

    Wire.beginTransmission(deviceAddress);
    for (std::size_t index = 0U; index < length; ++index)
    {
        Wire.write(buffer[index]);
    }

    return Wire.endTransmission() == 0 ? Result::Ok() : Result(ResultCode::CommunicationError);
}

Result ReadBuffer(std::uint8_t deviceAddress,
                  std::uint8_t* buffer,
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

    if (Wire.requestFrom(static_cast<int>(deviceAddress), static_cast<int>(length)) != static_cast<int>(length))
    {
        return Result(ResultCode::CommunicationError);
    }

    for (std::size_t index = 0U; index < length; ++index)
    {
        buffer[index] = static_cast<std::uint8_t>(Wire.read());
    }

    return Result::Ok();
}

} // namespace NAS::Platform::I2C
