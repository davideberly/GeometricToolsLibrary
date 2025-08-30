#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/ND/IntpBSplineUniform.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/UnitTests/WICFileIONative.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpBSplineUniform
    {
    public:
        UnitTestIntpBSplineUniform();

    private:
        void Test1();
        void Test2();
        void Test3();
    };
}

UnitTestIntpBSplineUniform::UnitTestIntpBSplineUniform()
{
    UTInformation("Mathematics/Interpolation/ND/IntpBSplineUniform");

    Test1();
    Test2();
    Test3();
}

void UnitTestIntpBSplineUniform::Test1()
{
    class Controls
    {
    public:
        using Type = double;
        Controls(std::size_t numElements) : signal(numElements) {}
        std::size_t GetSize(std::size_t) const { return signal.size(); }
        Type operator() (std::size_t const* tuple) const { return signal[tuple[0]]; }
        Type operator() (std::size_t x) const { return signal[x]; }
        std::vector<double> signal;
    };

    using Interpolator = IntpBSplineUniform<double, Controls>;

    // Compare the results to the values on the graph in the PDF.
    Controls controls(6);
    controls.signal[0] = 1.0;
    controls.signal[1] = 2.0;
    controls.signal[2] = 1.5;
    controls.signal[3] = 0.25;
    controls.signal[4] = 1.25;
    controls.signal[5] = 1.25;

    Interpolator interp({ 2 }, controls, 0.0, Interpolator::NO_CACHING);

    double const epsilon = 1e-08;
    double result = interp.Evaluate({ 0 }, { -0.5 });  // 1.5000000000000000
    UTAssert(std::fabs(result - 1.5) <= epsilon, "Unexpected result.");

    result = interp.Evaluate({ 0 }, { 0.0 });   // 1.7499999999999998
    UTAssert(std::fabs(result - 1.75) <= epsilon, "Unexpected result.");

    result = interp.Evaluate({ 0 }, { 1.0 });   // 1.7500000000000000
    UTAssert(std::fabs(result - 1.75) <= epsilon, "Unexpected result.");

    result = interp.Evaluate({ 0 }, { 2.0 });   // 1.2500000000000002
    UTAssert(std::fabs(result - 1.25) <= epsilon, "Unexpected result.");

    result = interp.Evaluate({ 0 }, { 3.0 });   // 0.58333333333333348
    UTAssert(std::fabs(result - 7.0 / 12.0) <= epsilon, "Unexpected result.");

    result = interp.Evaluate({ 0 }, { 4.0 });   // 0.75000000000000000
    UTAssert(std::fabs(result - 0.75) <= epsilon, "Unexpected result.");

    result = interp.Evaluate({ 0 }, { 5.0 });   // 1.1944444444444444
    UTAssert(std::fabs(result - 43.0 / 36.0) <= epsilon, "Unexpected result.");

    result = interp.Evaluate({ 0 }, { 5.5 });   // 1.2500000000000000
    UTAssert(std::fabs(result - 1.25) <= epsilon, "Unexpected result.");

    // Compare derivative computations with finite difference estimates
    // to verify correctness of derivative computation.  Let's use t = 2.5
    // for the test case.
    double dfdt = interp.Evaluate({ 1 }, { 2.5 });  // -0.83333333333333326
    UTAssert(std::fabs(dfdt + 5.0 / 6.0) <= epsilon, "Unexpected result.");
    double h = 1e-06;
    double f0 = interp.Evaluate({ 0 }, { 2.5 - h });  // 0.87500083333316681
    double f1 = interp.Evaluate({ 0 }, { 2.5 + h });  // 0.87499916666716648
    double dfdtEstimate = (f1 - f0) / (2.0 * h);  // -0.83333300016086298
    UTAssert(std::fabs(dfdt - dfdtEstimate) <= 1e-06, "Unexpected result.");

    // The spline is piecewise quadratic, so the 2nd derivative is
    // piecewise constant.  The subdomains are [i,i+1), so the 2nd
    // derivative at i-epsilon is different from the 2nd derivative
    // at i.
    double d2fdt2 = interp.Evaluate({ 2 }, { 2.49999 }); // -0.33333333333333331
    UTAssert(std::fabs(d2fdt2 + 1.0 / 3.0) <= epsilon, "Unexpected result.");

    d2fdt2 = interp.Evaluate({ 2 }, { 2.499999 }); // -0.33333333333333331
    UTAssert(std::fabs(d2fdt2 + 1.0 / 3.0) <= epsilon, "Unexpected result.");

    d2fdt2 = interp.Evaluate({ 2 }, { 2.5 }); // 1.0000000000000000
    UTAssert(std::fabs(d2fdt2 - 1.0) <= epsilon, "Unexpected result.");

    d2fdt2 = interp.Evaluate({ 2 }, { 2.500001 }); // 1.0000000000000000
    UTAssert(std::fabs(d2fdt2 - 1.0) <= epsilon, "Unexpected result.");

    d2fdt2 = interp.Evaluate({ 2 }, { 2.50001 }); // 1.0000000000000000
    UTAssert(std::fabs(d2fdt2 - 1.0) <= epsilon, "Unexpected result.");
}

void UnitTestIntpBSplineUniform::Test2()
{
    struct ColorType
    {
        ColorType() : color{ 0.0f, 0.0f, 0.0f, 0.0f } {}
        ColorType(Vector<float, 4> const& inColor) : color(inColor) {}
        ColorType operator+(ColorType const& other) { return color + other.color; }
        ColorType operator*(float scalar) { return color * scalar; }
        Vector<float, 4> color;
    };

    struct Controls
    {
        using Type = ColorType;
        std::size_t GetSize(std::size_t i) const { return size[i]; }

        Type operator()(std::size_t const* tuple) const
        {
            std::uint32_t const* texels = reinterpret_cast<std::uint32_t const*>(image.data());
            std::uint32_t texel = texels[tuple[0] + size[0] * tuple[1]];
            Vector<float, 4> color{};
            color[0] = static_cast<float>(texel & 0x000000FF);
            color[1] = static_cast<float>((texel & 0x0000FF00) >> 8);
            color[2] = static_cast<float>((texel & 0x00FF0000) >> 16);
            color[3] = static_cast<float>((texel & 0xFF000000) >> 24);
            return color;
        }

        Type operator()(std::size_t x, std::size_t y) const
        {
            std::uint32_t const* texels = reinterpret_cast<std::uint32_t const*>(image.data());
            std::uint32_t texel = texels[x + size[0] * y];
            Vector<float, 4> color{};
            color[0] = static_cast<float>(texel & 0x000000FF);
            color[1] = static_cast<float>((texel & 0x0000FF00) >> 8);
            color[2] = static_cast<float>((texel & 0x00FF0000) >> 16);
            color[3] = static_cast<float>((texel & 0xFF000000) >> 24);
            return color;
        }

        std::array<std::size_t, 2> size;
        std::vector<std::uint8_t> image;
    };

    using Interpolator = IntpBSplineUniform<double, Controls>;

    std::string filename = "Mathematics/Interpolation/2D/Input/Magician.png";
    std::uint32_t format{};
    Controls controls{};
    WICFileIONative::Load(filename, format, controls.size[0], controls.size[1], controls.image);

    IntpBSplineUniform<float, Controls> interp({ 3, 3 }, controls, ColorType{}, Interpolator::NO_CACHING);

    std::vector<std::uint8_t> outImage(4 * controls.size[0] * controls.size[1], 0xFF);
    std::uint32_t* texels = reinterpret_cast<std::uint32_t*>(outImage.data());
    for (std::size_t y = 0; y < controls.size[1]; ++y)
    {
        float t1 = static_cast<float>(y);
        for (std::size_t x = 0; x < controls.size[0]; ++x)
        {
            float t0 = static_cast<float>(x);
            ColorType result = interp.Evaluate({ 0, 0 }, { t0, t1 });
            std::uint32_t r = static_cast<std::uint32_t>(result.color[0]);
            std::uint32_t g = static_cast<std::uint32_t>(result.color[1]);
            std::uint32_t b = static_cast<std::uint32_t>(result.color[2]);
            std::uint32_t a = static_cast<std::uint32_t>(result.color[3]);
            std::uint32_t& color = texels[x + controls.size[0] * y];
            color = r | (g << 8) | (b << 16) | (a << 24);
        }
    }

    filename = "Mathematics/Interpolation/2D/Output/MagicianBSpline.png";
    WICFileIONative::SaveToPNG(filename, format, controls.size[0], controls.size[1], outImage.data());
}

void UnitTestIntpBSplineUniform::Test3()
{
    struct Controls
    {
        using Type = float;
        std::size_t GetSize(std::size_t i) const { return size[i]; }

        Type operator()(std::size_t const* tuple) const
        {
            std::size_t index = tuple[0] + size[0] * (tuple[1] + size[1] * tuple[2]);
            std::uint8_t value = image[index];
            return static_cast<float>(value);
        }

        float operator()(std::size_t x, std::size_t y, std::size_t z) const
        {
            std::size_t index = x + size[0] * (y + size[1] * z);
            std::uint8_t value = image[index];
            return static_cast<float>(value);
        }

        std::array<std::size_t, 3> size;
        std::vector<std::uint8_t> image;
    };

    using Interpolator = IntpBSplineUniform<float, Controls>;

    std::string filename = "Mathematics/Interpolation/3D/Input/molecule97x97x116.binary";
    std::ifstream input(filename, std::ios::binary);

    Controls controls{};
    input.read(reinterpret_cast<char*>(&controls.size[0]), sizeof(uint32_t));
    input.read(reinterpret_cast<char*>(&controls.size[1]), sizeof(uint32_t));
    input.read(reinterpret_cast<char*>(&controls.size[2]), sizeof(uint32_t));
    controls.image.resize(controls.size[0] * controls.size[1] * controls.size[2]);
    input.read(reinterpret_cast<char*>(controls.image.data()), controls.image.size());
    input.close();

    // Write the input 3D image as an array of 2D slices.
    std::size_t textureWidth = 16 * controls.size[0];
    std::size_t textureHeight = 8 * controls.size[1];
    std::vector<std::uint8_t> texture(textureWidth* textureHeight, 0);
    auto* texels = texture.data();
    for (std::size_t yTile = 0, z = 0; yTile < 8; ++yTile)
    {
        std::size_t yMin = yTile * controls.size[1];
        for (std::size_t xTile = 0; xTile < 16; ++xTile)
        {
            std::size_t xMin = xTile * controls.size[0];
            for (std::size_t y = 0; y < controls.size[1]; ++y)
            {
                for (std::size_t x = 0; x < controls.size[0]; ++x)
                {
                    std::size_t src = x + controls.size[0] * (y + controls.size[1] * z);
                    std::size_t trg = (xMin + x) + textureWidth * (yMin + y);
                    texels[trg] = controls.image[src];
                }
            }

            if (++z == controls.size[2])
            {
                break;
            }
        }

        if (z == controls.size[2])
        {
            break;
        }
    }
    filename = "Mathematics/Interpolation/3D/Output/moleculeOriginal.png";
    WICFileIONative::SaveToPNG(filename, WICFileIONative::R8, textureWidth, textureHeight, texture.data());

    float zero = 0.0f;
    Interpolator interp({ 3, 3, 2 }, controls, zero, Interpolator::NO_CACHING);
    std::vector<uint8_t> output(controls.image.size());
    for (std::size_t z = 0, i = 0; z < controls.size[2]; ++z)
    {
        float t2 = static_cast<float>(z);
        for (std::size_t y = 0; y < controls.size[1]; ++y)
        {
            float t1 = static_cast<float>(y);
            for (std::size_t x = 0; x < controls.size[0]; ++x, ++i)
            {
                float t0 = static_cast<float>(x);
                output[i] = static_cast<uint8_t>(interp.Evaluate({ 0, 0, 0 }, { t0, t1, t2 }));
            }
        }
    }

    // Write the output 3D image as an array of 2D slices.
    std::memset(texels, 0, texture.size());
    for (std::size_t yTile = 0, z = 0; yTile < 8; ++yTile)
    {
        std::size_t yMin = yTile * controls.size[1];
        for (std::size_t xTile = 0; xTile < 16; ++xTile)
        {
            std::size_t xMin = xTile * controls.size[0];
            for (std::size_t y = 0; y < controls.size[1]; ++y)
            {
                for (std::size_t x = 0; x < controls.size[0]; ++x)
                {
                    std::size_t src = x + controls.size[0] * (y + controls.size[1] * z);
                    std::size_t trg = (xMin + x) + textureWidth * (yMin + y);
                    texels[trg] = output[src];
                }
            }

            if (++z == controls.size[2])
            {
                break;
            }
        }

        if (z == controls.size[2])
        {
            break;
        }
    }
    filename = "Mathematics/Interpolation/3D/Output/moleculeBSpline.png";
    WICFileIONative::SaveToPNG(filename, WICFileIONative::R8, textureWidth, textureHeight, texture.data());
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/ND/IntpBSplineUniform.h>

namespace gtl
{
    template <typename T>
    class Controls
    {
    public:
        // The type of the control points.
        using Type = T;

        Controls()
            :
            mSignal{}
        {
        }

        std::size_t GetSize(std::size_t) const
        {
            return mSignal.size();
        }

        Type operator() (std::size_t const* tuple) const
        {
            return mSignal[tuple[0]];
        }

        Type operator() (std::size_t x) const
        {
            return mSignal[x];
        }

    private:
        std::vector<T> mSignal;
    };

    template class IntpBSplineUniformN<float, Controls<float>>;
    template class IntpBSplineUniformN<double, Controls<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpBSplineUniformN<Rational, Controls<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpBSplineUniform)
