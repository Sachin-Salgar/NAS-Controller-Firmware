#pragma once

#include <cstdint>

namespace NAS::Core
{

/**
 * @brief Standard firmware result codes.
 */
enum class ResultCode : uint16_t
{
    Success = 0,

    // Generic
    Failed,
    InvalidArgument,
    InvalidState,
    NotInitialized,
    AlreadyInitialized,
    Timeout,
    Busy,
    Unsupported,
    NotSupported,
    AccessDenied,

    // Memory
    OutOfMemory,
    NullPointer,
    BufferTooSmall,

    // Communication
    CommunicationError,
    ChecksumError,

    // Hardware
    HardwareError,
    DeviceNotFound,

    // Configuration
    InvalidConfiguration,

    // System
    InternalError
};

/**
 * @brief Lightweight result object returned by firmware operations.
 *
 * Result contains only a ResultCode and provides helper methods for
 * determining whether an operation succeeded.
 */
class Result
{
public:

    /// Constructs a Result.
    constexpr explicit Result(ResultCode code = ResultCode::Success) noexcept
        : code_(code)
    {
    }

    /// Returns true if the operation succeeded.
    [[nodiscard]]
    constexpr bool IsSuccess() const noexcept
    {
        return code_ == ResultCode::Success;
    }

    /// Returns true if the operation failed.
    [[nodiscard]]
    constexpr bool IsFailure() const noexcept
    {
        return !IsSuccess();
    }

    /// Returns the stored result code.
    [[nodiscard]]
    constexpr ResultCode Code() const noexcept
    {
        return code_;
    }

    /// Allows Result to be used directly in boolean expressions.
    [[nodiscard]]
    constexpr explicit operator bool() const noexcept
    {
        return IsSuccess();
    }

    /// Convenience function for returning success.
    [[nodiscard]]
    static constexpr Result Ok() noexcept
    {
        return Result(ResultCode::Success);
    }

private:

    ResultCode code_;
};

} // namespace NAS::Core
