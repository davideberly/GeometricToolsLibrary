#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/ReparameterizeByArclength.h>
#include <GTL/Mathematics/Curves/BezierCurve.h>
#include <GTL/Mathematics/Curves/PolynomialCurve.h>
using namespace gtl;

namespace gtl
{
    class UnitTestReparameterizeByArclength
    {
    public:
        UnitTestReparameterizeByArclength();

    private:
        void Test();
    };
}

UnitTestReparameterizeByArclength::UnitTestReparameterizeByArclength()
{
    UTInformation("Mathematics/Curves/ReparameterizeByArclength");

    Test();
}

void UnitTestReparameterizeByArclength::Test()
{
    // The curve is Y(t) = (t^3, t^5) for t in [-1,1].
    std::array<Polynomial1<double>, 2> coeff{};
    coeff[0] = { 0.0, 0.0, 0.0, 1.0 };
    coeff[1] = { 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 };
    double tMin = -1.0, tMax = 1.0;
    auto curve = std::make_shared<PolynomialCurve<double, 2>>(tMin, tMax, coeff);
    double totalArclength = curve->GetTotalLength();
    // totalArclength  = 2.9053418626475693
    double arclength{};

    double constexpr maxError = 1e-15;
    double expectedT{}, expectedArclength{};

    ReparameterizeByArclength<double, 2> repar2(curve);
    ReparameterizeByArclength<double, 2>::Output output2{};

    // use bisection
    output2 = repar2.GetT(0.123, true);
    arclength = curve->GetLength(tMin, output2.t);
    // output2.t = -0.97809022308903937
    // output2.f = -2.9143354396410359e-16
    // output2.iterations = 50
    // arclength = 0.12299999999999971
    expectedT = -0.97809022308903937;
    expectedArclength = 0.123;
    UTAssert(std::fabs(output2.t - expectedT) <= maxError, "Invalid t.");
    UTAssert(std::fabs(arclength - expectedArclength) <= maxError, "Invalid arclength.");

    // use Newton-bisection hybrid
    output2 = repar2.GetT(0.123, false);
    arclength = curve->GetLength(tMin, output2.t);
    // output2.t = -0.97809022308903926
    // output2.f = 2.7755575615628914e-16
    // output2.iterations = 6
    // arclength = 0.12300000000000028
    expectedT = -0.97809022308903937;
    expectedArclength = 0.123;
    UTAssert(std::fabs(output2.t - expectedT) <= maxError, "Invalid t.");
    UTAssert(std::fabs(arclength - expectedArclength) <= maxError, "Invalid arclength.");

    // use bisection
    output2 = repar2.GetT(0.5 * totalArclength, true);
    arclength = curve->GetLength(tMin, output2.t);
    // output2.t = -8.3535909652709961e-05
    // output2.f = 0.0
    // output2.iterations = 25
    // arclength = 1.4526709313237847
    expectedT = -8.3535909652709961e-05;
    expectedArclength = 0.5 * totalArclength;
    UTAssert(std::fabs(output2.t - expectedT) <= maxError, "Invalid t.");
    UTAssert(std::fabs(arclength - expectedArclength) <= maxError, "Invalid arclength.");

    // use Newton-bisection hybrid
    output2 = repar2.GetT(0.5 * totalArclength, false);
    arclength = curve->GetLength(tMin, output2.t);
    // output2.t = -8.3543227023068973e-05
    // output2.f = 0.0
    // output2.iterations = 29
    // arclength = 1.4526709313237847
    expectedT = -8.3535909652709961e-05;
    expectedArclength = 0.5 * totalArclength;
    UTAssert(std::fabs(output2.t - expectedT) <= 1e-08, "Invalid t.");
    UTAssert(std::fabs(arclength - expectedArclength) <= maxError, "Invalid arclength.");

    // The curve is a Bezier curve of degree 8.
    std::array<Vector3<double>, 9> controls{};
    controls[0] = { 0.0, 0.0, 0.0 };
    controls[1] = { 1.0, 0.0, 0.125 };
    controls[2] = { 0.0, 1.0, 0.25 };
    controls[3] = { -1.0, 0.0, 0.375 };
    controls[4] = { 0.0, -1.0, 0.5 };
    controls[5] = { 2.0, 0.0, 0.625 };
    controls[6] = { 0.0, 2.0, 0.75 };
    controls[7] = { -2.0, 0.0, 0.875 };
    controls[8] = { 0.0, -2.0, 1.0 };
    auto bezier = std::make_shared<BezierCurve<double, 3>>(8, controls.data());
    ReparameterizeByArclength<double, 3> repar3(bezier);
    ReparameterizeByArclength<double, 3>::Output output3{};
    totalArclength = bezier->GetTotalLength();
    // totalArclength = 4.7072995195418841

    // use bisection
    output3 = repar3.GetT(0.75 * totalArclength, true);
    arclength = bezier->GetLength(0.0, output3.t);
    // output3.t = 0.93554114969681224
    // output3.f = 0.0
    // output3.iterations = 50
    // arclength = 3.5304746396564131
    expectedT = 0.93554114969681224;
    expectedArclength = 0.75 * totalArclength;
    UTAssert(std::fabs(output3.t - expectedT) <= maxError, "Invalid t.");
    UTAssert(std::fabs(arclength - expectedArclength) <= maxError, "Invalid arclength.");

    // use Newton-bisection hybrid
    output3 = repar3.GetT(0.75 * totalArclength, false);
    arclength = bezier->GetLength(0.0, output3.t);
    // output3.t = 0.93554114969681224
    // output3.f = 0.0
    // output3.iterations = 7
    // arclength = 3.5304746396564131
    expectedT = 0.93554114969681224;
    expectedArclength = 0.75 * totalArclength;
    UTAssert(std::fabs(output3.t - expectedT) <= maxError, "Invalid t.");
    UTAssert(std::fabs(arclength - expectedArclength) <= maxError, "Invalid arclength.");
}

#else

#include <GTL/Mathematics/Curves/ReparameterizeByArclength.h>

namespace gtl
{
    template class ReparameterizeByArclength<float, 2>;
    template class ReparameterizeByArclength<double, 3>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ReparameterizeByArclength)
