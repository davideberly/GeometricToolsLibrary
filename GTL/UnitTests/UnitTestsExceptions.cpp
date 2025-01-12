// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#include <fstream>
#include <iostream>

static std::ofstream gsResults("UTResults.txt");

void GTLUnitTestInformation(std::string const& message)
{
    std::cout << message << std::endl;
    gsResults << message << std::endl;
}
