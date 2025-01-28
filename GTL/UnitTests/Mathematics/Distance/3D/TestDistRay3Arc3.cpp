#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistRay3Arc3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRay3Arc3
    {
    public:
        UnitTestDistRay3Arc3();

    private:
        using RAQuery = DCPQuery<double, Ray3<double>, Arc3<double>>;
        using RAOutput = RAQuery::Output;

        void Validate();

        void TestBoundaryTMinAMin();
        void TestBoundaryTMinAMax();
        void TestBoundaryTAllAMin();
        void TestBoundaryTAllAMax();
        void TestBoundaryTMinAAll();

        Ray3<double> mRay;
        Arc3<double> mArc;
        RAQuery mQuery;
        RAOutput mOutput;
        RAOutput mExpectedOutput;
    };
}

UnitTestDistRay3Arc3::UnitTestDistRay3Arc3()
    :
    mRay{},
    mArc{},
    mQuery{},
    mOutput{},
    mExpectedOutput{}
{
    UTInformation("Mathematics/Distance/3D/DistRay3Arc3");

    mArc.center = { 0.0, 0.0, 0.0 };
    mArc.normal = { 0.0, 0.0, 1.0 };
    mArc.radius = 1.0;
    mArc.end[0] = { 1.0, 0.0, 0.0 };
    mArc.end[1] = { 0.0, 1.0, 0.0 };

    TestBoundaryTMinAMin();
    TestBoundaryTMinAMax();
    TestBoundaryTAllAMin();
    TestBoundaryTAllAMax();
    TestBoundaryTMinAAll();
}

void UnitTestDistRay3Arc3::Validate()
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

void UnitTestDistRay3Arc3::TestBoundaryTMinAMin()
{
    // Minimum on boundaries (t,minAngle) and (0,minAngle).
    mRay.origin = { 1.0, -0.01, +0.01 };
    mRay.direction = { 3.0, -2.0, 1.0 };
    Normalize(mRay.direction);
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.0, -0.01, +0.01 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.014142135623730951;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mArc);
    Validate();
    // Mathematica:
    // s = 0.0
    // t = 0.0
    // d = 0.01414213562373095
    // P = (1.0, -0.01, 0.01)
    // K = (1.0, 0.0, 0.0)
}

void UnitTestDistRay3Arc3::TestBoundaryTMinAMax()
{
    // Minimum on boundaries (t,maxAngle) and (0,maxAngle).
    mRay.origin = { -0.01, 1.0, +0.01 };
    mRay.direction = { -2.0, 3.0, 1.0 };
    Normalize(mRay.direction);
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.01, 1.0, +0.01 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.0, 1.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.014142135623730951;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mArc);
    Validate();
    // Mathematica:
    // s = 1.570796335797374
    // t = -9.66666498511712*10^(-9)
    // d = 0.01414210875196112
    // P = (-0.00999998066667003, 0.999999971000005, 0.00999999033333501)
    // K = (-9.00247714690973*10^(-9), 1.0, 0.0)
}

void UnitTestDistRay3Arc3::TestBoundaryTAllAMin()
{
    // Minimum on boundary (t,minAngle).
    mRay.origin = { 0.9, 0.0, 0.0 };
    mRay.direction = { -1.0, 2.0, 0.5 };
    Normalize(mRay.direction);
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.90000000000000002, 0.0, 0.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.099999999999999978;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mArc);
    Validate();
    // Mathematica:
    // s = 0.0
    // t = -2.72990159230317 * 10 ^ (-6)
    // d = 0.0999972702567747
    // P = (0.900002729901592,-5.459803184606339 * 10 ^ (-6),-1.364950796151585 * 10 ^ (-6))
    // K = (1.,0.,0.)
}

void UnitTestDistRay3Arc3::TestBoundaryTAllAMax()
{
    // Minimum on boundary (t,maxAngle).
    mRay.origin = { 0.0, 0.9, 0.0 };
    mRay.direction = { 2.0, -1.0, 0.5 };
    Normalize(mRay.direction);
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.0, 0.90000000000000002, 0.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.0, 1.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.099999999999999978;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mArc);
    Validate();
    // Mathematica:
    // s = 1.570756133099527
    // t = -9.66689433480556 * 10 ^ (-9)
    // d = 0.0999999976107783
    // P = (-1.933378866961111 * 10 ^ (-8),0.900000009666894,-4.833447167402777 * 10 ^ (-9))
    // K = (0.00004019369535924436,0.999999999192234,0.)
}

void UnitTestDistRay3Arc3::TestBoundaryTMinAAll()
{
    // Minimum on boundary (0,angle).
    mRay.origin = { 1.0, 1.0, 0.0 };
    mRay.direction = { 1.0, 2.0, 3.0 };
    Normalize(mRay.direction);
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.0, 1.0, 0.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.70710678118654746, 0.70710678118654746, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.41421356237309515;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mArc);
    Validate();
    // Mathematica:
    // s = 0.7853981584731693
    // t = -9.81022356392639 * 10 ^ (-9)
    // d = 0.4142135415624693
    // P = (0.999999990189776,0.999999980379553,-2.943067069177918 * 10 ^ (-8))
    // K = (0.7071067846685386,0.7071067777045564,0.)
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistRay3Arc3.h>

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
GTL_TEST_FUNCTION(DistRay3Arc3)
