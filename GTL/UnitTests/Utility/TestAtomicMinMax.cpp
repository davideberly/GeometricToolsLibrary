// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Utility/AtomicMinMax.h>
#include <array>
#include <random>
#include <thread>
#include <vector>
using namespace gtl;

namespace gtl
{
    class UnitTestAtomicMinMax
    {
    public:
        UnitTestAtomicMinMax();

    private:
        void TestSingleThreaded();
        void TestMultiThreaded();
    };
}

UnitTestAtomicMinMax::UnitTestAtomicMinMax()
{
    UTInformation("Mathematics/Utility/AtomicMinMax");

    TestSingleThreaded();
    TestMultiThreaded();
}

void UnitTestAtomicMinMax::TestSingleThreaded()
{
    float v0 = 1.23456f, v1 = 7.890123f;
    std::atomic<float> av0(v0), av1(v1);
    AtomicMin(av0, v1);
    AtomicMax(av1, v0);
    float vmin = av0, vmax = av1;
    UTAssert(vmin == v0, "Invalid minimum");
    UTAssert(vmax == v1, "Invalid maximum");
}

void UnitTestAtomicMinMax::TestMultiThreaded()
{
    // Generate a set of random float-valued numbers.
    float const minNumber = 0.0f, maxNumber = 1.0;
    std::default_random_engine dre;
    std::uniform_real_distribution<float> urd(minNumber, maxNumber);
    std::size_t const numElements = 1024;
    std::vector<float> numbers(numElements);
    for (auto& number : numbers)
    {
        number = urd(dre);
    }

    // Start with extreme values that are outside the interval
    // [minNumber,maxNumber] so that vmin and vmax must have
    // their initial values replaced.
    std::atomic<float> vmin(maxNumber), vmax(minNumber);

    // Partition the min-max search into two threads of execution.
    std::array<std::size_t, 2> imin = { 0, numElements / 2 };
    std::array<std::size_t, 2> isup = { numElements / 2, numElements };
    std::array<std::thread, 2> process;
    for (std::size_t t = 0; t < 2; ++t)
    {
        process[t] = std::thread(
            [t, &imin, &isup, &numbers, &vmin, &vmax]()
            {
                for (std::size_t i = imin[t]; i < isup[t]; ++i)
                {
                    float number = numbers[i];
                    AtomicMin(vmin, number);
                    AtomicMax(vmax, number);
                }
            }
        );
    }

    // Wait for the threads to complete.
    for (std::size_t t = 0; t < 2; ++t)
    {
        process[t].join();
    }

    auto location = std::minmax_element(numbers.begin(), numbers.end());
    UTAssert(vmin == *location.first, "Invalid minimum");
    UTAssert(vmax == *location.second, "Invalid maximum");
}

#else

#include <GTL/Utility/AtomicMinMax.h>

namespace gtl
{
    template float AtomicMax(std::atomic<float>&, float const&);
    template float AtomicMin(std::atomic<float>&, float const&);
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(AtomicMinMax)
