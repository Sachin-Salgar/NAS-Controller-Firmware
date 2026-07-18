#pragma once

#include "../Core/Result.h"

namespace NAS::Tests
{

class SelfTest
{
public:

    static NAS::Core::Result Run();

private:

    static NAS::Core::Result TestCore();
    static NAS::Core::Result TestPlatform();
    static NAS::Core::Result TestDrivers();
    static NAS::Core::Result TestObjects();
    static NAS::Core::Result TestServices();
    static NAS::Core::Result TestProtocol();
    static NAS::Core::Result TestSystem();
};

}