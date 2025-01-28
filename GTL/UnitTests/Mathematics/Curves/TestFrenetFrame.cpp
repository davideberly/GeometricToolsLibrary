#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/FrenetFrame.h>
#include <GTL/Mathematics/Curves/PolynomialCurve.h>
using namespace gtl;

namespace gtl
{
    class UnitTestFrenetFrame
    {
    public:
        UnitTestFrenetFrame();

    private:
        void Test2();
        void Test3();
    };
}

UnitTestFrenetFrame::UnitTestFrenetFrame()
{
    UTInformation("Mathematics/Curves/FrenetFrame");

    Test2();
    Test3();
}

void UnitTestFrenetFrame::Test2()
{
    double const maxError = 1e-15;
    std::array<Polynomial1<double>, 2> components{};
    components[0] = Polynomial1<double>{ 1.0, 2.0, 3.0, 4.0 };
    components[1] = Polynomial1<double>{ -1.0, 0.0, 1.0, -2.0 };

    PolynomialCurve<double, 2> curve(0.0, 2.0, components);

    // Verified using Mathematica.
    Vector2<double> position{}, tangent{}, normal{};
    FrenetFrame2<double>::GetFrame(curve, 0.5, position, tangent, normal);
    UTAssert(
        std::fabs(position[0] - 3.25) <= maxError &&
        std::fabs(position[1] + 1.0) <= maxError,
        "Invalid position.");
    UTAssert(
        std::fabs(tangent[0] - 0.99805257848288853) <= maxError &&
        std::fabs(tangent[1] + 0.062378286155180533) <= maxError,
        "Invalid tangent.");
    UTAssert(
        std::fabs(normal[0] - 0.062378286155180533) <= maxError &&
        std::fabs(normal[1] - 0.99805257848288853) <= maxError,
        "Invalid normal.");

    double curvature = FrenetFrame2<double>::GetCurvature(curve, 0.5);
    UTAssert(std::fabs(curvature + 0.044659940282308241) <= maxError,
        "Invalid curvature.");
}

void UnitTestFrenetFrame::Test3()
{
    double const maxError = 1e-15;
    std::array<Polynomial1<double>, 3> components{};
    components[0] = Polynomial1<double>{ 1.0, 2.0, 3.0, 4.0 };
    components[1] = Polynomial1<double>{ -1.0, 0.0, 1.0, -2.0 };
    components[2] = Polynomial1<double>{ 1.1, -2.2, 3.3, -4.4, 5.5};

    PolynomialCurve<double, 3> curve(0.0, 2.0, components);

    // Verified using Mathematica.
    Vector3<double> position{}, tangent{}, normal{}, binormal{};
    FrenetFrame3<double>::GetFrame(curve, 0.5, position, tangent, normal, binormal);
    UTAssert(
        std::fabs(position[0] - 3.25) <= maxError &&
        std::fabs(position[1] + 1.0) <= maxError &&
        std::fabs(position[2] - 0.61875) <= maxError,
        "Invalid position.");
    UTAssert(
        std::fabs(tangent[0] - 0.99571134125002103) <= maxError &&
        std::fabs(tangent[1] + 0.062231958828126314) <= maxError &&
        std::fabs(tangent[2] - 0.068455154710938868) <= maxError,
        "Invalid tangent.");
    UTAssert(
        std::fabs(normal[0] + 0.084513165111746849) <= maxError &&
        std::fabs(normal[1] + 0.31084543421001509) <= maxError &&
        std::fabs(normal[2] - 0.94669564325266753) <= maxError,
        "Invalid normal.");
    UTAssert(
        std::fabs(binormal[0] + 0.037635752003631014) <= maxError &&
        std::fabs(binormal[1] + 0.94842095049150077) <= maxError &&
        std::fabs(binormal[2] + 0.31477174403036817) <= maxError,
        "Invalid binormal.");

    double curvature = FrenetFrame3<double>::GetCurvature(curve, 0.5);
    UTAssert(std::fabs(curvature - 0.14088426680655319) <= maxError,
        "Invalid curvature.");

    double torsion = FrenetFrame3<double>::GetTorsion(curve, 0.5);
    UTAssert(std::fabs(torsion + 0.027195836714473204) <= maxError,
        "Invalid torsion.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/FrenetFrame.h>

namespace gtl
{
    template class FrenetFrame2<float>;
    template class FrenetFrame3<float>;

    template class FrenetFrame2<double>;
    template class FrenetFrame3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class FrenetFrame2<Rational>;
    template class FrenetFrame3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(FrenetFrame)
