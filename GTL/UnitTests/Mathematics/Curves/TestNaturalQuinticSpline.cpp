#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/NaturalQuinticSpline.h>
using namespace gtl;

namespace gtl
{
    class UnitTestNaturalQuinticSpline
    {
    public:
        UnitTestNaturalQuinticSpline();

    private:
        void TestFree();
        void TestClamped();
        void TestClosed();
    };
}

UnitTestNaturalQuinticSpline::UnitTestNaturalQuinticSpline()
{
    UTInformation("Mathematics/Curves/NaturalQuinticSpline");

    TestFree();
    TestClamped();
    TestClosed();
}

void UnitTestNaturalQuinticSpline::TestFree()
{
    std::vector<double> t(5);
    std::vector<Vector<double, 1>> f0(5), f1(5);
    t[0] = 0.0;
    t[1] = 0.2;
    t[2] = 0.452;
    t[3] = 0.611;
    t[4] = 1.0;
    f0[0][0] = -0.72904599140643900;
    f0[1][0] = 0.67001717998915900;
    f0[2][0] = 0.93773554224846278;
    f0[3][0] = -0.55793191403459019;
    f0[4][0] = -0.38366589898599346;
    f1[0][0] = -0.77507096788763941;
    f1[1][0] = 0.27952671419630559;
    f1[2][0] = 0.75686129079768771;
    f1[3][0] = -0.0073253554103394070;
    f1[4][0] = -0.59585723032045212;

    NaturalQuinticSpline<double, 1> spline(true, f0, f1, t);

    // Verify the constraints.
    double const maxError = 1e-10;
    double const epsilon = 1e-16;
    std::array<Vector<double, 1>, 6> jet0{}, jet1{};
    double error{};

    // P[i,0](t[0]) = f0[0]
    spline.Evaluate(t[0], 5, jet0.data());
    error = Length(jet0[0] - f0[0]);
    UTAssert(error <= maxError, "Failed P[i,0](t[0]) = f0[0].");

    // P[1,0](t[1]) = f0[1]
    spline.Evaluate(t[1], 5, jet0.data());
    error = Length(jet0[0] - f0[1]);
    UTAssert(error <= maxError, "Failed P[1,0](t[1]) = f0[1].");

    // P[2,0](t[2]) = f0[2]
    spline.Evaluate(t[2], 5, jet0.data());
    error = Length(jet0[0] - f0[2]);
    UTAssert(error <= maxError, "Failed P[2,0](t[2]) = f0[2].");

    // P[3,0](t[3]) = f0[3]
    spline.Evaluate(t[3], 5, jet0.data());
    error = Length(jet0[0] - f0[3]);
    UTAssert(error <= maxError, "Failed P[3,0](t[3]) = f0[3].");

    // P[3,0](t[4]) = f0[4]
    spline.Evaluate(t[4], 5, jet0.data());
    error = Length(jet0[0] - f0[4]);
    UTAssert(error <= maxError, "Failed P[3,0](t[4]) = f0[4].");


    // P[i,1](t[0]) = f1[0]
    spline.Evaluate(t[0], 5, jet0.data());
    error = Length(jet0[1] - f1[0]);
    UTAssert(error <= maxError, "Failed P[i,1](t[0]) = f1[0].");

    // P[1,1](t[1]) = f1[1]
    spline.Evaluate(t[1], 5, jet0.data());
    error = Length(jet0[1] - f1[1]);
    UTAssert(error <= maxError, "Failed P[1,1](t[1]) = f1[1].");

    // P[2,1](t[2]) = f1[2]
    spline.Evaluate(t[2], 5, jet0.data());
    error = Length(jet0[1] - f1[2]);
    UTAssert(error <= maxError, "Failed P[2,1](t[2]) = f1[2].");

    // P[3,1](t[3]) = f1[3]
    spline.Evaluate(t[3], 5, jet0.data());
    error = Length(jet0[1] - f1[3]);
    UTAssert(error <= maxError, "Failed P[3,1](t[3]) = f1[3].");

    // P[3,1](t[4]) = f1[4]
    spline.Evaluate(t[4], 5, jet0.data());
    error = Length(jet0[1] - f1[4]);
    UTAssert(error <= maxError, "Failed P[3,1](t[4]) = f1[4].");

    // P[0,0](t[1]) = P[1,0](t[1])
    spline.Evaluate(t[1] - epsilon, 5, jet0.data());
    error = Length(jet0[0] - f0[1]);
    UTAssert(error <= maxError, "Failed P[0,0](t[1]) = P[1,0](t[1]).");

    // P[1,0](t[2]) = P[2,0](t[2])
    spline.Evaluate(t[2] - epsilon, 5, jet0.data());
    error = Length(jet0[0] - f0[2]);
    UTAssert(error <= maxError, "Failed P[1,0](t[2]) = P[2,0](t[2]).");

    // P[2,0](t[3]) = P[3,0](t[3])
    spline.Evaluate(t[3] - epsilon, 5, jet0.data());
    error = Length(jet0[0] - f0[3]);
    UTAssert(error <= maxError, "Failed P[2,0](t[3]) = P[3,0](t[3]).");


    // P[0,1](t[1]) = P[1,1](t[1])
    spline.Evaluate(t[1] - epsilon, 5, jet0.data());
    spline.Evaluate(t[1], 5, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[0,1](t[1]) = P[1,1](t[1]).");

    // P[1,1](t[2]) = P[2,1](t[2])
    spline.Evaluate(t[2] - epsilon, 5, jet0.data());
    spline.Evaluate(t[2], 5, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[1,1](t[2]) = P[2,1](t[2]).");

    // P[2,1](t[3]) = P[3,1](t[3])
    spline.Evaluate(t[3] - epsilon, 5, jet0.data());
    spline.Evaluate(t[3], 5, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[2,1](t[3]) = P[3,1](t[3]).");


    // P[0,2](t[1]) = P[1,2](t[2])
    spline.Evaluate(t[1] - epsilon, 5, jet0.data());
    spline.Evaluate(t[1], 5, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[0,2](t[1]) = P[1,2](t[2]).");

    // P[1,2](t[2]) = P[2,2](t[2])
    spline.Evaluate(t[2] - epsilon, 5, jet0.data());
    spline.Evaluate(t[2], 5, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[1,2](t[2]) = P[2,2](t[2]).");

    // P[2,2](t[3]) = P[3,2](t[3])
    spline.Evaluate(t[3] - epsilon, 5, jet0.data());
    spline.Evaluate(t[3], 5, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[2,2](t[3]) = P[3,2](t[3]).");


    // P[0,3](t[1]) = P[1,3](t[2])
    spline.Evaluate(t[1] - epsilon, 5, jet0.data());
    spline.Evaluate(t[1], 5, jet1.data());
    error = Length(jet0[3] - jet1[3]);
    UTAssert(error <= maxError, "Failed P[0,3](t[1]) = P[1,3](t[2]).");

    // P[1,3](t[2]) = P[2,3](t[2])
    spline.Evaluate(t[2] - epsilon, 5, jet0.data());
    spline.Evaluate(t[2], 5, jet1.data());
    error = Length(jet0[3] - jet1[3]);
    UTAssert(error <= maxError, "Failed P[1,3](t[2]) = P[2,3](t[2]).");

    // P[2,3](t[3]) = P[3,3](t[3])
    spline.Evaluate(t[3] - epsilon, 5, jet0.data());
    spline.Evaluate(t[3], 5, jet1.data());
    error = Length(jet0[3] - jet1[3]);
    UTAssert(error <= maxError, "Failed P[2,3](t[3]) = P[3,3](t[3]).");


    // P[0,3](t[0]) = 0
    spline.Evaluate(t[0], 5, jet0.data());
    error = Length(jet0[3]);
    UTAssert(error <= maxError, "Failed P[0,3](t[0]) = 0.");

    // P[3,3](t[4]) = 0
    spline.Evaluate(t[4], 5, jet0.data());
    error = Length(jet0[3]);
    UTAssert(error <= maxError, "Failed P[3,3](t[3]) = 0.");
}

void UnitTestNaturalQuinticSpline::TestClamped()
{
    std::vector<double> t(5);
    std::vector<Vector<double, 1>> f0(5), f1(5);
    t[0] = 0.0;
    t[1] = 0.2;
    t[2] = 0.452;
    t[3] = 0.611;
    t[4] = 1.0;
    f0[0][0] = -0.72904599140643900;
    f0[1][0] = 0.67001717998915900;
    f0[2][0] = 0.93773554224846278;
    f0[3][0] = -0.55793191403459019;
    f0[4][0] = -0.38366589898599346;
    f1[0][0] = -0.77507096788763941;
    f1[1][0] = 0.27952671419630559;
    f1[2][0] = 0.75686129079768771;
    f1[3][0] = -0.0073253554103394070;
    f1[4][0] = -0.59585723032045212;
    Vector<double, 1> derivative0{ -0.987 };
    Vector<double, 1> derivative1{ +0.654 };

    NaturalQuinticSpline<double, 1> spline(f0, f1, t, derivative0, derivative1);

    // Verify the constraints.
    double const maxError = 1e-10;
    double const epsilon = 1e-16;
    std::array<Vector<double, 1>, 6> jet0{}, jet1{};
    double error{};

    // P[i,0](t[0]) = f0[0]
    spline.Evaluate(t[0], 5, jet0.data());
    error = Length(jet0[0] - f0[0]);
    UTAssert(error <= maxError, "Failed P[i,0](t[0]) = f0[0].");

    // P[1,0](t[1]) = f0[1]
    spline.Evaluate(t[1], 5, jet0.data());
    error = Length(jet0[0] - f0[1]);
    UTAssert(error <= maxError, "Failed P[1,0](t[1]) = f0[1].");

    // P[2,0](t[2]) = f0[2]
    spline.Evaluate(t[2], 5, jet0.data());
    error = Length(jet0[0] - f0[2]);
    UTAssert(error <= maxError, "Failed P[2,0](t[2]) = f0[2].");

    // P[3,0](t[3]) = f0[3]
    spline.Evaluate(t[3], 5, jet0.data());
    error = Length(jet0[0] - f0[3]);
    UTAssert(error <= maxError, "Failed P[3,0](t[3]) = f0[3].");

    // P[3,0](t[4]) = f0[4]
    spline.Evaluate(t[4], 5, jet0.data());
    error = Length(jet0[0] - f0[4]);
    UTAssert(error <= maxError, "Failed P[3,0](t[4]) = f0[4].");


    // P[i,1](t[0]) = f1[0]
    spline.Evaluate(t[0], 5, jet0.data());
    error = Length(jet0[1] - f1[0]);
    UTAssert(error <= maxError, "Failed P[i,1](t[0]) = f1[0].");

    // P[1,1](t[1]) = f1[1]
    spline.Evaluate(t[1], 5, jet0.data());
    error = Length(jet0[1] - f1[1]);
    UTAssert(error <= maxError, "Failed P[1,1](t[1]) = f1[1].");

    // P[2,1](t[2]) = f1[2]
    spline.Evaluate(t[2], 5, jet0.data());
    error = Length(jet0[1] - f1[2]);
    UTAssert(error <= maxError, "Failed P[2,1](t[2]) = f1[2].");

    // P[3,1](t[3]) = f1[3]
    spline.Evaluate(t[3], 5, jet0.data());
    error = Length(jet0[1] - f1[3]);
    UTAssert(error <= maxError, "Failed P[3,1](t[3]) = f1[3].");

    // P[3,1](t[4]) = f1[4]
    spline.Evaluate(t[4], 5, jet0.data());
    error = Length(jet0[1] - f1[4]);
    UTAssert(error <= maxError, "Failed P[3,1](t[4]) = f1[4].");


    // P[0,0](t[1]) = P[1,0](t[1])
    spline.Evaluate(t[1] - epsilon, 5, jet0.data());
    error = Length(jet0[0] - f0[1]);
    UTAssert(error <= maxError, "Failed P[0,0](t[1]) = P[1,0](t[1]).");

    // P[1,0](t[2]) = P[2,0](t[2])
    spline.Evaluate(t[2] - epsilon, 5, jet0.data());
    error = Length(jet0[0] - f0[2]);
    UTAssert(error <= maxError, "Failed P[1,0](t[2]) = P[2,0](t[2]).");

    // P[2,0](t[3]) = P[3,0](t[3])
    spline.Evaluate(t[3] - epsilon, 5, jet0.data());
    error = Length(jet0[0] - f0[3]);
    UTAssert(error <= maxError, "Failed P[2,0](t[3]) = P[3,0](t[3]).");


    // P[0,1](t[1]) = P[1,1](t[1])
    spline.Evaluate(t[1] - epsilon, 5, jet0.data());
    spline.Evaluate(t[1], 5, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[0,1](t[1]) = P[1,1](t[1]).");

    // P[1,1](t[2]) = P[2,1](t[2])
    spline.Evaluate(t[2] - epsilon, 5, jet0.data());
    spline.Evaluate(t[2], 5, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[1,1](t[2]) = P[2,1](t[2]).");

    // P[2,1](t[3]) = P[3,1](t[3])
    spline.Evaluate(t[3] - epsilon, 5, jet0.data());
    spline.Evaluate(t[3], 5, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[2,1](t[3]) = P[3,1](t[3]).");


    // P[0,2](t[1]) = P[1,2](t[2])
    spline.Evaluate(t[1] - epsilon, 5, jet0.data());
    spline.Evaluate(t[1], 5, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[0,2](t[1]) = P[1,2](t[2]).");

    // P[1,2](t[2]) = P[2,2](t[2])
    spline.Evaluate(t[2] - epsilon, 5, jet0.data());
    spline.Evaluate(t[2], 5, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[1,2](t[2]) = P[2,2](t[2]).");

    // P[2,2](t[3]) = P[3,2](t[3])
    spline.Evaluate(t[3] - epsilon, 5, jet0.data());
    spline.Evaluate(t[3], 5, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[2,2](t[3]) = P[3,2](t[3]).");


    // P[0,3](t[1]) = P[1,3](t[2])
    spline.Evaluate(t[1] - epsilon, 5, jet0.data());
    spline.Evaluate(t[1], 5, jet1.data());
    error = Length(jet0[3] - jet1[3]);
    UTAssert(error <= maxError, "Failed P[0,3](t[1]) = P[1,3](t[2]).");

    // P[1,3](t[2]) = P[2,3](t[2])
    spline.Evaluate(t[2] - epsilon, 5, jet0.data());
    spline.Evaluate(t[2], 5, jet1.data());
    error = Length(jet0[3] - jet1[3]);
    UTAssert(error <= maxError, "Failed P[1,3](t[2]) = P[2,3](t[2]).");

    // P[2,3](t[3]) = P[3,3](t[3])
    spline.Evaluate(t[3] - epsilon, 5, jet0.data());
    spline.Evaluate(t[3], 5, jet1.data());
    error = Length(jet0[3] - jet1[3]);
    UTAssert(error <= maxError, "Failed P[2,3](t[3]) = P[3,3](t[3]).");


    // P[0,2](t[0]) = derivative0
    spline.Evaluate(t[0], 5, jet0.data());
    error = Length(jet0[2] - derivative0);
    UTAssert(error <= maxError, "Failed P[0,2](t[0]) = derivative0.");

    // P[3,2](t[4]) = derivative1
    spline.Evaluate(t[4], 5, jet0.data());
    error = Length(jet0[2] - derivative1);
    UTAssert(error <= maxError, "Failed P[3,2](t[3]) = derivative1.");
}

void UnitTestNaturalQuinticSpline::TestClosed()
{
    std::vector<double> t(5);
    std::vector<Vector<double, 1>> f0(5), f1(5);
    t[0] = 0.0;
    t[1] = 0.2;
    t[2] = 0.452;
    t[3] = 0.611;
    t[4] = 1.0;
    f0[0][0] = -0.72904599140643900;
    f0[1][0] = 0.67001717998915900;
    f0[2][0] = 0.93773554224846278;
    f0[3][0] = -0.55793191403459019;
    f0[4][0] = f0[0][0];
    f1[0][0] = -0.77507096788763941;
    f1[1][0] = 0.27952671419630559;
    f1[2][0] = 0.75686129079768771;
    f1[3][0] = -0.0073253554103394070;
    f1[4][0] = f1[0][0];

    NaturalQuinticSpline<double, 1> spline(false, f0, f1, t);

    // Verify the constraints.
    double const maxError = 1e-8;
    double const epsilon = 1e-16;
    std::array<Vector<double, 1>, 6> jet0{}, jet1{};
    double error{};

    // P[i,0](t[0]) = f0[0]
    spline.Evaluate(t[0], 5, jet0.data());
    error = Length(jet0[0] - f0[0]);
    UTAssert(error <= maxError, "Failed P[i,0](t[0]) = f0[0].");

    // P[1,0](t[1]) = f0[1]
    spline.Evaluate(t[1], 5, jet0.data());
    error = Length(jet0[0] - f0[1]);
    UTAssert(error <= maxError, "Failed P[1,0](t[1]) = f0[1].");

    // P[2,0](t[2]) = f0[2]
    spline.Evaluate(t[2], 5, jet0.data());
    error = Length(jet0[0] - f0[2]);
    UTAssert(error <= maxError, "Failed P[2,0](t[2]) = f0[2].");

    // P[3,0](t[3]) = f0[3]
    spline.Evaluate(t[3], 5, jet0.data());
    error = Length(jet0[0] - f0[3]);
    UTAssert(error <= maxError, "Failed P[3,0](t[3]) = f0[3].");

    // P[3,0](t[4]) = f0[4]
    spline.Evaluate(t[4], 5, jet0.data());
    error = Length(jet0[0] - f0[4]);
    UTAssert(error <= maxError, "Failed P[3,0](t[4]) = f0[4].");


    // P[i,1](t[0]) = f1[0]
    spline.Evaluate(t[0], 5, jet0.data());
    error = Length(jet0[1] - f1[0]);
    UTAssert(error <= maxError, "Failed P[i,1](t[0]) = f1[0].");

    // P[1,1](t[1]) = f1[1]
    spline.Evaluate(t[1], 5, jet0.data());
    error = Length(jet0[1] - f1[1]);
    UTAssert(error <= maxError, "Failed P[1,1](t[1]) = f1[1].");

    // P[2,1](t[2]) = f1[2]
    spline.Evaluate(t[2], 5, jet0.data());
    error = Length(jet0[1] - f1[2]);
    UTAssert(error <= maxError, "Failed P[2,1](t[2]) = f1[2].");

    // P[3,1](t[3]) = f1[3]
    spline.Evaluate(t[3], 5, jet0.data());
    error = Length(jet0[1] - f1[3]);
    UTAssert(error <= maxError, "Failed P[3,1](t[3]) = f1[3].");

    // P[3,1](t[4]) = f1[4]
    spline.Evaluate(t[4], 5, jet0.data());
    error = Length(jet0[1] - f1[4]);
    UTAssert(error <= maxError, "Failed P[3,1](t[4]) = f1[4].");


    // P[0,0](t[1]) = P[1,0](t[1])
    spline.Evaluate(t[1] - epsilon, 5, jet0.data());
    error = Length(jet0[0] - f0[1]);
    UTAssert(error <= maxError, "Failed P[0,0](t[1]) = P[1,0](t[1]).");

    // P[1,0](t[2]) = P[2,0](t[2])
    spline.Evaluate(t[2] - epsilon, 5, jet0.data());
    error = Length(jet0[0] - f0[2]);
    UTAssert(error <= maxError, "Failed P[1,0](t[2]) = P[2,0](t[2]).");

    // P[2,0](t[3]) = P[3,0](t[3])
    spline.Evaluate(t[3] - epsilon, 5, jet0.data());
    error = Length(jet0[0] - f0[3]);
    UTAssert(error <= maxError, "Failed P[2,0](t[3]) = P[3,0](t[3]).");


    // P[0,1](t[1]) = P[1,1](t[1])
    spline.Evaluate(t[1] - epsilon, 5, jet0.data());
    spline.Evaluate(t[1], 5, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[0,1](t[1]) = P[1,1](t[1]).");

    // P[1,1](t[2]) = P[2,1](t[2])
    spline.Evaluate(t[2] - epsilon, 5, jet0.data());
    spline.Evaluate(t[2], 5, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[1,1](t[2]) = P[2,1](t[2]).");

    // P[2,1](t[3]) = P[3,1](t[3])
    spline.Evaluate(t[3] - epsilon, 5, jet0.data());
    spline.Evaluate(t[3], 5, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[2,1](t[3]) = P[3,1](t[3]).");


    // P[0,2](t[1]) = P[1,2](t[2])
    spline.Evaluate(t[1] - epsilon, 5, jet0.data());
    spline.Evaluate(t[1], 5, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[0,2](t[1]) = P[1,2](t[2]).");

    // P[1,2](t[2]) = P[2,2](t[2])
    spline.Evaluate(t[2] - epsilon, 5, jet0.data());
    spline.Evaluate(t[2], 5, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[1,2](t[2]) = P[2,2](t[2]).");

    // P[2,2](t[3]) = P[3,2](t[3])
    spline.Evaluate(t[3] - epsilon, 5, jet0.data());
    spline.Evaluate(t[3], 5, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[2,2](t[3]) = P[3,2](t[3]).");


    // P[0,3](t[1]) = P[1,3](t[2])
    spline.Evaluate(t[1] - epsilon, 5, jet0.data());
    spline.Evaluate(t[1], 5, jet1.data());
    error = Length(jet0[3] - jet1[3]);
    UTAssert(error <= maxError, "Failed P[0,3](t[1]) = P[1,3](t[2]).");

    // P[1,3](t[2]) = P[2,3](t[2])
    spline.Evaluate(t[2] - epsilon, 5, jet0.data());
    spline.Evaluate(t[2], 5, jet1.data());
    error = Length(jet0[3] - jet1[3]);
    UTAssert(error <= maxError, "Failed P[1,3](t[2]) = P[2,3](t[2]).");

    // P[2,3](t[3]) = P[3,3](t[3])
    spline.Evaluate(t[3] - epsilon, 5, jet0.data());
    spline.Evaluate(t[3], 5, jet1.data());
    error = Length(jet0[3] - jet1[3]);
    UTAssert(error <= maxError, "Failed P[2,3](t[3]) = P[3,3](t[3]).");


    // P[0,0](t[0]) = P[3,0](t[4])
    spline.Evaluate(t[0], 5, jet0.data());
    spline.Evaluate(t[4], 5, jet1.data());
    error = Length(jet0[0] - jet1[0]);
    UTAssert(error <= maxError, "Failed P[0,0](t[0]) = P[3,0](t[4]).");

    // P[0,1](t[0]) = P[3,1](t[4])
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[0,1](t[0]) = P[3,1](t[4]).");

    // P[0,2](t[0]) = P[3,2](t[4])
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[0,2](t[0]) = P[3,2](t[4]).");

    // P[0,3](t[0]) = P[3,3](t[4])
    error = Length(jet0[3] - jet1[3]);
    UTAssert(error <= maxError, "Failed P[0,3](t[0]) = P[3,3](t[4]).");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/NaturalQuinticSpline.h>

namespace gtl
{
    template class NaturalQuinticSpline<float, 2>;
    template class NaturalQuinticSpline<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class NaturalQuinticSpline<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(NaturalQuinticSpline)
