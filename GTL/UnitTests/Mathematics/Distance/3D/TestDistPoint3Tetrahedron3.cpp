#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistPoint3Tetrahedron3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPoint3Tetrahedron3
    {
    public:
        UnitTestDistPoint3Tetrahedron3();

    private:
        using PTQuery = DCPQuery<double, Vector3<double>, Tetrahedron3<double>>;
        using PTOutput = typename PTQuery::Output;

        void Test();

        void Validate(PTOutput const& output,
            double sqrDistance,
            std::array<double, 4> const& parameter,
            Vector3<double> const& closest0,
            Vector3<double> const& closest1);
    };
}

UnitTestDistPoint3Tetrahedron3::UnitTestDistPoint3Tetrahedron3()
{
    UTInformation("Mathematics/Distance/3D/DistPoint3Tetrahedron3");

    Test();
}

void UnitTestDistPoint3Tetrahedron3::Validate(PTOutput const& output,
    double sqrDistance,
    std::array<double, 4> const& barycentric,
    Vector3<double> const& closest0,
    Vector3<double> const& closest1)
{
    double const maxError = 1e-14;

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
    error = std::fabs(output.barycentric[3] - barycentric[3]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - closest0);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[1] - closest1);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
}

void UnitTestDistPoint3Tetrahedron3::Test()
{
    PTQuery query{};
    PTOutput output{};
    Vector3<double> point{};

    Tetrahedron3<double> tetrahedron{};
    tetrahedron.v[0] = { 0.0, 0.0, 0.0 };
    tetrahedron.v[1] = { 1.0, 0.0, 0.0 };
    tetrahedron.v[2] = { 0.0, 1.0, 0.0 };
    tetrahedron.v[3] = { 0.0, 0.0, 1.0 };

    // inside tetrahedron
    point = { 0.25, 0.25, 0.25 };
    output = query(point, tetrahedron);
    Validate(output, 0.0,
        { 0.25, 0.25, 0.25, 0.25 },
        { 0.25, 0.25, 0.25 },
        { 0.25, 0.25, 0.25 });

    // visible face <v1,v2,v3>
    point = { 0.3, 0.4, 0.5 };
    output = query(point, tetrahedron);
    Validate(output, 0.013333333333333343,
        { 0.0, 0.23333333333333323, 0.33333333333333343, 0.43333333333333340 },
        { 0.3, 0.4, 0.5 },
        { 0.23333333333333323, 0.33333333333333343, 0.43333333333333340 });

    // visible face <v0,v1,v3>
    point = { 0.25, -0.125, 0.5 };
    output = query(point, tetrahedron);
    Validate(output, 0.015625,
        { 0.25, 0.25, 0.0, 0.5 },
        { 0.25, -0.125, 0.5 },
        { 0.25, 0.0, 0.5 });

    // visible face <v0,v3,v2>
    point = { -0.125, 0.25, 0.5 };
    output = query(point, tetrahedron);
    Validate(output, 0.015625,
        { 0.25, 0.0, 0.25, 0.5 },
        { -0.125, 0.25, 0.5 },
        { 0.0, 0.25, 0.5 });

    // visible face <v0,v2,v1>
    point = { 0.25, 0.5, -0.125 };
    output = query(point, tetrahedron);
    Validate(output, 0.015625,
        { 0.25, 0.25, 0.5, 0.0 },
        { 0.25, 0.5, -0.125 },
        { 0.25, 0.5, 0.0 });

    // visible faces <v0,v1,v3> and <v1,v2,v3>
    point = { 1.5, -0.125, 2.0 };
    output = query(point, tetrahedron);
    Validate(output, 3.140625,
        { 0.0, 0.25, 0.0, 0.75 },
        { 1.5, -0.125, 2.0 },
        { 0.25, 0.0, 0.75 });

    // visible faces <v0,v1,v3>, <v1,v2,v3> and <v0,v3,v2>
    point = { -0.1, -0.2, 2.0 };
    output = query(point, tetrahedron);
    Validate(output, 1.05,
        { 0.0, 0.0, 0.0, 1.0 },
        { -0.1, -0.2, 2.0 },
        { 0.0, 0.0, 1.0 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistPoint3Tetrahedron3.h>

namespace gtl
{
    template class DCPQuery<float, Vector3<float>, Tetrahedron3<float>>;
    template class DCPQuery<double, Vector3<double>, Tetrahedron3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector3<Rational>, Tetrahedron3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPoint3Tetrahedron3)
