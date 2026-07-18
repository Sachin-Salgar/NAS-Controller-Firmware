/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : UsbDriver.cpp
 *
 ******************************************************************************/

#include "UsbDriver.h"

#include "../Platform/USB.h"

using namespace NAS::Core;

namespace NAS::Drivers
{

bool UsbDriver::initialized_ = false;

Result UsbDriver::Initialize(std::uint32_t baudRate) noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    auto result = NAS::Platform::USB::Initialize(baudRate);

    // AlreadyInitialized is acceptable (platform layer may have initialized already)
    if (!result.IsSuccess() && result.Code() != ResultCode::AlreadyInitialized)
    {
        return result;
    }

    initialized_ = true;

    return Result::Ok();
}

bool UsbDriver::IsConnected() noexcept
{
    return NAS::Platform::USB::IsConnected();
}

std::size_t UsbDriver::Available() noexcept
{
    return NAS::Platform::USB::Available();
}

Result UsbDriver::Read(char& character) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Platform::USB::Read(character);
}

Result UsbDriver::ReadLine(char* buffer,
                           std::size_t length) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Platform::USB::ReadLine(buffer, length);
}

Result UsbDriver::Write(const char* text) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Platform::USB::Write(text);
}

Result UsbDriver::WriteLine(const char* text) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Platform::USB::WriteLine(text);
}

Result UsbDriver::Flush() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Platform::USB::Flush();
}

} // namespace NAS::Drivers
