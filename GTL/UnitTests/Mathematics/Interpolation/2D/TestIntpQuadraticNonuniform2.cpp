#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/2D/IntpQuadraticNonuniform2.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpQuadraticNonuniform2
    {
    public:
        UnitTestIntpQuadraticNonuniform2();

    private:
        void TestSingleTriangle();
        void TestTwoTriangles();
    };
}

UnitTestIntpQuadraticNonuniform2::UnitTestIntpQuadraticNonuniform2()
{
    UTInformation("Mathematics/Interpolation/2D/IntpQuadraticNonuniform2");

    //TestSingleTriangle();
    TestTwoTriangles();
}

void UnitTestIntpQuadraticNonuniform2::TestSingleTriangle()
{
    std::vector<Vector2<double>> positions(3);
    positions[0] = { 0.0, 0.0 };
    positions[1] = { 1.0, 0.0 };
    positions[2] = { 0.0, 1.0 };
    std::vector<std::array<std::size_t, 3>> triangles(1);
    triangles[0] = std::array<std::size_t, 3>{ 0, 1, 2 };
    std::size_t constexpr numThreads = 0;
    PlanarMesh<double> mesh(positions, triangles, numThreads);

    std::vector<double> F = { 1.0, 2.0, 1.5 };
    std::vector<double> DFDX = { 0.123, -0.789, -0.654 };
    std::vector<double> DFDY = { 0.456, 0.321, -0.111 };
    IntpQuadraticNonuniform2<double> interpolator(mesh, F, DFDX, DFDY, true, 0);
    Vector2<double> point{};
    double Finterp{}, DFDXinterp{}, DFDYinterp{};

    // These all agree with Mathematica. The ordering is based on the point
    // being in triangle 0, 1, 2, 3, 4, or 5.
    point[0] = 0.3232233;
    point[1] = 0.0732233;
    interpolator.Evaluate(point, Finterp, DFDXinterp, DFDYinterp);
    // F = 1.3827170471191836, DFDX = 1.8178351620886088, DFDY = 1.4301612136034907
    point[0] = 0.698223;
    point[1] = 0.0732233;
    interpolator.Evaluate(point, Finterp, DFDXinterp, DFDYinterp);
    // F = 2.0419362615072520, DFDX = 0.80933712091139043, DFDY = 0.90825077607511140
    point[0] = 0.573223;
    point[1] = 0.323223;
    interpolator.Evaluate(point, Finterp, DFDXinterp, DFDYinterp);
    // F = 2.0086039678442527, DFDX = 0.72583796199066986, DFDY = -0.35115926900932914
    point[0] = 0.198223;
    point[1] = 0.698223;
    interpolator.Evaluate(point, Finterp, DFDXinterp, DFDYinterp);
    // F = 1.5234096937192523, DFDX = 0.44483796199067177, DFDY = -0.18153426900932854
    point[0] = 0.0732233;
    point[1] = 0.573223;
    interpolator.Evaluate(point, Finterp, DFDXinterp, DFDYinterp);
    // F = 1.3994582767660342, DFDX = 0.55591315879511760, DFDY = 0.56533845841817776
    point[0] = 0.0732233;
    point[1] = 0.198223;
    interpolator.Evaluate(point, Finterp, DFDXinterp, DFDYinterp);
    // F = 1.1373549941184307, DFDX = 0.73149885271494197, DFDY = 0.61421265138182335

    std::ofstream output("Mathematics/Interpolation/2D/Support/CendesWong1.txt");
    std::size_t numX = 128, numY = 128;
    for (std::size_t x = 0; x <= numX; ++x)
    {
        point[0] = static_cast<double>(x) / static_cast<double>(numX);
        for (std::size_t y = numY - x; y != std::numeric_limits<std::size_t>::max(); --y)
        {
            point[1] = static_cast<double>(y) / static_cast<double>(numY);
            if (interpolator.Evaluate(point, Finterp, DFDXinterp, DFDYinterp))
            {
                output << point[0] << ", " << point[1] << ", " << Finterp << std::endl;
            }
        }
    }
    output.close();
}

void UnitTestIntpQuadraticNonuniform2::TestTwoTriangles()
{
    std::vector<Vector2<double>> positions(4);
    positions[0] = { 0.0, 0.0 };
    positions[1] = { 1.0, 0.0 };
    positions[2] = { 0.0, 1.0 };
    positions[3] = { 1.0, 1.0 };
    std::vector<std::array<std::size_t, 3>> triangles(2);
    triangles[0] = std::array<std::size_t, 3>{ 0, 1, 2 };
    triangles[1] = std::array<std::size_t, 3>{ 1, 3, 2 };
    std::size_t constexpr numThreads = 0;
    PlanarMesh<double> mesh(positions, triangles, numThreads);

    std::vector<double> F = { 1.0, 2.0, 1.5, 2.5 };
    std::vector<double> DFDX = { 0.123, -0.789, -0.654, 0.75 };
    std::vector<double> DFDY = { 0.456, 0.321, -0.111, 0.5 };
    IntpQuadraticNonuniform2<double> interpolator(mesh, F, DFDX, DFDY, true, numThreads);
    Vector2<double> point{};
    double Finterp{}, DFDXinterp{}, DFDYinterp{};

    // The plot in Mathematica appears to be correct.
    std::ofstream output("Mathematics/Interpolation/2D/Support/CendesWong2.txt");
    std::size_t numX = 128, numY = 128;
    for (std::size_t x = 0; x <= numX; ++x)
    {
        point[0] = static_cast<double>(x) / static_cast<double>(numX);
        for (std::size_t y = 0; y <= numY; ++y)
        {
            point[1] = static_cast<double>(y) / static_cast<double>(numY);
            if (interpolator.Evaluate(point, Finterp, DFDXinterp, DFDYinterp))
            {
                output << point[0] << ", " << point[1] << ", " << Finterp << std::endl;
            }
        }
    }
    output.close();
}

#else

#include <GTL/Mathematics/Interpolation/2D/IntpQuadraticNonuniform2.h>

namespace gtl
{
    template class IntpQuadraticNonuniform2<float>;
    template class IntpQuadraticNonuniform2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpQuadraticNonuniform2)
