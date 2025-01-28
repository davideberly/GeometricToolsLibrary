#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistTriangle3CanonicalBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistTriangle3CanonicalBox3
    {
    public:
        UnitTestDistTriangle3CanonicalBox3();

    private:
        using TBQuery = DCPQuery<double, Triangle3<double>, CanonicalBox3<double>>;
        using TBOutput = TBQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(TBOutput const& output,
            double sqrDistance,
            std::array<double, 3> const& barycentric,
            Vector<double, 3> const& closest0,
            Vector<double, 3> const& closest1);
    };
}

UnitTestDistTriangle3CanonicalBox3::UnitTestDistTriangle3CanonicalBox3()
{
    UTInformation("Mathematics/Distance/3D/DistTriangle3CanonicalBox3");

    Test();
}

void UnitTestDistTriangle3CanonicalBox3::Validate(TBOutput const& output,
    double sqrDistance,
    std::array<double, 3> const& barycentric,
    Vector<double, 3> const& closest0,
    Vector<double, 3> const& closest1)
{
    double constexpr maxError = 1e-14;

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
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[1] - closest1);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
}

void UnitTestDistTriangle3CanonicalBox3::Test()
{
    TBQuery query{};
    TBOutput output{};
    Triangle3<double> triangle{};
    CanonicalBox3<double> box{};

    box.extent = { 1.5, 1.0, 0.5 };

    // Closest plane point inside the triangle.
    triangle.v[0] = { -10.0, 0.0, 0.0 };
    triangle.v[1] = { 0.0, -10.0, 0.0 };
    triangle.v[2] = { 0.0, 0.0, -10.0 };
    output = query(triangle, box);
    Validate(output, 16.333333333333321,
        { 0.38333333333333341, 0.33333333333333331, 0.28333333333333333 },
        { -3.8333333333333326, -3.3333333333333326, -2.8333333333333326 },
        { -1.5, -1.0, -0.5 });

    // Closest plane point outside the triangle.
    triangle.v[0] = { 90.0, -200.0, 100.0 };
    triangle.v[1] = { 100.0, -210.0, 100.0 };
    triangle.v[2] = { 100.0, -200.0, 90.0 };
    output = query(triangle, box);
    Validate(output, 57273.0,
        { 0.45, 0.0, 0.55 },
        { 95.5, -200.0, 94.5 },
        { 1.5, -1.0, 0.5 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistTriangle3CanonicalBox3.h>

namespace gtl
{
    template class DCPQuery<float, Triangle3<float>, CanonicalBox3<float>>;
    template class DCPQuery<double, Triangle3<double>, CanonicalBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Triangle3<Rational>, CanonicalBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistTriangle3CanonicalBox3)
