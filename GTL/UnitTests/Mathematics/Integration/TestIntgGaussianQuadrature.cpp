#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Integration/IntgGaussianQuadrature.h>
#include <GTL/Mathematics/Algebra/Vector.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntgGaussianQuadrature
    {
    public:
        UnitTestIntgGaussianQuadrature();

    private:
        void Test();
    };
}

UnitTestIntgGaussianQuadrature::UnitTestIntgGaussianQuadrature()
{
    UTInformation("Mathematics/Integration/IntgGaussianQuadrature");

    // Integral(sin(x);0,pi/2) = 1
    Test();
}

void UnitTestIntgGaussianQuadrature::Test()
{
    std::size_t const degree = 5;
    std::size_t const maxBisections = 1024;
    std::size_t const precision = 64;
    std::vector<double> roots, coefficients;
    IntgGaussianQuadrature<double>::ComputeRootsAndCoefficients(
        degree, maxBisections, precision, roots, coefficients);

    double const x0 = std::sqrt(5.0 - 2.0 * std::sqrt(10.0 / 7.0)) / 3.0;
    double const x1 = std::sqrt(5.0 + 2.0 * std::sqrt(10.0 / 7.0)) / 3.0;
    std::vector<double> expectedRoots =
    {
        -x1, -x0, 0.0, x0, x1
    };

    double const w0 = (322.0 + 13.0 * std::sqrt(70.0)) / 900.0;
    double const w1 = (322.0 - 13.0 * std::sqrt(70.0)) / 900.0;
    std::vector<double> expectedCoefficients =
    {
        w1, w0, 128.0 / 225.0, w0, w1
    };

    double error0 = Length(Vector<double, 5>(roots) - Vector<double, 5>(expectedRoots));
    double error1 = Length(Vector<double, 5>(coefficients) - Vector<double, 5>(expectedCoefficients));
    UTAssert(
        error0 <= 1e-14 && error1 <= 1e-14,
        "Unexpected roots or coefficients.");

    std::function<double(double)> integrand = [](double x) { return sin(x); };
    double const a = 0.0, b = C_PI_DIV_2<double>;
    double value = IntgGaussianQuadrature<double>::Integrate(
        roots, coefficients, a, b, integrand);
    double error = std::fabs(value - 1.0);
    UTAssert(
        error <= 1e-10,
        "GaussianQuadrature failed."
    );
}

#else

#include <GTL/Mathematics/Integration/IntgGaussianQuadrature.h>

namespace gtl
{
    template class IntgGaussianQuadrature<float>;
    template class IntgGaussianQuadrature<double>;

    using Rational = BSRational<UIntegerAP32>;
    template class IntgGaussianQuadrature<Rational>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntgGaussianQuadrature)
