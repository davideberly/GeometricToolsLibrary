#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistSegment3CanonicalBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistSegment3CanonicalBox3
    {
    public:
        UnitTestDistSegment3CanonicalBox3();

    private:
        using SBQuery = DCPQuery<double, Segment3<double>, CanonicalBox3<double>>;
        using SBOutput = SBQuery::Output;

        void Test();
    };
}

UnitTestDistSegment3CanonicalBox3::UnitTestDistSegment3CanonicalBox3()
{
    UTInformation("Mathematics/Distance/3D/DistSegment3CanonicalBox3");

    Test();
}

void UnitTestDistSegment3CanonicalBox3::Test()
{
    SBQuery query{};
    SBOutput output{};
    Segment3<double> segment{};
    CanonicalBox3<double> box{};
    double const maxError = 1e-14;
    Vector3<double> direction{ 0.55793191403459019, 0.81406026771105011, 0.16130052645888099 };

    box.extent = { 1.5, 1.0, 0.5 };
    segment.p[0] = {-2.5, -2.0, -1.5};
    segment.p[1] = segment.p[0] + 10.0 * direction;
    output = query(segment, box);
    UTAssert(std::fabs(output.distance - 0.39783584635892300) <= maxError,
        "Invalid distance");
    UTAssert(0.0 < output.parameter && output.parameter < 1.0, "Invalid parameter.");

    // Verified with Mathematica.
    segment.p[0] += 6.0 * direction;
    segment.p[1] += 6.0 * direction;
    output = query(segment, box);
    UTAssert(std::fabs(output.distance - 1.8846366492660536) <= maxError,
        "Invalid distance");
    UTAssert(output.parameter == 0.0, "Invalid parameter.");

    segment.p[0] -= 15.0 * direction;
    segment.p[1] -= 15.0 * direction;
    output = query(segment, box);
    UTAssert(std::fabs(output.distance - 0.96613383316751633) <= maxError,
        "Invalid distance");
    UTAssert(output.parameter == 1.0, "Invalid parameter.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistSegment3CanonicalBox3.h>

namespace gtl
{
    template class DCPQuery<float, Segment3<float>, CanonicalBox3<float>>;
    template class DCPQuery<double, Segment3<double>, CanonicalBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Segment3<Rational>, CanonicalBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistSegment3CanonicalBox3)
