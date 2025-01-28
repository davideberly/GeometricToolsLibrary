#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistTriangle3AlignedBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistTriangle3AlignedBox3
    {
    public:
        UnitTestDistTriangle3AlignedBox3();

    private:
        using TBQuery = DCPQuery<double, Triangle3<double>, AlignedBox3<double>>;
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

UnitTestDistTriangle3AlignedBox3::UnitTestDistTriangle3AlignedBox3()
{
    UTInformation("Mathematics/Distance/3D/DistTriangle3AlignedBox3");

    Test();
}

void UnitTestDistTriangle3AlignedBox3::Validate(TBOutput const& output,
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

void UnitTestDistTriangle3AlignedBox3::Test()
{
    TBQuery query{};
    TBOutput output{};
    Triangle3<double> triangle{};
    AlignedBox3<double> box{};

    box.min = { 0.0, 0.0, 0.0 };
    box.max = { 3.0, 2.0, 1.0 };

    // Closest plane point inside the triangle.
    triangle.v[0] = { -8.5, 1.0, 0.5 };
    triangle.v[1] = { 1.5, -9.0, 0.5 };
    triangle.v[2] = { 1.5, 1.0, -9.5 };
    output = query(triangle, box);
    Validate(output, 16.333333333333321,
        { 0.38333333333333341, 0.33333333333333331, 0.28333333333333333 },
        { -2.3333333333333326, -2.3333333333333326, -2.3333333333333326 },
        { 0.0, 0.0, 0.0 });

    // Closest plane point outside the triangle.
    triangle.v[0] = { 91.5, -199.0, 100.5 };
    triangle.v[1] = { 101.5, -209.0, 100.5 };
    triangle.v[2] = { 101.5, -199.0, 90.5 };
    output = query(triangle, box);
    Validate(output, 57273.0,
        { 0.45, 0.0, 0.55 },
        { 97.0, -199.0, 95.0 },
        { 3.0, 0.0, 1.0 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistTriangle3AlignedBox3.h>

namespace gtl
{
    template class DCPQuery<float, Triangle3<float>, AlignedBox3<float>>;
    template class DCPQuery<double, Triangle3<double>, AlignedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Triangle3<Rational>, AlignedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistTriangle3AlignedBox3)
