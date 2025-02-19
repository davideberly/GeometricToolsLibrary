#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/TCBSplineCurve.h>
#include "DrawLine.h"
#include <random>
#if defined(GTL_USE_MSWINDOWS)
#include "WICFileIONative.h"
#endif
using namespace gtl;

namespace gtl
{
    class UnitTestTCBSplineCurve
    {
    public:
        UnitTestTCBSplineCurve();

    private:
        void Test();
        void Draw(std::string const& name, TCBSplineCurve<double, 2> const& spline);
    };
}

UnitTestTCBSplineCurve::UnitTestTCBSplineCurve()
{
    UTInformation("Mathematics/Curves/TCBSplineCurve");

    Test();
}

void UnitTestTCBSplineCurve::Test()
{
    std::vector<Vector2<double>> point =
    {
        Vector2<double>{  14.0, 256.0 },
        Vector2<double>{  14.0,  86.0 },
        Vector2<double>{ 142.0,  86.0 },
        Vector2<double>{ 142.0, 256.0 },
        Vector2<double>{ 270.0, 256.0 },
        Vector2<double>{ 270.0,  86.0 }
    };

    // Catmull-Rom curve
    std::vector<double> time = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0 };
    std::vector<double> tension = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    std::vector<double> continuity = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    std::vector<double> bias = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    std::vector<double> lambda{};

    // The filename prefix is of the form TCB. The T, C and B values are
    // P (value is +1), N (value is -1) or Z (value is 0).
    TCBSplineCurve<double, 2> spline(point, time, tension, continuity, bias,
        lambda, nullptr, nullptr);
    Draw("TCBSplineCurveZZZ", spline);

    // The tension, continuity and bias are modified at point[3].

    // tension = 1
    tension[3] = 1.0;
    spline = TCBSplineCurve<double, 2>(point, time, tension, continuity, bias,
        lambda, nullptr, nullptr);
    Draw("TCBSplineCurvePZZ", spline);

    // tension = -1
    tension[3] = -1.0;
    spline = TCBSplineCurve<double, 2>(point, time, tension, continuity, bias,
        lambda, nullptr, nullptr);
    Draw("TCBSplineCurveNZZ", spline);
    tension[3] = 0.0;

    // continuity = 1
    continuity[3] = 1.0;
    spline = TCBSplineCurve<double, 2>(point, time, tension, continuity, bias,
        lambda, nullptr, nullptr);
    Draw("TCBSplineCurveZPZ", spline);

    // continuity = -1
    continuity[3] = -1.0;
    spline = TCBSplineCurve<double, 2>(point, time, tension, continuity, bias,
        lambda, nullptr, nullptr);
    Draw("TCBSplineCurveZNZ", spline);
    continuity[3] = 0.0;

    // bias = 1
    bias[3] = 1.0;
    spline = TCBSplineCurve<double, 2>(point, time, tension, continuity, bias,
        lambda, nullptr, nullptr);
    Draw("TCBSplineCurveZZP", spline);

    // bias = -1
    bias[3] = -1.0;
    spline = TCBSplineCurve<double, 2>(point, time, tension, continuity, bias,
        lambda, nullptr, nullptr);
    Draw("TCBSplineCurveZZN", spline);
    bias[3] = 0.0;

    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(0.1, 2.0);
    lambda.resize(point.size());
    for (auto& value : lambda)
    {
        value = urd(dre);
    }

    Vector<double, 2> firstOutTangent{ 1.0, -1.0 };
    Vector<double, 2> lastInTangent{ 1.0, 1.0 };

    tension[3] = 0.125;
    continuity[3] = -0.5;
    bias[3] = 0.75;
    spline = TCBSplineCurve<double, 2>(point, time, tension, continuity, bias,
        lambda, &firstOutTangent, &lastInTangent);
    Draw("TCBSplineCurveSpecial", spline);
}

void UnitTestTCBSplineCurve::Draw(std::string const& name,
    TCBSplineCurve<double, 2> const& spline)
{
    std::vector<Vector2<double>> samples(2048);
    double tDelta = spline.GetTimes().back() / static_cast<double>(samples.size());
    std::array<Vector2<double>, 4> jet{};
    for (std::size_t i = 0; i < samples.size(); ++i)
    {
        double t = tDelta * static_cast<double>(i);
        spline.Evaluate(t, 3, jet.data());
        samples[i] = jet[0];
    }

    std::size_t xSize = 300, ySize = 300;
    std::vector<std::uint32_t> canvas(xSize * ySize, 0xFFFFFFFF);
    auto DrawPixel = [xSize, ySize, &canvas](std::int32_t x, std::int32_t y)
    {
        std::size_t sx = static_cast<std::size_t>(x);
        std::size_t sy = static_cast<std::size_t>(y);
        if (0 <= sx && sx < xSize && 0 <= sy && sy < ySize)
        {
            std::size_t index = sx + xSize * (ySize - 1 - sy);
            canvas[index] = 0xFF000000;
        }
    };

    std::int32_t x0{}, y0{}, x1{}, y1{};
    x0 = static_cast<std::int32_t>(samples[0][0]);
    y0 = static_cast<std::int32_t>(samples[0][1]);
    for (std::size_t i = 1; i < samples.size(); ++i)
    {
        x1 = static_cast<std::int32_t>(samples[i][0]);
        y1 = static_cast<std::int32_t>(samples[i][1]);
        DrawLine(x0, y0, x1, y1, DrawPixel);
        x0 = x1;
        y0 = y1;
}

#if defined(GTL_USE_MSWINDOWS)
    WICFileIONative::SaveToPNG("Mathematics/Curves/_Output/" + name + ".png",
        WICFileIONative::R8G8B8A8, xSize, ySize,
        reinterpret_cast<std::uint8_t*>(canvas.data()));
#endif
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/TCBSplineCurve.h>

namespace gtl
{
    template class TCBSplineCurve<float, 2>;
    template class TCBSplineCurve<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TCBSplineCurve<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(TCBSplineCurve)
