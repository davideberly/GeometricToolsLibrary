// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.28

#include "IncrementalDelaunay2Window2.h"
#include <iostream>

int32_t main()
{
    try
    {
        Window::Parameters parameters(L"IncrementalDelaunay2Window2", 0, 0, 1024, 1024);
        auto window = TheWindowSystem.Create<IncrementalDelaunay2Window2>(parameters);
        TheWindowSystem.MessagePump(window, TheWindowSystem.NO_IDLE_LOOP);
        TheWindowSystem.Destroy(window);
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
