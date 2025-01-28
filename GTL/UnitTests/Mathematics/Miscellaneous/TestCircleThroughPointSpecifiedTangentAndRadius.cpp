#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Miscellaneous/CircleThroughPointSpecifiedTangentAndRadius.h>
using namespace gtl;

namespace gtl
{
    class UnitTestCircleThroughPointSpecifiedTangentAndRadius
    {
    public:
        UnitTestCircleThroughPointSpecifiedTangentAndRadius();

    private:
        // This is the implementation using the book pseudocode, but that
        // pseudocode needed several fixes.
        std::size_t FromGTFCG(Vector2<double> const& P, Vector2<double> const& A,
            Vector2<double> const& N, double const& r,
            std::array<Circle2<double>, 2>& circle);

        void Test();
    };
}

UnitTestCircleThroughPointSpecifiedTangentAndRadius::UnitTestCircleThroughPointSpecifiedTangentAndRadius()
{
    UTInformation("Mathematics/Miscellaneous/CircleThroughPointSpecifiedTangentAndRadius");

    Test();
}

std::size_t UnitTestCircleThroughPointSpecifiedTangentAndRadius::FromGTFCG(
    Vector2<double> const& P, Vector2<double> const& A, Vector2<double> const& N,
    double const& r, std::array<Circle2<double>, 2>& circle)
{
    double lineC = -Dot(N, A);
    double cPrime = Dot(N, P) + lineC;
    if (std::fabs(cPrime) == 0.0)
    {
        circle[0].center = P - r * N;
        circle[0].radius = r;
        circle[1].center = P + r * N;
        circle[1].radius = r;
        return 2;
    }

    double a, b, c;
    if (cPrime < 0.0)
    {
        a = -N[0];
        b = -N[1];
        c = -lineC;
        cPrime = -cPrime;
    }
    else
    {
        a = N[0];
        b = N[1];
        c = lineC;
    }

    double const epsilon = 1e-08;
    double tmp1 = c - r;
    double tmp2 = r * r - tmp1 * tmp1;
    if (tmp2 < -epsilon)
    {
        circle[0].center = { 0.0, 0.0 };
        circle[0].radius = 0.0;
        circle[1].center = { 0.0, 0.0 };
        circle[1].radius = 0.0;
        return 0;
    }

    if (tmp2 < epsilon)
    {
        circle[0].center = P - tmp1 * Vector2<double>{ a, b };
        circle[0].radius = r;
        circle[1].center = { 0.0, 0.0 };
        circle[1].radius = 0.0;
        return 1;
    }

    tmp2 = std::sqrt(tmp2);
    Vector2<double> tmpPt = P - tmp1 * Vector2<double>{ a, b };
    circle[0].center = tmpPt - tmp2 * Vector2<double>{ b, -a };
    circle[0].radius = r;
    circle[1].center = tmpPt + tmp2 * Vector2<double>{ b, -a };
    circle[1].radius = r;
    return 2;
}

void UnitTestCircleThroughPointSpecifiedTangentAndRadius::Test()
{
    double const maxError = 1e-15;
    Vector2<double> P{}, A{}, N{};
    double r{};
    std::array<Circle2<double>, 2> circle{};
    std::size_t numIntersections{};
    std::array<Vector2<double>, 2> expected{};

    // Case (1).
    P = { 2.0, 1.0 };
    A = { -1.0, 1.0 };
    N = { 0.0, 1.0 };
    r = 1.0;
    numIntersections = CircleThroughPointSpecifiedTangentAndRadius(P, A, N, r, circle);
    //numIntersections = FromGTFCG(P, A, N, r, circle);
    expected[0] = { 2.0, 0.0 };
    expected[1] = { 2.0, 2.0 };
    UTAssert(
        numIntersections == 2 &&
        circle[0].center == expected[0] && circle[0].radius == r &&
        circle[1].center == expected[1] && circle[1].radius == r,
        "Invalid result."
    );

    // Case (2), no sign change on s.
    P = { 2.0, 2.0 };
    A = { -1.0, 1.0 };
    N = { 0.0, 1.0 };
    r = 1.0;
    numIntersections = CircleThroughPointSpecifiedTangentAndRadius(P, A, N, r, circle);
    //numIntersections = FromGTFCG(P, A, N, r, circle);
    expected[0] = { 1.0, 2.0 };
    expected[1] = { 3.0, 2.0 };
    UTAssert(
        numIntersections == 2 &&
        circle[0].center == expected[0] && circle[0].radius == r &&
        circle[1].center == expected[1] && circle[1].radius == r,
        "Invalid result."
    );

    // Case (2), sign change on s.
    P = { 2.0, 2.0 };
    A = { -1.0, 1.0 };
    N = { 0.0, -1.0 };
    r = 1.0;
    numIntersections = CircleThroughPointSpecifiedTangentAndRadius(P, A, N, r, circle);
    //numIntersections = FromGTFCG(P, A, N, r, circle);
    expected[0] = { 1.0, 2.0 };
    expected[1] = { 3.0, 2.0 };
    UTAssert(
        numIntersections == 2 &&
        circle[0].center == expected[0] && circle[0].radius == r &&
        circle[1].center == expected[1] && circle[1].radius == r,
        "Invalid result."
    );

    // Case (3).
    P = { 2.0, 3.0 };
    A = { -1.0, 1.0 };
    N = { 0.0, 1.0 };
    r = 1.0;
    numIntersections = CircleThroughPointSpecifiedTangentAndRadius(P, A, N, r, circle);
    //numIntersections = FromGTFCG(P, A, N, r, circle);
    expected[0] = { 2.0, 2.0 };
    expected[1] = { 0.0, 0.0 };
    UTAssert(
        numIntersections == 1 &&
        circle[0].center == expected[0] && circle[0].radius == r &&
        circle[1].center == expected[1] && circle[1].radius == 0.0,
        "Invalid result."
    );

    // Case (4).
    P = { 2.0, 3.01 };
    A = { -1.0, 1.0 };
    N = { 0.0, 1.0 };
    r = 1.0;
    numIntersections = CircleThroughPointSpecifiedTangentAndRadius(P, A, N, r, circle);
    //numIntersections = FromGTFCG(P, A, N, r, circle);
    UTAssert(
        numIntersections == 0 &&
        IsZero(circle[0].center) && circle[0].radius == 0.0 &&
        IsZero(circle[1].center) && circle[1].radius == 0.0,
        "Invalid result."
    );

    // Case (5a).
    P = { 2.0, 1.01 };
    A = { -1.0, 1.0 };
    N = { 0.0, 1.0 };
    r = 1.0;
    numIntersections = CircleThroughPointSpecifiedTangentAndRadius(P, A, N, r, circle);
    //numIntersections = FromGTFCG(P, A, N, r, circle);
    expected[0] = { 1.8589326402033410, 2.0 };
    expected[1] = { 2.1410673597966587, 2.0 };
    UTAssert(
        numIntersections == 2 &&
        circle[0].center == expected[0] && circle[0].radius == r &&
        circle[1].center == expected[1] && circle[1].radius == r,
        "Invalid result."
    );
    double lengthPmC0 = Length(P - circle[0].center);
    UTAssert(std::fabs(lengthPmC0 - circle[0].radius) <= maxError, "Incorrect length P-C0");
    double lengthPmC1 = Length(P - circle[1].center);
    UTAssert(std::fabs(lengthPmC1 - circle[1].radius) <= maxError, "Incorrect length P-C1");

    // Case (5b).
    P = { 2.0, 2.99 };
    A = { -1.0, 1.0 };
    N = { 0.0, 1.0 };
    r = 1.0;
    numIntersections = CircleThroughPointSpecifiedTangentAndRadius(P, A, N, r, circle);
    //numIntersections = FromGTFCG(P, A, N, r, circle);
    expected[0] = { 1.8589326402033426, 2.0 };
    expected[1] = { 2.1410673597966574, 2.0 };
    UTAssert(
        numIntersections == 2 &&
        circle[0].center == expected[0] && circle[0].radius == r &&
        circle[1].center == expected[1] && circle[1].radius == r,
        "Invalid result."
    );

    lengthPmC0 = Length(P - circle[0].center);
    UTAssert(std::fabs(lengthPmC0 - circle[0].radius) <= maxError, "Incorrect length P-C0");
    lengthPmC1 = Length(P - circle[1].center);
    UTAssert(std::fabs(lengthPmC1 - circle[1].radius) <= maxError, "Incorrect length P-C1");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Miscellaneous/CircleThroughPointSpecifiedTangentAndRadius.h>

namespace gtl
{
    template std::size_t gtl::CircleThroughPointSpecifiedTangentAndRadius(
        Vector2<float> const&, Vector2<float> const&, Vector2<float>,
        float const&, std::array<Circle2<float>, 2>&);

    template std::size_t gtl::CircleThroughPointSpecifiedTangentAndRadius(
        Vector2<double> const&, Vector2<double> const&, Vector2<double>,
        double const&, std::array<Circle2<double>, 2>&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template std::size_t gtl::CircleThroughPointSpecifiedTangentAndRadius(
        Vector2<Rational> const&, Vector2<Rational> const&, Vector2<Rational>,
        Rational const&, std::array<Circle2<Rational>, 2>&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(CircleThroughPointSpecifiedTangentAndRadius)
