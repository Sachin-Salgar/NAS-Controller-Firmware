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

    return NAS::Platform::Flash::Read(key,
                                      buffer,
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

    return NAS::Platform::Flash::Write(key,
                                       buffer,
                                       length);
}

Result StorageDriver::Remove(const char* key) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Platform::Flash::Remove(key);
}

Result StorageDriver::Clear() noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    return NAS::Platform::Flash::Clear();
}

} // namespace NAS::Drivers