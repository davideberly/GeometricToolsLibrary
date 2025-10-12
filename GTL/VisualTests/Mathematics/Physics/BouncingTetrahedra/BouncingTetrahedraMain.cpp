// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.12

#include "BouncingTetrahedraWindow3.h"
#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        Window::Parameters parameters(L"BouncingTetrahedraWindow3", 0, 0, 1024, 768);
        auto window = TheWindowSystem.Create<BouncingTetrahedraWindow3>(parameters);
        TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
        TheWindowSystem.Destroy(window);
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
