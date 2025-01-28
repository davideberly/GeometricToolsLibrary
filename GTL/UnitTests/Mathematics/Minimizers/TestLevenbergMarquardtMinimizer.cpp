#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Mathematics/Minimizers/LevenbergMarquardtMinimizer.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestLevenbergMarquardtMinimizer
    {
    public:
        UnitTestLevenbergMarquardtMinimizer();

    private:
        void Test();
    };
}

UnitTestLevenbergMarquardtMinimizer::UnitTestLevenbergMarquardtMinimizer()
{
    UTInformation("Mathematics/Minimizers/LevenbergMarquardtMinimizer");

    Test();
}

void UnitTestLevenbergMarquardtMinimizer::Test()
{
    std::default_random_engine dre;
    std::uniform_real_distribution<double> urd(0.0, C_TWO_PI<double>);
    std::uniform_real_distribution<double> perturb(-0.1, 0.1);
    Vector2<double> center{ 0.1, 0.2 };
    double a = 2.0, b = 1.0;
    std::uint32_t const numPoints = 1024;
    std::vector<Vector2<double>> points(numPoints);
    for (std::uint32_t i = 0; i < numPoints; ++i)
    {
        double angle = urd(dre);
        points[i][0] = center[0] + a * cos(angle) + perturb(dre);
        points[i][1] = center[1] + b * sin(angle) + perturb(dre);
    }

    // F_{i}(C,r) = |C - X_{i}|^2 - r^2
    auto F = [&points](Vector<double> const& input, Vector<double>& output)
    {
        for (std::size_t i = 0; i < output.size(); ++i)
        {
            Vector2<double> diff{ input[0] - points[i][0], input[1] - points[i][1] };
            output[i] = Dot(diff, diff) - input[2] * input[2];
        }
    };

    // dF_{i}/dc0 = 2 * (C[0] - X[i][0])
    // dF_{i}/dc1 = 2 * (C[1] - X[i][1])
    // dF_{i}/dr = -2*r
    auto J = [&points](Vector<double> const& input, Matrix<double>& output)
    {
        for (std::size_t row = 0; row < output.GetNumRows(); ++row)
        {
            output(row, 0) = 2.0 * (input[0] - points[row][0]);
            output(row, 1) = 2.0 * (input[1] - points[row][1]);
            output(row, 2) = -2.0 * input[2];
        }
    };

    LevenbergMarquardtMinimizer<double> minimizer(3, numPoints,
        (LevenbergMarquardtMinimizer<double>::FFunction const&)F,
        (LevenbergMarquardtMinimizer<double>::JFunction const&)J);
    Vector<double> initial{ 0.0, 0.0, 0.5 };
    std::size_t const maxIterations = 32;
    double const updateLengthTolerance = 1e-04;
    double const errorDifferenceTolerance = 1e-08;
    double const lambda = 0.001;
    double const lambdaAdjust = 10.0;
    std::size_t const maxAdjustments = 8;
    auto output = minimizer(initial, maxIterations, updateLengthTolerance,
        errorDifferenceTolerance, lambda, lambdaAdjust, maxAdjustments);

    // Mathematica produces
    //   x = Import["PATH/\\points.txt", "CSV"]
    //   c = { c0, c1 }
    //   f = Sum[(Dot[c - x[[i]], c - x[[i]]] - r^2)^2, {i,Length[x]}]
    //   NMinimize[f, {c0, c1, r}, WorkingPrecision -> 20]
    //   {1186.5631964741460251, {c0 -> 0.12271802779481759546, c1 -> 0.20086178583326718225, r -> -1.5889557425462114503}}
    // Note that r and -r are both solutions.
    Vector<double> expected{ 0.12271802779481759546, 0.20086178583326718225, 1.5889557425462114503 };
    output.minLocation[2] = std::fabs(output.minLocation[2]);

    // NOTE: On Fedora and gcc 12.2.1 with default math flags does an awful
    // job of computing. The error using MSVS 2022 17.4.3 on Windows 11 for
    // the minimum location is off by about 2e-07. On Fedora it is off by
    // about 0.020251. The minimum value is off by about 0.002 on Windows 11
    // but 29.070341 on Fedora.
#if defined(GTL_USE_LINUX)
    double error = Length(output.minLocation - expected);
    UTAssert(error <= 0.03, "Invalid minimum location, " + std::to_string(error));
    error = std::fabs(output.minError - 1186.5631964741460251);
    UTAssert(error <= 29.1, "Invalid minimum value, " + std::to_string(error));
#else
    double error = Length(output.minLocation - expected);
    UTAssert(error <= 2e-07, "Invalid minimum location, " + std::to_string(error));
    error = std::fabs(output.minError - 1186.5631964741460251);
    UTAssert(error <= 0.002, "Invalid minimum value, " + std::to_string(error));
#endif
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Minimizers/LevenbergMarquardtMinimizer.h>

namespace gtl
{
    template class LevenbergMarquardtMinimizer<float>;
    template class LevenbergMarquardtMinimizer<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class LevenbergMarquardtMinimizer<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(LevenbergMarquardtMinimizer)
