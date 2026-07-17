/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : Commands.cpp
 *
 ******************************************************************************/

#include "Commands.h"

namespace NAS::Protocol::Commands
{

bool IsValid(CommandId command) noexcept
{
    switch (command)
    {
        case Ping:
        case GetVersion:
        case GetBuildInfo:
        case GetSystemStatus:
        case Restart:

        case RelayGet:
        case RelaySet:
        case RelayToggle:

        case FanGet:
        case FanSetSpeed:
        case FanSetMode:

        case TemperatureGet:
        case TemperatureGetAll:

        case LedGet:
        case LedSetColor:
        case LedSetMode:
        case LedOff:

        case DriveGet:
        case DriveGetAll:
        case DrivePowerOn:
        case DrivePowerOff:

        case ConfigurationLoad:
        case ConfigurationSave:
        case ConfigurationReset:

        case StatisticsGet:
        case StatisticsReset:

        case EventRead:
        case EventClear:

            return true;

        default:
            return false;
    }
}

} // namespace NAS::Protocol::Commands