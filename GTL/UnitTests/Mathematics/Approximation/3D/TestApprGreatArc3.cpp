#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/3D/ApprGreatArc3.h>
#include <fstream>
using namespace gtl;

//#define INTERNAL_GENERATE_DATA
#if defined(INTERNAL_GENERATE_DATA)
#include <random>
#endif

namespace gtl
{
    class UnitTestApprGreatArc3
    {
    public:
        UnitTestApprGreatArc3();

    private:
        void Test();
    };
}

UnitTestApprGreatArc3::UnitTestApprGreatArc3()
{
    UTInformation("Mathematics/Approximation/3D/ApprGreatArc3");

    Test();
}

void UnitTestApprGreatArc3::Test()
{
    Vector3<double> N(1.0, 2.0, 3.0), U{}, V{};
    ComputeOrthonormalBasis(1, N, U, V);

    std::vector<Vector3<double>> points(1024);
#if defined(INTERNAL_GENERATE_DATA)
    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0, 1.0);
    for (auto& p : points)
    {
        double angle = C_PI_DIV_4<double> * urd(dre);
        double cosAngle = std::cos(angle);
        double sinAngle = std::sin(angle);
        p = cosAngle * U + sinAngle * V + 1.0e-03 * urd(dre) * N;
        Normalize(p);
    }
    std::ofstream outFile("Mathematics/Approximation/3D/Input/ApprGreatArc3Points.binary", std::ios::binary);
    UTAssert(outFile, "Failed to open outFile file.");
    outFile.write(reinterpret_cast<char const*>(points.data()), points.size() * sizeof(Vector3<double>));
    outFile.close();

    // For Mathematica visualization.
    outFile.open("Mathematics/Approximation/3D/Input/ApprGreatArc3Points.txt");
    UTAssert(outFile, "Failed to open outFile file.");
    for (auto const& p : points)
    {
        outFile << std::setprecision(17) << p[0] << "," << p[1] << "," << p[2] << std::endl;
    }
    outFile.close();
#else
    std::ifstream inFile("Mathematics/Approximation/3D/Input/ApprGreatArc3Points.binary", std::ios::binary);
    UTAssert(inFile, "Failed to open input file.");
    inFile.read(reinterpret_cast<char*>(points.data()), points.size() * sizeof(Vector3<double>));
    inFile.close();
#endif

    // NOTE: There are 2 possible normals based on how the eigensolver
    // processes the covariance matrix of the points. The output 'normal'
    // might lead to Dot(N, normal) < 0.
    Vector3<double> normal{}, arcEnd0{}, arcEnd1{};
    ApprGreatArc3<double>::Fit(points, normal, arcEnd0, arcEnd1);

    // Compute the least-squares error.
    double error = 0.0;
    for (auto const& p : points)
    {
        double cosAngle = Dot(normal, p);
        error += cosAngle * cosAngle;
    }
    error = std::sqrt(error / static_cast<double>(points.size()));
    UTAssert(error <= 1.0e-03, "The error is too large.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/3D/ApprGreatArc3.h>

namespace gtl
{
    template class ApprGreatArc3<float>;
    template class ApprGreatArc3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprGreatArc3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprGreatArc3)
