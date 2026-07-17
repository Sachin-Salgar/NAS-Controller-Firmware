#include "SPI.h"

#include <Arduino.h>
#include <SPI.h>

using namespace NAS::Core;

namespace NAS::Platform::SPI
{
namespace
{
bool initialized = false;
std::uint8_t chipSelectPin = 0U;
}

Result Initialize(std::uint8_t sckPin,
                  std::uint8_t misoPin,
                  std::uint8_t mosiPin,
                  std::uint8_t csPin) noexcept
{
    if (initialized)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    chipSelectPin = csPin;
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH);
    ::SPI.begin(sckPin, misoPin, mosiPin, csPin);
    initialized = true;
    return Result::Ok();
}

Result Transfer(const std::uint8_t* txBuffer,
                std::uint8_t* rxBuffer,
                std::size_t length) noexcept
{
    if (!initialized)
    {
        return Result(ResultCode::NotInitialized);
    }

    if ((txBuffer == nullptr) || (rxBuffer == nullptr))
    {
        return Result(ResultCode::InvalidArgument);
    }

    digitalWrite(chipSelectPin, LOW);
    ::SPI.beginTransaction(SPISettings(1000000U, MSBFIRST, SPI_MODE0));
    ::SPI.transferBytes(txBuffer, rxBuffer, static_cast<size_t>(length));
    ::SPI.endTransaction();
    digitalWrite(chipSelectPin, HIGH);
    return Result::Ok();
}

} // namespace NAS::Platform::SPI
