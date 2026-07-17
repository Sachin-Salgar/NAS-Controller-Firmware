#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Core
 * File    : Logger.h
 *
 * Description:
 * Lightweight firmware logger.
 *
 ******************************************************************************/

#include <cstdint>

#include "Result.h"

namespace NAS::Core
{

enum class LogLevel : std::uint8_t
{
    Debug = 0,
    Info,
    Warning,
    Error,
    Critical
};

class Logger
{
public:

    Logger() noexcept;

    [[nodiscard]]
    Result Initialize() noexcept;

    [[nodiscard]]
    Result SetLevel(LogLevel level) noexcept;

    [[nodiscard]]
    LogLevel Level() const noexcept;

    [[nodiscard]]
    Result Debug(const char* message) noexcept;

    [[nodiscard]]
    Result Info(const char* message) noexcept;

    [[nodiscard]]
    Result Warning(const char* message) noexcept;

    [[nodiscard]]
    Result Error(const char* message) noexcept;

    [[nodiscard]]
    Result Critical(const char* message) noexcept;

private:

    [[nodiscard]]
    Result Write(LogLevel level, const char* message) noexcept;

    LogLevel level_;

    bool initialized_;
};

} // namespace NAS::Core