#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistSegment3Arc3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistSegment3Arc3
    {
    public:
        UnitTestDistSegment3Arc3();

    private:
        using SAQuery = DCPQuery<double, Segment3<double>, Arc3<double>>;
        using SAOutput = SAQuery::Output;

        void Validate();

        Segment3<double> mSegment;
        Arc3<double> mArc;
        SAQuery mQuery;
        SAOutput mOutput;
        SAOutput mExpectedOutput;
    };
}

UnitTestDistSegment3Arc3::UnitTestDistSegment3Arc3()
    :
    mSegment{},
    mArc{},
    mQuery{},
    mOutput{},
    mExpectedOutput{}
{
    // This has the same structural logic as DistRay3Arc3.h, so I believe
    // this code will work correctly. Add the unit tests later.
    UTInformation("Mathematics/Distance/3D/DistSegment3Arc3");
}

void UnitTestDistSegment3Arc3::Validate()
{
    double constexpr maxError = 1e-16;
    double error{};
    double maxMaxError = 0.0;

    UTAssert(mOutput.numClosestPairs == mExpectedOutput.numClosestPairs, "invalid number of pairs");
    UTAssert(mOutput.equidistant == mExpectedOutput.equidistant, "invalid classification");

    error = std::fabs(mOutput.distance - mExpectedOutput.distance);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "incorrect distance");

    for (std::size_t i = 0; i < 3; ++i)
    {
        error = Length(mOutput.linearClosest[i] - mExpectedOutput.linearClosest[i]);
        maxMaxError = std::max(maxMaxError, error);
        UTAssert(error <= maxError, "invalid line point " + std::to_string(i));

        error = Length(mOutput.circularClosest[i] - mExpectedOutput.circularClosest[i]);
        maxMaxError = std::max(maxMaxError, error);
        UTAssert(error <= maxError, "invalid arc point " + std::to_string(i));
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistSegment3Arc3.h>

namespace gtl
{
    template class DCPQuery<float, Ray3<float>, Circle3<float>>;
    template class DCPQuery<double, Ray3<double>, Circle3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Ray3<Rational>, Circle3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistSegment3Arc3)
