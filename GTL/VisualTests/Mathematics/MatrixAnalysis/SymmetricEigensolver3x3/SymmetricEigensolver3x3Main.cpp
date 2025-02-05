// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#include "SymmetricEigensolver3x3Console.h"
#include <iostream>

int32_t main()
{
    try
    {
        Console::Parameters parameters(L"SymmetricEigensolver3x3Console");
        auto console = TheConsoleSystem.Create<SymmetricEigensolver3x3Console>(parameters);
        TheConsoleSystem.Execute(console);
        TheConsoleSystem.Destroy(console);
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
