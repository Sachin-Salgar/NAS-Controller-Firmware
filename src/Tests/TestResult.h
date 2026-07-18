#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Tests
 * File    : TestResult.h
 *
 * Description:
 * Single definition for LayerResult struct used by all test layers.
 * Eliminates duplication and provides one maintenance point.
 *
 ******************************************************************************/

#include "../Core/Result.h"

namespace NAS::Tests
{

struct LayerResult
{
    NAS::Core::Result result;
    const char* failedComponent;
    NAS::Core::ResultCode failureCode;
    int passCount;
    int failCount;
    int skippedCount;
};

} // namespace NAS::Tests
