#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Statistics.h
 *
 * Description:
 * Runtime and lifetime firmware statistics.
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"

namespace NAS::Objects
{

class Statistics
{
public:

    Statistics() = default;

    [[nodiscard]]
    NAS::Core::Result Initialize() noexcept;

    void IncrementBootCount() noexcept;
    void IncrementUsbPacketsReceived() noexcept;
    void IncrementUsbPacketsTransmitted() noexcept;
    void IncrementProtocolErrors() noexcept;
    void IncrementRelayCycles(std::uint8_t relayId) noexcept;

    void AddFanRuntime(std::uint32_t seconds) noexcept;
    void AddSystemUptime(std::uint32_t seconds) noexcept;

    [[nodiscard]]
    std::uint32_t GetBootCount() const noexcept;

    [[nodiscard]]
    std::uint32_t GetUsbPacketsReceived() const noexcept;

    [[nodiscard]]
    std::uint32_t GetUsbPacketsTransmitted() const noexcept;

    [[nodiscard]]
    std::uint32_t GetProtocolErrors() const noexcept;

    [[nodiscard]]
    std::uint32_t GetRelayCycles(std::uint8_t relayId) const noexcept;

    [[nodiscard]]
    std::uint32_t GetFanRuntime() const noexcept;

    [[nodiscard]]
    std::uint32_t GetSystemUptime() const noexcept;

    [[nodiscard]]
    NAS::Core::Result Reset() noexcept;

private:

    static constexpr std::uint8_t RelayCount = 2U;

    std::uint32_t bootCount_{0U};

    std::uint32_t usbPacketsReceived_{0U};

    std::uint32_t usbPacketsTransmitted_{0U};

    std::uint32_t protocolErrors_{0U};

    std::uint32_t relayCycles_[RelayCount]{};

    std::uint32_t fanRuntimeSeconds_{0U};

    std::uint32_t systemUptimeSeconds_{0U};

    bool initialized_{false};
};

} // namespace NAS::Objects