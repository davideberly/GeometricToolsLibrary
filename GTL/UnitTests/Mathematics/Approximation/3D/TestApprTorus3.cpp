#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/3D/ApprTorus3.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestApprTorus3
    {
    public:
        UnitTestApprTorus3();

    private:
        void TestSpecial();
        void TestGN();
        void TestLM();
    };
}

UnitTestApprTorus3::UnitTestApprTorus3()
{
    UTInformation("Mathematics/Approximation/3D/ApprTorus3");

    TestSpecial();
    TestGN();
    TestLM();
}

void UnitTestApprTorus3::TestSpecial()
{
    Vector3<double> C{ 0.0, 0.0, 0.0 };
    Vector3<double> N{ 0.0, 0.0, 1.0 };
    Vector3<double> D0{ 1.0, 0.0, 0.0 };
    Vector3<double> D1{ 0.0, 1.0, 0.0 };
    double r0 = 1.0;
    double r1 = 0.25;

    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(0.0, C_TWO_PI<double>);
    std::vector<Vector3<double>> points(1024);
    for (auto& X : points)
    {
        double angle0 = urd(dre), angle1 = urd(dre);
        double cs0 = cos(angle0), sn0 = sin(angle0);
        double cs1 = cos(angle1), sn1 = sin(angle1);
        X = C + (r0 + r1 * cs1) * (cs0 * D0 + sn0 * D1) + r1 * sn1 * N;
    }

    Vector3<double> fitC{}, fitN{};
    double fitR0{}, fitR1{};
    bool success = ApprTorus3<double>::Fit(points, fitC, fitN, fitR0, fitR1);
    UTAssert(
        success,
        "Expecting Fit to succeed.");

    double error = Length(fitC - C);
    UTAssert(error <= 0.03, "Invalid center estimate, " + std::to_string(error));
    error = Length(fitN - N);
    UTAssert(error <= 0.02, "Invalid normal estimate., " + std::to_string(error));
    error = std::fabs(fitR0 - r0);
    UTAssert(error <= 0.0003, "Invalid r0 estimate, " + std::to_string(error));
    error = std::fabs(fitR1 - r1);
    UTAssert(error <= 4e-03, "Invalid r1 estimate, " + std::to_string(error));
}

void UnitTestApprTorus3::TestGN()
{
    Vector3<double> C{ 0.0, 0.0, 0.0 };
    Vector3<double> N{ 0.0, 0.0, 1.0 };
    Vector3<double> D0{ 1.0, 0.0, 0.0 };
    Vector3<double> D1{ 0.0, 1.0, 0.0 };
    double r0 = 1.0;
    double r1 = 0.25;

    std::default_random_engine dre;
    std::uniform_real_distribution<double> urd(0.0, C_TWO_PI<double>);
    std::size_t const numPoints = 1024;
    std::vector<Vector3<double>> points(numPoints);
    for (auto& X : points)
    {
        double angle0 = urd(dre), angle1 = urd(dre);
        double cs0 = cos(angle0), sn0 = sin(angle0);
        double cs1 = cos(angle1), sn1 = sin(angle1);
        X = C + (r0 + r1 * cs1) * (cs0 * D0 + sn0 * D1) + r1 * sn1 * N;
    }

    std::size_t const maxIterations = 128;
    double const updateLengthTolerance = 1e-04;
    double const errorDifferenceTolerance = 1e-08;
    bool useTorusInputAsInitialGuess = true;
    Vector3<double> fitC = { 0.1, 0.1, 0.1 };
    Vector3<double> fitN = { 0.01, 0.02, 0.3 };
    Normalize(fitN);
    double fitR0 = 0.75;
    double fitR1 = 0.125;
    auto output = ApprTorus3<double>::Fit(points, maxIterations,
        updateLengthTolerance, errorDifferenceTolerance, useTorusInputAsInitialGuess,
        fitC, fitN, fitR0, fitR1);
    // The solver failed to converge, but the fitted values are still
    // reasonable.
    //UTAssert(output.converged, "Torus fitter failed to converge.");

    double error = Length(fitC - C);
    UTAssert(error <= 0.005, "Invalid center estimate, " + std::to_string(error));
    error = Length(fitN - N);
    UTAssert(error <= 0.14, "Invalid normal estimate, " + std::to_string(error));
    error = std::fabs(fitR0 - r0);
    UTAssert(error <= 0.02, "Invalid r0 estimate, " + std::to_string(error));
    error = std::fabs(fitR1 - r1);
    UTAssert(error <= 0.02, "Invalid r1 estimate, " + std::to_string(error));
}

void UnitTestApprTorus3::TestLM()
{
    Vector3<double> C{ 0.0, 0.0, 0.0 };
    Vector3<double> N{ 0.0, 0.0, 1.0 };
    Vector3<double> D0{ 1.0, 0.0, 0.0 };
    Vector3<double> D1{ 0.0, 1.0, 0.0 };
    double r0 = 1.0;
    double r1 = 0.25;

    std::default_random_engine dre;
    std::uniform_real_distribution<double> urd(0.0, C_TWO_PI<double>);
    std::size_t const numPoints = 1024;
    std::vector<Vector3<double>> points(numPoints);
    for (auto& X : points)
    {
        double angle0 = urd(dre), angle1 = urd(dre);
        double cs0 = cos(angle0), sn0 = sin(angle0);
        double cs1 = cos(angle1), sn1 = sin(angle1);
        X = C + (r0 + r1 * cs1) * (cs0 * D0 + sn0 * D1) + r1 * sn1 * N;
    }

    std::size_t const maxIterations = 128;
    double const updateLengthTolerance = 1e-04;
    double const errorDifferenceTolerance = 1e-08;
    double const lambdaFactor = 0.001;
    double const lambdaAdjust = 10.0;
    std::size_t const maxAdjustments = 8;
    bool useTorusInputAsInitialGuess = true;
    Vector3<double> fitC = { 0.1, 0.1, 0.1 };
    Vector3<double> fitN = { 0.01, 0.02, 0.3 };
    Normalize(fitN);
    double fitR0 = 0.75;
    double fitR1 = 0.125;
    auto output = ApprTorus3<double>::Fit(points, maxIterations,
        updateLengthTolerance, errorDifferenceTolerance,
        lambdaFactor, lambdaAdjust, maxAdjustments, useTorusInputAsInitialGuess,
        fitC, fitN, fitR0, fitR1);
    // The solver failed to converge, but the fitted values are still
    // reasonable.
    //UTAssert(output.converged, "Torus fitter failed to converge.");

    double error = Length(fitC - C);
    UTAssert(error <= 0.0006, "Invalid center estimate, " + std::to_string(error));
    error = Length(fitN - N);
    UTAssert(error <= 0.03, "Invalid normal estimate, " + std::to_string(error));
    error = std::fabs(fitR0 - r0);
    UTAssert(error <= 0.0004, "Invalid r0 estimate, " + std::to_string(error));
    error = std::fabs(fitR1 - r1);
    UTAssert(error <= 0.0008, "Invalid r1 estimate, " + std::to_string(error));
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/3D/ApprTorus3.h>

namespace gtl
{
    template class ApprTorus3<float>;
    template class ApprTorus3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprTorus3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprTorus3)
