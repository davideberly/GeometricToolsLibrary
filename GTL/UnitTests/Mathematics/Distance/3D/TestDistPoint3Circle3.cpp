#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistPoint3Circle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPoint3Circle3
    {
    public:
        UnitTestDistPoint3Circle3();

    private:
        using PCQuery = DCPQuery<double, Vector3<double>, Circle3<double>>;
        using PCOutput = typename PCQuery::Output;

        void Validate();

        void Test();

        Vector3<double> mPoint;
        Circle3<double> mCircle;
        PCQuery mQuery;
        PCOutput mOutput;
        PCOutput mExpectedOutput;
        Matrix3x3<double> mRot;
        Vector3<double> mTrn;
    };
}

UnitTestDistPoint3Circle3::UnitTestDistPoint3Circle3()
    :
    mPoint{},
    mCircle{},
    mQuery{},
    mOutput{},
    mExpectedOutput{},
    mRot{},
    mTrn{}
{
    UTInformation("Mathematics/Distance/3D/DistPoint3Circle3");

    Quaternion<double> q{ 1.0, 2.0, 3.0, 4.0 };
    Normalize(q);
    RigidMotion<double>::Convert(q, mRot);
    mTrn = Vector3<double>{ 0.1234, 5.6789, -1.9735 };

    Test();
}

void UnitTestDistPoint3Circle3::Validate()
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

void UnitTestDistPoint3Circle3::Test()
{
    mPoint = { 2.0, 0.0, 1.0 };
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.distance = 1.4142135623730951;
    mExpectedOutput.closest[0] = { 2.0, 0.0, 1.0 };
    mExpectedOutput.closest[1] = { 1.0, 0.0, 0.0 };
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mPoint, mCircle);
    Validate();

    mPoint = mRot * mPoint + mTrn;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mCircle.radius = 1.0;
    mExpectedOutput.distance = 1.4142135623730954;
    mExpectedOutput.closest[0] = { 1.1234000000000002, 7.6788999999999996, -1.9734999999999998 };
    mExpectedOutput.closest[1] = { 0.25673333333333354, 6.6122333333333332, -2.3068333333333335 };
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mPoint, mCircle);
    Validate();

    mPoint = { 0.0, 0.5, 1.0 };
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.distance = 1.1180339887498949;
    mExpectedOutput.closest[0] = { 0.0, 0.5, 1.0 };
    mExpectedOutput.closest[1] = { 0.0, 1.0, 0.0 };
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mPoint, mCircle);
    Validate();

    mPoint = mRot * mPoint + mTrn;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mCircle.radius = 1.0;
    mExpectedOutput.distance = 1.1180339887498949;
    mExpectedOutput.closest[0] = { 0.52339999999999987, 5.9788999999999994, -0.97350000000000003 };
    mExpectedOutput.closest[1] = { -0.54326666666666701, 6.0122333333333327, -1.3068333333333335 };
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mPoint, mCircle);
    Validate();

    mPoint = { 0.0, 0.0, 1.0 };
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.distance = 1.4142135623730951;
    mExpectedOutput.closest[0] = { 0.0, 0.0, 1.0 };
    mExpectedOutput.closest[1] = { -1.0, 0.0, 0.0 };
    mExpectedOutput.equidistant = true;
    mOutput = mQuery(mPoint, mCircle);
    Validate();

    // Rounding errors in the rigid motion cause the rotated/translated P not
    // to be equidistant to all circle points.
    mPoint = mRot * mPoint + mTrn;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mCircle.radius = 1.0;
    mExpectedOutput.distance = 1.4142135623730949;
    mExpectedOutput.closest[0] = { 0.85673333333333312, 5.8122333333333334, -1.3068333333333333 };
    mExpectedOutput.closest[1] = { 0.024741323547207497, 6.6699712498212334, -2.0631897058661752 };
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mPoint, mCircle);
    Validate();
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistPoint3Circle3.h>

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
GTL_TEST_FUNCTION(DistPoint3Circle3)
