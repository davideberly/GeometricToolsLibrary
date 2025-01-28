#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistSegment2AlignedBox2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistSegment2AlignedBox2
    {
    public:
        UnitTestDistSegment2AlignedBox2();

    private:
        using SBQuery = DCPQuery<double, Segment2<double>, AlignedBox2<double>>;
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

UnitTestDistSegment2AlignedBox2::UnitTestDistSegment2AlignedBox2()
{
    UTInformation("Mathematics/Distance/2D/DistSegment2AlignedBox2");

    Test();
}

void UnitTestDistSegment2AlignedBox2::Validate(SBOutput const& output,
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

void UnitTestDistSegment2AlignedBox2::Test()
{
    SBQuery query{};
    SBOutput output{};
    Segment2<double> segment{};
    AlignedBox2<double> box{};

    box.min = { 200.0, 200.0 };
    box.max = { 328.0, 264.0 };

    // segment interior is closest
    segment.p[0] = { 193.0, 256.0 };
    segment.p[1] = { 254.52074854005241, 273.64079077228797 };
    output = query(segment, box);
    Validate(output, 0.0, 0.45349440981791189,
        { 220.89931555072724, 264.00000000000000 },
        { 220.89931555072724, 264.00000000000000 });

    // segment.p[0] is closest
    segment.p[0] = { 256.0, 291.0 };
    segment.p[1] = { 314.00369837034560, 318.04756875140475 };
    output = query(segment, box);
    Validate(output, 729.0, 0.0,
        { 256.0, 291.0 },
        { 256.0, 264.0 });

    // segment.p[1] is closest
    segment.p[0] = { 314.00369837034560, 318.04756875140475 };
    segment.p[1] = { 256.0, 291.0 };
    output = query(segment, box);
    Validate(output, 729.0, 1.0,
        { 256.0, 291.0 },
        { 256.0, 264.0 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistSegment2AlignedBox2.h>

namespace gtl
{
    template class DCPQuery<float, Segment<float, 2>, AlignedBox<float, 2>>;
    template class DCPQuery<double, Segment<double, 2>, AlignedBox<double, 2>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Segment<Rational, 2>, AlignedBox<Rational, 2>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistSegment2AlignedBox2)
