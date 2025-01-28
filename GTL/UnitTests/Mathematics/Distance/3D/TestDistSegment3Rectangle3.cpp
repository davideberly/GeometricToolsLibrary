#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistSegment3Rectangle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistSegment3Rectangle3
    {
    public:
        UnitTestDistSegment3Rectangle3();

    private:
        using SRQuery = DCPQuery<double, Segment<double, 3>, Rectangle<double, 3>>;
        using SROutput = SRQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(SROutput const& output,
            double sqrDistance,
            double parameter,
            Vector<double, 2> const& rectangle,
            Vector<double, 3> const& closest0,
            Vector<double, 3> const& closest1);
    };
}

UnitTestDistSegment3Rectangle3::UnitTestDistSegment3Rectangle3()
{
    UTInformation("Mathematics/Distance/3D/DistSegment3Rectangle3");

    Test();
}

void UnitTestDistSegment3Rectangle3::Validate(SROutput const& output,
    double sqrDistance,
    double parameter,
    Vector<double, 2> const& cartesian,
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
    error = std::fabs(output.cartesian[0] - cartesian[0]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] - cartesian[1]);
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

void UnitTestDistSegment3Rectangle3::Test()
{
    SRQuery query{};
    SROutput output{};
    Segment<double, 3> segment{};
    Rectangle<double, 3> rectangle{};

    Vector<double, 3> translate{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rotate{};
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, rotate);

    // line intersects rectangle, segment.p[0] origin closest
    segment.p[0] = { 0.0, 0.0, 0.25 };
    segment.p[1] = Vector<double, 3>{ -0.0625, -0.0125, 1.0 } + segment.p[0];
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    output = query(segment, rectangle);
    Validate(output, 0.0625, 0.0,
        { 0.0, 0.0 },
        { 0.0, 0.0, 0.25 },
        { 0.0, 0.0, 0.0 });

    // line intersects rectangle, segment.p[0] origin closest
    // arbitrary rigid motion first
    segment.p[0] = { 0.0, 0.0, 0.25 };
    segment.p[1] = Vector<double, 3>{ -0.0625, -0.0125, 1.0 } + segment.p[0];
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    segment.p[0] = rotate * segment.p[0] + translate;
    segment.p[1] = rotate * segment.p[1] + translate;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(segment, rectangle);
    Validate(output, 0.0625, 0.0,
        { 0.0, 0.0 },
        rotate* Vector<double, 3>{ 0.0, 0.0, 0.25 } + translate,
        rotate* Vector<double, 3>{ 0.0, 0.0, 0.0 } + translate);

    // line intersects rectangle, segment.p[1] origin closest
    segment.p[1] = { 0.0, 0.0, 0.25 };
    segment.p[0] = Vector<double, 3>{ -0.0625, -0.0125, 1.0 } + segment.p[1];
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    output = query(segment, rectangle);
    Validate(output, 0.0625, 1.0,
        { 0.0, 0.0 },
        { 0.0, 0.0, 0.25 },
        { 0.0, 0.0, 0.0 });

    // line intersects rectangle, segment.p[1] origin closest
    // arbitrary rigid motion first
    segment.p[1] = { 0.0, 0.0, 0.25 };
    segment.p[0] = Vector<double, 3>{ -0.0625, -0.0125, 1.0 } + segment.p[1];
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    segment.p[0] = rotate * segment.p[0] + translate;
    segment.p[1] = rotate * segment.p[1] + translate;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(segment, rectangle);
    Validate(output, 0.0625, 1.0,
        { 0.0, 0.0 },
        rotate* Vector<double, 3>{ 0.0, 0.0, 0.25 } + translate,
        rotate* Vector<double, 3>{ 0.0, 0.0, 0.0 } + translate);
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistSegment3Rectangle3.h>

namespace gtl
{
    template class DCPQuery<float, Segment<float, 3>, Rectangle<float, 3>>;
    template class DCPQuery<double, Segment<double, 3>, Rectangle<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Segment<Rational, 3>, Rectangle<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistSegment3Rectangle3)
