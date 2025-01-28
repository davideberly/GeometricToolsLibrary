#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/SegmentMesh.h>
using namespace gtl;

namespace gtl
{
    class UnitTestSegmentMesh
    {
    public:
        UnitTestSegmentMesh();

    private:
        using SegMesh2 = SegmentMesh<double, 2>;
        void Test();
    };
}

UnitTestSegmentMesh::UnitTestSegmentMesh()
{
    UTInformation("Mathematics/Primitives/ND/SegmentMesh");

    Test();
}

void UnitTestSegmentMesh::Test()
{
    SegMesh2 smDefault{};
    UTAssert(smDefault.GetTopology() == SegMesh2::TOPOLOGY_UNKNOWN, "Invalid topology.");
    UTAssert(smDefault.GetVertices().size() == 0, "Invalid number of vertices.");
    UTAssert(smDefault.GetIndices().size() == 0, "Invalid number of indices.");

    std::vector<Vector2<double>> vertices(4);
    vertices[0] = { 0.0, 0.0 };
    vertices[1] = { 1.0, 0.0 };
    vertices[2] = { 1.0, 1.0 };
    vertices[3] = { 0.0, 1.0 };

    std::vector<std::array<std::size_t, 2>> indices{};

    SegMesh2 smDisjoint(vertices);
    UTAssert(smDisjoint.GetTopology() == SegMesh2::TOPOLOGY_DISJOINT, "Invalid topology.");
    UTAssert(smDisjoint.GetVertices() == vertices, "Invalid vertices.");
    indices = smDisjoint.GetIndices();
    UTAssert(indices.size() == vertices.size() / 2, "Invalid number of indices.");
    for (std::size_t i = 0; i < indices.size(); ++i)
    {
        UTAssert(indices[i][0] == 2 * i && indices[i][1] == 2 * i + 1, "Invalid index pair.");
    }

    SegMesh2 smContiguousOpen(vertices, true);
    UTAssert(smContiguousOpen.GetTopology() == SegMesh2::TOPOLOGY_CONTIGUOUS_OPEN, "Invalid topology.");
    UTAssert(smContiguousOpen.GetVertices() == vertices, "Invalid vertices.");
    indices = smContiguousOpen.GetIndices();
    UTAssert(indices.size() == vertices.size() - 1, "Invalid number of indices.");
    for (std::size_t i0 = 0, i1 = 1; i0 < indices.size(); i0 = i1++)
    {
        UTAssert(indices[i0][0] == i0 && indices[i0][1] == i1, "Invalid index pair.");
    }

    SegMesh2 smContiguousClosed(vertices, false);
    UTAssert(smContiguousClosed.GetTopology() == SegMesh2::TOPOLOGY_CONTIGUOUS_CLOSED, "Invalid topology.");
    UTAssert(smContiguousClosed.GetVertices() == vertices, "Invalid vertices.");
    indices = smContiguousClosed.GetIndices();
    UTAssert(indices.size() == vertices.size(), "Invalid number of indices.");
    for (std::size_t i0 = indices.size() - 1, i1 = 0; i1 < indices.size(); i0 = i1++)
    {
        UTAssert(indices[i1][0] == i0 && indices[i1][1] == i1, "Invalid index pair.");
    }

    indices.resize(4);
    indices[0] = { 0, 1 };
    indices[1] = { 1, 3 };
    indices[2] = { 3, 2 };
    indices[3] = { 2, 0 };
    SegMesh2 smIndexed(vertices, indices, true);
    UTAssert(smIndexed.GetTopology() == SegMesh2::TOPOLOGY_INDEXED, "Invalid topology.");
    UTAssert(smIndexed.GetVertices() == vertices, "Invalid vertices.");
    UTAssert(smIndexed.GetIndices() == indices, "Invalid indices.");

    UTAssert(smDefault < smDisjoint, "Invalid less-than.");
    UTAssert(smContiguousClosed > smContiguousOpen, "Invalid greater-than.");
    UTAssert(smIndexed == smIndexed, "Invalid equal.");
    UTAssert(smIndexed != smDefault, "Invalid not-equal.");
    UTAssert(smIndexed >= smContiguousOpen, "Invalid greater-equal.");
    UTAssert(smContiguousClosed <= smIndexed, "Invalid less-equal.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/SegmentMesh.h>

namespace gtl
{
    template class SegmentMesh<float, 2>;
    template bool operator==(SegmentMesh<float, 2> const&, SegmentMesh<float, 2> const&);
    template bool operator!=(SegmentMesh<float, 2> const&, SegmentMesh<float, 2> const&);
    template bool operator< (SegmentMesh<float, 2> const&, SegmentMesh<float, 2> const&);
    template bool operator<=(SegmentMesh<float, 2> const&, SegmentMesh<float, 2> const&);
    template bool operator> (SegmentMesh<float, 2> const&, SegmentMesh<float, 2> const&);
    template bool operator>=(SegmentMesh<float, 2> const&, SegmentMesh<float, 2> const&);

    template class SegmentMesh<double, 3>;
    template bool operator==(SegmentMesh<double, 3> const&, SegmentMesh<double, 3> const&);
    template bool operator!=(SegmentMesh<double, 3> const&, SegmentMesh<double, 3> const&);
    template bool operator< (SegmentMesh<double, 3> const&, SegmentMesh<double, 3> const&);
    template bool operator<=(SegmentMesh<double, 3> const&, SegmentMesh<double, 3> const&);
    template bool operator> (SegmentMesh<double, 3> const&, SegmentMesh<double, 3> const&);
    template bool operator>=(SegmentMesh<double, 3> const&, SegmentMesh<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class SegmentMesh<Rational, 4>;
    template bool operator==(SegmentMesh<Rational, 4> const&, SegmentMesh<Rational, 4> const&);
    template bool operator!=(SegmentMesh<Rational, 4> const&, SegmentMesh<Rational, 4> const&);
    template bool operator< (SegmentMesh<Rational, 4> const&, SegmentMesh<Rational, 4> const&);
    template bool operator<=(SegmentMesh<Rational, 4> const&, SegmentMesh<Rational, 4> const&);
    template bool operator> (SegmentMesh<Rational, 4> const&, SegmentMesh<Rational, 4> const&);
    template bool operator>=(SegmentMesh<Rational, 4> const&, SegmentMesh<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(SegmentMesh)
