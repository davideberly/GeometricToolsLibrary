// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#pragma once

#include <GTL/Utility/Timer.h>
#include <iostream>

namespace gtl
{
    class UTLibrary
    {
    public:
        static inline void Precall()
        {
            timer.Reset();
        }

        static inline void Postcall()
        {
            milliseconds = timer.GetMilliseconds();
            std::cout << "time = " << milliseconds << std::endl << std::endl;
        }

        static Timer timer;
        static std::int64_t milliseconds;
    };
}
