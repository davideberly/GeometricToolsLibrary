#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Minimizers/PowellsMinimizer.h>
#include <limits>
#include <vector>
using namespace gtl;

namespace gtl
{
    class UnitTestPowellsMinimizer
    {
    public:
        UnitTestPowellsMinimizer();

    private:
        void TestParaboloids2();
        void TestParaboloids3();
        //void TestSinusoidals();
    };
}

UnitTestPowellsMinimizer::UnitTestPowellsMinimizer()
{
    UTInformation("Mathematics/Minimizers/PowellsMinimizer");

    TestParaboloids2();
    TestParaboloids3();
}

void UnitTestPowellsMinimizer::TestParaboloids2()
{
    std::vector<std::array<double, 3>> evaluations{};

    auto F = [&evaluations](double const* x)
        {
            double result = x[0] * x[0] + 4.0 * x[1] * x[1];
            evaluations.push_back(std::array<double, 3>{ x[0], x[1], result});
            return result;
        };

    std::size_t maxSubdivisions = 8;
    std::size_t maxBisections = 8;
    double epsilon = 1e-08;
    double tolerance = 1e-04;
    PowellsMinimizer<double> minimizer(2, maxSubdivisions, maxBisections, epsilon, tolerance);

    std::array<double, 2> xMin{ std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
    double fMin = std::numeric_limits<double>::max();
    std::size_t maxIterations = 16;
    std::array<double, 2> x0{ -1.0, -0.5 };
    std::array<double, 2> x1{ +1.5, +0.75 };
    std::array<double, 2> xInitial{ 0.9, -0.4 };
    evaluations.clear();
    minimizer(F, maxIterations, x0.data(), x1.data(), xInitial.data(), xMin.data(), fMin);
    UTAssert( // evaluations.size() == 27
        xMin[0] <= 1e-16 && xMin[1] <= 1e-16 && fMin <= 1e-32,
        "Powells minimization failed.");

    xMin = { std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
    fMin = std::numeric_limits<double>::max();
    evaluations.clear();
    minimizer(F, maxIterations, x0.data(), x1.data(), xMin.data(), fMin);
    UTAssert( // evaluations.size() == 133
        xMin[0] <= 1e-16 && xMin[1] <= 1e-16 && fMin <= 1e-32,
        "Powells minimization failed.");
}

void UnitTestPowellsMinimizer::TestParaboloids3()
{
    std::vector<std::array<double, 4>> evaluations{};

    auto F = [&evaluations](double const* x)
        {
            double result = x[0] * x[0] + 4.0 * x[1] * x[1] + 9.0 * x[2] * x[2];
            evaluations.push_back(std::array<double, 4>{ x[0], x[1], x[2], result});
            return result;
        };

    std::size_t maxSubdivisions = 8;
    std::size_t maxBisections = 8;
    double epsilon = 1e-08;
    double tolerance = 1e-04;
    PowellsMinimizer<double> minimizer(3, maxSubdivisions, maxBisections, epsilon, tolerance);

    std::array<double, 3> xMin{ 
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::max() };
    double fMin = std::numeric_limits<double>::max();
    std::size_t maxIterations = 32;
    std::array<double, 3> x0{ -1.0, -0.5, -2.0 };
    std::array<double, 3> x1{ +1.5, +0.75, +4.0 };
    std::array<double, 3> xInitial{ 0.9, -0.4 };
    evaluations.clear();
    minimizer(F, maxIterations, x0.data(), x1.data(), xInitial.data(), xMin.data(), fMin);
    UTAssert( // evaluations.size() == 81
        xMin[0] <= 1e-16 && xMin[1] <= 1e-16 && xMin[2] <= 1e-16 && fMin <= 1e-32,
        "Powells minimization failed.");

    xMin = {
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::max() };
    fMin = std::numeric_limits<double>::max();
    evaluations.clear();
    minimizer(F, maxIterations, x0.data(), x1.data(), xMin.data(), fMin);
    UTAssert( // evaluations.size() == 185
        xMin[0] <= 1e-16 && xMin[1] <= 1e-16 && xMin[2] <= 1e-16 && fMin <= 1e-32,
        "Powells minimization failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Minimizers/PowellsMinimizer.h>

namespace gtl
{
    template class PowellsMinimizer<float>;
    template class PowellsMinimizer<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class PowellsMinimizer<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(PowellsMinimizer)
