#include "UART.h"

#include <Arduino.h>

using namespace NAS::Core;

namespace NAS::Platform::UART
{
namespace
{
bool initialized = false;
constexpr std::size_t BufferSize = 64U;
std::uint8_t buffer[BufferSize]{};
}

Result Initialize(std::uint8_t txPin,
                  std::uint8_t rxPin,
                  std::uint32_t baudRate) noexcept
{
    if (initialized)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    Serial2.begin(baudRate, SERIAL_8N1, rxPin, txPin);
    initialized = true;
    return Result::Ok();
}

std::size_t Available() noexcept
{
    return static_cast<std::size_t>(Serial2.available());
}

Result Write(const std::uint8_t* bufferData,
             std::size_t length) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (bufferData == nullptr)
    {
        return Result(ResultCode::InvalidArgument);
    }

    Serial2.write(bufferData, length);
    return Result::Ok();
}

Result Read(std::uint8_t* bufferData,
            std::size_t length) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (bufferData == nullptr)
    {
        return Result(ResultCode::InvalidArgument);
    }

    for (std::size_t index = 0U; index < length; ++index)
    {
        if (Serial2.available() == 0)
        {
            return Result(ResultCode::Timeout);
        }

        bufferData[index] = static_cast<std::uint8_t>(Serial2.read());
    }

    return Result::Ok();
}

} // namespace NAS::Platform::UART
