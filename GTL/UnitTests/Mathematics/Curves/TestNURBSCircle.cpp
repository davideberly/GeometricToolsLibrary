#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Mathematics/Curves/NURBSCircle.h>
#include "DrawLine.h"
#if defined(GTL_USE_MSWINDOWS)
#include "WICFileIONative.h"
#endif
using namespace gtl;

namespace gtl
{
    class UnitTestNURBSCircle
    {
    public:
        UnitTestNURBSCircle();

    private:
        void Test();

        void Draw(NURBSCurve<double, 2> const* curve, double maxAngle,
            std::int32_t iXCenter, std::int32_t iYCenter, std::int32_t iRadius, std::size_t size,
            std::vector<std::uint32_t>& canvas);
    };
}

UnitTestNURBSCircle::UnitTestNURBSCircle()
{
    UTInformation("Mathematics/Curves/NURBSCircle [tested by GTL/VisualTests/Curves/NURBSCurveExample]");

    Test();
}

void UnitTestNURBSCircle::Test()
{
    std::int32_t imageSize = 512;
    std::int32_t const dx = imageSize / 4;
    std::int32_t const dy = imageSize / 4;
    std::int32_t const radius = imageSize / 8;

    std::size_t size = static_cast<std::size_t>(imageSize);
    std::vector<std::uint32_t> canvas(size * size, 0xFFFFFFFF);

    NURBSQuarterCircleDegree2<double> quarterCircleDegree2{};
    NURBSQuarterCircleDegree4<double> quarterCircleDegree4{};
    NURBSHalfCircleDegree3<double> halfCircleDegree3{};
    NURBSFullCircleDegree3<double> fullCircleDegree3{};

    Draw(&quarterCircleDegree2, C_PI_DIV_2<double>, dx, dy, radius, size, canvas);
    Draw(&quarterCircleDegree4, C_PI_DIV_2<double>, 3 * dx, dy, radius, size, canvas);
    Draw(&halfCircleDegree3, C_PI<double>, dx, 3 * dy, radius, size, canvas);
    Draw(&fullCircleDegree3, C_TWO_PI<double>, 3 * dx, 3 * dy, radius, size, canvas);

#if defined(GTL_USE_MSWINDOWS)
    WICFileIONative::SaveToPNG("Mathematics/Curves/Output/NURBSCircle.png",
        WICFileIONative::R8G8B8A8, (std::uint32_t)size, (std::uint32_t)size,
        reinterpret_cast<std::uint8_t*>(canvas.data()));
#endif
}

void UnitTestNURBSCircle::Draw(NURBSCurve<double, 2> const* curve, double maxAngle,
    std::int32_t iXCenter, std::int32_t iYCenter, std::int32_t iRadius, std::size_t size,
    std::vector<std::uint32_t>& canvas)
{
    Vector2<double> const center
    {
        static_cast<double>(iXCenter),
        static_cast<double>(iYCenter)
    };

    double const radius = static_cast<double>(iRadius);

    std::int32_t x0{}, y0{}, x1{}, y1{};

    // Draw the true circle in green with thickness to allow contrast between
    // the true circle and the NURBS circle.
    std::int32_t imageSize = static_cast<std::int32_t>(size);
    auto DrawThickPixel = [imageSize, &canvas](std::int32_t x, std::int32_t y)
    {
        if (1 <= x && x < imageSize - 1 && 1 <= y && y < imageSize - 1)
        {
            for (std::int32_t dy = -1; dy <= 1; ++dy)
            {
                for (std::int32_t dx = -1; dx <= 1; ++dx)
                {
                    std::int32_t index = (x + dx) + imageSize * (y + dy);
                    canvas[static_cast<std::size_t>(index)] = 0xFF00FF00u;
                }
            }
        }
    };

    std::int32_t const numSamples = 1024;
    x0 = static_cast<std::int32_t>(std::lrint(center[0] + radius));
    y0 = static_cast<std::int32_t>(std::lrint(center[1]));
    double const divisor = static_cast<double>(numSamples - 1);
    for (std::int32_t i = 1; i < numSamples; ++i)
    {
        double angle = maxAngle * static_cast<double>(i) / divisor;
        double cs = std::cos(angle);
        double sn = std::sin(angle);
        x1 = static_cast<std::int32_t>(std::lrint(center[0] + radius * cs));
        y1 = static_cast<std::int32_t>(std::lrint(center[1] + radius * sn));
        DrawLine(x0, y0, x1, y1, DrawThickPixel);
        x0 = x1;
        y0 = y1;
    }

    // Draw the NURBS circle in blue.
    auto DrawPixel = [imageSize, &canvas](std::int32_t x, std::int32_t y)
    {
        if (0 <= x && x < imageSize && 0 <= y && y < imageSize)
        {
            std::int32_t index = x + imageSize * y;
            canvas[static_cast<std::size_t>(index)] = 0xFFFF0000u;
        }
    };

    Vector2<double> values[4];
    curve->Evaluate(0.0f, 0, values);
    x0 = static_cast<std::int32_t>(std::lrint(center[0] + radius * values[0][0]));
    y0 = static_cast<std::int32_t>(std::lrint(center[1] + radius * values[0][1]));
    for (std::int32_t i = 1; i < numSamples; ++i)
    {
        double t = static_cast<double>(i) / divisor;
        curve->Evaluate(t, 0, values);
        x1 = static_cast<std::int32_t>(std::lrint(center[0] + radius * values[0][0]));
        y1 = static_cast<std::int32_t>(std::lrint(center[1] + radius * values[0][1]));
        DrawLine(x0, y0, x1, y1, DrawPixel);
        x0 = x1;
        y0 = y1;
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/NURBSCircle.h>

namespace gtl
{
    template class NURBSQuarterCircleDegree2<float>;
    template class NURBSQuarterCircleDegree4<float>;
    template class NURBSHalfCircleDegree3<float>;
    template class NURBSFullCircleDegree3<float>;
    template class NURBSCircularArcDegree2<float>;

    template class NURBSQuarterCircleDegree2<double>;
    template class NURBSQuarterCircleDegree4<double>;
    template class NURBSHalfCircleDegree3<double>;
    template class NURBSFullCircleDegree3<double>;
    template class NURBSCircularArcDegree2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class NURBSQuarterCircleDegree2<Rational>;
    template class NURBSQuarterCircleDegree4<Rational>;
    template class NURBSHalfCircleDegree3<Rational>;
    template class NURBSFullCircleDegree3<Rational>;
    template class NURBSCircularArcDegree2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(NURBSCircle)
