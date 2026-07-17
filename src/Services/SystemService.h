#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : SystemService.h
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"
#include "../Objects/SystemStatus.h"

namespace NAS::Services
{

class SystemService
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetState(
        NAS::Objects::SystemState state) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetUsbConnected(
        bool connected) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetFreeHeap(
        std::uint32_t bytes) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetCpuTemperature(
        float temperature) noexcept;

    [[nodiscard]]
    static NAS::Core::Result SetUptime(
        std::uint64_t milliseconds) noexcept;

    [[nodiscard]]
    static NAS::Objects::SystemStatus&
    GetStatus() noexcept;

private:

    static NAS::Objects::SystemStatus status_;

    static bool initialized_;
};

} // namespace NAS::Services