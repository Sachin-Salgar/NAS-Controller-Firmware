#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Core
 * File    : ResultCodeHelper.h
 *
 * Description:
 * Helper functions for ResultCode display and description.
 *
 ******************************************************************************/

#include "Result.h"

namespace NAS::Core
{

/**
 * @brief Returns the string representation of a ResultCode.
 */
[[nodiscard]]
const char* ResultCodeToString(ResultCode code) noexcept;

/**
 * @brief Returns a human-readable description of a ResultCode.
 */
[[nodiscard]]
const char* ResultCodeDescription(ResultCode code) noexcept;

} // namespace NAS::Core
