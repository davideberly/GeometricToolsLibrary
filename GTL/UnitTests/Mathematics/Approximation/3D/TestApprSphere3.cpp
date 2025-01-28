#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/3D/ApprSphere3.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestApprSphere3
    {
    public:
        UnitTestApprSphere3();

    private:
        void Test();
    };
}

UnitTestApprSphere3::UnitTestApprSphere3()
{
    UTInformation("Mathematics/Approximation/3D/ApprSphere3");

    Test();
}

void UnitTestApprSphere3::Test()
{
    Vector3<double> center = { 10.0, 20.0, 30.0 };
    double radius = 9.0;
    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0, 1.0);
    std::vector<Vector3<double>> points(1000);
    for (std::size_t i = 0; i < points.size(); ++i)
    {
        double x = urd(dre);
        double y = (1.0 - x) * urd(dre);
        double z = 1.0 - x - y;
        Vector3<double> direction = { x, y, z };
        Normalize(direction);
        double amplitude = radius + 0.0001 * urd(dre);
        points[i][0] = center[0] + 0.0001 * urd(dre) + amplitude * direction[0];
        points[i][1] = center[1] + 0.0001 * urd(dre) + amplitude * direction[1];
        points[i][2] = center[2] + 0.0001 * urd(dre) + amplitude * direction[2];
    }

    ApprSphere3<double> fitter{};
    Sphere3<double> estimate{};

    std::size_t maxIterations = static_cast<std::size_t>(1) << 16;
    double epsilon = 1e-06;
    std::size_t numIterations{};
    bool success{};
    double error{};
    
    numIterations = fitter.FitUsingLengths(points, maxIterations, false, estimate, epsilon);
    error = Length(center - estimate.center);
    UTAssert(error <= 8e-05, "Invalid FitUsingLengths center estimate, " + std::to_string(error));
    error = std::fabs(radius - estimate.radius);
    UTAssert(error <= 5e-05, "Invalid FitUsingLengths radius estimate, " + std::to_string(error));

    numIterations = fitter.FitUsingLengths(points, maxIterations, true, estimate, epsilon);
    error = Length(center - estimate.center);
    UTAssert(error <= 4e-05, "Invalid FitUsingLengths center estimate, " + std::to_string(error));
    error = std::fabs(radius - estimate.radius);
    UTAssert(error <= 3e-05, "Invalid FitUsingLengths radius estimate, " + std::to_string(error));

    success = fitter.FitUsingSquaredLengths(points, estimate);
    error = Length(center - estimate.center);
    UTAssert(error <= 4e-05, "Invalid FitUsingSquaredLengths center estimate, " + std::to_string(error));
    error = std::fabs(radius - estimate.radius);
    UTAssert(error <= 3e-05, "Invalid FitUsingSquaredLengths radius estimate, " + std::to_string(error));

    (void)numIterations;
    (void)success;
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/3D/ApprSphere3.h>

namespace gtl
{
    template class ApprSphere3<float>;
    template class ApprSphere3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprSphere3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprSphere3)
