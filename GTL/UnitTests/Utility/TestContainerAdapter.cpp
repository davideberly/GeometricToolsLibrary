// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Utility/ContainerAdapter.h>
using namespace gtl;

namespace gtl
{
    class UnitTestContainerAdapter
    {
    public:
        UnitTestContainerAdapter();

    private:
        void TestConstructionN();
        void TestConstructionDynamic();
    };
}

UnitTestContainerAdapter::UnitTestContainerAdapter()
{
    UTInformation("Mathematics/Utility/ContainerAdapter");

    TestConstructionN();
    TestConstructionDynamic();
}

void UnitTestContainerAdapter::TestConstructionN()
{
    std::size_t constexpr numElements = 8;
    double* elements0 = new double[numElements];
    double* elements1 = new double[numElements];
    for (std::size_t i = 0; i < numElements; ++i)
    {
        elements0[i] = static_cast<double>(i) + 1.0;
        elements1[i] = -elements0[i];
    }

    ContainerAdapter<double, numElements> container0(elements0);
    ContainerAdapter<double, numElements> container1(elements1);
    container1 = container0;
    for (std::size_t i = 0; i < container1.size(); ++i)
    {
        UTAssert(
            container1[i] == elements0[i],
            "Container copy failed.");
    }

    for (std::size_t i = 0; i < container1.size(); ++i)
    {
        UTAssert(
            container1.at(i) == elements0[i],
            "Container at() failed.");
    }

    container1.reset(elements1);
    std::size_t j = 0;
    for (auto const& element : container1)
    {
        UTAssert(
            element == elements1[j],
            "Container begin/end failed.");

        ++j;
    }

    j = numElements - 1;
    for (auto iter = container1.rbegin(); iter != container1.rend(); ++iter)
    {
        UTAssert(
            *iter == elements1[j],
            "Container rbegin/rend failed.");

        --j;
    }

    j = 0;
    for (auto iter = container1.cbegin(); iter != container1.cend(); ++iter)
    {
        UTAssert(
            *iter == elements1[j],
            "Container cbegin/cend failed.");

        ++j;
    }

    j = numElements - 1;
    for (auto iter = container1.crbegin(); iter != container1.crend(); ++iter)
    {
        UTAssert(
            *iter == elements1[j],
            "Container crbegin/crend failed.");

        --j;
    }

    delete[] elements1;
    delete[] elements0;
}

void UnitTestContainerAdapter::TestConstructionDynamic()
{
    std::size_t constexpr numElements = 8;
    double* elements0 = new double[numElements];
    double* elements1 = new double[numElements];
    for (std::size_t i = 0; i < numElements; ++i)
    {
        elements0[i] = static_cast<double>(i) + 1.0;
        elements1[i] = -elements0[i];
    }

    ContainerAdapter<double> container0(elements0, numElements);
    ContainerAdapter<double> container1(elements1, numElements);
    container1 = container0;
    for (std::size_t i = 0; i < container1.size(); ++i)
    {
        UTAssert(
            container1[i] == elements0[i],
            "Container copy failed.");
    }

    for (std::size_t i = 0; i < container1.size(); ++i)
    {
        UTAssert(
            container1.at(i) == elements0[i],
            "Container at() failed.");
    }

    container1.reset(elements1, numElements);
    std::size_t j = 0;
    for (auto const& element : container1)
    {
        UTAssert(
            element == elements1[j],
            "Container begin/end failed.");

        ++j;
    }

    j = numElements - 1;
    for (auto iter = container1.rbegin(); iter != container1.rend(); ++iter)
    {
        UTAssert(
            *iter == elements1[j],
            "Container rbegin/rend failed.");

        --j;
    }

    j = 0;
    for (auto iter = container1.cbegin(); iter != container1.cend(); ++iter)
    {
        UTAssert(
            *iter == elements1[j],
            "Container cbegin/cend failed.");

        ++j;
    }

    j = numElements - 1;
    for (auto iter = container1.crbegin(); iter != container1.crend(); ++iter)
    {
        UTAssert(
            *iter == elements1[j],
            "Container crbegin/crend failed.");

        --j;
    }

    delete[] elements1;
    delete[] elements0;
}

#else

#include <GTL/Utility/ContainerAdapter.h>
#include <array>
#include <cstdint>

namespace gtl
{
    template class ContainerAdapter<float, 2>;
    template class ContainerAdapter<float>;

    struct Element
    {
        std::int32_t i;
        double d;
        std::array<std::uint32_t, 3> a;
    };

    template class ContainerAdapter<Element, 3>;
    template class ContainerAdapter<Element>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ContainerAdapter)
