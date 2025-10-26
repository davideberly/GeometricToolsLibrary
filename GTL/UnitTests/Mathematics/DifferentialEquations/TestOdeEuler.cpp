#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/DifferentialEquations/OdeEuler.h>
#include "DrawLine.h"
#if defined(GTL_USE_MSWINDOWS)
#include "WICFileIONative.h"
#endif
#include <fstream>
using namespace gtl;

// TODO: Avoid including support for PNG files. Instead, compare the
// drawn image to a known image (of type, say, Image2D<std::uint32_t>).

namespace gtl
{
    class UnitTestOdeEuler
    {
    public:
        UnitTestOdeEuler();

    private:
        void Test1();
        void Test2();
        void TestDynamic();
    };
}

UnitTestOdeEuler::UnitTestOdeEuler()
{
    UTInformation("Mathematics/DifferentialEquations/OdeEuler");

    Test1();
    Test2();
    TestDynamic();
}

void UnitTestOdeEuler::Test1()
{
    // dx/dt = F(t,x) = -xt, x(0) = 3, solution is x(t) = 3*exp(-t^2/2).
    // x(1) = 3*exp(-1/2) = 1.8195919791379002708113986049735...
    auto F = [](double const& t, double const& x)
    {
        return -t * x;
    };

    double const tDelta = 1.0 / 1024.0;
    OdeEuler<double, 1> solver(tDelta, F);

    double t = 0.0, x = 3.0;
    for (std::size_t i = 1; i <= 1024; ++i)
    {
        solver.Update(t, x, t, x);
    }
    // x = 1.8201846800182502
    double error = std::fabs(x - 3.0 * std::exp(-0.5));
    UTAssert(
        error <= 1e-03,
        "Too much error in Euler's method.");
}

void UnitTestOdeEuler::Test2()
{
    // d2x/dt2 + K*sin(x) = 0, x(0) = 0.1, x'(0) = 1
    // Define y = x'(t); then
    //   dx/dt = y,         x(0) = 0.1
    //   dy/dt = -K*sin(x), y(0) = 1
    double K = 1.0;
    auto F = [&K](double const&, Vector<double, 2> const& x)
    {
        return Vector<double, 2>{ x[1], -K * std::sin(x[0]) };
    };

    double const tDelta = 0.1;
    OdeEuler<double, 2> solver(tDelta, F);

    std::size_t numOutputs = 512;
    double t = 0.0;
    Vector<double, 2> u{ 0.1, 1.0 };
    std::vector<double> output(numOutputs);
    for (std::size_t i = 0; i < output.size(); ++i)
    {
        solver.Update(t, u, t, u);
        output[i] = u[0];
    }

    // Write the approximation solution as text.
    std::ofstream outFile("Mathematics/DifferentialEquations/Output/PendulumEuler.txt");
    UTAssert(outFile, "Failed to open output file.");
    for (std::size_t i = 0; i < output.size(); ++i)
    {
        outFile << "i = " << i << ", " << output[i] << std::endl;
    }
    outFile.close();

    // Draw the approximate solution.
    std::int32_t size = static_cast<std::int32_t>(numOutputs);
    std::vector<std::uint32_t> canvas(numOutputs * numOutputs, 0xFFFFFFFF);
    auto DrawPixel = [size, &canvas](std::int32_t x, std::int32_t y)
    {
        if (0 <= x && x < size && 0 <= y && y < size)
        {
            std::int32_t index = x + size * y;
            canvas[static_cast<std::size_t>(index)] = 0xFF000000;
        }
    };

    double y = 256.0 * (output[0] + 3.0) / 6.0;
    std::int32_t iY0 = size - 1 - static_cast<std::int32_t>(y);
    for (std::int32_t i = 1; i < size; ++i)
    {
        y = 256.0 * (output[i] + 3.0) / 6.0;
        std::int32_t iY1 = size - 1 - static_cast<std::int32_t>(y);
        DrawLine(i - 1, iY0, i, iY1, DrawPixel);
        iY0 = iY1;
    }
#if defined(GTL_USE_MSWINDOWS)
    WICFileIONative::SaveToPNG("Mathematics/DifferentialEquations/Output/PendulumEuler.png",
        WICFileIONative::R8G8B8A8, numOutputs, numOutputs,
        reinterpret_cast<std::uint8_t*>(canvas.data()));
#endif
}

void UnitTestOdeEuler::TestDynamic()
{
    // x"(t) + K*sin(x(t)) = 0, x(0) = 0.1, x'(0) = 1
    // Define y = x'(t); then
    //   dx/dt = y,         x(0) = 0.1
    //   dy/dt = -K*sin(x), y(0) = 1
    double K = 1.0;
    auto F = [&K](double const&, Vector<double> const& x)
    {
        return Vector<double>{ x[1], -K * std::sin(x[0]) };
    };

    double const tDelta = 0.1;
    OdeEuler<double> solver(tDelta, F);

    std::size_t numOutputs = 512;
    double t = 0.0;
    Vector<double> u{ 0.1, 1.0 };
    std::vector<double> output(numOutputs);
    for (std::size_t i = 0; i < output.size(); ++i)
    {
        solver.Update(t, u, t, u);
        output[i] = u[0];
    }

    // Write the approximation solution as text.
    std::ofstream outFile("Mathematics/DifferentialEquations/Output/PendulumEulerD.txt");
    UTAssert(outFile, "Failed to open output file.");
    for (std::size_t i = 0; i < output.size(); ++i)
    {
        outFile << "i = " << i << ", " << output[i] << std::endl;
    }
    outFile.close();

    // Draw the approximate solution.
    std::int32_t size = static_cast<std::int32_t>(numOutputs);
    std::vector<std::uint32_t> canvas(numOutputs * numOutputs, 0xFFFFFFFF);
    auto DrawPixel = [size, &canvas](std::int32_t x, std::int32_t y)
    {
        if (0 <= x && x < size && 0 <= y && y < size)
        {
            std::int32_t index = x + size * y;
            canvas[static_cast<std::size_t>(index)] = 0xFF000000;
        }
    };

    double y = 256.0 * (output[0] + 3.0) / 6.0;
    std::int32_t iY0 = size - 1 - static_cast<std::int32_t>(y);
    for (std::int32_t i = 1; i < size; ++i)
    {
        y = 256.0 * (output[i] + 3.0) / 6.0;
        std::int32_t iY1 = size - 1 - static_cast<std::int32_t>(y);
        DrawLine(i - 1, iY0, i, iY1, DrawPixel);
        iY0 = iY1;
    }
#if defined(GTL_USE_MSWINDOWS)
    WICFileIONative::SaveToPNG("Mathematics/DifferentialEquations/Output/PendulumEulerD.png",
        WICFileIONative::R8G8B8A8, numOutputs, numOutputs,
        reinterpret_cast<std::uint8_t*>(canvas.data()));
#endif
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/DifferentialEquations/OdeEuler.h>

namespace gtl
{
    template class OdeEuler<float, 1>;
    template class OdeEuler<float, 2>;
    template class OdeEuler<float>;

    template class OdeEuler<double, 1>;
    template class OdeEuler<double, 2>;
    template class OdeEuler<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class OdeEuler<Rational, 1>;
    template class OdeEuler<Rational, 2>;
    template class OdeEuler<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(OdeEuler)
