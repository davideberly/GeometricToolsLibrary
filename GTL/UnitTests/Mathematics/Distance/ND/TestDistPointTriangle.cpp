#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistPointTriangle.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPointTriangle
    {
    public:
        UnitTestDistPointTriangle();

    private:
        using PT2Query = DCPQuery<double, Vector2<double>, Triangle2<double>>;
        using PT2Output = PT2Query::Output;
        using PT3Query = DCPQuery<double, Vector3<double>, Triangle3<double>>;
        using PT3Output = PT3Query::Output;

        void Test2D();
        void Test3D();

        template <std::size_t N>
        void Validate(
            typename DCPQuery<double, Vector<double, N>, Triangle<double, N>>::Output const& output,
            double sqrDistance,
            std::array<double, 3> const& barycentric,
            Vector<double, N> const& closest0,
            Vector<double, N> const& closest1)
        {
            double const maxError = 1e-15;

            double error = std::fabs(output.sqrDistance - sqrDistance);
            UTAssert(
                error <= maxError,
                "Invalid DCPQuery.");
            error = std::fabs(output.distance - std::sqrt(sqrDistance));
            UTAssert(
                error <= maxError,
                "Invalid DCPQuery.");
            error = std::fabs(output.barycentric[0] - barycentric[0]);
            UTAssert(
                error <= maxError,
                "Invalid DCPQuery.");
            error = std::fabs(output.barycentric[1] - barycentric[1]);
            UTAssert(
                error <= maxError,
                "Invalid DCPQuery.");
            error = std::fabs(output.barycentric[2] - barycentric[2]);
            UTAssert(
                error <= maxError,
                "Invalid DCPQuery.");
            error = Length(output.closest[0] - closest0);
            UTAssert(
                error == 0.0,
                "Invalid DCPQuery.");
            error = Length(output.closest[1] - closest1);
            UTAssert(
                error <= maxError,
                "Invalid DCPQuery.");
        }
    };
}

UnitTestDistPointTriangle::UnitTestDistPointTriangle()
{
    UTInformation("Mathematics/Distance/ND/DistPointTriangle");

    Test2D();
    Test3D();
}

void UnitTestDistPointTriangle::Test2D()
{
    PT2Query query{};
    PT2Output output{};
    Vector<double, 2> point{};

    Triangle<double, 2> triangle{};
    triangle.v[0] = { 1.0, 1.0 };
    triangle.v[1] = { 2.0, 1.0 };
    triangle.v[2] = { 4.0, 2.0 };

    // region 0 (inside <v0,v1,v2>)
    point = 0.1 * triangle.v[0] + 0.3 * triangle.v[1] + 0.6 * triangle.v[2];
    output = query(point, triangle);
    Validate(output, 0.0,
        { 0.1, 0.3, 0.6 },
        point,
        { 3.1, 1.6 });

    // region 1 (closest to <v1,v2>)
    point = { 3.0, 1.125 };
    output = query(point, triangle);
    Validate(output, 0.1125,
        { 0.0, 0.575, 0.425 },
        point,
        { 2.85, 1.425 });

    // region 1 (closest to <v2>)
    point = { 4.0, 2.5 };
    output = query(point, triangle);
    Validate(output, 0.25,
        { 0.0, 0.0, 1.0 },
        point,
        { 4.0, 2.0 });

    // region 3 (closest to <v2>)
    point = { 4.0, 2.1 };
    output = query(point, triangle);
    Validate(output, 0.01,
        { 0.0, 0.0, 1.0 },
        point,
        { 4.0, 2.0 });

    // region 3 (closest to <v2,v0>)
    point = { 1.5, 2.0 };
    output = query(point, triangle);
    Validate(output, 0.625,
        { 0.75, 0.0, 0.25 },
        point,
        { 1.75, 1.25 });

    // region 3 (closest to <v0>)
    point = { 0.75, 1.125 };
    output = query(point, triangle);
    Validate(output, 0.078125,
        { 1.0, 0.0, 0.0 },
        point,
        { 1.0, 1.0 });

    // region 4 (closest to <v0>)
    point = { 0.75, 0.125 };
    output = query(point, triangle);
    Validate(output, 0.828125,
        { 1.0, 0.0, 0.0 },
        point,
        { 1.0, 1.0 });

    // region 5 (closest to <v0>)
    point = { 0.75, 0.5 };
    output = query(point, triangle);
    Validate(output, 0.3125,
        { 1.0, 0.0, 0.0 },
        point,
        { 1.0, 1.0 });

    // region 5 (closest to <v0,v1>)
    point = { 1.5, 0.95 };
    output = query(point, triangle);
    Validate(output, 0.0025,
        { 0.5, 0.5, 0.0 },
        point,
        { 1.5, 1.0 });

    // region 6 (closest to <v1>)
    point = { 2.5, 0.0 };
    output = query(point, triangle);
    Validate(output, 1.25,
        { 0.0, 1.0, 0.0 },
        point,
        { 2.0, 1.0 });

    // region 6 (closest to <v1,v2>)
    point = { 2.5, 1.0 };
    output = query(point, triangle);
    Validate(output, 0.05,
        { 0.0, 0.8, 0.2 },
        point,
        { 2.4, 1.2 });
}

void UnitTestDistPointTriangle::Test3D()
{
    PT3Query query{};
    PT3Output output{};
    Vector<double, 3> point{};

    Triangle<double, 3> triangle{};
    triangle.v[0] = { 1.0, 1.0, 1.0 };
    triangle.v[1] = { 2.0, 1.0, 1.0 };
    triangle.v[2] = { 4.0, 2.0, 1.0 };

    // region 0 (inside <v0,v1,v2>)
    point = 0.1 * triangle.v[0] + 0.3 * triangle.v[1] + 0.6 * triangle.v[2];
    point[2] += 1.0;
    output = query(point, triangle);
    Validate(output, 1.0,
        { 0.1, 0.3, 0.6 },
        point,
        { 3.1, 1.6, 1.0 });

    // region 1 (closest to <v1,v2>)
    point = { 3.0, 1.125, 2.0 };
    output = query(point, triangle);
    Validate(output, 1.1125,
        { 0.0, 0.575, 0.425 },
        point,
        { 2.85, 1.425, 1.0 });

    // region 1 (closest to <v2>)
    point = { 4.0, 2.5, 2.0 };
    output = query(point, triangle);
    Validate(output, 1.25,
        { 0.0, 0.0, 1.0 },
        point,
        { 4.0, 2.0, 1.0 });

    // region 3 (closest to <v2>)
    point = { 4.0, 2.1, 2.0 };
    output = query(point, triangle);
    Validate(output, 1.01,
        { 0.0, 0.0, 1.0 },
        point,
        { 4.0, 2.0, 1.0 });

    // region 3 (closest to <v2,v0>)
    point = { 1.5, 2.0, 2.0 };
    output = query(point, triangle);
    Validate(output, 1.625,
        { 0.75, 0.0, 0.25 },
        point,
        { 1.75, 1.25, 1.0 });

    // region 3 (closest to <v0>)
    point = { 0.75, 1.125, 2.0 };
    output = query(point, triangle);
    Validate(output, 1.078125,
        { 1.0, 0.0, 0.0 },
        point,
        { 1.0, 1.0, 1.0 });

    // region 4 (closest to <v0>)
    point = { 0.75, 0.125, 2.0 };
    output = query(point, triangle);
    Validate(output, 1.828125,
        { 1.0, 0.0, 0.0 },
        point,
        { 1.0, 1.0, 1.0 });

    // region 5 (closest to <v0>)
    point = { 0.75, 0.5, 2.0 };
    output = query(point, triangle);
    Validate(output, 1.3125,
        { 1.0, 0.0, 0.0 },
        point,
        { 1.0, 1.0, 1.0 });

    // region 5 (closest to <v0,v1>)
    point = { 1.5, 0.95, 2.0 };
    output = query(point, triangle);
    Validate(output, 1.0025,
        { 0.5, 0.5, 0.0 },
        point,
        { 1.5, 1.0, 1.0 });

    // region 6 (closest to <v1>)
    point = { 2.5, 0.0, 2.0 };
    output = query(point, triangle);
    Validate(output, 2.25,
        { 0.0, 1.0, 0.0 },
        point,
        { 2.0, 1.0, 1.0 });

    // region 6 (closest to <v1,v2>)
    point = { 2.5, 1.0, 2.0 };
    output = query(point, triangle);
    Validate(output, 1.05,
        { 0.0, 0.8, 0.2 },
        point,
        { 2.4, 1.2, 1.0 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistPointTriangle.h>

namespace gtl
{
    template class DCPQuery<float, Vector<float, 2>, Triangle<float, 2>>;
    template class DCPQuery<double, Vector<double, 3>, Triangle<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector<Rational, 4>, Triangle<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPointTriangle)
