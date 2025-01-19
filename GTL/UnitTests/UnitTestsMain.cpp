// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <Utility/UTUtility.h>
#include <GTL/Mathematics/Arithmetic/UIntegerAP32.h>
#include <Mathematics/UTMathematics.h>
#include <stdexcept>
using namespace gtl;

namespace gtl
{
    std::atomic<std::size_t> gsUIntegerAP32MaxBlocks;
    std::atomic<std::size_t> gsUIntegerFP32MaxBlocks;
}

std::int32_t main()
{
    try
    {
        UTUtility::Execute();
        UTMathematics::Execute();
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    std::cout << "Unit tests passed." << std::endl;
    return 0;
}
#endif
