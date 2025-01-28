#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/PolygonTree.h>
using namespace gtl;

namespace gtl
{
    class UnitTestPolygonTree
    {
    public:
        UnitTestPolygonTree();
    };
}

UnitTestPolygonTree::UnitTestPolygonTree()
{
    UTInformation("Mathematics/Geometry/2D/PolygonTree [tested by TestTriangulateCDT]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Geometry/2D/PolygonTree.h>

namespace gtl
{
    template std::pair<std::size_t, std::size_t> PolygonTreeEx::GetContainingTriangle(Vector2<float> const&, Vector2<float> const*);
    template std::pair<std::size_t, std::size_t> PolygonTreeEx::GetContainingTriangle(Vector2<float> const&,
        std::vector<std::array<std::size_t, 3>> const&, std::vector<std::size_t> const&, Vector2<float> const*);
    template std::size_t PolygonTreeEx::GetContainingTriangle(Vector2<float> const&,
        std::vector<std::array<std::size_t, 3>> const&, std::int64_t, Vector2<float> const* points);

    template std::pair<std::size_t, std::size_t> PolygonTreeEx::GetContainingTriangle(Vector2<double> const&, Vector2<double> const*);
    template std::pair<std::size_t, std::size_t> PolygonTreeEx::GetContainingTriangle(Vector2<double> const&,
        std::vector<std::array<std::size_t, 3>> const&, std::vector<std::size_t> const&, Vector2<double> const*);
    template std::size_t PolygonTreeEx::GetContainingTriangle(Vector2<double> const&,
        std::vector<std::array<std::size_t, 3>> const&, std::int64_t, Vector2<double> const* points);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSNumber<UIntegerAP32>;
    template std::pair<std::size_t, std::size_t> PolygonTreeEx::GetContainingTriangle(Vector2<Rational> const&, Vector2<Rational> const*);
    template std::pair<std::size_t, std::size_t> PolygonTreeEx::GetContainingTriangle(Vector2<Rational> const&,
        std::vector<std::array<std::size_t, 3>> const&, std::vector<std::size_t> const&, Vector2<Rational> const*);
    template std::size_t PolygonTreeEx::GetContainingTriangle(Vector2<Rational> const&,
        std::vector<std::array<std::size_t, 3>> const&, std::int64_t, Vector2<Rational> const* points);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(PolygonTree)
