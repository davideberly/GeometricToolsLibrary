// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <Utility/UTUtility.h>
#include <stdexcept>
using namespace gtl;

std::int32_t main()
{
    try
    {
        UTUtility::Execute();
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
