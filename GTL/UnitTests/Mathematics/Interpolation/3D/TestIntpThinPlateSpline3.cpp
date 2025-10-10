#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/3D/IntpThinPlateSpline3.h>
#include <fstream>
using namespace gtl;

//#define INTERNAL_GENERATE_DATA

namespace gtl
{
    class UnitTestIntpThinPlateSpline3
    {
    public:
        UnitTestIntpThinPlateSpline3();

    private:
        void Test();
    };
}

UnitTestIntpThinPlateSpline3::UnitTestIntpThinPlateSpline3()
{
    UTInformation("Mathematics/Interpolation/3D/IntpThinPlateSpline3");

    Test();
}

void UnitTestIntpThinPlateSpline3::Test()
{
    // Tabulated data on a 3x3x3 regular grid, points (x,y,z,f(x,y,z)).
    std::size_t const numPoints = 27;
    std::vector<Vector4<double>> points(numPoints);
    points[ 0] = { 0.0, 0.0, 0.0, 0.13547700429678045 };
    points[ 1] = { 0.5, 0.0, 0.0, 0.83500858999457950 };
    points[ 2] = { 1.0, 0.0, 0.0, 0.96886777112423128 };
    points[ 3] = { 0.0, 0.5, 0.0, 0.22103404298270490 };
    points[ 4] = { 0.5, 0.5, 0.0, 0.30816705050700322 };
    points[ 5] = { 1.0, 0.5, 0.0, 0.54722059636785192 };
    points[ 6] = { 0.0, 1.0, 0.0, 0.18838197604718110 };
    points[ 7] = { 0.5, 1.0, 0.0, 0.99288130191780655 };
    points[ 8] = { 1.0, 1.0, 0.0, 0.99646132554800870 };
    points[ 9] = { 0.0, 0.0, 0.5, 0.96769493701050247 };
    points[10] = { 0.5, 0.0, 0.5, 0.72583896321188957 };
    points[11] = { 1.0, 0.0, 0.5, 0.98110969177693885 };
    points[12] = { 0.0, 0.5, 0.5, 0.10986175084420635 };
    points[13] = { 0.5, 0.5, 0.5, 0.79810585674954937 };
    points[14] = { 1.0, 0.5, 0.5, 0.29702944955795074 };
    points[15] = { 0.0, 1.0, 0.5, 0.47834844193156423 };
    points[16] = { 0.5, 1.0, 0.5, 0.11246451605618024 };
    points[17] = { 1.0, 1.0, 0.5, 0.63976335709815269 };
    points[18] = { 0.0, 0.0, 1.0, 0.87843064539884386 };
    points[19] = { 0.5, 0.0, 1.0, 0.50366267770516970 };
    points[20] = { 1.0, 0.0, 1.0, 0.79792861516022595 };
    points[21] = { 0.0, 0.5, 1.0, 0.36129400134918077 };
    points[22] = { 0.5, 0.5, 1.0, 0.21192433239173358 };
    points[23] = { 1.0, 0.5, 1.0, 0.68135953856026588 };
    points[24] = { 0.0, 1.0, 1.0, 0.39873851991229103 };
    points[25] = { 0.5, 1.0, 1.0, 0.74064724467642140 };
    points[26] = { 1.0, 1.0, 1.0, 0.47475868061723470 };

    // Resample on a 7x7x7 regular grid.
    std::size_t const numResample = 6;
    std::size_t const numSamples = (numResample + 1) * (numResample + 1) * (numResample + 1);
    double const invResample = 1.0 / static_cast<double>(numResample);
    std::vector<double> output(numSamples);
    double smooth{}, x{}, y{}, z{};

    // No smoothing, exact interpolation at grid points.
    smooth = 0.0;
    IntpThinPlateSpline3<double> noSmooth(points, smooth, false);
    for (std::size_t k = 0, index = 0; k <= numResample; ++k)
    {
        z = invResample * static_cast<double>(k);
        for (std::size_t j = 0; j <= numResample; ++j)
        {
            y = invResample * static_cast<double>(j);
            for (std::size_t i = 0; i <= numResample; ++i, ++index)
            {
                x = invResample * static_cast<double>(i);
                output[index] = noSmooth(x, y, z);
            }
        }
    }
    double outFunctional = noSmooth.ComputeFunctional();

    std::string prefix = "Mathematics/Interpolation/3D/Input/";
#if defined(INTERNAL_GENERATE_DATA)
    std::ofstream outfile(prefix + "Input3NoSmooth.binary", std::ios::binary);
    outfile.write(reinterpret_cast<char const*>(output.data()), output.size() * sizeof(double));
    outfile.write(reinterpret_cast<char const*>(&outFunctional), sizeof(double));
    outfile.close();
#else
    std::vector<double> input(numSamples);
    double inFunctional{}, error{};
    std::ifstream infile(prefix + "Input3NoSmooth.binary", std::ios::binary);
    infile.read(reinterpret_cast<char*>(input.data()), input.size() * sizeof(double));
    infile.read(reinterpret_cast<char*>(&inFunctional), sizeof(double));
    infile.close();
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
    for (int32_t ell = 0; ell < 6; ++ell, smooth *= 10.0)
    {
        IntpThinPlateSpline3<double> spline(points, smooth, false);
        for (std::size_t k = 0, index = 0; k < 6; ++k)
        {
            z = invResample * static_cast<double>(k);
            for (std::size_t j = 0; j <= numResample; ++j)
            {
                y = invResample * static_cast<double>(j);
                for (std::size_t i = 0; i <= numResample; ++i, ++index)
                {
                    x = invResample * static_cast<double>(i);
                    output[index] = spline(x, y, z);
                }
            }
        }
        outFunctional = spline.ComputeFunctional();

#if defined(INTERNAL_GENERATE_DATA)
        outfile.open(prefix + "Input3Power" + std::to_string(power[ell]) + ".binary", std::ios::binary);
        outfile.write(reinterpret_cast<char const*>(output.data()), output.size() * sizeof(double));
        outfile.write(reinterpret_cast<char const*>(&outFunctional), sizeof(double));
        outfile.close();
#else
        infile.open(prefix + "Input3Power" + std::to_string(power[ell]) + ".binary", std::ios::binary);
        infile.read(reinterpret_cast<char*>(input.data()), input.size() * sizeof(double));
        infile.read(reinterpret_cast<char*>(&inFunctional), sizeof(double));
        infile.close();
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
#include <GTL/Mathematics/Interpolation/3D/IntpThinPlateSpline3.h>

namespace gtl
{
    template class IntpThinPlateSpline3<float>;
    template class IntpThinPlateSpline3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpThinPlateSpline3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpThinPlateSpline3)
