#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/NURBSCurve.h>
using namespace gtl;

namespace gtl
{
    class UnitTestNURBSCurve
    {
    public:
        UnitTestNURBSCurve();

    private:
        void Test();
    };
}

UnitTestNURBSCurve::UnitTestNURBSCurve()
{
    UTInformation("Mathematics/Curves/NURBSCurve [tested by GTL/VisualTests/Curves/NURBSCurveExample]");

    Test();
}

void UnitTestNURBSCurve::Test()
{
    double constexpr maxError = 1e-15;

    BasisFunction<double>::Input input{};
    input.numControls = 3;
    input.degree = 2;
    input.uniform = true;
    input.periodic = false;
    input.uniqueKnots.resize(input.numControls - input.degree + 1);
    input.uniqueKnots[0].t = 0.0;
    input.uniqueKnots[1].t = 1.0;
    input.uniqueKnots[0].multiplicity = 3;
    input.uniqueKnots[1].multiplicity = 3;

    std::vector<Vector2<double>> controls(input.numControls);
    controls[0] = { 1.0, 0.0 };
    controls[1] = { 1.0, 1.0 };
    controls[2] = { 0.0, 1.0 };

    std::vector<double> weights(input.numControls);
    weights[0] = 1.0;
    weights[1] = sqrt(0.5);
    weights[2] = 1.0;

    NURBSCurve<double, 2> curve(input, controls.data(), weights.data());
    double tmin = curve.GetTMin();
    UTAssert(tmin == 0.0, "GetTMin() failed.");

    double tmax = curve.GetTMax();
    UTAssert(tmax == 1.0, "GetTMax() failed.");

    std::size_t numSegments = curve.GetNumSegments();
    UTAssert(numSegments == 1, "GetNumSegments() failed.");

    auto const& times = curve.GetTimes();
    UTAssert(times[0] == 0.0 && times[1] == 1.0, "GetTimes() failed.");

    std::array<Vector2<double>, 4> value{};
    std::array<Vector2<double>, 4> expectedValue =
    {
        Vector2<double>{ 0.70710678118654746, 0.70710678118654746 },
        Vector2<double>{ -1.1715728752538099, 1.1715728752538099 },
        Vector2<double>{ -1.9411254969542810, -1.9411254969542810 },
        Vector2<double>{ 4.8242430426400631, -4.8242430426400631 }
    };
    curve.Evaluate(0.5, 3, value.data());
    for (std::size_t i = 0; i < 4; ++i)
    {
        UTAssert(Length(value[i] - expectedValue[i]) <= maxError,
            "Invalid point " + std::to_string(i) + ".");
    }

    value[0] = curve.GetPosition(0.5);
    expectedValue[0] = { 0.70710678118654746, 0.70710678118654746 };
    UTAssert(Length(value[0] - expectedValue[0]) <= maxError, "Invalid position.");

    value[0] = curve.GetTangent(0.5);
    expectedValue[0] = { -0.70710678118654757, 0.70710678118654757 };
    UTAssert(Length(value[0] - expectedValue[0]) <= maxError, "Invalid tangent.");

    double speed = curve.GetSpeed(0.5);
    double expectedSpeed = 1.6568542494923801;
    UTAssert(std::fabs(speed - expectedSpeed) <= maxError, "GetSpeed(t) failed.");

    double length = curve.GetLength(0.0, 0.5);
    double expectedLength = 0.78539816339744817;
    UTAssert(std::fabs(length - expectedLength) <= maxError, "GetLength(t0,t1) failed.");

    double totalLength = curve.GetTotalLength();
    double expectedTotalLength = 1.5707963267948966;
    UTAssert(std::fabs(totalLength - expectedTotalLength) <= maxError, "GetTotalLength() failed.");

    length = curve.GetLength(0.1, 0.2);
    expectedLength = 0.15278075799840354;
    UTAssert(std::fabs(length - expectedLength) <= maxError, "GetLength(t0,t1) failed.");

    double time = curve.GetTime(curve.GetLength(0.0, 0.5));
    double expectedTime = 0.5;
    UTAssert(std::fabs(time - expectedTime) <= maxError, "GetTime(length) failed.");

    time = curve.GetTime(curve.GetLength(0.0, 0.789));
    expectedTime = 0.78899999999999970;
    UTAssert(std::fabs(time - expectedTime) <= maxError, "GetTime(length) failed.");

    curve.SubdivideByTime(4, value.data());
    expectedValue[0] = { 1.0, 0.0 };
    expectedValue[1] = { 0.87226041910271701, 0.48904167641086821 };
    expectedValue[2] = { 0.48904167641086826, 0.87226041910271690 };
    expectedValue[3] = { 0.0, 1.0 };
    for (std::size_t i = 0; i < 4; ++i)
    {
        UTAssert(Length(value[i] - expectedValue[i]) <= maxError,
            "Invalid subdivision point " + std::to_string(i) + ".");
    }

    curve.SubdivideByLength(4, value.data());
    expectedValue[0] = { 1.0, 0.0 };
    expectedValue[1] = { 0.86602540378443882, 0.49999999999999989 };
    expectedValue[2] = { 0.50000000000000000, 0.86602540378443871 };
    expectedValue[3] = { 0.0, 1.0 };
    for (std::size_t i = 0; i < 4; ++i)
    {
        UTAssert(Length(value[i] - expectedValue[i]) <= maxError,
            "Invalid subdivision point " + std::to_string(i) + ".");
    }

    Vector2<double> position = curve.GetPosition(curve.GetTime(totalLength / 3.0f));
    Vector2<double> diff = value[1] - position;
    length = Length(diff);
    UTAssert(length <= maxError, "SubdivideByLength(n,points) failed.");

    position = curve.GetPosition(curve.GetTime(2.0f * totalLength / 3.0f));
    diff = value[2] - position;
    length = Length(diff);
    UTAssert(length <= maxError, "SubdivideByLength(n,points) failed.");

    position = curve.GetPosition(curve.GetTime(totalLength));
    diff = value[3] - position;
    length = Length(diff);
    UTAssert(length <= maxError, "SubdivideByLength(n,points) failed.");

    UTAssert(curve.GetNumControls() == input.degree + 1, "GetNumControls() failed.");

    auto const* temp = curve.GetControls();
    for (std::size_t i = 0; i < 3; ++i)
    {
        UTAssert(temp[i] == controls[i], "GetControls() failed at " + std::to_string(i) + ".");
    }

    double const* wtemp = curve.GetWeights();
    for (std::size_t i = 0; i < 3; ++i)
    {
        UTAssert(wtemp[i] == weights[i], "GetWeights() failed at " + std::to_string(i) + ".");
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/NURBSCurve.h>

namespace gtl
{
    template class NURBSCurve<float, 2>;
    template class NURBSCurve<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class NURBSCurve<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(NURBSCurve)
