/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Core
 * File    : ResultCodeHelper.cpp
 *
 * Description:
 * Helper functions for ResultCode display and description.
 *
 ******************************************************************************/

#include "ResultCodeHelper.h"

namespace NAS::Core
{

const char* ResultCodeToString(ResultCode code) noexcept
{
    switch (code)
    {
        case ResultCode::Success:
            return "Success";
        case ResultCode::Failed:
            return "Failed";
        case ResultCode::InvalidArgument:
            return "InvalidArgument";
        case ResultCode::InvalidState:
            return "InvalidState";
        case ResultCode::NotInitialized:
            return "NotInitialized";
        case ResultCode::AlreadyInitialized:
            return "AlreadyInitialized";
        case ResultCode::Timeout:
            return "Timeout";
        case ResultCode::Busy:
            return "Busy";
        case ResultCode::Unsupported:
            return "Unsupported";
        case ResultCode::NotSupported:
            return "NotSupported";
        case ResultCode::AccessDenied:
            return "AccessDenied";
        case ResultCode::OutOfMemory:
            return "OutOfMemory";
        case ResultCode::NullPointer:
            return "NullPointer";
        case ResultCode::BufferTooSmall:
            return "BufferTooSmall";
        case ResultCode::BufferFull:
            return "BufferFull";
        case ResultCode::CommunicationError:
            return "CommunicationError";
        case ResultCode::ChecksumError:
            return "ChecksumError";
        case ResultCode::CrcError:
            return "CrcError";
        case ResultCode::InvalidHeader:
            return "InvalidHeader";
        case ResultCode::InvalidLength:
            return "InvalidLength";
        case ResultCode::NotFound:
            return "NotFound";
        case ResultCode::HardwareError:
            return "HardwareError";
        case ResultCode::DeviceNotFound:
            return "DeviceNotFound";
        case ResultCode::InvalidConfiguration:
            return "InvalidConfiguration";
        case ResultCode::InternalError:
            return "InternalError";
        default:
            return "UnknownError";
    }
}

const char* ResultCodeDescription(ResultCode code) noexcept
{
    switch (code)
    {
        case ResultCode::Success:
            return "Operation completed successfully.";
        case ResultCode::Failed:
            return "Operation failed.";
        case ResultCode::InvalidArgument:
            return "Invalid argument provided.";
        case ResultCode::InvalidState:
            return "Device is in invalid state.";
        case ResultCode::NotInitialized:
            return "Component not initialized.";
        case ResultCode::AlreadyInitialized:
            return "Component already initialized.";
        case ResultCode::Timeout:
            return "Operation timed out.";
        case ResultCode::Busy:
            return "Device is busy.";
        case ResultCode::Unsupported:
            return "Operation not supported.";
        case ResultCode::NotSupported:
            return "Feature not supported.";
        case ResultCode::AccessDenied:
            return "Access denied.";
        case ResultCode::OutOfMemory:
            return "Out of memory.";
        case ResultCode::NullPointer:
            return "Null pointer encountered.";
        case ResultCode::BufferTooSmall:
            return "Buffer size too small.";
        case ResultCode::BufferFull:
            return "Buffer is full.";
        case ResultCode::CommunicationError:
            return "Communication error.";
        case ResultCode::ChecksumError:
            return "Checksum error detected.";
        case ResultCode::CrcError:
            return "CRC error detected.";
        case ResultCode::InvalidHeader:
            return "Invalid header.";
        case ResultCode::InvalidLength:
            return "Invalid length.";
        case ResultCode::NotFound:
            return "Resource not found.";
        case ResultCode::HardwareError:
            return "Hardware error.";
        case ResultCode::DeviceNotFound:
            return "Device not found.";
        case ResultCode::InvalidConfiguration:
            return "Invalid configuration.";
        case ResultCode::InternalError:
            return "Internal error.";
        default:
            return "Unknown error.";
    }
}

} // namespace NAS::Core
