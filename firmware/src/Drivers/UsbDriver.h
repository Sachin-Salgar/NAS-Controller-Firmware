#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Drivers
 * File    : UsbDriver.h
 *
 * Description:
 * USB CDC Driver.
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Drivers
{

class UsbDriver
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize(std::uint32_t baudRate = 115200U) noexcept;

    [[nodiscard]]
    static bool IsConnected() noexcept;

    [[nodiscard]]
    static std::size_t Available() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Read(char& character) noexcept;

    [[nodiscard]]
    static NAS::Core::Result ReadLine(char* buffer,
                                      std::size_t length) noexcept;

    [[nodiscard]]
    static NAS::Core::Result Write(const char* text) noexcept;

    [[nodiscard]]
    static NAS::Core::Result WriteLine(const char* text) noexcept;

    [[nodiscard]]
    static NAS::Core::Result Flush() noexcept;

private:

    static bool initialized_;
};

} // namespace NAS::Drivers