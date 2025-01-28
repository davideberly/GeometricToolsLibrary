#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/2D/Polygon2.h>
#include <numeric>
using namespace gtl;

namespace gtl
{
    class UnitTestPolygon2
    {
    public:
        UnitTestPolygon2();

    private:
        void Test();
    };
}

UnitTestPolygon2::UnitTestPolygon2()
{
    UTInformation("Mathematics/Primitives/2D/Polygon2");

    Test();
}

void UnitTestPolygon2::Test()
{
    double const maxError = 1e-12;

    std::size_t const n = 1024;
    double angle = C_TWO_PI<double> / static_cast<double>(n);
    double cs = std::cos(angle), sn = std::sin(angle);
    double b = std::sqrt((1.0 - cs) * (1.0 - cs) + sn * sn);
    double h = 0.5 * std::sqrt((1.0 + cs) * (1.0 + cs) + sn * sn);
    double trueArea = 0.5 * b * h * static_cast<double>(n);
    Vector2<double> trueAverage{ 0.0, 0.0 };
    double truePerimeter = static_cast<double>(n) * b;
    Vector2<double> average{};
    double perimeter{}, area{};

    Polygon2<double> polygon0(n, true);
    for (std::size_t i = 0; i < n; ++i)
    {
        double theta = angle * static_cast<double>(i);
        polygon0.vertices[i] = { std::cos(theta), std::sin(theta) };
    }
    average = polygon0.ComputeVertexAverage();
    perimeter = polygon0.ComputePerimeterLength();
    area = polygon0.ComputeArea();
    UTAssert(Length(average - trueAverage) <= maxError, "Invalid average.");
    UTAssert(std::fabs(perimeter - truePerimeter) <= maxError, "Invalid perimeter");
    UTAssert(std::fabs(area - trueArea) <= maxError, "Invalid area");

    std::vector<Vector2<double>> vertices(n);
    for (std::size_t i = 0; i < n; ++i)
    {
        double theta = angle * static_cast<double>(i);
        vertices[i] = { std::cos(theta), std::sin(theta) };
    }

    Polygon2<double> polygon1(vertices, true);
    average = polygon1.ComputeVertexAverage();
    perimeter = polygon1.ComputePerimeterLength();
    area = polygon1.ComputeArea();
    UTAssert(Length(average - trueAverage) <= maxError, "Invalid average.");
    UTAssert(std::fabs(perimeter - truePerimeter) <= maxError, "Invalid perimeter");
    UTAssert(std::fabs(area - trueArea) <= maxError, "Invalid area");

    std::vector<Vector2<double>> vertexPool(2 * n);
    std::vector<std::size_t> indices(n);
    for (std::size_t i = 0; i < n; ++i)
    {
        double theta = angle * static_cast<double>(i);
        vertexPool[2 * i] = { std::cos(theta), std::sin(theta) };
        vertexPool[2 * i + 1] = { 0.0, 0.0 };
        indices[i] = 2 * i;
    }

    Polygon2<double> polygon2(vertexPool, indices, true);
    average = polygon2.ComputeVertexAverage();
    perimeter = polygon2.ComputePerimeterLength();
    area = polygon2.ComputeArea();
    UTAssert(Length(average - trueAverage) <= maxError, "Invalid average.");
    UTAssert(std::fabs(perimeter - truePerimeter) <= maxError, "Invalid perimeter");
    UTAssert(std::fabs(area - trueArea) <= maxError, "Invalid area");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/2D/Polygon2.h>

namespace gtl
{
    template class Polygon2<float>;
    template class Polygon2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Polygon2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Polygon2)
