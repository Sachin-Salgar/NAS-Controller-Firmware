/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : RelayService.cpp
 *
 ******************************************************************************/

#include "RelayService.h"

using namespace NAS::Core;

namespace NAS::Services
{

NAS::Objects::Relay RelayService::relays_[RelayCount];

bool RelayService::initialized_ = false;

Result RelayService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    auto result = relays_[0].Initialize(1U, 26U);

    if (!result.IsSuccess())
    {
        return result;
    }

    result = relays_[1].Initialize(2U, 27U);

    if (!result.IsSuccess())
    {
        return result;
    }

    initialized_ = true;

    return Result::Ok();
}

Result RelayService::TurnOn(std::uint8_t relayId) noexcept
{
    if ((relayId == 0U) || (relayId > RelayCount))
    {
        return Result(ResultCode::InvalidArgument);
    }

    return relays_[relayId - 1U].TurnOn();
}

Result RelayService::TurnOff(std::uint8_t relayId) noexcept
{
    if ((relayId == 0U) || (relayId > RelayCount))
    {
        return Result(ResultCode::InvalidArgument);
    }

    return relays_[relayId - 1U].TurnOff();
}

Result RelayService::Toggle(std::uint8_t relayId) noexcept
{
    if ((relayId == 0U) || (relayId > RelayCount))
    {
        return Result(ResultCode::InvalidArgument);
    }

    return relays_[relayId - 1U].Toggle();
}

NAS::Objects::Relay& RelayService::GetRelay(std::uint8_t relayId) noexcept
{
    return relays_[relayId - 1U];
}

}