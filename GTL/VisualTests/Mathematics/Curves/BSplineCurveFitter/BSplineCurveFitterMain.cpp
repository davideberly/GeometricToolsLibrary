// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.28

#include "BSplineCurveFitterWindow3.h"
#include <iostream>

int32_t main()
{
    try
    {
        Window::Parameters parameters(L"BSplineCurveFitterWindow3", 0, 0, 512, 512);
        auto window = TheWindowSystem.Create<BSplineCurveFitterWindow3>(parameters);
        TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
        TheWindowSystem.Destroy(window);
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
