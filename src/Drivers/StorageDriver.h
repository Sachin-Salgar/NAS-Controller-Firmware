#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : StorageDriver.h
 *
 * Description:
 * Non-volatile storage driver.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Drivers
{

class StorageDriver
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Read(const char* key,
                                  void* buffer,
                                  std::size_t length) noexcept;

    [[nodiscard]]
    static NAS::Core::Result Write(const char* key,
                                   const void* buffer,
                                   std::size_t length) noexcept;

    [[nodiscard]]
    static NAS::Core::Result Remove(const char* key) noexcept;

    [[nodiscard]]
    static NAS::Core::Result Clear() noexcept;

private:

    static bool initialized_;
};

} // namespace NAS::Drivers