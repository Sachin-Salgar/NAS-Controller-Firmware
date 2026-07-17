/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Core
 * File    : Logger.cpp
 *
 ******************************************************************************/

#include "Logger.h"

namespace NAS::Core
{

Logger::Logger() noexcept
    : level_(LogLevel::Info),
      initialized_(false)
{
}

Result Logger::Initialize() noexcept
{
    if (initialized_)
    {
        return Result(ResultCode::AlreadyInitialized);
    }

    initialized_ = true;

    return Result::Ok();
}

Result Logger::SetLevel(LogLevel level) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    level_ = level;

    return Result::Ok();
}

LogLevel Logger::Level() const noexcept
{
    return level_;
}

Result Logger::Debug(const char* message) noexcept
{
    return Write(LogLevel::Debug, message);
}

Result Logger::Info(const char* message) noexcept
{
    return Write(LogLevel::Info, message);
}

Result Logger::Warning(const char* message) noexcept
{
    return Write(LogLevel::Warning, message);
}

Result Logger::Error(const char* message) noexcept
{
    return Write(LogLevel::Error, message);
}

Result Logger::Critical(const char* message) noexcept
{
    return Write(LogLevel::Critical, message);
}

Result Logger::Write(LogLevel level, const char* message) noexcept
{
    if (!initialized_)
    {
        return Result(ResultCode::NotInitialized);
    }

    if (message == nullptr)
    {
        return Result(ResultCode::InvalidArgument);
    }

    if (static_cast<std::uint8_t>(level) < static_cast<std::uint8_t>(level_))
    {
        return Result::Ok();
    }

    // Logging backend will be implemented later
    // (USB CDC / Serial / Host)

    (void)message;

    return Result::Ok();
}

} // namespace NAS::Core