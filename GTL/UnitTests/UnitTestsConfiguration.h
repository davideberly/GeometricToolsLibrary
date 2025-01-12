// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#pragma once

// Top-level control over which tests are executed. The commented out tests
// with "//*" are failing and need investigating. Those commented out with
// "//?" need unit testing.

#define UT_UTILITY

// Utility
#if defined(UT_UTILITY)
#define UT_UTILITY_ATOMICMINMAX
#define UT_UTILITY_CONTAINERADAPTER
#define UT_UTILITY_HASHCOMBINE
#define UT_UTILITY_LATTICE
#define UT_UTILITY_MINHEAP
#define UT_UTILITY_MINIMUMSPANNINGTREE
#define UT_UTILITY_MULTIARRAY
#define UT_UTILITY_MULTIARRAYADAPTER
#define UT_UTILITY_RANGEITERATION
#define UT_UTILITY_RAWPTRCOMPARE
#define UT_UTILITY_SHAREDPTRCOMPARE
#define UT_UTILITY_STRINGUTILITY
#define UT_UTILITY_WEAKPTRCOMPARE
#endif
