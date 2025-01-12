// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#include <Utility/UTUtility.h>
#include <UnitTestsExceptions.h>
#include <UnitTestsConfiguration.h>
using namespace gtl;

void UTUtility::Execute()
{
#if defined(UT_UTILITY)
    UTInformation("Utility");

#if defined(UT_UTILITY_ATOMICMINMAX)
    Precall();
    extern void TestAtomicMinMax();
    TestAtomicMinMax();
    Postcall();
#endif

#if defined(UT_UTILITY_CONTAINERADAPTER)
    Precall();
    extern void TestContainerAdapter();
    TestContainerAdapter();
    Postcall();
#endif

#if defined(UT_UTILITY_HASHCOMBINE)
    Precall();
    extern void TestHashCombine();
    TestHashCombine();
    Postcall();
#endif

#if defined(UT_UTILITY_LATTICE)
    Precall();
    extern void TestLattice();
    TestLattice();
    Postcall();
#endif

#if defined(UT_UTILITY_MINHEAP)
    Precall();
    extern void TestMinHeap();
    TestMinHeap();
    Postcall();
#endif

#if defined(UT_UTILITY_MINIMUMSPANNINGTREE)
    Precall();
    extern void TestMinimumSpanningTree();
    TestMinimumSpanningTree();
    Postcall();
#endif

#if defined(UT_UTILITY_MULTIARRAY)
    Precall();
    extern void TestMultiarray();
    TestMultiarray();
    Postcall();
#endif

#if defined(UT_UTILITY_MULTIARRAYADAPTER)
    Precall();
    extern void TestMultiarrayAdapter();
    TestMultiarrayAdapter();
    Postcall();
#endif

#if defined(UT_UTILITY_RANGEITERATION)
    Precall();
    extern void TestRangeIteration();
    TestRangeIteration();
    Postcall();
#endif

#if defined(UT_UTILITY_RAWPTRCOMPARE)
    Precall();
    extern void TestRawPtrCompare();
    TestRawPtrCompare();
    Postcall();
#endif

#if defined(UT_UTILITY_SHAREDPTRCOMPARE)
    Precall();
    extern void TestSharedPtrCompare();
    TestSharedPtrCompare();
    Postcall();
#endif

#if defined(UT_UTILITY_STRINGUTILITY)
    Precall();
    extern void TestStringUtility();
    TestStringUtility();
    Postcall();
#endif

#if defined(UT_UTILITY_WEAKPTRCOMPARE)
    Precall();
    extern void TestWeakPtrCompare();
    TestWeakPtrCompare();
    Postcall();
#endif
#endif
}
