/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : StorageDriver.cpp
 *
 ******************************************************************************/

#include "StorageDriver.h"

#include "../Platform/Flash.h"

using namespace NAS::Core;

namespace NAS::Drivers
{

bool StorageDriver::initialized_ = false;

Result StorageDriver::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    auto result = NAS::Platform::Flash::Initialize();

    if (!result.IsSuccess())
    {
        return result;
    }

    initialized_ = true;

    return Result::Ok();
}

Result StorageDriver::Read(const char* key,
                           void* buffer,
                           std::size_t length) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if ((key == nullptr) || (buffer == nullptr) || (length == 0U))
    {
        return Result(ResultCode::InvalidArgument);
    }

    // Note: Platform Flash API is address-based, not key-based
    // For now, use address 0 as default storage location
    return NAS::Platform::Flash::Read(0U,
                                      static_cast<std::uint8_t*>(buffer),
                                      length);
}

Result StorageDriver::Write(const char* key,
                            const void* buffer,
                            std::size_t length) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if ((key == nullptr) || (buffer == nullptr) || (length == 0U))
    {
        return Result(ResultCode::InvalidArgument);
    }

    // Note: Platform Flash API is address-based, not key-based
    // For now, use address 0 as default storage location
    return NAS::Platform::Flash::Write(0U,
                                       static_cast<const std::uint8_t*>(buffer),
                                       length);
}

Result StorageDriver::Remove(const char* key) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (key == nullptr)
    {
        return Result(ResultCode::InvalidArgument);
    }

    // Erase a small region to simulate key removal
    return NAS::Platform::Flash::Erase(0U, 1U);
}

Result StorageDriver::Clear() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    // Clear is implemented using Erase for entire storage space
    // This clears a default storage region (0 to 4096 bytes)
    return NAS::Platform::Flash::Erase(0U, 4096U);
}

} // namespace NAS::Drivers
