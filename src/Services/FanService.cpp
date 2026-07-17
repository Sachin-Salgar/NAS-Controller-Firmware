/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Services
 * File    : FanService.cpp
 *
 ******************************************************************************/

#include "FanService.h"

using namespace NAS::Core;

namespace NAS::Services
{

NAS::Objects::Fan FanService::fans_[FanCount];

bool FanService::initialized_ = false;

Result FanService::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    fans_[0].Initialize(
        0U,
        25U,
        25000U,
        8U);

    fans_[1].Initialize(
        1U,
        33U,
        25000U,
        8U);

    initialized_ = true;

    return Result::Ok();
}

Result FanService::SetSpeed(std::uint8_t fanId,
                            std::uint8_t speed) noexcept
{
    if ((fanId == 0U) || (fanId > FanCount))
    {
        return Result(ResultCode::InvalidArgument);
    }

    return fans_[fanId - 1U].SetSpeed(speed);
}

Result FanService::SetMode(std::uint8_t fanId,
                           NAS::Objects::FanMode mode) noexcept
{
    if ((fanId == 0U) || (fanId > FanCount))
    {
        return Result(ResultCode::InvalidArgument);
    }

    return fans_[fanId - 1U].SetMode(mode);
}

NAS::Objects::Fan& FanService::GetFan(std::uint8_t fanId) noexcept
{
    return fans_[fanId - 1U];
}

} // namespace NAS::Services