#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : StatisticsService.h
 *
 ******************************************************************************/

#include <cstdint>

#include "../Core/Result.h"
#include "../Objects/Statistics.h"

namespace NAS::Services
{

class StatisticsService
{
public:

    [[nodiscard]]
    static NAS::Core::Result Initialize() noexcept;

    static void IncrementBootCount() noexcept;

    static void IncrementUsbPacketsReceived() noexcept;

    static void IncrementUsbPacketsTransmitted() noexcept;

    static void IncrementProtocolErrors() noexcept;

    static void IncrementRelayCycles(std::uint8_t relayId) noexcept;

    static void AddFanRuntime(std::uint32_t seconds) noexcept;

    static void AddSystemUptime(std::uint32_t seconds) noexcept;

    [[nodiscard]]
    static NAS::Objects::Statistics&
    GetStatistics() noexcept;

private:

    static NAS::Objects::Statistics statistics_;

    static bool initialized_;
};

} // namespace NAS::Services