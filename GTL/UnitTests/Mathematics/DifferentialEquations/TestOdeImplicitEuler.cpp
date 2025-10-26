#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/DifferentialEquations/OdeImplicitEuler.h>
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
    class UnitTestOdeImplicitEuler
    {
    public:
        UnitTestOdeImplicitEuler();

    private:
        void Test1();
        void Test2();
        void TestDynamic();
    };
}

UnitTestOdeImplicitEuler::UnitTestOdeImplicitEuler()
{
    UTInformation("Mathematics/DifferentialEquations/OdeImplicitEuler");

    Test1();
    Test2();
    TestDynamic();
}

void UnitTestOdeImplicitEuler::Test1()
{
    // dx/dt = F(t,x) = -xt, x(0) = 3, solution is x(t) = 3*exp(-t^2/2).
    // x(1) = 3*exp(-1/2) = 1.8195919791379002708113986049735...
    auto F = [](double const& t, double const& x)
    {
        return -t * x;
    };
    auto DF = [](double const& t, double const&)
    {
        return -t;
    };

    double const tDelta = 1.0 / 1024.0;
    std::size_t const numNewtonIterations = 1024;
    OdeImplicitEuler<double, 1> solver(tDelta, F, DF, numNewtonIterations);

    double t = 0.0, x = 3.0;
    for (std::size_t i = 1; i <= 1024; ++i)
    {
        solver.Update(t, x, t, x);
    }
    // x = 1.8190000495013103
    double error = std::fabs(x - 3.0 * std::exp(-0.5));
    UTAssert(
        error <= 1e-03,
        "Too much error in Euler's method.");
}

void UnitTestOdeImplicitEuler::Test2()
{
    // x"(t) + K*sin(x(t)) = 0, x(0) = 0.1, x'(0) = 1
    // Define y = x'(t); then
    //   dx/dt = y,         x(0) = 0.1
    //   dy/dt = -K*sin(x), y(0) = 1
    double K = 1.0;
    auto F = [&K](double const&, Vector<double, 2> const& x)
    {
        return Vector<double, 2>{ x[1], -K * std::sin(x[0]) };
    };
    auto DF = [&K](double const&, Vector<double, 2> const& x)
    {
        return Matrix<double, 2, 2>{{ 0.0, 1.0 }, { -K * std::cos(x[0]), 0.0 }};
    };

    double const tDelta = 0.1;
    std::size_t const numNewtonIterations = 32;
    OdeImplicitEuler<double, 2> solver(tDelta, F, DF, numNewtonIterations);

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
    std::ofstream outFile("Mathematics/DifferentialEquations/Output/PendulumImplicitEuler.txt");
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
    WICFileIONative::SaveToPNG("Mathematics/DifferentialEquations/Output/PendulumImplicitEuler.png",
        WICFileIONative::R8G8B8A8, numOutputs, numOutputs,
        reinterpret_cast<std::uint8_t*>(canvas.data()));
#endif
}

void UnitTestOdeImplicitEuler::TestDynamic()
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
    auto DF = [&K](double const&, Vector<double> const& x)
    {
        return Matrix<double>{ { 0.0, 1.0 }, { -K * std::cos(x[0]), 0.0 }};
    };

    double const tDelta = 0.1;
    std::size_t const numNewtonIterations = 32;
    OdeImplicitEuler<double> solver(tDelta, F, DF, numNewtonIterations);

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
    std::ofstream outFile("Mathematics/DifferentialEquations/Output/PendulumImplicitEulerD.txt");
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
    WICFileIONative::SaveToPNG("Mathematics/DifferentialEquations/Output/PendulumImplicitEulerD.png",
        WICFileIONative::R8G8B8A8, numOutputs, numOutputs,
        reinterpret_cast<std::uint8_t*>(canvas.data()));
#endif
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/DifferentialEquations/OdeImplicitEuler.h>

namespace gtl
{
    template class OdeImplicitEuler<float, 1>;
    template class OdeImplicitEuler<float, 2>;
    template class OdeImplicitEuler<float>;

    template class OdeImplicitEuler<double, 1>;
    template class OdeImplicitEuler<double, 2>;
    template class OdeImplicitEuler<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class OdeImplicitEuler<Rational, 1>;
    template class OdeImplicitEuler<Rational, 2>;
    template class OdeImplicitEuler<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(OdeImplicitEuler)
