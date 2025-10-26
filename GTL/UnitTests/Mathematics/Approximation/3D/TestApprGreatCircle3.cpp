#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/3D/ApprGreatCircle3.h>
#include <fstream>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestApprGreatCircle3
    {
    public:
        UnitTestApprGreatCircle3();

    private:
        void Test();
    };
}

UnitTestApprGreatCircle3::UnitTestApprGreatCircle3()
{
    UTInformation("Mathematics/Approximation/3D/ApprGreatCircle3");

    Test();
}

void UnitTestApprGreatCircle3::Test()
{
    Vector3<double> N(1.0, 2.0, 3.0), U{}, V{};
    ComputeOrthonormalBasis(1, N, U, V);

    std::vector<Vector3<double>> points(1024);
#if 0
    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0, 1.0);
    for (auto& p : points)
    {
        double angle = C_PI<double> * urd(dre);
        double cosAngle = std::cos(angle);
        double sinAngle = std::sin(angle);
        p = cosAngle * U + sinAngle * V + 1.0e-03 * urd(dre) * N;
        Normalize(p);
    }
    std::ofstream output("Mathematics/Approximation/3D/Input/ApprGreatCircle3Points.binary", std::ios::binary);
    output.write(reinterpret_cast<char const*>(points.data()), points.size() * sizeof(Vector3<double>));
    output.close();

    output.open("Mathematics/Approximation/3D/Input/ApprGreatCircle3Points.txt");
    for (auto const& p : points)
    {
        output << std::setprecision(17) << p[0] << "," << p[1] << "," << p[2] << std::endl;
    }
    output.close();
#else
    std::ifstream input("Mathematics/Approximation/3D/Input/ApprGreatCircle3Points.binary", std::ios::binary);
    input.read(reinterpret_cast<char*>(points.data()), points.size() * sizeof(Vector3<double>));
    input.close();
#endif

    // NOTE: There are 2 possible normals based on how the eigensolver
    // processes the covariance matrix of the points. The output 'normal'
    // might lead to Dot(N, normal) < 0.
    Vector3<double> normal{};
    ApprGreatCircle3<double>::Fit(points, normal);

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
#include <GTL/Mathematics/Approximation/3D/ApprGreatCircle3.h>

namespace gtl
{
    template class ApprGreatCircle3<float>;
    template class ApprGreatCircle3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprGreatCircle3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprGreatCircle3)
