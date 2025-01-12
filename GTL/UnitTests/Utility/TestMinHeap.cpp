// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Utility/MinHeap.h>
#include <array>
#include <cmath>
using namespace gtl;

namespace gtl
{
    class UnitTestMinHeap
    {
    public:
        UnitTestMinHeap();

    private:
        void Test();

        // The data structure for a doubly linked list of vertices
        // representing a simple polygon. The positions are stored
        // separately. The key is the location of the vertex
        // (handle,weight) in the min-heap.
        struct Vertex
        {
            std::size_t key;
            std::size_t prev, curr, next;
        };

        double Distance(std::array<double, 2> const& point,
            std::array<double, 2> const& end0, std::array<double, 2> const& end1);

        double ComputeWeight(std::vector<std::array<double, 2>> const& positions,
            Vertex const& vertex);

        void PolygonDecimation();
    };
}

UnitTestMinHeap::UnitTestMinHeap()
{
    UTInformation("Mathematics/Utility/MinHeap");

    Test();
    PolygonDecimation();
}

void UnitTestMinHeap::Test()
{
    std::vector<float> numbers =
    {
        3.0f,
        4.0f,
        2.0f,
        2.0f,
        1.0f,
        5.0f
    };

    std::vector<std::size_t> key(numbers.size());

    MinHeap<float> heap(numbers.size());
    std::size_t const maxElements = heap.GetMaxElements();
    UTAssert(
        maxElements == numbers.size(),
        "Invalid GetMaxElements.");
    UTAssert(heap.IsValid(),
        "Invalid heap.");

    // The initial min-heap is empty.
    //   heap.numElements = 0
    //   heap.mNodes = { x, x, x, x, x, x }  // x = unused
    //   heap.mKeys = { 0, 1, 2, 3, 4, 5 }
    //   heap.mIndices = { 0, 1, 2, 3, 4, 5 }
    UTAssert(
        heap.GetNumElements() == 0,
        "Invalid GetNumElements.");
    for (std::size_t i = 0; i < maxElements; ++i)
    {
        UTAssert(
            heap.mKeys[i] == i,
            "Invalid key.");
    }
    for (std::size_t i = 0; i < maxElements; ++i)
    {
        UTAssert(
            heap.mIndices[heap.mKeys[i]] == i,
            "Invalid index.");
    }
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Insert numbers[0].
    //   heap.numElements = 1
    //   heap.mNodes = { (0, 3.0f), x, x, x, x, x }
    //   heap.mKeys = { 0, 1, 2, 3, 4, 5 }
    //   heap.mIndices = { 0, 1, 2, 3, 4, 5 }
    key[0] = heap.Insert(0, numbers[0]);
    UTAssert(
        heap.GetNumElements() == 1,
        "Invalid GetNumElements.");
    UTAssert(
        key[0] == 0,
        "Invalid key.");
    for (std::size_t i = 0; i < heap.GetNumElements(); ++i)
    {
        UTAssert(
            heap.mNodes[i].handle == i && heap.mNodes[i].weight == numbers[i],
            "Invalid node.");
    }
    for (std::size_t i = 0; i < maxElements; ++i)
    {
        UTAssert(
            heap.mKeys[i] == i,
            "Invalid key.");
    }
    for (std::size_t i = 0; i < maxElements; ++i)
    {
        UTAssert(
            heap.mIndices[heap.mKeys[i]] == i,
            "Invalid index.");
    }
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Insert numbers[1].
    //   heap.numElements = 2
    //   heap.mNodes = { (0, 3.0f), (1, 4.0f), x, x, x, x }
    //   heap.mKeys = { 0, 1, 2, 3, 4, 5 }
    //   heap.mIndices = { 0, 1, 2, 3, 4, 5 }
    key[1] = heap.Insert(1, numbers[1]);
    UTAssert(
        heap.GetNumElements() == 2,
        "Invalid GetNumElements.");
    UTAssert(
        key[1] == 1,
        "Invalid key.");
    for (std::size_t i = 0; i < heap.GetNumElements(); ++i)
    {
        UTAssert(
            heap.mNodes[i].handle == i && heap.mNodes[i].weight == numbers[i],
            "Invalid node.");
    }
    for (std::size_t i = 0; i < maxElements; ++i)
    {
        UTAssert(
            heap.mKeys[i] == i,
            "Invalid key.");
    }
    for (std::size_t i = 0; i < maxElements; ++i)
    {
        UTAssert(
            heap.mIndices[heap.mKeys[i]] == i,
            "Invalid index.");
    }
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Insert numbers[2].
    //   heap.numElements = 3
    //   heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 2.0f), x, x, x }
    //   heap.mKeys = { 2, 1, 0, 3, 4, 5 }
    //   heap.mIndices = { 2, 1, 0, 3, 4, 5 }
    key[2] = heap.Insert(2, numbers[2]);
    UTAssert(
        heap.GetNumElements() == 3,
        "Invalid GetNumElements.");
    UTAssert(
        key[2] == 2,
        "Invalid key.");
    for (std::size_t i = 0; i < heap.GetNumElements(); ++i)
    {
        UTAssert(
            heap.mNodes[i].handle == i && heap.mNodes[i].weight == numbers[i],
            "Invalid node.");
    }
    std::vector<std::size_t> expectedKey = { 2, 1, 0, 3, 4, 5 };
    std::vector<std::size_t> expectedIndex = { 2, 1, 0, 3, 4, 5 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    for (std::size_t i = 0; i < maxElements; ++i)
    {
        UTAssert(
            heap.mIndices[heap.mKeys[i]] == i,
            "Invalid index.");
    }
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Insert numbers[3].
    //   heap.numElements = 4
    //   heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 2.0f), (3, 2.0f'), x, x }
    //   heap.mKeys = { 2, 3, 0, 1, 4, 5 }
    //   heap.mIndices = { 2, 3, 0, 1, 4, 5 }
    key[3] = heap.Insert(3, numbers[3]);
    UTAssert(
        heap.GetNumElements() == 4,
        "Invalid GetNumElements,");
    UTAssert(
        key[3] == 3,
        "Invalid key.");
    for (std::size_t i = 0; i < heap.GetNumElements(); ++i)
    {
        UTAssert(
            heap.mNodes[i].handle == i && heap.mNodes[i].weight == numbers[i],
            "Invalid node.");
    }
    expectedKey = { 2, 3, 0, 1, 4, 5 };
    expectedIndex = { 2, 3, 0, 1, 4, 5 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    for (std::size_t i = 0; i < maxElements; ++i)
    {
        UTAssert(
            heap.mIndices[heap.mKeys[i]] == i,
            "Invalid index.");
    }
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Insert numbers[4].
    //   heap.numElements = 5
    //   heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 2.0f), (3, 2.0f'), (4, 1.0f), x }
    //   heap.mKeys = { 4, 2, 0, 1, 3, 5 }
    //   heap.mIndices = { 2, 3, 1, 4, 0, 5 }
    key[4] = heap.Insert(4, numbers[4]);
    UTAssert(
        heap.GetNumElements() == 5,
        "Invalid GetNumElements.");
    UTAssert(
        key[4] == 4,
        "Invalid key.");
    for (std::size_t i = 0; i < heap.GetNumElements(); ++i)
    {
        UTAssert(
            heap.mNodes[i].handle == i && heap.mNodes[i].weight == numbers[i],
            "Invalid node.");
    }
    expectedKey = { 4, 2, 0, 1, 3, 5 };
    expectedIndex = { 2, 3, 1, 4, 0, 5 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    for (std::size_t i = 0; i < maxElements; ++i)
    {
        UTAssert(
            heap.mIndices[heap.mKeys[i]] == i,
            "Invalid index.");
    }
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Insert numbers[5].
    //   heap.numElements = 6
    //   heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 2.0f), (3, 2.0f'), (4, 1.0f), (5, 5.0f) }
    //   heap.mKeys = { 4, 2, 0, 1, 3, 5 }
    //   heap.mIndices = { 2, 3, 1, 4, 0, 5 }
    key[5] = heap.Insert(5, numbers[5]);
    UTAssert(
        heap.GetNumElements() == 6,
        "Invalid GetNumElements.");
    UTAssert(
        key[5] == 5,
        "Invalid key.");
    for (std::size_t i = 0; i < heap.GetNumElements(); ++i)
    {
        UTAssert(
            heap.mNodes[i].handle == i && heap.mNodes[i].weight == numbers[i],
            "Invalid node.");
    }
    expectedKey = { 4, 2, 0, 1, 3, 5 };
    expectedIndex = { 2, 3, 1, 4, 0, 5 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    for (std::size_t i = 0; i < maxElements; ++i)
    {
        UTAssert(
            heap.mIndices[heap.mKeys[i]] == i,
            "Invalid index.");
    }
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Test trap of full heap.
    std::size_t tempKey = heap.Insert(6, -1.0f);
    UTAssert(
        tempKey == heap.invalid,
        "The key should be invalid on insertion into a full heap.");

    // Save a copy for later testing.
    MinHeap<float> copyHeap = heap;
    UTAssert(
        copyHeap.GetMaxElements() == maxElements,
        "Invalid GetMaxElements.");
    UTAssert(
        copyHeap.GetNumElements() == 6,
        "Invalid GetNumElements.");
    for (std::size_t i = 0; i < copyHeap.GetNumElements(); ++i)
    {
        UTAssert(
            copyHeap.mNodes[i].handle == i && copyHeap.mNodes[i].weight == numbers[i],
            "Invalid node.");
    }
    UTAssert(
        copyHeap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        copyHeap.mIndices == expectedIndex,
        "Invalid index.");
    for (std::size_t i = 0; i < maxElements; ++i)
    {
        UTAssert(
            copyHeap.mIndices[heap.mKeys[i]] == i,
            "Invalid index.");
    }
    UTAssert(
        copyHeap.IsValid(),
        "Invalid heap.");

    // Heap is
    //   1 -- 3 -- 5
    //   |
    //   2 -- 2'
    //   |
    //   4
    //
    // heap.numElements = 6
    // heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 2.0f), (3, 2.0f'), (4, 1.0f), (5, 5.0f) }
    // heap.mKeys = { 4, 2, 0, 1, 3, 5 }
    // heap.mIndices = { 2, 3, 1, 4, 0, 5 }

    // Remove minimum 1.
    //   2 -- 3 -- 1x
    //   |
    //   2' -- 5
    //   |
    //   4
    //
    // heap.numElements = 5
    // heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 2.0f), (3, 2.0f'), (4, 1.0f), (5, 5.0f) }
    // heap.mKeys = { 2, 3, 0, 1, 5, 4 }
    // heap.mIndices = { 2, 3, 0, 1, 5, 4 }
    std::size_t handle = 0;
    float minWeight = 0.0f;
    tempKey = heap.GetMinimum(handle, minWeight);
    UTAssert(
        tempKey == 4 && handle == 4 && minWeight == 1.0f,
        "Invalid GetMinimum.");
    tempKey = heap.Remove(handle, minWeight);
    UTAssert(
        heap.GetNumElements() == 5 && tempKey == 4 && handle == 4 && minWeight == 1.0f,
        "Invalid Remove.");
    expectedKey = { 2, 3, 0, 1, 5, 4 };
    expectedIndex = { 2, 3, 0, 1, 5, 4 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Remove minimum 2.
    //   2' -- 3 -- 1x
    //   |
    //   4 -- 2x
    //   |
    //   5
    //
    // heap.numElements = 4
    // heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 2.0f), (3, 2.0f'), (4, 1.0f), (5, 5.0f) }
    // heap.mKeys = { 3, 1, 0, 5, 2, 4 }
    // heap.mIndices = { 2, 1, 4, 0, 5, 3 }
    tempKey = heap.GetMinimum(handle, minWeight);
    UTAssert(
        tempKey == 2 && handle == 2 && minWeight == 2.0f,
        "Invalid GetMinimum.");
    tempKey = heap.Remove(handle, minWeight);
    UTAssert(
        heap.GetNumElements() == 4 && tempKey == 2 && handle == 2 && minWeight == 2.0f,
        "Invalid Remove.");
    expectedKey = { 3, 1, 0, 5, 2, 4 };
    expectedIndex = { 2, 1, 4, 0, 5, 3 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Remove minimum 2'.
    //   3 -- 5 -- 1x
    //   |
    //   4 -- 2x
    //   |
    //   2'x
    //
    // heap.numElements = 3
    // heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 2.0f), (3, 2.0f'), (4, 1.0f), (5, 5.0f) }
    // heap.mKeys = { 0, 1, 5, 3, 2, 4 }
    // heap.mIndices = { 0, 1, 4, 3, 5, 2 }
    tempKey = heap.GetMinimum(handle, minWeight);
    UTAssert(
        tempKey == 3 && handle == 3 && minWeight == 2.0f,
        "Invalid GetMinimum.");
    tempKey = heap.Remove(handle, minWeight);
    UTAssert(
        heap.GetNumElements() == 3 && tempKey == 3 && handle == 3 && minWeight == 2.0f,
        "Invalid Remove.");
    expectedKey = { 0, 1, 5, 3, 2, 4 };
    expectedIndex = { 0, 1, 4, 3, 5, 2 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Remove minimum 3.
    //   4 -- 3x -- 1x
    //   |
    //   5 -- 2x
    //   |
    //   2'x
    //
    // heap.numElements = 2
    // heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 2.0f), (3, 2.0f'), (4, 1.0f), (5, 5.0f) }
    // heap.mKeys = { 1, 5, 0, 3, 2, 4 }
    // heap.mIndices = { 2, 0, 4, 3, 5, 1 }
    tempKey = heap.GetMinimum(handle, minWeight);
    UTAssert(
        tempKey == 0 && handle == 0 && minWeight == 3.0f,
        "Invalid GetMinimum.");
    tempKey = heap.Remove(handle, minWeight);
    UTAssert(
        heap.GetNumElements() == 2 && tempKey == 0 && handle == 0 && minWeight == 3.0f,
        "Invalid Remove.");
    expectedKey = { 1, 5, 0, 3, 2, 4 };
    expectedIndex = { 2, 0, 4, 3, 5, 1 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Remove minimum 4.
    //   5 -- 3x -- 1x
    //   |
    //   4x -- 2x
    //   |
    //   2'x
    //
    // heap.numElements = 1
    // heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 2.0f), (3, 2.0f'), (4, 1.0f), (5, 5.0f) }
    // heap.mKeys = { 5, 1, 0, 3, 2, 4 }
    // heap.mIndices = { 2, 1, 4, 3, 5, 0 }
    tempKey = heap.GetMinimum(handle, minWeight);
    UTAssert(
        tempKey == 1 && handle == 1 && minWeight == 4.0f,
        "Invalid GetMinimum.");
    tempKey = heap.Remove(handle, minWeight);
    UTAssert(
        heap.GetNumElements() == 1 && tempKey == 1 && handle == 1 && minWeight == 4.0f,
        "Invalid Remove.");
    expectedKey = { 5, 1, 0, 3, 2, 4 };
    expectedIndex = { 2, 1, 4, 3, 5, 0 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Remove minimum 5.
    //   5x -- 3x -- 1x
    //   |
    //   4x -- 2x
    //   |
    //   2'x
    //
    // heap.numElements = 0
    // heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 2.0f), (3, 2.0f'), (4, 1.0f), (5, 5.0f) }
    // heap.mKeys = { 0, 1, 2, 3, 4, 5 }
    // heap.mIndices = { 0, 1, 2, 3, 4, 5 }
    tempKey = heap.GetMinimum(handle, minWeight);
    UTAssert(
        tempKey == 5 && handle == 5 && minWeight == 5.0f,
        "Invalid GetMinimum.");
    tempKey = heap.Remove(handle, minWeight);
    UTAssert(
        heap.GetNumElements() == 0 && tempKey == 5 && handle == 5 && minWeight == 5.0f,
        "Invalid Remove.");
    expectedKey = { 0, 1, 2, 3, 4, 5 };
    expectedIndex = { 0, 1, 2, 3, 4, 5 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    UTAssert(
        heap.IsValid(), "Invalid heap.");

    // Recreate the heap for further experiments with it.
    for (std::size_t i = 0; i < numbers.size(); ++i)
    {
        key[i] = heap.Insert(i, numbers[i]);
    }
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Heap is
    //   1 -- 3 -- 5
    //   |
    //   2 -- 2'
    //   |
    //   4
    //
    // heap.numElements = 6
    // heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 2.0f), (3, 2.0f'), (4, 1.0f), (5, 5.0f) }
    // heap.mKeys = { 4, 2, 0, 1, 3, 5 }
    // heap.mIndices = { 2, 3, 1, 4, 0, 5 }
    expectedKey = { 4, 2, 0, 1, 3, 5 };
    expectedIndex = { 2, 3, 1, 4, 0, 5 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");

    // Modify 2' to 1'.
    //   1 -- 3 -- 5
    //   |
    //   1' -- 2
    //   |
    //   4
    //
    // heap.numElements = 6
    // heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 2.0f), (3, 1.0f'), (4, 1.0f), (5, 5.0f) }
    // heap.mKeys = { 4, 3, 0, 1, 2, 5 }
    // heap.mIndices = { 2, 3, 4, 1, 0, 5 }
    bool updated = heap.Update(key[3], 1.0f);
    UTAssert(
        updated,
        "Expecting the update to be successful.");
    expectedKey = { 4, 3, 0, 1, 2, 5 };
    expectedIndex = { 2, 3, 4, 1, 0, 5 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Modify 2 to 0.
    //   0 -- 3 -- 5
    //   |
    //   1 -- 1'
    //   |
    //   4
    //
    // heap.numElements = 6
    // heap.mNodes = { (0, 3.0f), (1, 4.0f), (2, 0.0f), (3, 1.0f'), (4, 1.0f), (5, 5.0f) }
    // heap.mKeys = { 2, 4, 0, 1, 3, 5 }
    // heap.mIndices = { 2, 3, 0, 4, 1, 5 }
    updated = heap.Update(key[2], 0.0f);
    UTAssert(
        updated,
        "Expecting the update to be successful.");
    expectedKey = { 2, 4, 0, 1, 3, 5 };
    expectedIndex = { 2, 3, 0, 4, 1, 5 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Modify 3 to 6.
    //   0 -- 5 -- 6
    //   |
    //   1 -- 1'
    //   |
    //   4
    //
    // heap.numElements = 6
    // heap.mNodes = { (0, 6.0f), (1, 4.0f), (2, 0.0f), (3, 1.0f'), (4, 1.0f), (5, 5.0f) }
    // heap.mKeys = { 2, 4, 5, 1, 3, 0 }
    // heap.mIndices = { 5, 3, 0, 4, 1, 2 }
    updated = heap.Update(key[0], 6.0f);
    UTAssert(
        updated,
        "Expecting the update to be successful.");
    expectedKey = { 2, 4, 5, 1, 3, 0 };
    expectedIndex = { 5, 3, 0, 4, 1, 2 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Modify 0 to 3.
    //   1 -- 5 -- 6
    //   |
    //   1' -- 3
    //   |
    //   4
    //
    // heap.numElements = 6
    // heap.mNodes = { (0, 6.0f), (1, 4.0f), (2, 3.0f), (3, 1.0f'), (4, 1.0f), (5, 5.0f) }
    // heap.key   = { 4, 3, 5, 1, 2, 0 }
    // heap.index = { 5, 3, 4, 1, 0, 2 }
    updated = heap.Update(key[2], 3.0f);
    UTAssert(
        updated,
        "Expecting the update to be successful.");
    expectedKey = { 4, 3, 5, 1, 2, 0 };
    expectedIndex = { 5, 3, 4, 1, 0, 2 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    // Modify 1' to 7.
    //   1 -- 5 -- 6
    //   |
    //   3 -- 7
    //   |
    //   4
    //
    // heap.numElements = 6
    // heap.value = { 6.0f, 4.0f, 3.0f, 1.0f', 1.0f, 5.0f }
    // heap.key   = { 4, 2, 5, 1, 3, 0 }
    // heap.index = { 5, 3, 1, 4, 0, 2 }
    updated = heap.Update(key[3], 7.0f);
    UTAssert(
        updated,
        "Expecting the update to be successful.");
    expectedKey = { 4, 2, 5, 1, 3, 0 };
    expectedIndex = { 5, 3, 1, 4, 0, 2 };
    UTAssert(
        heap.mKeys == expectedKey,
        "Invalid key.");
    UTAssert(
        heap.mIndices == expectedIndex,
        "Invalid index.");
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");

    heap = copyHeap;
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");
    heap.Reset(3);
    UTAssert(
        heap.GetNumElements() == 0 &&
        heap.mNodes.size() == 3 &&
        heap.mKeys.size() == 3 &&
        heap.mIndices.size() == 3,
        "Invalid Reset.");

    heap = std::move(copyHeap);
    UTAssert(
        heap.GetNumElements() == numbers.size() && 
        heap.mNodes.size() == numbers.size() &&
        heap.mKeys.size() == numbers.size() &&
        heap.mIndices.size() == numbers.size(),
        "Invalid Reset.");
    UTAssert(
        heap.IsValid(),
        "Invalid heap.");
}

double UnitTestMinHeap::Distance(std::array<double, 2> const& point,
    std::array<double, 2> const& end0, std::array<double, 2> const& end1)
{
    // The direction vector is not unit length.  The normalization is
    // deferred until it is needed.
    std::array<double, 2> direction = { end1[0] - end0[0], end1[1] - end0[1] };
    std::array<double, 2> diff = { point[0] - end1[0], point[1] - end1[1] };
    std::array<double, 2> closest{};
    double t = direction[0] * diff[0] + direction[1] * diff[1];
    if (t >= 0.0)
    {
        closest = end1;
        return std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
    }

    diff = { point[0] - end0[0], point[1] - end0[1] };
    t = direction[0] * diff[0] + direction[1] * diff[1];
    if (t <= 0.0)
    {
        return std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
    }

    double sqrLength = direction[0] * direction[0] + direction[1] * direction[1];
    closest[0] = end0[0] + (t / sqrLength) * direction[0];
    closest[1] = end0[1] + (t / sqrLength) * direction[1];
    diff[0] -= (t / sqrLength) * direction[0];
    diff[1] -= (t / sqrLength) * direction[1];
    return std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
}

double UnitTestMinHeap::ComputeWeight(
    std::vector<std::array<double, 2>> const& positions,
    Vertex const& vertex)
{
    // The position of the vertex whose weight must be modified.
    std::array<double, 2> posCurr = positions[vertex.curr];

    // The positions of the neighboring vertices.
    std::array<double, 2> posPrev = positions[vertex.prev];
    std::array<double, 2> posNext = positions[vertex.next];

    // Implement your favorite weight function. For example, a scale-invariant
    // weight function is shown here.
    double distance = Distance(posCurr, posPrev, posNext);
    std::array<double, 2> diff = { posNext[0] - posPrev[0], posNext[1] - posPrev[1] };
    double length = std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
    double weight = distance / length;
    return weight;
}

void UnitTestMinHeap::PolygonDecimation()
{
    std::size_t const N = 8;
    std::vector<double> angles =
    {
        0.0,
        1.3093002290045481,
        1.5752296219476425,
        1.7393568117223051,
        2.1773350778399365,
        3.3789492594255890,
        4.8255294939361066,
        5.4983791260200432
    };

    // The positions for the vertices, assumed to be initialized before
    // the smoothing.
    std::vector<std::array<double, 2>> positions(N);
    for (std::size_t i = 0; i < N; ++i)
    {
        positions[i] = { std::cos(angles[i]), std::sin(angles[i]) };
    }

    // The abstraction of the vertex weight computations.

    // Create the closed polyline doubly linked list. The vertex members curr
    // and key are the same, because the vertices are inserted into the min-heap
    // in natural order. Generally, if insertions and removals for a polyline are
    // interleaved, the members will be different.
    std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();
    std::vector<Vertex> vertices(N);
    vertices[0] = { invalid, N - 1, 0, 1 };
    for (std::size_t i = 1; i < N - 1; ++i)
    {
        vertices[i] = { invalid, i - 1, i, i + 1 };
    }
    vertices[N - 1] = { invalid, N - 2, N - 1, 0 };

    // Initialize the min-heap.
    MinHeap<double> minHeap(N);
    for (std::size_t i = 0; i < N; ++i)
    {
        vertices[i].key = minHeap.Insert(i, ComputeWeight(positions, vertices[i]));
        UTAssert(
            vertices[i].key != minHeap.invalid,
            "Expecting Insert to succeed.");

        // The vertices are inserted in natural order with no interleaved
        // removals.
        UTAssert(
            vertices[i].key == i,
            "Mismatch of key and handle.");

        // NOTE: It is possible to provide handles that are not the
        // consecutive indices into the array of vertices. Generally, the
        // key and the handle can be different.
    }

    // Smooth by removing one vertex at a time until the final polyline is
    // a triangle. The minHeap before the decimation is:
    //   minHeap.numElements = 8
    //   minHeap.keys = { 2, 3, 1, 7, 4, 5, 6, 0 }
    //   minHeap.indices = { 7, 2, 0, 1, 4, 5, 6, 3 }
    //   minHeap.nodes =
    //   {
    //       (0, 0.26888569665111983),
    //       (1, 0.11390513218138611),
    //       (2, 0.050930178636699894),
    //       (3, 0.060054612309021399),
    //       (4, 0.16800021721713373),
    //       (5, 0.38583356427562143),
    //       (6, 0.25049729962792699),
    //       (7, 0.18954933023987491)
    //   }
    std::size_t handle = invalid;
    double weight = -std::numeric_limits<double>::max();
    while (minHeap.GetNumElements() > 3)
    {
        // Remove the root node of the min-heap. This node represents the
        // vertex of minimum weight. The weight is not needed in this simple
        // illustration, so it is not consumed later in the code.
        std::size_t key = minHeap.Remove(handle, weight);
        UTAssert(
            key != minHeap.invalid,
            "Expecting Remove to succeed.");

        // This is the vertex of minimum weight.
        Vertex& vertexCurr = vertices[handle];

        // Remove the vertex from the doubly linked list.
        Vertex& vertexPrev = vertices[vertexCurr.prev];
        Vertex& vertexNext = vertices[vertexCurr.next];
        vertexPrev.next = vertexCurr.next;
        vertexNext.prev = vertexCurr.prev;
        vertexCurr.key = invalid;
        vertexCurr.prev = invalid;
        vertexCurr.next = invalid;

        // Update the neighbors' weights in the min-heap. The
        bool updated;
        updated = minHeap.Update(vertexPrev.key,
            ComputeWeight(positions, vertexPrev));
        UTAssert(
            updated,
            "Expecting Update to succeed.");

        updated = minHeap.Update(vertexNext.key,
            ComputeWeight(positions, vertexNext));
        UTAssert(
            updated,
            "Expecting Update to succeed.");
    }

    // Iteration 1.
    //   minHeap.numElements = 7
    //   minHeap.keys = { 3, 4, 1, 7, 0, 5, 6, 2 }
    //   minHeap.indices = { 4, 2, 7, 0, 1, 5, 6, 3 }
    //   minHeap.nodes =
    //   {
    //       (0, 0.26888569665111983),
    //       (1, 0.17002554753747984),  // modified
    //       (2, removed),
    //       (3, 0.11023099461064528),  // modified
    //       (4, 0.16800021721713373),
    //       (5, 0.38583356427562143),
    //       (6, 0.25049729962792699),
    //       (7, 0.18954933023987491)
    //   }

    // Iteration 2.
    //   minHeap.numElements = 6
    //   minHeap.keys = { 7, 6, 1, 4, 0, 5, 3, 2 }
    //   minHeap.indices = { 4, 2, 7, 6, 3, 5, 1, 0 }
    //   minHeap.nodes =
    //   {
    //       (0, 0.26888569665111983),
    //       (1, 0.28898821550388476),  // modified
    //       (2, *),
    //       (3, removed),
    //       (4, 0.27649515486339993),  // modified
    //       (5, 0.38583356427562143),
    //       (6, 0.25049729962792699),
    //       (7, 0.18954933023987491)
    //   }

    // Iteration 3.
    //   minHeap.numElements = 5
    //   minHeap.keys = { 4, 5, 1, 6, 0, 7, 3, 2 }
    //   minHeap.indices = { 4, 2, 7, 6, 0, 1, 3, 5 }
    //   minHeap.nodes =
    //   {
    //       (0, 0.41273251413336454),  // modified
    //       (1, 0.28898821550388476),
    //       (2, *),
    //       (3, *),
    //       (4, 0.27649515486339993),
    //       (5, 0.38583356427562143),
    //       (6, 0.44391487866790591),  // modified
    //       (7, removed)
    //   }

    // Iteration 4.
    //   minHeap.numElements = 4
    //   minHeap.keys = { 0, 6, 5, 1, 4, 7, 3, 2 }
    //   minHeap.indices = { 0, 3, 7, 6, 4, 2, 1, 5 }
    //   minHeap.nodes =
    //   {
    //       (0, 0.41273251413336454),
    //       (1, 0.52720830925252427),  // modified
    //       (2, *),
    //       (3, *),
    //       (4, removed),
    //       (5, 0.57917600588083229),  // modified
    //       (6, 0.44391487866790591),
    //       (7, *)
    //   }

    // Iteration 5.
    //   minHeap.numElements = 3
    //   minHeap.keys = { 5, 6, 1, 0, 4, 7, 3, 2 }
    //   minHeap.indices = { 3, 2, 7, 6, 4, 0, 1, 5 }
    //   minHeap.nodes =
    //   {
    //       (0, removed),
    //       (1, 1.2763118300752101),  // modified
    //       (2, *),
    //       (3, *),
    //       (4, *),
    //       (5, 0.57917600588083229),
    //       (6, 0.75633580064878481),  // modified
    //       (7, *)
    //   }

    // Iteration 6.
    //   minHeap.numElements = 3
    //   minHeap.keys = { 5, 6, 1, 0, 4, 7, 3, 2 }
    //   minHeap.indices = { 3, 2, 7, 6, 4, 0, 1, 5 }
    //   minHeap.nodes =
    //   {
    //       (0, removed),
    //       (1, 1.2763118300752101),  // modified
    //       (2, *),
    //       (3, *),
    //       (4, *),
    //       (5, 0.57917600588083229),
    //       (6, 0.75633580064878481),  // modified
    //       (7, *)
    //   }

    // The loop exits because the decimated polygon is a triangle
    // with vertices whose positions are position[1], position[5] and
    // position[6].
}

#else

#include <GTL/Utility/MinHeap.h>

namespace gtl
{
    template class MinHeap<float>;
    template class MinHeap<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(MinHeap)
