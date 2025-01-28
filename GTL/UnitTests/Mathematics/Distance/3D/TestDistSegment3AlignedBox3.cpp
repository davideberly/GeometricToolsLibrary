#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistSegment3AlignedBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistSegment3AlignedBox3
    {
    public:
        UnitTestDistSegment3AlignedBox3();

    private:
        using SBQuery = DCPQuery<double, Segment3<double>, AlignedBox3<double>>;
        using SBOutput = SBQuery::Output;

        void Test();
    };
}

UnitTestDistSegment3AlignedBox3::UnitTestDistSegment3AlignedBox3()
{
    UTInformation("Mathematics/Distance/3D/DistSegment3AlignedBox3");

    Test();
}

void UnitTestDistSegment3AlignedBox3::Test()
{
    SBQuery query{};
    SBOutput output{};
    Segment3<double> segment{};
    AlignedBox3<double> box{};
    double const maxError = 1e-14;
    Vector3<double> direction{ -0.13375998748853216, -0.49589068532333880, 0.85802138315814536 };

    box.min = { 1.0, 2.0, 3.0 };
    box.max = { 3.0, 5.0, 7.0 };
    segment.p[0] = {-1.0, -1.0, -1.0};
    segment.p[1] = segment.p[0] + 10.0 * direction;
    output = query(segment, box);
    UTAssert(std::fabs(output.distance - 5.1174239793088221) <= maxError,
        "Invalid distance");
    UTAssert(0.0 < output.parameter && output.parameter < 1.0,
        "Invalid parameter.");

    // Verified with Mathematica.
    segment.p[0] += 6.0 * direction;
    segment.p[1] += 6.0 * direction;
    output = query(segment, box);
    UTAssert(std::fabs(output.distance - 6.5999302563683777) <= maxError,
        "Invalid distance");
    UTAssert(output.parameter == 0.0, "Invalid parameter.");

    segment.p[0] -= 15.0 * direction;
    segment.p[1] -= 15.0 * direction;
    output = query(segment, box);
    UTAssert(std::fabs(output.distance - 5.1619969969604780) <= maxError,
        "Invalid distance");
    UTAssert(output.parameter == 1.0, "Invalid parameter.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistSegment3AlignedBox3.h>

namespace gtl
{
    template class DCPQuery<float, Segment<float, 3>, AlignedBox<float, 3>>;
    template class DCPQuery<double, Segment3<double>, AlignedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Segment<Rational, 3>, AlignedBox<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistSegment3AlignedBox3)
