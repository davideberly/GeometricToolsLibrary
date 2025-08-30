#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/3D/IntpBSplineUniform3.h>
#include <GTL/UnitTests/WICFileIONative.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpBSplineUniform3
    {
    public:
        UnitTestIntpBSplineUniform3();

    private:
        struct Controls
        {
            using Type = float;

            std::size_t GetSize(std::size_t i) const
            {
                return size[i];
            }

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

        using Interpolator = IntpBSplineUniform3<float, Controls>;

        void Test();
    };
}

UnitTestIntpBSplineUniform3::UnitTestIntpBSplineUniform3()
{
    UTInformation("Mathematics/Interpolation/3D/IntpBSplineUniform3");

    Test();
}

void UnitTestIntpBSplineUniform3::Test()
{
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
    std::vector<std::uint8_t> texture(textureWidth * textureHeight, 0);
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
#include <GTL/Mathematics/Interpolation/3D/IntpBSplineUniform3.h>

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
            mDimensions{ 0, 0, 0 },
            mVolume{}
        {
        }

        std::size_t GetSize(std::size_t i) const
        {
            return mDimensions[i];
        }

        Type operator()(std::size_t const* tuple) const
        {
            return mVolume[tuple[0] + mDimensions[0] * (tuple[1] + mDimensions[1] * tuple[2])];
        }

        Type operator()(std::size_t x, std::size_t y, std::size_t z) const
        {
            return mVolume[x + mDimensions[0] * (y + mDimensions[1] * z)];
        }

    private:
        std::array<std::size_t, 3> mDimensions;
        std::vector<T> mVolume;
    };

    template class IntpBSplineUniform3<float, Controls<float>>;
    template class IntpBSplineUniform3<double, Controls<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpBSplineUniform3<Rational, Controls<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpBSplineUniform3)
