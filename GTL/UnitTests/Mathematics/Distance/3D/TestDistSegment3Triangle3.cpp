#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistSegment3Triangle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistSegment3Triangle3
    {
    public:
        UnitTestDistSegment3Triangle3();

    private:
        using STQuery = DCPQuery<double, Segment<double, 3>, Triangle<double, 3>>;
        using STOutput = STQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(STOutput const& output,
            double sqrDistance,
            double parameter,
            Vector<double, 3> const& barycentric,
            Vector<double, 3> const& closest0,
            Vector<double, 3> const& closest1);
    };
}

UnitTestDistSegment3Triangle3::UnitTestDistSegment3Triangle3()
{
    UTInformation("Mathematics/Distance/3D/DistSegment3Triangle3");

    Test();
}

void UnitTestDistSegment3Triangle3::Validate(STOutput const& output,
    double sqrDistance,
    double parameter,
    Vector<double, 3> const& barycentric,
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
    error = std::fabs(output.parameter - parameter);
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

void UnitTestDistSegment3Triangle3::Test()
{
    STQuery query{};
    STOutput output{};
    Segment<double, 3> segment{};
    Triangle<double, 3> triangle{};

    Vector<double, 3> translate{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rotate{};
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, rotate);

    // line intersects triangle, segment does not, segment.p[0 closest
    segment.p[0] = { 0.25, 0.25, 0.25 };
    segment.p[1] = { 0.0, 0.0, 1.0 };
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    output = query(segment, triangle);
    Validate(output, 0.0625, 0.0,
        { 0.5, 0.25, 0.25 },
        { 0.25, 0.25, 0.25 },
        { 0.25, 0.25, 0.0 });

    // line intersects triangle, segment does not, segment.p[0] closest
    // arbitrary rigid motion first
    segment.p[0] = { 0.25, 0.25, 0.25 };
    segment.p[1] = { 0.0, 0.0, 1.0 };
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    segment.p[0] = rotate * segment.p[0] + translate;
    segment.p[1] = rotate * segment.p[1] + translate;
    triangle.v[0] = rotate * triangle.v[0] + translate;
    triangle.v[1] = rotate * triangle.v[1] + translate;
    triangle.v[2] = rotate * triangle.v[2] + translate;
    output = query(segment, triangle);
    Validate(output, 0.0625, 0.0,
        { 0.5, 0.25, 0.25 },
        rotate * Vector<double, 3>{ 0.25, 0.25, 0.25 } + translate,
        rotate * Vector<double, 3>{ 0.25, 0.25, 0.0 } + translate);

    // line intersects triangle, segment does not, segment.p[1] closest
    segment.p[0] = { 0.0, 0.0, 1.0 };
    segment.p[1] = { 0.25, 0.25, 0.25 };
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    output = query(segment, triangle);
    Validate(output, 0.0625, 1.0,
        { 0.5, 0.25, 0.25 },
        { 0.25, 0.25, 0.25 },
        { 0.25, 0.25, 0.0 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistSegment3Triangle3.h>

namespace gtl
{
    template class DCPQuery<float, Segment<float, 3>, Triangle<float, 3>>;
    template class DCPQuery<double, Segment<double, 3>, Triangle<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Segment<Rational, 3>, Triangle<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistSegment3Triangle3)
