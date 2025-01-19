// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Utility/MinimumSpanningTree.h>
#include <numeric>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestMinimumSpanningTree
    {
    public:
        UnitTestMinimumSpanningTree();

    private:
        using MST = MinimumSpanningTree<std::uint32_t>;
        static std::size_t constexpr smax = std::numeric_limits<std::size_t>::max();

        void TestCormenExample();
        void TestJohnsExample();
        void TestJohnsPermutedExample();
    };
}

UnitTestMinimumSpanningTree::UnitTestMinimumSpanningTree()
{
    UTInformation("Mathematics/Utility/MinimumSpanningTree");

    TestCormenExample();
    TestJohnsExample();
    TestJohnsPermutedExample();
}

void UnitTestMinimumSpanningTree::TestCormenExample()
{
    std::size_t constexpr numEdges = 14;
    std::vector<std::array<std::size_t, 2>> edges(numEdges);
    edges[0] = { 0, 1 };
    edges[1] = { 0, 8 };
    edges[2] = { 1, 2 };
    edges[3] = { 1, 7 };
    edges[4] = { 2, 3 };
    edges[5] = { 2, 5 };
    edges[6] = { 2, 8 };
    edges[7] = { 3, 4 };
    edges[8] = { 3, 5 };
    edges[9] = { 4, 5 };
    edges[10] = { 5, 6 };
    edges[11] = { 6, 7 };
    edges[12] = { 6, 8 };
    edges[13] = { 7, 8 };

    std::vector<std::uint32_t> weights(numEdges);
    weights[0] = 4;
    weights[1] = 8;
    weights[2] = 8;
    weights[3] = 11;
    weights[4] = 7;
    weights[5] = 4;
    weights[6] = 2;
    weights[7] = 9;
    weights[8] = 14;
    weights[9] = 10;
    weights[10] = 2;
    weights[11] = 1;
    weights[12] = 6;
    weights[13] = 7;

    std::vector<std::array<std::size_t, 2>> minimumSpanningTree{};
    std::vector<std::array<std::size_t, 2>> backEdges{};
    MST::Execute(edges, weights, true, minimumSpanningTree, backEdges);

    std::vector<std::array<std::size_t, 2>> expectedTree =
    {
        {smax, 0},
        {0, 1},
        {0, 8},
        {8, 2},
        {2, 5},
        {5, 6},
        {6, 7},
        {2, 3},
        {3, 4}
    };

    UTAssert(minimumSpanningTree == expectedTree, "Incorrect tree.");

    std::vector<std::array<std::size_t, 2>> expectedBackEdges =
    {
        {1, 2},
        {1, 7},
        {8, 7},
        {8, 6},
        {3, 5},
        {5, 4}
    };

    UTAssert(backEdges == expectedBackEdges, "Incorrect tree.");
}

void UnitTestMinimumSpanningTree::TestJohnsExample()
{
    std::size_t constexpr numEdges = 15;
    std::vector<std::array<std::size_t, 2>> edges(numEdges);
    edges[0] = { 1, 2 };
    edges[1] = { 2, 3 };
    edges[2] = { 3, 4 };
    edges[3] = { 4, 6 };
    edges[4] = { 4, 7 };
    edges[5] = { 5, 6 };
    edges[6] = { 3, 5 };
    edges[7] = { 7, 8 };
    edges[8] = { 6, 10 };
    edges[9] = { 5, 9 };
    edges[10] = { 10, 9 };
    edges[11] = { 10, 11 };
    edges[12] = { 11, 12 };
    edges[13] = { 11, 13 };
    edges[14] = { 12, 13 };

    std::vector<std::uint32_t> weights(numEdges);
    std::fill(weights.begin(), weights.end(), 1);

    std::vector<std::array<std::size_t, 2>> minimumSpanningTree{};
    std::vector<std::array<std::size_t, 2>> backEdges{};
    MST::Execute(edges, weights, true, minimumSpanningTree, backEdges);

    std::vector<std::array<std::size_t, 2>> expectedTree =
    {
        {smax, 1},
        {1, 2},
        {2, 3},
        {3, 4},
        {3, 5},
        {4, 6},
        {4, 7},
        {6, 10},
        {7, 8},
        {10, 11},
        {5, 9},
        {11, 13},
        {11, 12}
    };

    UTAssert(minimumSpanningTree == expectedTree, "Incorrect tree.");

    std::vector<std::array<std::size_t, 2>> expectedBackEdges =
    {
        {6, 5},
        {10, 9},
        {12, 13}
    };

    UTAssert(backEdges == expectedBackEdges, "Incorrect tree.");
}

void UnitTestMinimumSpanningTree::TestJohnsPermutedExample()
{
    std::size_t constexpr numEdges = 15;
    std::vector<std::array<std::size_t, 2>> edges(numEdges);
    edges[0] = { 1, 2 };
    edges[1] = { 2, 3 };
    edges[2] = { 3, 4 };
    edges[3] = { 4, 6 };
    edges[4] = { 4, 7 };
    edges[5] = { 5, 6 };
    edges[6] = { 3, 5 };
    edges[7] = { 7, 8 };
    edges[8] = { 6, 10 };
    edges[9] = { 5, 9 };
    edges[10] = { 10, 9 };
    edges[11] = { 10, 11 };
    edges[12] = { 11, 12 };
    edges[13] = { 11, 13 };
    edges[14] = { 12, 13 };

    std::vector<std::size_t> permuted(numEdges);
    std::iota(permuted.begin(), permuted.end(), 0);
    std::default_random_engine dre{};
    std::shuffle(permuted.begin(), permuted.end(), dre);
    std::vector<std::array<std::size_t, 2>> permEdges(numEdges);
    for (std::size_t i = 0; i < numEdges; ++i)
    {
        permEdges[i] = edges[permuted[i]];
    }
    edges = permEdges;

    std::vector<std::uint32_t> weights(numEdges);
    std::fill(weights.begin(), weights.end(), 1);

    std::vector<std::array<std::size_t, 2>> minimumSpanningTree{};
    std::vector<std::array<std::size_t, 2>> backEdges{};
    MST::Execute(edges, weights, true, minimumSpanningTree, backEdges);

    std::vector<std::array<std::size_t, 2>> expectedTree =
    {
        {smax, 4},
        {4, 7},
        {4, 6},
        {7, 8},
        {6, 10},
        {6, 5},
        {10, 9},
        {10, 11},
        {4, 3},
        {11, 13},
        {3, 2},
        {11, 12},
        {2, 1}
    };

    UTAssert(minimumSpanningTree == expectedTree, "Incorrect tree.");

    std::vector<std::array<std::size_t, 2>> expectedBackEdges =
    {
        {3, 5},
        {13, 12},
        {5, 9}
    };

    UTAssert(backEdges == expectedBackEdges, "Incorrect tree.");
}

#else

#include <GTL/Utility/MinimumSpanningTree.h>

namespace gtl
{
    template class MinimumSpanningTree<float>;
    template class MinimumSpanningTree<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(MinimumSpanningTree)
