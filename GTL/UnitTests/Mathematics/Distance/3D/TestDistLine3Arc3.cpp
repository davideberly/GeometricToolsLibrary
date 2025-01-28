#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistLine3Arc3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLine3Arc3
    {
    public:
        UnitTestDistLine3Arc3();

    private:
        using LAQuery = DCPQuery<double, Line3<double>, Arc3<double>>;
        using LAOutput = LAQuery::Output;

        void Validate();

        void TestLinePerpContainsCenter();
        void TestC();
        void TestA();
        void TestCC();
        void TestCA();
        void TestAA();
        void TestCAA();

        Line3<double> mLine;
        Arc3<double> mArc;
        LAQuery mQuery;
        LAOutput mOutput;
        LAOutput mExpectedOutput;
    };
}

UnitTestDistLine3Arc3::UnitTestDistLine3Arc3()
    :
    mLine{},
    mArc{},
    mQuery{},
    mOutput{},
    mExpectedOutput{}
{
    UTInformation("Mathematics/Distance/3D/DistLine3Arc3");

    // The Mathematica output varies with each run (RandomSearch).
    //TestLinePerpContainsCenter();
    TestC();
    TestA();
    TestCC();
    TestCA();
    TestAA();
    TestCAA();
}

void UnitTestDistLine3Arc3::Validate()
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

void UnitTestDistLine3Arc3::TestLinePerpContainsCenter()
{
    mLine.origin = { 0.0, 0.0, 1.25 };
    mLine.direction = { 0.0, 0.0, 1.0 };
    mArc.center = { 0.0, 0.0, 0.0 };
    mArc.normal = { 0.0, 0.0, 1.0 };
    mArc.radius = 1.0;
    mArc.end[0] = { std::cos(0.1), std::sin(0.1), 0.0 };
    mArc.end[1] = { std::cos(0.2), std::sin(0.2), 0.0 };
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99500416527802582, 0.099833416646828155, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 1.0;
    mExpectedOutput.equidistant = true;
    mOutput = mQuery(mLine, mArc);
    Validate();
    // Mathematica:
    // s = 0.1255003659067231
    // t = -1.249999999999746
    // d = 1.0
    // P = (0.,0.,2.540190280342358 * 10 ^ (-13))
    // K = (0.992135160039618,0.1251711796427639, 0.0)
}

void UnitTestDistLine3Arc3::TestC()
{
    mLine.origin = { 1.125, 0.0, 0.0 };
    mLine.direction = { 0.0, 1.0, 0.0 };
    mArc.center = { 0.0, 0.0, 0.0 };
    mArc.normal = { 0.0, 0.0, 1.0 };
    mArc.radius = 1.0;
    mArc.end[0] = { std::cos(-0.1), std::sin(-0.1), 0.0 };
    mArc.end[1] = { std::cos(0.2), std::sin(0.2), 0.0 };
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.125, 0.0, 0.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.125;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mLine, mArc);
    Validate();
    // Mathematica:
    // s = -9.67787456553095 * 10 ^ (-10)
    // t = -9.67817632023914 * 10 ^ (-10)
    // d = 0.125
    // P = (1.125, -9.67817632023914 * 10 ^ (-10), 0.0)
    // K = (1.0, -9.67787456553095 * 10 ^ (-10), 0.0)

    mLine.origin = { 1.125, 0.0, 0.0 };
    mLine.direction = { 0.0, 1.0, 0.0 };
    mArc.center = { 0.0, 0.0, 0.0 };
    mArc.normal = { 0.0, 0.0, 1.0 };
    mArc.radius = 1.0;
    mArc.end[0] = { std::cos(-0.1), std::sin(-0.1), 0.0 };
    mArc.end[1] = { std::cos(C_PI<double>), std::sin(C_PI<double>), 0.0 };
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.125, 0.0, 0.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.125;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mLine, mArc);
    Validate();
    // Mathematica:
    // s = 0.0
    // t = 0.0
    // d = 0.125
    // P = (1.125, 0.0, 0.0)
    // K = (1.0, 0.0, 0.0)

    mLine.origin = { 0.75, 0.01, 0.0 };
    mLine.direction = { 0.0, 0.99, 0.01 };
    mArc.center = { 0.0, 0.0, 0.0 };
    mArc.normal = { 0.0, 0.0, 1.0 };
    mArc.radius = 1.0;
    mArc.end[0] = { std::cos(-C_PI_DIV_2<double>), std::sin(-C_PI_DIV_2<double>), 0.0 };
    mArc.end[1] = { std::cos(C_PI_DIV_2<double>), std::sin(C_PI_DIV_2<double>), 0.0 };
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.75000000000000000, 0.66128591040278217, 0.0065786455596240625 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.75007536562196842, 0.66135236136803077, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.0065794128238710293;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mLine, mArc);
    Validate();
    // Mathematica:
    // s = 0.7226203031931236
    // t = 0.6513191394973186
    // d = 0.006579412823871865
    // P = (0.75, 0.6612859148619259, 0.006578645604665918)
    // K = (0.7500753624132023, 0.6613523650072654, 0.0)
}

void UnitTestDistLine3Arc3::TestA()
{
    mLine.origin = { 1.125, 0.0, 0.0 };
    mLine.direction = { 0.0, 1.0, 0.0 };
    mArc.center = { 0.0, 0.0, 0.0 };
    mArc.normal = { 0.0, 0.0, 1.0 };
    mArc.radius = 1.0;
    mArc.end[0] = { std::cos(0.2), std::sin(0.2), 0.0 };
    mArc.end[1] = { std::cos(1.5), std::sin(1.5), 0.0 };
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.125, 0.19866933079506122, 0.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.98006657784124163, 0.19866933079506122, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.14493342215875837;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mLine, mArc);
    Validate();
    // Mathematica:
    // s = 0.2000000717613693
    // t = 0.1986694024157708
    // d = 0.1449334364155441
    // P = (1.125, 0.1986694024157708, 0.0)
    // K = (0.980066563584456,0.1986694011259804, 0.0)
}

void UnitTestDistLine3Arc3::TestCC()
{
    mLine.origin = { 0.75, 0.0, 0.125 };
    mLine.direction = { 0.0, 1.0, 0.0 };
    mArc.center = { 0.0, 0.0, 0.0 };
    mArc.normal = { 0.0, 0.0, 1.0 };
    mArc.radius = 1.0;
    mArc.end[0] = { std::cos(-C_PI_DIV_2<double>), std::sin(-C_PI_DIV_2<double>), 0.0 };
    mArc.end[1] = { std::cos(C_PI_DIV_2<double>), std::sin(C_PI_DIV_2<double>), 0.0 };
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { 0.75000000000000000, -0.66143782776614768, 0.12500000000000000 };
    mExpectedOutput.linearClosest[1] = { 0.75000000000000000, 0.66143782776614768, 0.12500000000000000 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.75000000000000000, -0.66143782776614768, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.75000000000000000, 0.66143782776614768, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.12500000000000000;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mLine, mArc);
    Validate();
    // Mathematica:
    // s = 0.7227342547133285
    // t = 0.6614378341080055
    // d = 0.1250000000000001
    // P = (0.75, 0.6614378341080055, 0.125)
    // K = (0.7499999954361366, 0.6614378329410822, 0.0)
}

void UnitTestDistLine3Arc3::TestCA()
{
    mLine.origin = { 0.5, 0.0, 0.0 };
    mLine.direction = { 0.8, 0.0, 0.6 };
    mArc.center = { 0.0, 0.0, 0.0 };
    mArc.normal = { 0.0, 0.0, 1.0 };
    mArc.radius = 1.0;
    mArc.end[0] = { 7.0 / 16.0, 3.0 * std::sqrt(23.0) / 16.0, 0.0};
    mArc.end[1] = { std::cos(3.2), std::sin(3.2), 0.0 };
#if 0
    // Theoretical distance is 0.9, numerical closest[0] distance is 0.89999999999999991,
    // numerical closest[1] distance is 0.90000000000000002. Theoretically there are
    // two pair of closest points. I set a breakpoint in DistLine3Arc3.h and modified
    // candidates[1].distance to be the same as candidates[0].distancde.
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.46000000000000019, 0.0, -0.72000000000000008 };  // (-0.46, 0.0, -0.72)
    mExpectedOutput.linearClosest[1] = { 0.45999999999999996, 0.0, -0.029999999999999999 };  // (-0.46, 0.0, -0.03)
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.43750000000000000, 0.89921841062113483, 0.0 }; // (0.4375, sqrt(1-0.4375^2))
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.89999999999999991;  // 0.9
    mExpectedOutput.equidistant = false;
#else
    // With rounding errors:
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.46000000000000019, 0.0, -0.72000000000000008 };  // (-0.46, 0.0, -0.72)
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.89999999999999991;  // 0.9
    mExpectedOutput.equidistant = false;
#endif
    mOutput = mQuery(mLine, mArc);
    Validate();
    // Mathematica:
    // s = 1.117979722073656
    // t = -0.04999999285916362
    // d = 0.89999999541488
    // P = (0.4600000057126691, 0.0, -0.02999999571549817)
    // K = (0.4375000089708861, 0.899218406256497, 0.0)
}

void UnitTestDistLine3Arc3::TestAA()
{
    mLine.origin = { 0.5, 0.0, 0.0 };
    mLine.direction = { 0.8, 0.0, 0.6 };
    mArc.center = { 0.0, 0.0, 0.0 };
    mArc.normal = { 0.0, 0.0, 1.0 };
    mArc.radius = 1.0;
    mArc.end[0] = { std::cos(0.1), std::sin(0.1), 0.0 };
    mArc.end[1] = { std::cos(-0.1), std::sin(-0.1), 0.0 };
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { 0.81680266577793659, 0.0, 0.23760199933345238 };
    mExpectedOutput.linearClosest[1] = { 0.81680266577793659, 0.0, 0.23760199933345238 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99500416527802582, 0.099833416646828155, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.99500416527802582, -0.099833416646828155, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.31333240431004483;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mLine, mArc);
    Validate();
    // Mathematica. The solver uses angles for the arc endpoints. The run
    // uses angles 0.1 and 2.0*Pi-0.1
    // s = 0.0999999901422721
    // t = 0.3960033330104003
    // d = 0.3133324017445887
    // P = (0.81680266640832, 0.0, 0.2376019998062401)
    // K = (0.995004166262156, 0.0998334068383479, 0.0)
}

void UnitTestDistLine3Arc3::TestCAA()
{
    mLine.origin = { 0.5, 0.0, 0.0 };
    mLine.direction = { 0.8, 0.0, 0.6 };
    mArc.center = { 0.0, 0.0, 0.0 };
    mArc.normal = { 0.0, 0.0, 1.0 };
    mArc.radius = 1.0;
    mArc.end[0] = { 7.0 / 16.0, 3.0 * std::sqrt(23.0) / 16.0, 0.0 };
    mArc.end[1] = { 7.0 / 16.0, -3.0 * std::sqrt(23.0) / 16.0, 0.0 };
#if 0
    // Theoretical distance is 0.9, numerical closest[0] distance is 0.89999999999999991,
    // numerical closest[1,2] distance is 0.90000000000000002. Theoretically there are
    // two pair of closest points. I set a breakpoint in DistLine3Arc3.h and modified
    // candidates[0].distance to be the same as candidates[1,2].distancde.
    mExpectedOutput.numClosestPairs = 3;
    mExpectedOutput.linearClosest[0] = { -0.46000000000000019, 0.0, -0.72000000000000008 };  // (-0.46, 0.0, -0.72)
    mExpectedOutput.linearClosest[1] = { 0.45999999999999996, 0.0, -0.029999999999999999 };  // (-0.46, 0.0, -0.03)
    mExpectedOutput.linearClosest[2] = { 0.45999999999999996, 0.0, -0.029999999999999999 };  // (-0.46, 0.0, -0.03)
    mExpectedOutput.circularClosest[0] = { -1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.43750000000000000, 0.89921841062113483, 0.0 }; // (0.4375, sqrt(1-0.4375^2), 0.0)
    mExpectedOutput.circularClosest[2] = { 0.43750000000000000, -0.89921841062113483, 0.0 }; // (0.4375, -sqrt(1-0.4375^2), 0.0)
    mExpectedOutput.distance = 0.90000000000000002;  // 0.9
    mExpectedOutput.equidistant = false;
#else
    // With rounding errors:
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.46000000000000019, 0.0, -0.72000000000000008 };  // (-0.46, 0.0, -0.72)
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[2] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.89999999999999991;  // 0.9
    mExpectedOutput.equidistant = false;
#endif
    mOutput = mQuery(mLine, mArc);
    Validate();
    // Mathematica:
    // s = 1.117979722073009
    // t = -0.04999999290765122
    // d = 0.899999995414583
    // P = (0.460000005673879, 0.0, -0.02999999574459073)
    // K = (0.4375000089714677, 0.899218406256214, 0.0)
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistLine3Arc3.h>

namespace gtl
{
    template class DCPQuery<float, Line3<float>, Arc3<float>>;
    template class DCPQuery<double, Line3<double>, Arc3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Arc3<Rational>, Circle3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLine3Arc3)
