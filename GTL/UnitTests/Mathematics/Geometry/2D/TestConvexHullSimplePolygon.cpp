#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/ConvexHullSimplePolygon.h>
using namespace gtl;

namespace gtl
{
    class UnitTestConvexHullSimplePolygon
    {
    public:
        UnitTestConvexHullSimplePolygon();

    private:
        void Test0StartConvex();
        void Test0StartReflex();
        void Test1StartConvex();
        void Test1StartReflex();

        ConvexHullSimplePolygon<float> mCHSP;
    };
}

UnitTestConvexHullSimplePolygon::UnitTestConvexHullSimplePolygon()
{
    UTInformation("Mathematics/Geometry/2D/ConvexHullSimplePolygon");
    Test0StartConvex();
    Test0StartReflex();
    Test1StartConvex();
    Test1StartReflex();
}

void UnitTestConvexHullSimplePolygon::Test0StartConvex()
{
    std::vector<Vector2<float>> polygon(8);
    std::vector<std::size_t> hull{};

    // Start with convex vertex.
    polygon[0] = { 128.0f, 256.0f };
    polygon[1] = { 200.0f, 100.0f };
    polygon[2] = { 256.0f, 150.0f };
    polygon[3] = { 400.0f, 100.0f };
    polygon[4] = { 400.0f, 400.0f };
    polygon[5] = { 300.0f, 300.0f };
    polygon[6] = { 230.0f, 300.0f };
    polygon[7] = { 220.0f, 450.0f };

    mCHSP(polygon, hull);
    std::vector<std::size_t> expectedHull = { 7, 0, 1, 3, 4 };
    UTAssert(hull == expectedHull, "Incorrect hull.");
}

void UnitTestConvexHullSimplePolygon::Test0StartReflex()
{
    std::vector<Vector2<float>> polygon(8);
    std::vector<std::size_t> hull{};

    // Start with convex vertex.
    polygon[0] = { 256.0f, 150.0f };
    polygon[1] = { 400.0f, 100.0f };
    polygon[2] = { 400.0f, 400.0f };
    polygon[3] = { 300.0f, 300.0f };
    polygon[4] = { 230.0f, 300.0f };
    polygon[5] = { 220.0f, 450.0f };
    polygon[6] = { 128.0f, 256.0f };
    polygon[7] = { 200.0f, 100.0f };

    mCHSP(polygon, hull);
    std::vector<std::size_t> expectedHull = { 7, 1, 2, 5, 6 };
    UTAssert(hull == expectedHull, "Incorrect hull.");
}

void UnitTestConvexHullSimplePolygon::Test1StartConvex()
{
    std::vector<Vector2<float>> polygon(16);
    std::vector<std::size_t> hull{};

    // Complicated topology, start with convex vertex.
    polygon[0] = { 11.0f, 214.0f };
    polygon[1] = { 19.0f, 53.0f };
    polygon[2] = { 239.0f, 282.0f };
    polygon[3] = { 111.0f, 110.0f };
    polygon[4] = { 138.0f, 25.0f };
    polygon[5] = { 201.0f, 6.0f };
    polygon[6] = { 241.0f, 79.0f };
    polygon[7] = { 194.0f, 25.0f };
    polygon[8] = { 153.0f, 34.0f };
    polygon[9] = { 147.0f, 120.0f };
    polygon[10] = { 234.0f, 214.0f };
    polygon[11] = { 170.f, 53.0f };
    polygon[12] = { 295.0f, 180.0f };
    polygon[13] = { 315.0f, 439.0f };
    polygon[14] = { 166.0f, 232.0f };
    polygon[15] = { 190.0f, 365.0f };

    mCHSP(polygon, hull);
    std::vector<std::size_t> expectedHull = { 15, 0, 1, 5, 6, 12, 13 };
    UTAssert(hull == expectedHull, "Incorrect hull.");
}

void UnitTestConvexHullSimplePolygon::Test1StartReflex()
{
    std::vector<Vector2<float>> polygon(16);
    std::vector<std::size_t> hull{};

    // Complicated topology, start with convex vertex.
    polygon[0] = { 147.0f, 120.0f };
    polygon[1] = { 234.0f, 214.0f };
    polygon[2] = { 170.f, 53.0f };
    polygon[3] = { 295.0f, 180.0f };
    polygon[4] = { 315.0f, 439.0f };
    polygon[5] = { 166.0f, 232.0f };
    polygon[6] = { 190.0f, 365.0f };
    polygon[7] = { 11.0f, 214.0f };
    polygon[8] = { 19.0f, 53.0f };
    polygon[9] = { 239.0f, 282.0f };
    polygon[10] = { 111.0f, 110.0f };
    polygon[11] = { 138.0f, 25.0f };
    polygon[12] = { 201.0f, 6.0f };
    polygon[13] = { 241.0f, 79.0f };
    polygon[14] = { 194.0f, 25.0f };
    polygon[15] = { 153.0f, 34.0f };

    mCHSP(polygon, hull);
    std::vector<std::size_t> expectedHull = { 13, 3, 4, 6, 7, 8, 12 };
    UTAssert(hull == expectedHull, "Incorrect hull.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Geometry/2D/ConvexHullSimplePolygon.h>

namespace gtl
{
    template class ConvexHullSimplePolygon<float>;
    template class ConvexHullSimplePolygon<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSNumber<UIntegerAP32>;
    template class ConvexHullSimplePolygon<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ConvexHullSimplePolygon)
