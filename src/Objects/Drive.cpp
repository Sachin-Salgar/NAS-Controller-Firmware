/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Objects
 * File    : Drive.cpp
 *
 ******************************************************************************/

#include "Drive.h"

#include <cstring>

using namespace NAS::Core;

namespace NAS::Objects
{

Result Drive::Initialize(std::uint8_t id) noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    id_ = id;

    present_ = false;
    state_ = DriveState::Offline;
    health_ = DriveHealth::Unknown;
    temperature_ = 0.0F;
    capacity_ = 0U;

    serial_[0] = '\0';

    initialized_ = true;

    return Result::Ok();
}

Result Drive::SetPresent(bool present) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    present_ = present;

    if (!present_)
    {
        state_ = DriveState::Offline;
    }

    return Result::Ok();
}

Result Drive::SetState(DriveState state) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    state_ = state;

    return Result::Ok();
}

Result Drive::SetTemperature(float temperature) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    temperature_ = temperature;

    return Result::Ok();
}

Result Drive::SetHealth(DriveHealth health) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    health_ = health;

    return Result::Ok();
}

Result Drive::SetCapacity(std::uint64_t bytes) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    capacity_ = bytes;

    return Result::Ok();
}

Result Drive::SetSerialNumber(const char* serial) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (serial == nullptr)
    {
        return Result(ResultCode::InvalidArgument);
    }

    std::strncpy(serial_, serial, SerialLength - 1U);
    serial_[SerialLength - 1U] = '\0';

    return Result::Ok();
}

std::uint8_t Drive::GetId() const noexcept
{
    return id_;
}

bool Drive::IsPresent() const noexcept
{
    return present_;
}

DriveState Drive::GetState() const noexcept
{
    return state_;
}

DriveHealth Drive::GetHealth() const noexcept
{
    return health_;
}

float Drive::GetTemperature() const noexcept
{
    return temperature_;
}

std::uint64_t Drive::GetCapacity() const noexcept
{
    return capacity_;
}

const char* Drive::GetSerialNumber() const noexcept
{
    return serial_;
}

} // namespace NAS::Objects