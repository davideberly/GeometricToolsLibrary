#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistLine2AlignedBox2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLine2AlignedBox2
    {
    public:
        UnitTestDistLine2AlignedBox2();

    private:
        using LBQuery = DCPQuery<double, Line2<double>, AlignedBox2<double>>;
        using LBOutput = LBQuery::Output;

        void TestDoQuery2D();
        void TestDoQuery1D();
        void TestDoQuery0D();

        // The expected values are passed to this function.
        void Validate(LBOutput const& output,
            double sqrDistance,
            double parameter,
            Vector2<double> const& closest0,
            Vector2<double> const& closest1);
    };
}

UnitTestDistLine2AlignedBox2::UnitTestDistLine2AlignedBox2()
{
    UTInformation("Mathematics/Distance/2D/DistLine2AlignedBox2");

    TestDoQuery2D();
    TestDoQuery1D();
    TestDoQuery0D();
}

void UnitTestDistLine2AlignedBox2::Validate(LBOutput const& output,
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

void UnitTestDistLine2AlignedBox2::TestDoQuery2D()
{
    LBQuery query{};
    LBOutput output{};
    Line2<double> line{};
    AlignedBox2<double> box{};

    box.min = { 200.0, 200.0 };
    box.max = { 328.0, 264.0 };

    // closest box point is (-e0,e1)
    line.origin = { 256.0, 297.0 };
    line.direction = { 0.97814760073380569, 0.20791169081775931 };
    output = query(line, box);
    Validate(output, 425.83690769871941, -61.637351438079165,
        { 195.70957257525649, 284.18487404498052 },
        { 200.0, 264.0 });

    // closest box point is (e0,-e1)
    line.origin = { 280.0, 152.0 };
    line.direction = { 0.92718385456678731, 0.37460659341591218 };
    output = query(line, box);
    Validate(output, 703.50711446247010, 62.485941503169570,
        { 337.93595609914354, 175.40764568288830 },
        { 328.0, 200.0 });

    // DoQuery2D code block involving K2, closest on top edge of box
    line.origin = { 257.0, 256.0 };
    line.direction = { 0.98480775301220802, 0.17364817766693033 };
    output = query(line, box);
    Validate(output, 0.0, 46.070163865149070,
        { 302.37025455694169, 264.0 },
        { 302.37025455694169, 264.0 });

    // DoQuery2D code block involving K2, closest on right edge of box
    line.origin = { 256.0, 229.0 };
    line.direction = { 0.97814760073380569, 0.20791169081775931 };
    output = query(line, box);
    Validate(output, 0.0, 73.608522830282098,
        { 328.0, 244.30407244024158 },
        { 328.0, 244.30407244024158 });
}

void UnitTestDistLine2AlignedBox2::TestDoQuery1D()
{
    LBQuery query{};
    LBOutput output{};
    Line2<double> line{};
    AlignedBox2<double> box{};

    box.min = { 200.0, 200.0 };
    box.max = { 328.0, 264.0 };

    // direction signs (+,0), closest on top edge
    line.origin = { 256.0, 277.0 };
    line.direction = { 1.0, 0.0 };
    output = query(line, box);
    Validate(output, 169.0, 72.0,
        { 328.0, 277.0 },
        { 328.0, 264.0 });

    // direction signs (+,0), closest on bottom edge
    line.origin = { 256.0, 181.0 };
    line.direction = { 1.0, 0.0 };
    output = query(line, box);
    Validate(output, 361.0, 72.0,
        { 328.0, 181.0 },
        { 328.0, 200.0 });

    // direction signs (+,0), closest on horizonal edge interior
    line.origin = { 256.0, 255.0 };
    line.direction = { 1.0, 0.0 };
    output = query(line, box);
    Validate(output, 0.0, 72.0,
        { 328.0, 255.0 },
        { 328.0, 255.0 });

    // direction signs (0,+), closest on left edge
    line.origin = { 174.0, 256.0 };
    line.direction = { 0.0, 1.0 };
    output = query(line, box);
    Validate(output, 676.0, 8.0,
        { 174.0, 264.0 },
        { 200.0, 264.0 });

    // direction signs (0,+), closest on right edge
    line.origin = { 346.0, 256.0 };
    line.direction = { 0.0, 1.0 };
    output = query(line, box);
    Validate(output, 324.0, 8.0,
        { 346.0, 264.0 },
        { 328.0, 264.0 });

    // direction signs (0,+), closest on vertical edge interior
    line.origin = { 254.0, 256.0 };
    line.direction = { 0.0, 1.0 };
    output = query(line, box);
    Validate(output, 0.0, 8.0,
        { 254.0, 264.0 },
        { 254.0, 264.0 });
}

void UnitTestDistLine2AlignedBox2::TestDoQuery0D()
{
    LBQuery query{};
    LBOutput output{};
    Line2<double> line{};
    AlignedBox2<double> box{};

    box.min = { 200.0, 200.0 };
    box.max = { 328.0, 264.0 };

    // degenerate line
    line.origin = { 174.0, 256.0 };
    line.direction = { 0.0, 0.0 };
    output = query(line, box);
    Validate(output, 676.0, 0.0,
        { 174.0, 256.0 },
        { 200.0, 256.0 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistLine2AlignedBox2.h>

namespace gtl
{
    template class DCPQuery<float, Line<float, 2>, AlignedBox<float, 2>>;
    template class DCPQuery<double, Line<double, 2>, AlignedBox<double, 2>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line<Rational, 2>, AlignedBox<Rational, 2>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLine2AlignedBox2)
