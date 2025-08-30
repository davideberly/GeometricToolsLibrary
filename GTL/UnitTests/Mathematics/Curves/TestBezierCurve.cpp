#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/BezierCurve.h>
#include "DrawLine.h"
#if defined(GTL_USE_MSWINDOWS)
#include "WICFileIONative.h"
#endif
using namespace gtl;

namespace gtl
{
    class UnitTestBezierCurve
    {
    public:
        UnitTestBezierCurve();

    private:
        void Test();
        void TestDeferred();
    };
}

UnitTestBezierCurve::UnitTestBezierCurve()
{
    UTInformation("Mathematics/Curves/BezierCurve");

    Test();
    TestDeferred();
}

void UnitTestBezierCurve::Test()
{
    std::size_t constexpr degree = 5;
    std::array<Vector2<double>, degree + 1> controls{};
    controls[0] = { 0.0, 0.0 };
    controls[1] = { 1.0, 0.0 };
    controls[2] = { 2.0, 1.0 };
    controls[3] = { 1.0, 2.0 };
    controls[4] = { 0.0, 1.0 };
    controls[5] = controls[0];

    BezierCurve<double, 2> curve(degree, controls.data());
    std::size_t constexpr numSamples = 1024;
    std::vector<Vector2<double>> X(numSamples), DXDT(numSamples);
    std::vector<Vector2<double>> D2XDT2(numSamples), D3XDT3(numSamples);
    std::array<Vector2<double>, 4> jet{};
    for (std::size_t i = 0; i < numSamples; ++i)
    {
        double t = static_cast<double>(i) / static_cast<double>(numSamples);
        curve.Evaluate(t, 3, jet.data());
        X[i] = jet[0];
        DXDT[i] = jet[1];
        D2XDT2[i] = jet[2];
        D3XDT3[i] = jet[3];
    }

    std::size_t const size = 256;
    std::vector<std::uint32_t> canvas(size * size, 0xFFFFFFFF);
    std::int32_t isize = static_cast<std::int32_t>(size);
    auto DrawPixel = [isize, &canvas](std::int32_t x, std::int32_t y)
    {
        if (0 <= x && x < isize && 0 <= y && y < isize)
        {
            std::int32_t index = x + isize * y;
            canvas[static_cast<std::size_t>(index)] = 0xFF000000;
        }
    };

    std::int32_t x0{}, y0{}, x1{}, y1{};
    x0 = 0;
    y0 = 0;
    for (std::size_t i = 1; i < numSamples; ++i)
    {
        x1 = static_cast<std::int32_t>(256.0 * X[i][0] / 2.0);
        y1 = static_cast<std::int32_t>(256.0 * X[i][1] / 2.0);
        DrawLine(x0, y0, x1, y1, DrawPixel);
        x0 = x1;
        y0 = y1;
    }

#if defined(GTL_USE_MSWINDOWS)
    WICFileIONative::SaveToPNG("Mathematics/Curves/Output/BezierCurveX.png",
        WICFileIONative::R8G8B8A8, size, size,
        reinterpret_cast<std::uint8_t*>(canvas.data()));
#endif
}

void UnitTestBezierCurve::TestDeferred()
{
    std::size_t constexpr degree = 5;
    std::array<Vector2<double>, degree + 1> controls{};
    controls[0] = { 0.0, 0.0 };
    controls[1] = { 1.0, 0.0 };
    controls[2] = { 2.0, 1.0 };
    controls[3] = { 1.0, 2.0 };
    controls[4] = { 0.0, 1.0 };
    controls[5] = controls[0];

    BezierCurve<double, 2> curve(degree, nullptr);

    for (std::size_t i = 0; i < controls.size(); ++i)
    {
        curve.SetControl(i, controls[i]);
    }

    std::size_t constexpr numSamples = 1024;
    std::vector<Vector2<double>> X(numSamples), DXDT(numSamples);
    std::vector<Vector2<double>> D2XDT2(numSamples), D3XDT3(numSamples);
    std::array<Vector2<double>, 4> jet{};
    for (std::size_t i = 0; i < numSamples; ++i)
    {
        double t = static_cast<double>(i) / static_cast<double>(numSamples);
        curve.Evaluate(t, 3, jet.data());
        X[i] = jet[0];
        DXDT[i] = jet[1];
        D2XDT2[i] = jet[2];
        D3XDT3[i] = jet[3];
    }

    std::size_t const size = 256;
    std::vector<std::uint32_t> canvas(size * size, 0xFFFFFFFF);
    std::int32_t isize = static_cast<std::int32_t>(size);
    auto DrawPixel = [isize, &canvas](std::int32_t x, std::int32_t y)
    {
        if (0 <= x && x < isize && 0 <= y && y < isize)
        {
            std::int32_t index = x + isize * y;
            canvas[static_cast<std::size_t>(index)] = 0xFF000000;
        }
    };

    std::int32_t x0{}, y0{}, x1{}, y1{};
    x0 = 0;
    y0 = 0;
    for (std::size_t i = 1; i < numSamples; ++i)
    {
        x1 = static_cast<std::int32_t>(256.0 * X[i][0] / 2.0);
        y1 = static_cast<std::int32_t>(256.0 * X[i][1] / 2.0);
        DrawLine(x0, y0, x1, y1, DrawPixel);
        x0 = x1;
        y0 = y1;
    }

#if defined(GTL_USE_MSWINDOWS)
    WICFileIONative::SaveToPNG("Mathematics/Curves/Output/BezierCurveXDeferred.png",
        WICFileIONative::R8G8B8A8, size, size,
        reinterpret_cast<std::uint8_t*>(canvas.data()));
#endif
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/BezierCurve.h>

namespace gtl
{
    template class BezierCurve<float, 2>;
    template class BezierCurve<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class BezierCurve<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BezierCurve)
