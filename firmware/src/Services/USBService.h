#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : USBService.h
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Services
{

class USBService
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Process() noexcept;

    [[nodiscard]]
    static NAS::Core::Result Send(const std::uint8_t* data,
                                  std::size_t length) noexcept;

    [[nodiscard]]
    static NAS::Core::Result Receive(std::uint8_t* data,
                                     std::size_t bufferSize,
                                     std::size_t& bytesReceived) noexcept;

    [[nodiscard]]
    static bool IsConnected() noexcept;

private:

    static bool initialized_;
};

} // namespace NAS::Services