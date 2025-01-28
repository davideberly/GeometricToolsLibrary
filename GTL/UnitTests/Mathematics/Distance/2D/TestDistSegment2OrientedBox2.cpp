#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistSegment2OrientedBox2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistSegment2OrientedBox2
    {
    public:
        UnitTestDistSegment2OrientedBox2();

    private:
        using SBQuery = DCPQuery<double, Segment2<double>, OrientedBox2<double>>;
        using SBOutput = SBQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(SBOutput const& output,
            double sqrDistance,
            double parameter,
            Vector2<double> const& closest0,
            Vector2<double> const& closest1);
    };
}

UnitTestDistSegment2OrientedBox2::UnitTestDistSegment2OrientedBox2()
{
    UTInformation("Mathematics/Distance/2D/DistSegment2OrientedBox2");

    Test();
}

void UnitTestDistSegment2OrientedBox2::Validate(SBOutput const& output,
    double sqrDistance,
    double parameter,
    Vector2<double> const& closest0,
    Vector2<double> const& closest1)
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
    error = Length(output.closest[0] - closest0);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[1] - closest1);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
}

void UnitTestDistSegment2OrientedBox2::Test()
{
    SBQuery query{};
    SBOutput output{};
    Segment2<double> segment{};
    OrientedBox2<double> box{};

    box.center = { 264.0, 232.0 };
    double const boxAngle = C_PI<double> / 6.0;
    box.axis[0] = { std::cos(boxAngle), std::sin(boxAngle) };
    box.axis[1] = { -std::sin(boxAngle), std::cos(boxAngle) };
    box.extent = { 64.0, 32.0 };

    // segment interior is closest
    segment.p[0] = { 255.0, 256.0 };
    segment.p[1] = { 301.80663690362690, 299.64789504399994 };
    output = query(segment, box);
    Validate(output, 0.0, 0.46644801860070423,
        { 276.83286304105934, 276.35947415936528 },
        { 276.83286304105934, 276.35947415936528 });

    // segment.p[0] is closest
    segment.p[0] = { 256.0, 170.0 };
    segment.p[1] = { 318.35968414625506, 155.60313252199265 };
    output = query(segment, box);
    Validate(output, 313.06259750626612, 0.0,
        { 256.0, 170.0 },
        { 247.15321248268239, 185.32308546376024 });

    // segment.p[1] is closest
    segment.p[0] = { 318.35968414625506, 155.60313252199265 };
    segment.p[1] = { 256.0, 170.0 };
    output = query(segment, box);
    Validate(output, 313.06259750626612, 1.0,
        { 256.0, 170.0 },
        { 247.15321248268239, 185.32308546376024 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistSegment2OrientedBox2.h>

namespace gtl
{
    template class DCPQuery<float, Segment<float, 2>, OrientedBox<float, 2>>;
    template class DCPQuery<double, Segment<double, 2>, OrientedBox<double, 2>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Segment<Rational, 2>, OrientedBox<Rational, 2>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistSegment2OrientedBox2)
