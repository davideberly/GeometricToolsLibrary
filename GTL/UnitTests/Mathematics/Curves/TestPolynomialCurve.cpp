#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/PolynomialCurve.h>
using namespace gtl;

namespace gtl
{
    class UnitTestPolynomialCurve
    {
    public:
        UnitTestPolynomialCurve();

    private:
        void Test();
    };
}

UnitTestPolynomialCurve::UnitTestPolynomialCurve()
{
    UTInformation("Mathematics/Curves/PolynomialCurve");

    Test();
}

void UnitTestPolynomialCurve::Test()
{
    std::array<Polynomial1<double>, 2> components{};
    components[0] = Polynomial1<double>{ 1.0, 2.0, 3.0, 4.0 };
    components[1] = Polynomial1<double>{ -1.0, 0.0, 1.0, -2.0 };

    PolynomialCurve<double, 2> curve(0.0, 2.0, components);

    auto const& p0x = curve.GetPolynomial(0);
    auto const& p0y = curve.GetPolynomial(1);
    UTAssert(p0x[0] == 1.0 && p0x[1] == 2.0 && p0x[2] == 3.0 && p0x[3] == 4.0,
        "Invalid polynomial p0x.");
    UTAssert(p0y[0] == -1.0 && p0y[1] == 0.0 && p0y[2] == 1.0 && p0y[3] == -2.0,
        "Invalid polynomial p0y.");

    auto const& p1x = curve.GetDer1Polynomial(0);
    auto const& p1y = curve.GetDer1Polynomial(1);
    UTAssert(p1x[0] == 2.0 && p1x[1] == 6.0 && p1x[2] == 12.0,
        "Invalid polynomial p1x.");
    UTAssert(p1y[0] == 0.0 && p1y[1] == 2.0 && p1y[2] == -6.0,
        "Invalid polynomial p1y.");

    auto const& p2x = curve.GetDer2Polynomial(0);
    auto const& p2y = curve.GetDer2Polynomial(1);
    UTAssert(p2x[0] == 6.0 && p2x[1] == 24.0,
        "Invalid polynomial p2x.");
    UTAssert(p2y[0] == 2.0 && p2y[1] == -12.0,
        "Invalid polynomial p2y.");

    auto const& p3x = curve.GetDer3Polynomial(0);
    auto const& p3y = curve.GetDer3Polynomial(1);
    UTAssert(p3x[0] == 24.0,
        "Invalid polynomial p3x.");
    UTAssert(p3y[0] == -12.0,
        "Invalid polynomial p3y.");

    std::array<Vector<double, 2>, 4> jet{};
    curve.Evaluate(1.0, 3, jet.data());
    UTAssert(jet[0][0] == 10.0 && jet[0][1] == -2.0,
        "Invalid value for polynomial");
    UTAssert(jet[1][0] == 20.0 && jet[1][1] == -4.0,
        "Invalid value for der1 polynomial");
    UTAssert(jet[2][0] == 30.0 && jet[2][1] == -10.0,
        "Invalid value for der2 polynomial");
    UTAssert(jet[3][0] == 24.0 && jet[3][1] == -12.0,
        "Invalid value for der3 polynomial");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/PolynomialCurve.h>

namespace gtl
{
    template class PolynomialCurve<float, 2>;
    template class PolynomialCurve<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class PolynomialCurve<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(PolynomialCurve)
