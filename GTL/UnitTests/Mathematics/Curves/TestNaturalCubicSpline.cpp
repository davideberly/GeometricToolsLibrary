#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/NaturalCubicSpline.h>
using namespace gtl;

namespace gtl
{
    class UnitTestNaturalCubicSpline
    {
    public:
        UnitTestNaturalCubicSpline();

    private:
        void TestFree();
        void TestClamped();
        void TestClosed();
    };
}

UnitTestNaturalCubicSpline::UnitTestNaturalCubicSpline()
{
    UTInformation("Mathematics/Curves/NaturalCubicSpline");

    TestFree();
    TestClamped();
    TestClosed();
}

void UnitTestNaturalCubicSpline::TestFree()
{
    std::vector<double> t(5);
    std::vector<Vector<double, 1>> f(5);
    t[0] = 0.0;
    t[1] = 0.2;
    t[2] = 0.452;
    t[3] = 0.611;
    t[4] = 1.0;
    f[0][0] = -0.72904599140643900;
    f[1][0] = 0.67001717998915900;
    f[2][0] = 0.93773554224846278;
    f[3][0] = -0.55793191403459019;
    f[4][0] = -0.38366589898599346;

    NaturalCubicSpline<double, 1> spline(true, 5, f.data(), t.data());

    // Verify the constraints.
    double const maxError = 1e-11;
    double const epsilon = 1e-16;
    std::array<Vector<double, 1>, 4> jet0{}, jet1{};
    double error{};

    // P[i,0](t[0]) = f[0]
    spline.Evaluate(t[0], 3, jet0.data());
    error = Length(jet0[0] - f[0]);
    UTAssert(error <= maxError, "Failed P[i,0](t[0]) = f[0].");

    // P[1,0](t[1]) = f[1]
    spline.Evaluate(t[1], 3, jet0.data());
    error = Length(jet0[0] - f[1]);
    UTAssert(error <= maxError, "Failed P[1,0](t[1]) = f[1].");

    // P[2,0](t[2]) = f[2]
    spline.Evaluate(t[2], 3, jet0.data());
    error = Length(jet0[0] - f[2]);
    UTAssert(error <= maxError, "Failed P[2,0](t[2]) = f[2].");

    // P[3,0](t[3]) = f[3]
    spline.Evaluate(t[3], 3, jet0.data());
    error = Length(jet0[0] - f[3]);
    UTAssert(error <= maxError, "Failed P[3,0](t[3]) = f[3].");

    // P[3,0](t[4]) = f[4]
    spline.Evaluate(t[4], 3, jet0.data());
    error = Length(jet0[0] - f[4]);
    UTAssert(error <= maxError, "Failed P[3,0](t[4]) = f[4].");

    // P[0,0](t[1]) = P[1,0](t[1])
    spline.Evaluate(t[1] - epsilon, 3, jet0.data());
    error = Length(jet0[0] - f[1]);
    UTAssert(error <= maxError, "Failed P[0,0](t[1]) = P[1,0](t[1]).");

    // P[1,0](t[2]) = P[2,0](t[2])
    spline.Evaluate(t[2] - epsilon, 3, jet0.data());
    error = Length(jet0[0] - f[2]);
    UTAssert(error <= maxError, "Failed P[1,0](t[2]) = P[2,0](t[2]).");

    // P[2,0](t[3]) = P[3,0](t[3])
    spline.Evaluate(t[3] - epsilon, 3, jet0.data());
    error = Length(jet0[0] - f[3]);
    UTAssert(error <= maxError, "Failed P[2,0](t[3]) = P[3,0](t[3]).");

    // P[0,1](t[1]) = P[1,1](t[1])
    spline.Evaluate(t[1] - epsilon, 3, jet0.data());
    spline.Evaluate(t[1], 3, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[0,1](t[1]) = P[1,1](t[1]).");

    // P[1,1](t[2]) = P[2,1](t[2])
    spline.Evaluate(t[2] - epsilon, 3, jet0.data());
    spline.Evaluate(t[2], 3, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[1,1](t[2]) = P[2,1](t[2]).");

    // P[2,1](t[3]) = P[3,1](t[3])
    spline.Evaluate(t[3] - epsilon, 3, jet0.data());
    spline.Evaluate(t[3], 3, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[2,1](t[3]) = P[3,1](t[3]).");

    // P[0,2](t[1]) = P[1,2](t[2])
    spline.Evaluate(t[1] - epsilon, 3, jet0.data());
    spline.Evaluate(t[1], 3, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[0,2](t[1]) = P[1,2](t[2]).");

    // P[1,2](t[2]) = P[2,2](t[2])
    spline.Evaluate(t[2] - epsilon, 3, jet0.data());
    spline.Evaluate(t[2], 3, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[1,2](t[2]) = P[2,2](t[2]).");

    // P[2,2](t[3]) = P[3,2](t[3])
    spline.Evaluate(t[3] - epsilon, 3, jet0.data());
    spline.Evaluate(t[3], 3, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[2,2](t[3]) = P[3,2](t[3]).");

    // P[0,2](t[0]) = 0
    spline.Evaluate(t[0], 3, jet0.data());
    error = Length(jet0[2]);
    UTAssert(error <= maxError, "Failed P[0,2](t[0]) = 0.");

    // P[3,2](t[4]) = 0
    spline.Evaluate(t[4], 3, jet0.data());
    error = Length(jet0[2]);
    UTAssert(error <= maxError, "Failed P[3,2](t[3]) = 0.");
}

void UnitTestNaturalCubicSpline::TestClamped()
{
    std::vector<double> t(5);
    std::vector<Vector<double, 1>> f(5);
    t[0] = 0.0;
    t[1] = 0.2;
    t[2] = 0.452;
    t[3] = 0.611;
    t[4] = 1.0;
    f[0][0] = -0.72904599140643900;
    f[1][0] = 0.67001717998915900;
    f[2][0] = 0.93773554224846278;
    f[3][0] = -0.55793191403459019;
    f[4][0] = -0.38366589898599346;
    Vector<double, 1> derivative0{ -0.987 };
    Vector<double, 1> derivative1{ +0.654 };

    NaturalCubicSpline<double, 1> spline(5, f.data(), t.data(), derivative0, derivative1);

    // Verify the constraints.
    double const maxError = 1e-11;
    double const epsilon = 1e-16;
    std::array<Vector<double, 1>, 4> jet0{}, jet1{};
    double error{};

    // P[i,0](t[0]) = f[0]
    spline.Evaluate(t[0], 3, jet0.data());
    error = Length(jet0[0] - f[0]);
    UTAssert(error <= maxError, "Failed P[i,0](t[0]) = f[0].");

    // P[1,0](t[1]) = f[1]
    spline.Evaluate(t[1], 3, jet0.data());
    error = Length(jet0[0] - f[1]);
    UTAssert(error <= maxError, "Failed P[1,0](t[1]) = f[1].");

    // P[2,0](t[2]) = f[2]
    spline.Evaluate(t[2], 3, jet0.data());
    error = Length(jet0[0] - f[2]);
    UTAssert(error <= maxError, "Failed P[2,0](t[2]) = f[2].");

    // P[3,0](t[3]) = f[3]
    spline.Evaluate(t[3], 3, jet0.data());
    error = Length(jet0[0] - f[3]);
    UTAssert(error <= maxError, "Failed P[3,0](t[3]) = f[3].");

    // P[3,0](t[4]) = f[4]
    spline.Evaluate(t[4], 3, jet0.data());
    error = Length(jet0[0] - f[4]);
    UTAssert(error <= maxError, "Failed P[3,0](t[4]) = f[4].");

    // P[0,0](t[1]) = P[1,0](t[1])
    spline.Evaluate(t[1] - epsilon, 3, jet0.data());
    error = Length(jet0[0] - f[1]);
    UTAssert(error <= maxError, "Failed P[0,0](t[1]) = P[1,0](t[1]).");

    // P[1,0](t[2]) = P[2,0](t[2])
    spline.Evaluate(t[2] - epsilon, 3, jet0.data());
    error = Length(jet0[0] - f[2]);
    UTAssert(error <= maxError, "Failed P[1,0](t[2]) = P[2,0](t[2]).");

    // P[2,0](t[3]) = P[3,0](t[3])
    spline.Evaluate(t[3] - epsilon, 3, jet0.data());
    error = Length(jet0[0] - f[3]);
    UTAssert(error <= maxError, "Failed P[2,0](t[3]) = P[3,0](t[3]).");

    // P[0,1](t[1]) = P[1,1](t[1])
    spline.Evaluate(t[1] - epsilon, 3, jet0.data());
    spline.Evaluate(t[1], 3, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[0,1](t[1]) = P[1,1](t[1]).");

    // P[1,1](t[2]) = P[2,1](t[2])
    spline.Evaluate(t[2] - epsilon, 3, jet0.data());
    spline.Evaluate(t[2], 3, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[1,1](t[2]) = P[2,1](t[2]).");

    // P[2,1](t[3]) = P[3,1](t[3])
    spline.Evaluate(t[3] - epsilon, 3, jet0.data());
    spline.Evaluate(t[3], 3, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[2,1](t[3]) = P[3,1](t[3]).");

    // P[0,2](t[1]) = P[1,2](t[2])
    spline.Evaluate(t[1] - epsilon, 3, jet0.data());
    spline.Evaluate(t[1], 3, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[0,2](t[1]) = P[1,2](t[2]).");

    // P[1,2](t[2]) = P[2,2](t[2])
    spline.Evaluate(t[2] - epsilon, 3, jet0.data());
    spline.Evaluate(t[2], 3, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[1,2](t[2]) = P[2,2](t[2]).");

    // P[2,2](t[3]) = P[3,2](t[3])
    spline.Evaluate(t[3] - epsilon, 3, jet0.data());
    spline.Evaluate(t[3], 3, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[2,2](t[3]) = P[3,2](t[3]).");

    // P[0,1](t[0]) = derivative0
    spline.Evaluate(t[0], 3, jet0.data());
    error = Length(jet0[1] - derivative0);
    UTAssert(error <= maxError, "Failed P[0,1](t[0]) = derivative0.");

    // P[3,1](t[4]) = derivative1
    spline.Evaluate(t[4], 3, jet0.data());
    error = Length(jet0[1] - derivative1);
    UTAssert(error <= maxError, "Failed P[3,1](t[3]) = derivative1.");
}

void UnitTestNaturalCubicSpline::TestClosed()
{
    std::vector<double> t(5);
    std::vector<Vector<double, 1>> f(5);
    t[0] = 0.0;
    t[1] = 0.2;
    t[2] = 0.452;
    t[3] = 0.611;
    t[4] = 1.0;
    f[0][0] = -0.72904599140643900;
    f[1][0] = 0.67001717998915900;
    f[2][0] = 0.93773554224846278;
    f[3][0] = -0.55793191403459019;
    f[4][0] = f[0][0];

    NaturalCubicSpline<double, 1> spline(false, 5, f.data(), t.data());

    // Verify the constraints.
    double const maxError = 1e-11;
    double const epsilon = 1e-16;
    std::array<Vector<double, 1>, 4> jet0{}, jet1{};
    double error{};

    // P[i,0](t[0]) = f[0]
    spline.Evaluate(t[0], 3, jet0.data());
    error = Length(jet0[0] - f[0]);
    UTAssert(error <= maxError, "Failed P[i,0](t[0]) = f[0].");

    // P[1,0](t[1]) = f[1]
    spline.Evaluate(t[1], 3, jet0.data());
    error = Length(jet0[0] - f[1]);
    UTAssert(error <= maxError, "Failed P[1,0](t[1]) = f[1].");

    // P[2,0](t[2]) = f[2]
    spline.Evaluate(t[2], 3, jet0.data());
    error = Length(jet0[0] - f[2]);
    UTAssert(error <= maxError, "Failed P[2,0](t[2]) = f[2].");

    // P[3,0](t[3]) = f[3]
    spline.Evaluate(t[3], 3, jet0.data());
    error = Length(jet0[0] - f[3]);
    UTAssert(error <= maxError, "Failed P[3,0](t[3]) = f[3].");

    // P[3,0](t[4]) = f[4]
    spline.Evaluate(t[4], 3, jet0.data());
    error = Length(jet0[0] - f[4]);
    UTAssert(error <= maxError, "Failed P[3,0](t[4]) = f[4].");


    // P[0,0](t[1]) = P[1,0](t[1])
    spline.Evaluate(t[1] - epsilon, 3, jet0.data());
    error = Length(jet0[0] - f[1]);
    UTAssert(error <= maxError, "Failed P[0,0](t[1]) = P[1,0](t[1]).");

    // P[1,0](t[2]) = P[2,0](t[2])
    spline.Evaluate(t[2] - epsilon, 3, jet0.data());
    error = Length(jet0[0] - f[2]);
    UTAssert(error <= maxError, "Failed P[1,0](t[2]) = P[2,0](t[2]).");

    // P[2,0](t[3]) = P[3,0](t[3])
    spline.Evaluate(t[3] - epsilon, 3, jet0.data());
    error = Length(jet0[0] - f[3]);
    UTAssert(error <= maxError, "Failed P[2,0](t[3]) = P[3,0](t[3]).");


    // P[0,1](t[1]) = P[1,1](t[1])
    spline.Evaluate(t[1] - epsilon, 3, jet0.data());
    spline.Evaluate(t[1], 3, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[0,1](t[1]) = P[1,1](t[1]).");

    // P[1,1](t[2]) = P[2,1](t[2])
    spline.Evaluate(t[2] - epsilon, 3, jet0.data());
    spline.Evaluate(t[2], 3, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[1,1](t[2]) = P[2,1](t[2]).");

    // P[2,1](t[3]) = P[3,1](t[3])
    spline.Evaluate(t[3] - epsilon, 3, jet0.data());
    spline.Evaluate(t[3], 3, jet1.data());
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[2,1](t[3]) = P[3,1](t[3]).");


    // P[0,2](t[1]) = P[1,2](t[2])
    spline.Evaluate(t[1] - epsilon, 3, jet0.data());
    spline.Evaluate(t[1], 3, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[0,2](t[1]) = P[1,2](t[2]).");

    // P[1,2](t[2]) = P[2,2](t[2])
    spline.Evaluate(t[2] - epsilon, 3, jet0.data());
    spline.Evaluate(t[2], 3, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[1,2](t[2]) = P[2,2](t[2]).");

    // P[2,2](t[3]) = P[3,2](t[3])
    spline.Evaluate(t[3] - epsilon, 3, jet0.data());
    spline.Evaluate(t[3], 3, jet1.data());
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[2,2](t[3]) = P[3,2](t[3]).");


    // P[0,0](t[0]) = P[3,0](t[4])
    spline.Evaluate(t[0], 3, jet0.data());
    spline.Evaluate(t[4], 3, jet1.data());
    error = Length(jet0[0] - jet1[0]);
    UTAssert(error <= maxError, "Failed P[0,0](t[0]) = P[3,0](t[4]).");

    // P[0,1](t[0]) = P[3,1](t[4])
    error = Length(jet0[1] - jet1[1]);
    UTAssert(error <= maxError, "Failed P[0,1](t[0]) = P[3,1](t[4]).");

    // P[0,2](t[0]) = P[3,2](t[4])
    error = Length(jet0[2] - jet1[2]);
    UTAssert(error <= maxError, "Failed P[0,2](t[0]) = P[3,2](t[4]).");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/NaturalCubicSpline.h>

namespace gtl
{
    template class NaturalCubicSpline<float, 2>;
    template class NaturalCubicSpline<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class NaturalCubicSpline<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(NaturalCubicSpline)
