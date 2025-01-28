#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/BSplineCurve.h>
using namespace gtl;

namespace gtl
{
    class UnitTestBSplineCurve
    {
    public:
        UnitTestBSplineCurve();

    private:
        void Test();
    };
}

UnitTestBSplineCurve::UnitTestBSplineCurve()
{
    UTInformation("Mathematics/Curves/BSplineCurve [tested by GTL/VisualTests/Curves/BSplineCurveFitter]");

    Test();
}

void UnitTestBSplineCurve::Test()
{
    double constexpr maxError = 1e-7;

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

    double cs = -0.5, sn = sqrt(3.0) / 2.0;
    std::vector<Vector2<double>> controls(input.numControls);
    controls[0] = { 1.0, 0.0 };
    controls[1] = { cs, sn };
    controls[2] = { cs, -sn };

    BSplineCurve<double, 2> curve(input, controls.data());
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
        Vector2<double>{ -0.12500000000000000, 0.21650634706020355 },
        Vector2<double>{ -1.5000000000000000, -0.86602538824081421 },
        Vector2<double>{ 3.0000000000000000, -5.1961523294448853 },
        Vector2<double>{ 0.0, 0.0 }
    };
    curve.Evaluate(0.5, 3, value.data());
    for (std::size_t i = 0; i < 4; ++i)
    {
        double error = Length(value[i] - expectedValue[i]);
        UTAssert(error <= maxError, "Invalid point " + std::to_string(i));
    }

    value[0] = curve.GetPosition(0.5);
    expectedValue[0] = { -0.12500000000000000, 0.21650634706020355 };
    UTAssert(Length(value[0] - expectedValue[0]) <= maxError, "Invalid position.");

    value[0] = curve.GetTangent(0.5);
    expectedValue[0] = { -0.86602540767034475, -0.49999999326941313 };
    UTAssert(Length(value[0] - expectedValue[0]) <= maxError, "Invalid tangent.");

    double speed = curve.GetSpeed(0.5);
    double expectedSpeed = 1.7320507997970651;
    UTAssert(std::fabs(speed - expectedSpeed) <= maxError, "GetSpeed(t) failed.");

    double length = curve.GetLength(0.0, 0.5);
    double expectedLength = 1.1952648756070534;
    UTAssert(std::fabs(length - expectedLength) <= maxError, "GetLength(t0,t1) failed.");

    double totalLength = curve.GetTotalLength();
    double expectedTotalLength = 2.3905297356706128;
    UTAssert(std::fabs(totalLength - expectedTotalLength) <= maxError, "GetTotalLength() failed.");

    length = curve.GetLength(0.1, 0.2);
    expectedLength = 0.27243704697236598;
    UTAssert(std::fabs(length - expectedLength) <= maxError, "GetLength(t0,t1) failed.");

    double time = curve.GetTime(curve.GetLength(0.0, 0.5));
    double expectedTime = 0.50000000000000011;
    UTAssert(std::fabs(time - expectedTime) <= maxError, "GetTime(length) failed.");

    time = curve.GetTime(curve.GetLength(0.0, 0.789));
    expectedTime = 0.78899999999999992;
    UTAssert(std::fabs(time - expectedTime) <= maxError, "GetTime(length) failed.");

    curve.SubdivideByTime(4, value.data());
    expectedValue[0] = { 1.0, 0.0 };
    expectedValue[1] = { 0.16666666666666674, 0.28867512941360479 };
    expectedValue[2] = { -0.33333333333333331, 5.5511151231257827e-17 };
    expectedValue[3] = { -0.50000000000000000, -0.86602538824081421 };
    for (std::size_t i = 0; i < 4; ++i)
    {
        UTAssert(Length(value[i] - expectedValue[i]) <= maxError,
            "Invalid subdivision point " + std::to_string(i) + ".");
    }

    curve.SubdivideByLength(4, value.data());
    expectedValue[0] = { 1.0, 0.0 };
    expectedValue[1] = { 0.26089307633760966, 0.28328306074117349 };
    expectedValue[2] = { -0.37577686489884965, -0.084298486551903584 };
    expectedValue[3] = { -0.50000000000000000, -0.86602538824081421 };
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
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/BSplineCurve.h>

namespace gtl
{
    template class BSplineCurve<float, 2>;
    template class BSplineCurve<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class BSplineCurve<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BSplineCurve)
