#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Miscellaneous/CircleThroughTwoPointsSpecifiedRadius.h>
using namespace gtl;

namespace gtl
{
    class UnitTestCircleThroughTwoPointsSpecifiedRadius
    {
    public:
        UnitTestCircleThroughTwoPointsSpecifiedRadius();

    private:
        void Test();
    };
}

UnitTestCircleThroughTwoPointsSpecifiedRadius::UnitTestCircleThroughTwoPointsSpecifiedRadius()
{
    UTInformation("Mathematics/Miscellaneous/CircleThroughTwoPointsSpecifiedRadius");

    Test();
}

void UnitTestCircleThroughTwoPointsSpecifiedRadius::Test()
{
    // Length of P-Q is sqrt(13) = 3.605551...
    Vector2<double> P{ 1.0, 2.0 }, Q{ 3.0, 5.0 };
    double r = 1.0;
    std::array<Circle2<double>, 2> circle{};
    std::size_t numIntersections = CircleThroughTwoPointsSpecifiedRadius(P, Q, r, circle);
    UTAssert(numIntersections == 0, "Expecting 0 solutions.");
    for (std::size_t i = 0; i < 2; ++i)
    {
        UTAssert(IsZero(circle[i].center), "Expecting (0,0,0).");
        UTAssert(circle[i].radius == 0.0, "Expecting 0.");
    }

    r = 4.0;
    numIntersections = CircleThroughTwoPointsSpecifiedRadius(P, Q, r, circle);
    UTAssert(numIntersections == 2, "Expecting 2 solutions.");
    std::array<Vector2<double>, 2> expected
    { {
        { 4.9710138129808614, 1.5193241246794256 },
        { -0.97101381298086142, 5.4806758753205749 }
    } };
    for (std::size_t i = 0; i < 2; ++i)
    {
        UTAssert(circle[i].center == expected[i], "Incorrect center.");
        UTAssert(circle[i].radius == 4.0, "Expecting 4.");
    }

    double const maxError = 1e-15;
    double lengthPmC0 = Length(P - circle[0].center);
    UTAssert(std::fabs(lengthPmC0 - circle[0].radius) <= maxError, "Incorrect length P-C0");
    double lengthPmC1 = Length(P - circle[1].center);
    UTAssert(std::fabs(lengthPmC1 - circle[1].radius) <= maxError, "Incorrect length P-C1");
    double lengthQmC0 = Length(Q - circle[0].center);
    UTAssert(std::fabs(lengthQmC0 - circle[0].radius) <= maxError, "Incorrect length Q-C0");
    double lengthQmC1 = Length(Q - circle[1].center);
    UTAssert(std::fabs(lengthQmC1 - circle[1].radius) <= maxError, "Incorrect length Q-C1");

    Q = { 3.0, 2.0 };
    r = 1.0;
    numIntersections = CircleThroughTwoPointsSpecifiedRadius(P, Q, r, circle);
    UTAssert(numIntersections == 1, "Expecting 1 solution.");
    UTAssert((circle[0].center == Vector2<double>{ 2.0, 2.0 }), "Incorrect center.");
    UTAssert(circle[0].radius == 1.0, "Expecting 1.");
    UTAssert(IsZero(circle[1].center), "Expecting (0,0,0).");
    UTAssert(circle[1].radius == 0.0, "Expecting 0.");

    P = Q;
    numIntersections = CircleThroughTwoPointsSpecifiedRadius(P, Q, r, circle);
    UTAssert(numIntersections == 0, "Expecting 0 solutions.");
    for (std::size_t i = 0; i < 2; ++i)
    {
        UTAssert(IsZero(circle[i].center), "Expecting (0,0,0).");
        UTAssert(circle[i].radius == 0.0, "Expecting 0.");
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Miscellaneous/CircleThroughTwoPointsSpecifiedRadius.h>

namespace gtl
{
    template std::size_t gtl::CircleThroughTwoPointsSpecifiedRadius(
        Vector2<float> const&, Vector2<float> const&, float const&,
        std::array<Circle2<float>, 2>&);

    template std::size_t gtl::CircleThroughTwoPointsSpecifiedRadius(
        Vector2<double> const&, Vector2<double> const&, double const&,
        std::array<Circle2<double>, 2>&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template std::size_t gtl::CircleThroughTwoPointsSpecifiedRadius(
        Vector2<Rational> const&, Vector2<Rational> const&, Rational const&,
        std::array<Circle2<Rational>, 2>&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(CircleThroughTwoPointsSpecifiedRadius)
