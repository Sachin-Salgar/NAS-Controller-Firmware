#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : SystemStatus.h
 *
 * Description:
 * System status object.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Objects
{

enum class SystemState : std::uint8_t
{
    Booting = 0,
    Initializing,
    Ready,
    Warning,
    Error,
    Shutdown
};

class SystemStatus
{
public:

    SystemStatus() = default;

    [[nodiscard]]
    NAS::Core::Result Initialize() noexcept;

    [[nodiscard]]
    NAS::Core::Result SetState(SystemState state) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetUsbConnected(bool connected) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetFreeHeap(std::uint32_t bytes) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetCpuTemperature(float temperature) noexcept;

    [[nodiscard]]
    NAS::Core::Result SetUptime(std::uint64_t milliseconds) noexcept;

    [[nodiscard]]
    SystemState GetState() const noexcept;

    [[nodiscard]]
    bool IsUsbConnected() const noexcept;

    [[nodiscard]]
    std::uint32_t GetFreeHeap() const noexcept;

    [[nodiscard]]
    float GetCpuTemperature() const noexcept;

    [[nodiscard]]
    std::uint64_t GetUptime() const noexcept;

private:

    SystemState state_{SystemState::Booting};

    bool usbConnected_{false};

    std::uint32_t freeHeap_{0U};

    float cpuTemperature_{0.0F};

    std::uint64_t uptime_{0U};

    bool initialized_{false};
};

} // namespace NAS::Objects