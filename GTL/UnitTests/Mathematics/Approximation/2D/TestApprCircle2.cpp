#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/2D/ApprCircle2.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestApprCircle2
    {
    public:
        UnitTestApprCircle2();

    private:
        void Test();
    };
}

UnitTestApprCircle2::UnitTestApprCircle2()
{
    UTInformation("Mathematics/Approximation/2D/ApprCircle2");

    Test();
}

void UnitTestApprCircle2::Test()
{
    Vector2<double> center = { 10.0, 20.0 };
    double radius = 9.0;
    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0, 1.0);
    std::vector<Vector2<double>> points(1000);
    for (std::size_t i = 0; i < points.size(); ++i)
    {
        double angle = (double)i / (double)points.size();
        Vector2<double> direction = { std::cos(angle), std::sin(angle) };
        double amplitude = radius + 0.0001 * urd(dre);
        points[i][0] = center[0] + 0.0001 * urd(dre) + amplitude * direction[0];
        points[i][1] = center[1] + 0.0001 * urd(dre) + amplitude * direction[1];
    }

    ApprCircle2<double> fitter{};
    Circle2<double> estimate{};

    std::size_t maxIterations = static_cast<std::size_t>(1) << 16;
    double epsilon = 1e-06;
    std::size_t numIterations{};
    bool success{};
    double error{};

    numIterations = fitter.FitUsingLengths(points, maxIterations, false, estimate, epsilon);
    error = Length(center - estimate.center);
    UTAssert(error <= 8e-4, "Invalid FitUsingLengths center estimate, " + std::to_string(error));
    error = std::fabs(radius - estimate.radius);
    UTAssert(error <= 8e-4, "Invalid FitUsingLengths radius estimate, " + std::to_string(error));

    numIterations = fitter.FitUsingLengths(points, maxIterations, true, estimate, epsilon);
    error = Length(center - estimate.center);
    UTAssert(error <= 8e-4, "Invalid FitUsingLengths center estimate, " + std::to_string(error));
    error = std::fabs(radius - estimate.radius);
    UTAssert(error <= 8e-4, "Invalid FitUsingLengths radius estimate, " + std::to_string(error));

    success = fitter.FitUsingSquaredLengths(points, estimate);
    error = Length(center - estimate.center);
    UTAssert(error <= 4e-05, "Invalid FitUsingSquaredLengths center estimate, " + std::to_string(error));
    error = std::fabs(radius - estimate.radius);
    UTAssert(error <= 4e-05, "Invalid FitUsingSquaredLengths radius estimate, " + std::to_string(error));

    (void)numIterations;
    (void)success;
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/2D/ApprCircle2.h>

namespace gtl
{
    template class ApprCircle2<float>;
    template class ApprCircle2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprCircle2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprCircle2)
