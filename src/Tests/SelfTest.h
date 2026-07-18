#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Tests
 * File    : SelfTest.h
 *
 * Description:
 * Production-quality firmware self-test framework that validates every layer.
 *
 ******************************************************************************/

#include "../Core/Result.h"

namespace NAS::Tests
{

class SelfTest
{
public:

    [[nodiscard]]
    static NAS::Core::Result Run() noexcept;

private:

    SelfTest() = delete;
};

} // namespace NAS::Tests
