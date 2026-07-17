#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : Commands.h
 *
 * Description:
 * Protocol command identifiers.
 *
 ******************************************************************************/

#include <cstdint>

namespace NAS::Protocol::Commands
{

using CommandId = std::uint16_t;

//==========================================================================
// System Commands (0x0000)
//==========================================================================

constexpr CommandId Ping                = 0x0001;
constexpr CommandId GetVersion          = 0x0002;
constexpr CommandId GetBuildInfo        = 0x0003;
constexpr CommandId GetSystemStatus     = 0x0004;
constexpr CommandId Restart             = 0x0005;

//==========================================================================
// Relay Commands (0x1000)
//==========================================================================

constexpr CommandId RelayGet            = 0x1001;
constexpr CommandId RelaySet            = 0x1002;
constexpr CommandId RelayToggle         = 0x1003;

//==========================================================================
// Fan Commands (0x1100)
//==========================================================================

constexpr CommandId FanGet              = 0x1101;
constexpr CommandId FanSetSpeed         = 0x1102;
constexpr CommandId FanSetMode          = 0x1103;

//==========================================================================
// Temperature Commands (0x1200)
//==========================================================================

constexpr CommandId TemperatureGet      = 0x1201;
constexpr CommandId TemperatureGetAll   = 0x1202;

//==========================================================================
// LED Commands (0x1300)
//==========================================================================

constexpr CommandId LedGet              = 0x1301;
constexpr CommandId LedSetColor         = 0x1302;
constexpr CommandId LedSetMode          = 0x1303;
constexpr CommandId LedOff              = 0x1304;

//==========================================================================
// Drive Commands (0x1400)
//==========================================================================

constexpr CommandId DriveGet            = 0x1401;
constexpr CommandId DriveGetAll         = 0x1402;
constexpr CommandId DrivePowerOn        = 0x1403;
constexpr CommandId DrivePowerOff       = 0x1404;

//==========================================================================
// Configuration Commands (0x1500)
//==========================================================================

constexpr CommandId ConfigurationLoad   = 0x1501;
constexpr CommandId ConfigurationSave   = 0x1502;
constexpr CommandId ConfigurationReset  = 0x1503;

//==========================================================================
// Statistics Commands (0x1600)
//==========================================================================

constexpr CommandId StatisticsGet       = 0x1601;
constexpr CommandId StatisticsReset     = 0x1602;

//==========================================================================
// Event Commands (0x1700)
//==========================================================================

constexpr CommandId EventRead           = 0x1701;
constexpr CommandId EventClear          = 0x1702;

//==========================================================================

[[nodiscard]]
bool IsValid(CommandId command) noexcept;

} // namespace NAS::Protocol::Commands