// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2022 David Eberly
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include "AreaLightsWindow3.h"
#include <iostream>

int32_t main()
{
    try
    {
        Window::Parameters parameters(L"AreaLightsWindow3", 0, 0, 1024, 1024);
        auto window = TheWindowSystem.Create<AreaLightsWindow3>(parameters);
        TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
        TheWindowSystem.Destroy(window);
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
