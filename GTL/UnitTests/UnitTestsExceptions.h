// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#pragma once

//#define GTL_UNIT_TESTS_LOG_ASSERTS

#include <GTL/Utility/Exceptions.h>
#include <iomanip>
#include <iostream>
#include <sstream>

extern void GTLUnitTestInformation(std::string const&);
#define UTInformation(message) GTLUnitTestInformation(message)

#if defined(GTL_UNIT_TESTS_LOG_ASSERTS)
#define UTAssert(condition, message)\
if (condition == false)\
{\
    GTLUnitTestInformation(message);\
}
#else
#define UTAssert(condition, message) GTL_ASSERT(condition, std::runtime_error, message)
#endif
