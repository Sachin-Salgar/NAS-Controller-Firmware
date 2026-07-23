/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Protocol
 * File    : CommandDispatcher.cpp
 *
 ******************************************************************************/

#include "CommandDispatcher.h"

#include <cstring>

#include "Commands.h"
#include "../Core/BuildInfo.h"
#include "../Core/Version.h"
#include "../Objects/Drive.h"
#include "../Objects/Fan.h"
#include "../Objects/SystemStatus.h"
#include "../Platform/Restart.h"
#include "../Services/ConfigurationService.h"
#include "../Services/DriveService.h"
#include "../Services/EventService.h"
#include "../Services/FanService.h"
#include "../Services/LedService.h"
#include "../Services/RelayService.h"
#include "../Services/StatisticsService.h"
#include "../Services/SystemService.h"
#include "../Services/TemperatureService.h"

using namespace NAS::Core;

namespace
{

template <typename T>
Result WriteValue(std::uint8_t* buffer,
                  std::uint16_t bufferSize,
                  std::uint16_t& offset,
                  const T& value) noexcept
{
    if (buffer == nullptr)
    {
        return Result(ResultCode::NullPointer);
    }

    if ((static_cast<std::uint32_t>(offset) + sizeof(T)) > bufferSize)
    {
        return Result(ResultCode::BufferTooSmall);
    }

    std::memcpy(buffer + offset, &value, sizeof(T));
    offset = static_cast<std::uint16_t>(offset + sizeof(T));

    return Result::Ok();
}

template <typename T>
Result ReadValue(const std::uint8_t* buffer,
                 std::uint16_t length,
                 std::uint16_t& offset,
                 T& value) noexcept
{
    if (buffer == nullptr)
    {
        return Result(ResultCode::NullPointer);
    }

    if ((static_cast<std::uint32_t>(offset) + sizeof(T)) > length)
    {
        return Result(ResultCode::InvalidLength);
    }

    std::memcpy(&value, buffer + offset, sizeof(T));
    offset = static_cast<std::uint16_t>(offset + sizeof(T));

    return Result::Ok();
}

Result WriteBytes(std::uint8_t* buffer,
                  std::uint16_t bufferSize,
                  std::uint16_t& offset,
                  const char* value) noexcept
{
    const std::uint16_t length =
        static_cast<std::uint16_t>(std::strlen(value) + 1U);

    auto result = WriteValue(buffer, bufferSize, offset, length);

    if (!result.IsSuccess())
    {
        return result;
    }

    if ((static_cast<std::uint32_t>(offset) + length) > bufferSize)
    {
        return Result(ResultCode::BufferTooSmall);
    }

    std::memcpy(buffer + offset, value, length);
    offset = static_cast<std::uint16_t>(offset + length);

    return Result::Ok();
}

bool HasLength(std::uint16_t actual, std::uint16_t expected) noexcept
{
    return actual == expected;
}

} // namespace

namespace NAS::Protocol
{

Result CommandDispatcher::Dispatch(
    std::uint16_t command,
    const std::uint8_t* requestPayload,
    std::uint16_t requestLength,
    std::uint8_t* responsePayload,
    std::uint16_t responseBufferSize,
    std::uint16_t& responseLength) noexcept
{
    responseLength = 0U;

    if ((responsePayload == nullptr) && (responseBufferSize > 0U))
    {
        return Result(ResultCode::NullPointer);
    }

    switch (command & 0xFF00U)
    {
        case 0x0000U:
            return DispatchSystemCommand(command, requestPayload, requestLength, responsePayload, responseBufferSize, responseLength);
        case 0x1000U:
            return DispatchRelayCommand(command, requestPayload, requestLength, responsePayload, responseBufferSize, responseLength);
        case 0x1100U:
            return DispatchFanCommand(command, requestPayload, requestLength, responsePayload, responseBufferSize, responseLength);
        case 0x1200U:
            return DispatchTemperatureCommand(command, requestPayload, requestLength, responsePayload, responseBufferSize, responseLength);
        case 0x1300U:
            return DispatchLedCommand(command, requestPayload, requestLength, responsePayload, responseBufferSize, responseLength);
        case 0x1400U:
            return DispatchDriveCommand(command, requestPayload, requestLength, responsePayload, responseBufferSize, responseLength);
        case 0x1500U:
        {
            switch (command)
            {
                case Commands::ConfigurationLoad:
                    if (!HasLength(requestLength, 0U)) return Result(ResultCode::InvalidLength);
                    return NAS::Services::ConfigurationService::Load();
                case Commands::ConfigurationSave:
                    if (!HasLength(requestLength, 0U)) return Result(ResultCode::InvalidLength);
                    return NAS::Services::ConfigurationService::Save();
                case Commands::ConfigurationReset:
                    if (!HasLength(requestLength, 0U)) return Result(ResultCode::InvalidLength);
                    return NAS::Services::ConfigurationService::ResetToDefaults();
                default:
                    return Result(ResultCode::NotSupported);
            }
        }
        case 0x1600U:
        {
            switch (command)
            {
                case Commands::StatisticsGet:
                {
                    if (!HasLength(requestLength, 0U)) return Result(ResultCode::InvalidLength);
                    auto& s = NAS::Services::StatisticsService::GetStatistics();
                    Result r = WriteValue(responsePayload, responseBufferSize, responseLength, s.GetBootCount()); if (!r) return r;
                    r = WriteValue(responsePayload, responseBufferSize, responseLength, s.GetUsbPacketsReceived()); if (!r) return r;
                    r = WriteValue(responsePayload, responseBufferSize, responseLength, s.GetUsbPacketsTransmitted()); if (!r) return r;
                    r = WriteValue(responsePayload, responseBufferSize, responseLength, s.GetProtocolErrors()); if (!r) return r;
                    for (std::uint8_t i = 1U; i <= 2U; ++i) { r = WriteValue(responsePayload, responseBufferSize, responseLength, s.GetRelayCycles(i)); if (!r) return r; }
                    r = WriteValue(responsePayload, responseBufferSize, responseLength, s.GetFanRuntime()); if (!r) return r;
                    return WriteValue(responsePayload, responseBufferSize, responseLength, s.GetSystemUptime());
                }
                case Commands::StatisticsReset:
                    if (!HasLength(requestLength, 0U)) return Result(ResultCode::InvalidLength);
                    return NAS::Services::StatisticsService::GetStatistics().Reset();
                default:
                    return Result(ResultCode::NotSupported);
            }
        }
        case 0x1700U:
        {
            switch (command)
            {
                case Commands::EventRead:
                {
                    if (!HasLength(requestLength, 0U)) return Result(ResultCode::InvalidLength);
                    NAS::Services::Event event;
                    auto r = NAS::Services::EventService::GetNextEvent(event);
                    if (!r) return r;
                    r = WriteValue(responsePayload, responseBufferSize, responseLength, static_cast<std::uint16_t>(event.type)); if (!r) return r;
                    r = WriteValue(responsePayload, responseBufferSize, responseLength, event.parameter1); if (!r) return r;
                    return WriteValue(responsePayload, responseBufferSize, responseLength, event.parameter2);
                }
                case Commands::EventClear:
                    if (!HasLength(requestLength, 0U)) return Result(ResultCode::InvalidLength);
                    return NAS::Services::EventService::Clear();
                default:
                    return Result(ResultCode::NotSupported);
            }
        }
        default:
            return Result(ResultCode::NotSupported);
    }
}

Result CommandDispatcher::DispatchSystemCommand(std::uint16_t command, const std::uint8_t*, std::uint16_t requestLength, std::uint8_t* responsePayload, std::uint16_t responseBufferSize, std::uint16_t& responseLength) noexcept
{
    if (!HasLength(requestLength, 0U)) return Result(ResultCode::InvalidLength);
    switch (command)
    {
        case Commands::Ping: return Result::Ok();
        case Commands::GetVersion:
        {
            auto r = WriteValue(responsePayload, responseBufferSize, responseLength, Version::Major()); if (!r) return r;
            r = WriteValue(responsePayload, responseBufferSize, responseLength, Version::Minor()); if (!r) return r;
            return WriteValue(responsePayload, responseBufferSize, responseLength, Version::Patch());
        }
        case Commands::GetBuildInfo:
        {
            auto r = WriteBytes(responsePayload, responseBufferSize, responseLength, Version::FirmwareName()); if (!r) return r;
            r = WriteBytes(responsePayload, responseBufferSize, responseLength, Version::BuildDate()); if (!r) return r;
            r = WriteBytes(responsePayload, responseBufferSize, responseLength, Version::BuildTime()); if (!r) return r;
            r = WriteBytes(responsePayload, responseBufferSize, responseLength, Version::BuildConfiguration()); if (!r) return r;
            r = WriteBytes(responsePayload, responseBufferSize, responseLength, Version::Compiler()); if (!r) return r;
            r = WriteBytes(responsePayload, responseBufferSize, responseLength, Version::TargetPlatform()); if (!r) return r;
            r = WriteBytes(responsePayload, responseBufferSize, responseLength, Version::TargetBoard()); if (!r) return r;
            return WriteBytes(responsePayload, responseBufferSize, responseLength, Version::GitCommit());
        }
        case Commands::GetSystemStatus:
        {
            auto& s = NAS::Services::SystemService::GetStatus();
            auto r = WriteValue(responsePayload, responseBufferSize, responseLength, static_cast<std::uint8_t>(s.GetState())); if (!r) return r;
            r = WriteValue(responsePayload, responseBufferSize, responseLength, s.IsUsbConnected()); if (!r) return r;
            r = WriteValue(responsePayload, responseBufferSize, responseLength, s.GetFreeHeap()); if (!r) return r;
            r = WriteValue(responsePayload, responseBufferSize, responseLength, s.GetCpuTemperature()); if (!r) return r;
            return WriteValue(responsePayload, responseBufferSize, responseLength, s.GetUptime());
        }
        case Commands::Restart: return NAS::Platform::Restart::Soft();
        default: return Result(ResultCode::NotSupported);
    }
}

Result CommandDispatcher::DispatchRelayCommand(std::uint16_t command, const std::uint8_t* requestPayload, std::uint16_t requestLength, std::uint8_t* responsePayload, std::uint16_t responseBufferSize, std::uint16_t& responseLength) noexcept
{
    std::uint16_t offset = 0U; std::uint8_t id = 0U;
    if (!ReadValue(requestPayload, requestLength, offset, id)) return Result(ResultCode::InvalidLength);
    if ((id == 0U) || (id > NAS::Services::RelayService::RelayCount)) return Result(ResultCode::InvalidArgument);
    if (offset != requestLength) return Result(ResultCode::InvalidLength);
    switch (command)
    {
        case Commands::RelayGet:
        {
            auto& rly = NAS::Services::RelayService::GetRelay(id);
            auto r = WriteValue(responsePayload, responseBufferSize, responseLength, rly.GetId()); if (!r) return r;
            r = WriteValue(responsePayload, responseBufferSize, responseLength, rly.IsOn()); if (!r) return r;
            return WriteValue(responsePayload, responseBufferSize, responseLength, rly.IsEnabled());
        }
        case Commands::RelaySet:
        {
            bool on = false; offset = 1U; if (!ReadValue(requestPayload, requestLength, offset, on) || offset != requestLength) return Result(ResultCode::InvalidLength);
            auto r = on ? NAS::Services::RelayService::TurnOn(id) : NAS::Services::RelayService::TurnOff(id);
            if (r) NAS::Services::StatisticsService::IncrementRelayCycles(id);
            return r;
        }
        case Commands::RelayToggle:
        {
            auto r = NAS::Services::RelayService::Toggle(id);
            if (r) NAS::Services::StatisticsService::IncrementRelayCycles(id);
            return r;
        }
        default: return Result(ResultCode::NotSupported);
    }
}

Result CommandDispatcher::DispatchFanCommand(std::uint16_t command, const std::uint8_t* requestPayload, std::uint16_t requestLength, std::uint8_t* responsePayload, std::uint16_t responseBufferSize, std::uint16_t& responseLength) noexcept
{
    std::uint16_t offset = 0U; std::uint8_t id = 0U;
    if (!ReadValue(requestPayload, requestLength, offset, id)) return Result(ResultCode::InvalidLength);
    if ((id == 0U) || (id > NAS::Services::FanService::FanCount)) return Result(ResultCode::InvalidArgument);
    switch (command)
    {
        case Commands::FanGet:
        {
            if (offset != requestLength) return Result(ResultCode::InvalidLength);
            auto& fan = NAS::Services::FanService::GetFan(id);
            auto r = WriteValue(responsePayload, responseBufferSize, responseLength, fan.GetChannel()); if (!r) return r;
            r = WriteValue(responsePayload, responseBufferSize, responseLength, fan.GetSpeed()); if (!r) return r;
            return WriteValue(responsePayload, responseBufferSize, responseLength, static_cast<std::uint8_t>(fan.GetMode()));
        }
        case Commands::FanSetSpeed:
        { std::uint8_t speed = 0U; if (!ReadValue(requestPayload, requestLength, offset, speed) || offset != requestLength) return Result(ResultCode::InvalidLength); return NAS::Services::FanService::SetSpeed(id, speed); }
        case Commands::FanSetMode:
        { std::uint8_t mode = 0U; if (!ReadValue(requestPayload, requestLength, offset, mode) || offset != requestLength || mode > 1U) return Result(ResultCode::InvalidArgument); return NAS::Services::FanService::SetMode(id, static_cast<NAS::Objects::FanMode>(mode)); }
        default: return Result(ResultCode::NotSupported);
    }
}

Result CommandDispatcher::DispatchTemperatureCommand(std::uint16_t command, const std::uint8_t* requestPayload, std::uint16_t requestLength, std::uint8_t* responsePayload, std::uint16_t responseBufferSize, std::uint16_t& responseLength) noexcept
{
    if (command == Commands::TemperatureGetAll)
    {
        if (!HasLength(requestLength, 0U)) return Result(ResultCode::InvalidLength);
        for (std::uint8_t i = 0U; i < NAS::Services::TemperatureService::SensorCount; ++i)
        { auto r = WriteValue(responsePayload, responseBufferSize, responseLength, NAS::Services::TemperatureService::GetTemperature(i)); if (!r) return r; }
        return Result::Ok();
    }
    if (command == Commands::TemperatureGet)
    {
        std::uint16_t offset = 0U; std::uint8_t id = 0U; if (!ReadValue(requestPayload, requestLength, offset, id) || offset != requestLength) return Result(ResultCode::InvalidLength);
        if (id >= NAS::Services::TemperatureService::SensorCount) return Result(ResultCode::InvalidArgument);
        return WriteValue(responsePayload, responseBufferSize, responseLength, NAS::Services::TemperatureService::GetTemperature(id));
    }
    return Result(ResultCode::NotSupported);
}

Result CommandDispatcher::DispatchLedCommand(std::uint16_t command, const std::uint8_t* requestPayload, std::uint16_t requestLength, std::uint8_t*, std::uint16_t, std::uint16_t&) noexcept
{
    std::uint16_t offset = 0U; std::uint8_t id = 0U;
    if (!ReadValue(requestPayload, requestLength, offset, id)) return Result(ResultCode::InvalidLength);
    if (id >= NAS::Services::LedService::DriveLedCount) return Result(ResultCode::InvalidArgument);
    switch (command)
    {
        case Commands::LedGet:
            return Result(ResultCode::NotSupported);
        case Commands::LedSetColor:
            return Result(ResultCode::NotSupported);
        case Commands::LedSetMode:
        { std::uint8_t state = 0U; if (!ReadValue(requestPayload, requestLength, offset, state) || offset != requestLength || state > 6U) return Result(ResultCode::InvalidArgument); return NAS::Services::LedService::SetDriveState(id, static_cast<NAS::Objects::DriveLedState>(state)); }
        case Commands::LedOff:
            if (offset != requestLength) return Result(ResultCode::InvalidLength);
            return NAS::Services::LedService::SetDriveState(id, NAS::Objects::DriveLedState::Off);
        default: return Result(ResultCode::NotSupported);
    }
}

Result CommandDispatcher::DispatchDriveCommand(std::uint16_t command, const std::uint8_t* requestPayload, std::uint16_t requestLength, std::uint8_t* responsePayload, std::uint16_t responseBufferSize, std::uint16_t& responseLength) noexcept
{
    if (command == Commands::DriveGetAll)
    {
        if (!HasLength(requestLength, 0U)) return Result(ResultCode::InvalidLength);
        for (std::uint8_t i = 1U; i <= NAS::Services::DriveService::DriveCount; ++i)
        { auto& d = NAS::Services::DriveService::GetDrive(i); auto r = WriteValue(responsePayload, responseBufferSize, responseLength, d.GetId()); if (!r) return r; r = WriteValue(responsePayload, responseBufferSize, responseLength, d.IsPresent()); if (!r) return r; r = WriteValue(responsePayload, responseBufferSize, responseLength, static_cast<std::uint8_t>(d.GetState())); if (!r) return r; r = WriteValue(responsePayload, responseBufferSize, responseLength, static_cast<std::uint8_t>(d.GetHealth())); if (!r) return r; r = WriteValue(responsePayload, responseBufferSize, responseLength, d.GetTemperature()); if (!r) return r; r = WriteValue(responsePayload, responseBufferSize, responseLength, d.GetCapacity()); if (!r) return r; r = WriteBytes(responsePayload, responseBufferSize, responseLength, d.GetSerialNumber()); if (!r) return r; }
        return Result::Ok();
    }
    std::uint16_t offset = 0U; std::uint8_t id = 0U; if (!ReadValue(requestPayload, requestLength, offset, id)) return Result(ResultCode::InvalidLength);
    if ((id == 0U) || (id > NAS::Services::DriveService::DriveCount)) return Result(ResultCode::InvalidArgument);
    switch (command)
    {
        case Commands::DriveGet:
        { if (offset != requestLength) return Result(ResultCode::InvalidLength); auto& d = NAS::Services::DriveService::GetDrive(id); auto r = WriteValue(responsePayload, responseBufferSize, responseLength, d.GetId()); if (!r) return r; r = WriteValue(responsePayload, responseBufferSize, responseLength, d.IsPresent()); if (!r) return r; r = WriteValue(responsePayload, responseBufferSize, responseLength, static_cast<std::uint8_t>(d.GetState())); if (!r) return r; r = WriteValue(responsePayload, responseBufferSize, responseLength, static_cast<std::uint8_t>(d.GetHealth())); if (!r) return r; r = WriteValue(responsePayload, responseBufferSize, responseLength, d.GetTemperature()); if (!r) return r; r = WriteValue(responsePayload, responseBufferSize, responseLength, d.GetCapacity()); if (!r) return r; return WriteBytes(responsePayload, responseBufferSize, responseLength, d.GetSerialNumber()); }
        case Commands::DrivePowerOn:
            if (offset != requestLength) return Result(ResultCode::InvalidLength);
            return NAS::Services::DriveService::SetPresent(id, true);
        case Commands::DrivePowerOff:
            if (offset != requestLength) return Result(ResultCode::InvalidLength);
            return NAS::Services::DriveService::SetPresent(id, false);
        default: return Result(ResultCode::NotSupported);
    }
}

} // namespace NAS::Protocol
