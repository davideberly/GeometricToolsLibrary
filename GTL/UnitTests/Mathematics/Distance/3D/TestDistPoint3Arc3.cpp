#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistPoint3Arc3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPoint3Arc3
    {
    public:
        UnitTestDistPoint3Arc3();

    private:
        using PAQuery = DCPQuery<double, Vector3<double>, Arc3<double>>;
        using PAOutput = typename PAQuery::Output;

        void Validate();

        void Test0();
        void Test1();
        void Test2();

        Vector3<double> mPoint;
        Arc3<double> mArc;
        PAQuery mQuery;
        PAOutput mOutput;
        PAOutput mExpectedOutput;
        Matrix3x3<double> mRot;
        Vector3<double> mTrn;
    };
}

UnitTestDistPoint3Arc3::UnitTestDistPoint3Arc3()
    :
    mPoint{},
    mArc{},
    mQuery{},
    mOutput{},
    mExpectedOutput{},
    mRot{},
    mTrn{}
{
    UTInformation("Mathematics/Distance/3D/DistPoint3Arc3");

    Quaternion<double> q{ 1.0, 2.0, 3.0, 4.0 };
    Normalize(q);
    RigidMotion<double>::Convert(q, mRot);
    mTrn = Vector3<double>{ 0.1234, 5.6789, -1.9735 };

    Test0();
    Test1();
    Test2();
}

void UnitTestDistPoint3Arc3::Validate()
{
    double constexpr maxError = 1e-16;
    double error{};
    double maxMaxError = 0.0;

    UTAssert(mOutput.equidistant == mExpectedOutput.equidistant, "invalid classification");

    error = std::fabs(mOutput.distance - mExpectedOutput.distance);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(
        error <= maxError,
        "invalid distance");

    error = Length(mOutput.closest[0] - mExpectedOutput.closest[0]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(
        error <= maxError,
        "invalid closest[0]");

    error = Length(mOutput.closest[1] - mExpectedOutput.closest[1]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(
        error <= maxError,
        "invalid closest[1]");
}

void UnitTestDistPoint3Arc3::Test0()
{
    // closest arc point is interior

    mPoint = { 2.0, 1.0, 1.0 };
    mArc.center = { 0.0, 0.0, 0.0 };
    mArc.normal = { 0.0, 0.0, 1.0 };
    mArc.radius = 1.0;
    mArc.end[0] = { 1.0, 0.0, 0.0 };
    mArc.end[1] = { 0.0, 1.0, 0.0 };
    mExpectedOutput.distance = 1.5899257985831985;
    mExpectedOutput.closest[0] = { 2.0, 1.0, 1.0 };
    mExpectedOutput.closest[1] = { 0.89442719099991586, 0.44721359549995793, 0.0 };
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mPoint, mArc);
    Validate();
    // Mathematica:
    // s = 0.4636476087734059
    // d = 1.589925798583198
    // P = (2.0, 1.0, 1.0)
    // K = (0.894427191101612, 0.447213595296565, 0.0)

    mPoint = mRot * mPoint + mTrn;
    mArc.center = mRot * mArc.center + mTrn;
    mArc.normal = mRot * mArc.normal;
    mArc.radius = 1.0;
    mArc.end[0] = mRot * mArc.end[0] + mTrn;
    mArc.end[1] = mRot * mArc.end[1] + mTrn;
    mExpectedOutput.distance = 1.5899257985831978;
    mExpectedOutput.closest[0] = { 0.45673333333333360, 8.0122333333333327, -1.3068333333333333 };
    mExpectedOutput.closest[1] = { -0.055485438199983181, 6.6627699100999074, -1.9735000000000003 };
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mPoint, mArc);
    Validate();
    // Mathematica:
    // s = 0.4636476092103999
    // d = 1.589925798583198
    // P = (0.4567333333333336, 8.01223333333333, -1.306833333333333)
    // K = (-0.05548543833745835,6.662769910074911,-1.973499999843778)
}

void UnitTestDistPoint3Arc3::Test1()
{
    // closest arc point is arc.end[0]

    mPoint = { -1.0, -2.0, 1.0 };
    mArc.center = { 0.0, 0.0, 0.0 };
    mArc.normal = { 0.0, 0.0, 1.0 };
    mArc.radius = 1.0;
    mArc.end[0] = { 1.0, 0.0, 0.0 };
    mArc.end[1] = { 0.0, 1.0, 0.0 };
    mExpectedOutput.distance = 3.0;
    mExpectedOutput.closest[0] = { -1.0, -2.0, 1.0 };
    mExpectedOutput.closest[1] = { 1.0, 0.0, 0.0 };
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mPoint, mArc);
    Validate();
    // Mathematica:
    // s = 0.0
    // d = 3.0
    // P = (-1.0, -2.0, 1.0)
    // K = (1.0, 0.0, 0.0)

    mPoint = mRot * mPoint + mTrn;
    mArc.center = mRot * mArc.center + mTrn;
    mArc.normal = mRot * mArc.normal;
    mArc.radius = 1.0;
    mArc.end[0] = mRot * mArc.end[0] + mTrn;
    mArc.end[1] = mRot * mArc.end[1] + mTrn;
    mExpectedOutput.distance = 3.0;
    mExpectedOutput.closest[0] = { 2.0567333333333329, 4.2122333333333328, -2.3068333333333331 };
    mExpectedOutput.closest[1] = { 0.25673333333333354, 6.6122333333333332, -2.3068333333333335 };
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mPoint, mArc);
    Validate();
    // Mathematica:
    // s = 0.0
    // d = 3.0
    // P = (2.056733333333333, 4.212233333333333, -2.306833333333333)
    // K = (0.2567333333333335, 6.612233333333333, -2.306833333333334)
}

void UnitTestDistPoint3Arc3::Test2()
{
    // closest arc point is arc.end[1]

    mPoint = { -2.0, 1.0, 1.0 };
    mArc.center = { 0.0, 0.0, 0.0 };
    mArc.normal = { 0.0, 0.0, 1.0 };
    mArc.radius = 1.0;
    mArc.end[0] = { 1.0, 0.0, 0.0 };
    mArc.end[1] = { 0.0, 1.0, 0.0 };
    mExpectedOutput.distance = 2.2360679774997898;
    mExpectedOutput.closest[0] = { -2.0, 1.0, 1.0 };
    mExpectedOutput.closest[1] = { 0.0, 1.0, 0.0 };
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mPoint, mArc);
    Validate();
    // Mathematica:
    // s = 1.57079633602725
    // d = 2.236067969242122
    // P = (-2.0, 1.0, 1.0)
    // K = (-9.23235326505047*10^(-9),1.,0.)

    mPoint = mRot * mPoint + mTrn;
    mArc.center = mRot * mArc.center + mTrn;
    mArc.normal = mRot * mArc.normal;
    mArc.radius = 1.0;
    mArc.end[0] = mRot * mArc.end[0] + mTrn;
    mArc.end[1] = mRot * mArc.end[1] + mTrn;
    mExpectedOutput.distance = 2.2360679774997889;
    mExpectedOutput.closest[0] = { -0.076600000000000515, 4.2789000000000001, 0.026499999999999746 };
    mExpectedOutput.closest[1] = { -0.54326666666666668, 6.0122333333333327, -1.3068333333333335 };
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mPoint, mArc);
    Validate();
    // Mathematica:
    // s = 1.57079633602725
    // d = 2.236067969242121
    // P = (-0.07660000000000052,4.2789,0.02649999999999975)
    // K = (-0.5432666678976471,6.01223332471647,-1.306833330255882)
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistPoint3Arc3.h>

namespace gtl
{
    template class DCPQuery<float, Vector3<float>, Circle3<float>>;
    template class DCPQuery<double, Vector3<double>, Circle3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector3<Rational>, Circle3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPoint3Arc3)
