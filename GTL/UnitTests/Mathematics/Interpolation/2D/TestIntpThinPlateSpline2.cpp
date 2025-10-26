#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/2D/IntpThinPlateSpline2.h>
#include <fstream>
using namespace gtl;

//#define INTERNAL_GENERATE_DATA

namespace gtl
{
    class UnitTestIntpThinPlateSpline2
    {
    public:
        UnitTestIntpThinPlateSpline2();

    private:
        void Test();
    };
}

UnitTestIntpThinPlateSpline2::UnitTestIntpThinPlateSpline2()
{
    UTInformation("Mathematics/Interpolation/2D/IntpThinPlateSpline2");

    Test();
}

void UnitTestIntpThinPlateSpline2::Test()
{
    // Tabulated data on a 3x3 regular grid, points (x,y,f(x,y)).
    std::size_t const numPoints = 9;
    std::vector<Vector3<double>> points(numPoints);
    points[0] = { 0.0, 0.0, 1.0 };
    points[1] = { 0.5, 0.0, 2.0 };
    points[2] = { 1.0, 0.0, 3.0 };
    points[3] = { 0.0, 0.5, 3.0 };
    points[4] = { 0.5, 0.5, 2.0 };
    points[5] = { 1.0, 0.5, 1.0 };
    points[6] = { 0.0, 1.0, 1.0 };
    points[7] = { 0.5, 1.0, 2.0 };
    points[8] = { 1.0, 1.0, 3.0 };

    // Resample on a 7x7 regular grid.
    std::size_t const numResample = 6;
    std::size_t const numSamples = (numResample + 1) * (numResample + 1);
    double const invResample = 1.0 / static_cast<double>(numResample);
    std::vector<double> output(numSamples);
    double smooth{}, x{}, y{};

    // No smoothing, exact interpolation at grid points.
    smooth = 0.0;
    IntpThinPlateSpline2<double> noSmooth(points, smooth, false);
    for (std::size_t j = 0, index = 0; j <= numResample; ++j)
    {
        y = invResample * static_cast<double>(j);
        for (std::size_t i = 0; i <= numResample; ++i, ++index)
        {
            x = invResample * static_cast<double>(i);
            output[index] = noSmooth(x, y);
        }
    }
    double outFunctional = noSmooth.ComputeFunctional();

    std::string prefix = "Mathematics/Interpolation/2D/Input/";
#if defined(INTERNAL_GENERATE_DATA)
    std::ofstream outFile(prefix + "Input2NoSmooth.binary", std::ios::binary);
    UTAssert(outFile, "Failed to open output file.");
    outFile.write(reinterpret_cast<char const*>(output.data()), output.size() * sizeof(double));
    outFile.write(reinterpret_cast<char const*>(&outFunctional), sizeof(double));
    outFile.close();
#else
    std::vector<double> input(numSamples);
    double inFunctional{}, error{};
    std::ifstream inFile(prefix + "Input2NoSmooth.binary", std::ios::binary);
    UTAssert(inFile, "Failed to open input file.");
    inFile.read(reinterpret_cast<char*>(input.data()), input.size() * sizeof(double));
    inFile.read(reinterpret_cast<char*>(&inFunctional), sizeof(double));
    inFile.close();
    for (std::size_t index = 0; index < numSamples; ++index)
    {
        error = std::fabs(output[index] - input[index]);
        UTAssert(error <= 1e-16, "Invalid sample difference: " +
            "output[index] = " + std::to_string(output[index]) + ", "
            "input[index] = " + std::to_string(input[index]));
    }
    error = std::fabs(outFunctional - inFunctional);
    UTAssert(error <= 1e-16, "Invalid functional difference");
#endif

    // Increasing amounts of smoothing.
    std::array<std::int32_t, 6> power{ -1, 0, 1, 2, 3, 4 };
    smooth = 0.1;
    for (std::size_t k = 0; k < 6; ++k, smooth *= 10.0)
    {
        IntpThinPlateSpline2<double> spline(points, smooth, false);
        for (std::size_t j = 0, index = 0; j <= numResample; ++j)
        {
            y = invResample * static_cast<double>(j);
            for (std::size_t i = 0; i <= numResample; ++i, ++index)
            {
                x = invResample * static_cast<double>(i);
                output[index] = spline(x, y);
            }
        }
        outFunctional = spline.ComputeFunctional();

#if defined(INTERNAL_GENERATE_DATA)
        outFile.open(prefix + "Input2Power" + std::to_string(power[k]) + ".binary", std::ios::binary);
        UTAssert(outFile, "Failed to open output file.");
        outFile.write(reinterpret_cast<char const*>(output.data()), output.size() * sizeof(double));
        outFile.write(reinterpret_cast<char const*>(&outFunctional), sizeof(double));
        outFile.close();
#else
        inFile.open(prefix + "Input2Power" + std::to_string(power[k]) + ".binary", std::ios::binary);
        UTAssert(inFile, "Failed to open input file.");
        inFile.read(reinterpret_cast<char*>(input.data()), input.size() * sizeof(double));
        inFile.read(reinterpret_cast<char*>(&inFunctional), sizeof(double));
        inFile.close();
        for (std::size_t index = 0; index < numSamples; ++index)
        {
            error = std::fabs(output[index] - input[index]);
            UTAssert(error <= 1e-16, "Invalid sample difference: " +
                "output[index] = " + std::to_string(output[index]) + ", "
                "input[index] = " + std::to_string(input[index]));
        }
        error = std::fabs(outFunctional - inFunctional);
        UTAssert(error <= 1e-16, "Invalid functional difference");
#endif
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/2D/IntpThinPlateSpline2.h>

namespace gtl
{
    template class IntpThinPlateSpline2<float>;
    template class IntpThinPlateSpline2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpThinPlateSpline2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpThinPlateSpline2)
