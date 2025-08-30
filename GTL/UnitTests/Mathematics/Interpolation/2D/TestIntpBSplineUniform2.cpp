#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/2D/IntpBSplineUniform2.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/UnitTests/WICFileIONative.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpBSplineUniform2
    {
    public:
        UnitTestIntpBSplineUniform2();

    private:
        struct ColorType
        {
            // Create the color (0,0,0,0).
            ColorType()
                :
                color{ 0.0f, 0.0f, 0.0f, 0.0f }
            {
            }

            ColorType(Vector<float, 4> const& inColor)
                :
                color(inColor)
            {
            }

            ColorType operator+(ColorType const& other)
            {
                return color + other.color;
            }

            ColorType operator*(float scalar)
            {
                return color * scalar;
            }

            // The color is (r,g,b,a) with channel values in [0,255].
            Vector<float, 4> color;
        };

        struct Controls
        {
            using Type = ColorType;

            std::size_t GetSize(std::size_t i) const
            {
                return size[i];
            }

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

        using Interpolator = IntpBSplineUniform2<double, Controls>;

        void Test();
    };
}

UnitTestIntpBSplineUniform2::UnitTestIntpBSplineUniform2()
{
    UTInformation("Mathematics/Interpolation/2D/IntpBSplineUniform2");

    Test();
}

void UnitTestIntpBSplineUniform2::Test()
{
    std::string filename = "Mathematics/Interpolation/2D/Input/Magician.png";
    std::uint32_t format{};
    Controls controls{};
    WICFileIONative::Load(filename, format, controls.size[0], controls.size[1], controls.image);

    IntpBSplineUniform2<float, Controls> interp({ 3, 3 }, controls, ColorType{}, Interpolator::NO_CACHING);

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

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/2D/IntpBSplineUniform2.h>

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
            mDimensions{ 0, 0 },
            mImage{}
        {
        }

        std::size_t GetSize(std::size_t i) const
        {
            return mDimensions[i];
        }

        Type operator()(std::size_t const* tuple) const
        {
            return mImage[tuple[0] + mDimensions[0] * tuple[1]];
        }

        Type operator()(std::size_t x, std::size_t y) const
        {
            return mImage[x + mDimensions[0] * y];
        }

    private:
        std::array<std::size_t, 2> mDimensions;
        std::vector<T> mImage;
    };

    template class IntpBSplineUniform2<float, Controls<float>>;
    template class IntpBSplineUniform2<double, Controls<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpBSplineUniform2<Rational, Controls<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpBSplineUniform2)
