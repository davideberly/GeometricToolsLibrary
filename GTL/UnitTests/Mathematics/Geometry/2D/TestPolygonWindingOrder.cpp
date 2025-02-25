#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/PolygonWindingOrder.h>
using namespace gtl;

namespace gtl
{
    class UnitTestPolygonWindingOrder
    {
    public:
        UnitTestPolygonWindingOrder();

    private:
        void Test0StartConvex();
        void Test0StartReflex();
        void Test1StartConvex();
        void Test1StartReflex();

        PolygonWindingOrder<float> mOrder;
    };
}

UnitTestPolygonWindingOrder::UnitTestPolygonWindingOrder()
    :
    mOrder{}
{
    UTInformation("Mathematics/Geometry/2D/PolygonWindingOrder");
    Test0StartConvex();
    Test0StartReflex();
    Test1StartConvex();
    Test1StartReflex();
}

void UnitTestPolygonWindingOrder::Test0StartConvex()
{
    std::vector<Vector2<float>> polygon(8);

    // Start with convex vertex.
    polygon[0] = { 128.0f, 256.0f };
    polygon[1] = { 200.0f, 100.0f };
    polygon[2] = { 256.0f, 150.0f };
    polygon[3] = { 400.0f, 100.0f };
    polygon[4] = { 400.0f, 400.0f };
    polygon[5] = { 300.0f, 300.0f };
    polygon[6] = { 230.0f, 300.0f };
    polygon[7] = { 220.0f, 450.0f };

    UTAssert(
        mOrder(polygon),
        "The polygon must be counterclockwise.");
}

void UnitTestPolygonWindingOrder::Test0StartReflex()
{
    std::vector<Vector2<float>> polygon(8);

    // Start with convex vertex.
    polygon[0] = { 256.0f, 150.0f };
    polygon[1] = { 400.0f, 100.0f };
    polygon[2] = { 400.0f, 400.0f };
    polygon[3] = { 300.0f, 300.0f };
    polygon[4] = { 230.0f, 300.0f };
    polygon[5] = { 220.0f, 450.0f };
    polygon[6] = { 128.0f, 256.0f };
    polygon[7] = { 200.0f, 100.0f };

    UTAssert(
        mOrder(polygon),
        "The polygon must be counterclockwise.");
}

void UnitTestPolygonWindingOrder::Test1StartConvex()
{
    std::vector<Vector2<float>> polygon(16);

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

    UTAssert(
        mOrder(polygon),
        "The polygon must be counterclockwise.");
}

void UnitTestPolygonWindingOrder::Test1StartReflex()
{
    std::vector<Vector2<float>> polygon(16);

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

    UTAssert(
        mOrder(polygon),
        "The polygon must be counterclockwise.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Geometry/2D/PolygonWindingOrder.h>

namespace gtl
{
    template class PolygonWindingOrder<float>;
    template class PolygonWindingOrder<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSNumber<UIntegerAP32>;
    template class PolygonWindingOrder<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(PolygonWindingOrder)
