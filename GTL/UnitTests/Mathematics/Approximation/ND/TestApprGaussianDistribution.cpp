#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/ND/ApprGaussianDistribution.h>
#include <GTL/Mathematics/MatrixAnalysis/GaussianElimination.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestApprGaussianDistribution
    {
    public:
        UnitTestApprGaussianDistribution();

    private:
        void Test2Vector();
        void Test3Vector();
        void TestDynamicVector();
    };
}

UnitTestApprGaussianDistribution::UnitTestApprGaussianDistribution()
{
    UTInformation("Mathematics/Approximation/ND/ApprGaussianDistribution");

    Test2Vector();
    Test3Vector();
    TestDynamicVector();
}

void UnitTestApprGaussianDistribution::Test2Vector()
{
    std::vector<Vector<double, 2>> points(1024);
    std::ifstream inFile("Mathematics/Approximation/2D/Input/RandomUnitPoints2D_Double_1024.binary", std::ios::binary);
    UTAssert(inFile, "Failed to open input file.");
    inFile.read((char*)points.data(), points.size() * sizeof(points[0]));
    inFile.close();

    Vector<double, 2> mean;
    std::array<double, 2> eigenvalue{};
    std::array<Vector<double, 2>, 2> eigenvector{};
    (void)ApprGaussianDistribution<double, 2>::Fit(points, mean, eigenvalue, eigenvector);

    Vector<double, 2> expectedMean{
        0.98697055157543789,
        1.8354747720667344
    };

    Vector<double, 2> expectedEigenvalue{
        2.0026994704603212,
        12.370867279674776
    };

    Vector<double, 2> expectedEigenvector0{
        0.70261083405224756,
        0.71157432210023208
    };

    Vector<double, 2> expectedEigenvector1{
        -0.71157432210023208,
        0.70261083405224756
    };

    double error0 = Length(mean - expectedMean);
    double error1 = Length(eigenvector[0] - expectedEigenvector0);
    double error2 = Length(eigenvector[1] - expectedEigenvector1);

    UTAssert(
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15,
        "Invalid Gaussian distribution.");

    double det = eigenvector[0][0] * eigenvector[1][1] - eigenvector[0][1] * eigenvector[1][0];
    error0 = std::fabs(det - 1.0);
    UTAssert(
        error0 <= 1e-15,
        "Matrix is not a rotation.");
}

void UnitTestApprGaussianDistribution::Test3Vector()
{
    std::vector<Vector<double, 3>> points(1024);
    std::ifstream inFile("Mathematics/Approximation/3D/Input/RandomUnitPoints3D_Double_1024.binary", std::ios::binary);
    UTAssert(inFile, "Failed to open input file.");
    inFile.read((char*)points.data(), points.size() * sizeof(points[0]));
    inFile.close();

    Vector<double, 3> mean;
    std::array<double, 3> eigenvalue{};
    std::array<Vector<double, 3>, 3> eigenvector{};
    (void)ApprGaussianDistribution<double, 3>::Fit(points, mean, eigenvalue, eigenvector);

    Vector<double, 3> expectedMean{
        -0.0060393620597056358,
        0.029958720410304059,
        0.0061964438903821988
    };

    Vector<double, 3> expectedEigenvalue{
        0.32645807529746584,
        0.33227236938394200,
        0.36737003186653572
    };

    Vector<double, 3> expectedEigenvector0{
        -0.39534966792327286,
        0.80120644304401778,
        -0.44918467883233859
    };

    Vector<double, 3> expectedEigenvector1{
        0.20080176786804430,
        -0.40180796738742636,
        -0.89343662750362607
    };

    Vector<double, 3> expectedEigenvector2{
        -0.89631316519061910,
        -0.44341695160282063,
        -0.0020290237149620705
    };

    double constexpr maxError = 1e-14;
    double error0 = Length(mean - expectedMean);
    double error1 = Length(eigenvector[0] - expectedEigenvector0);
    double error2 = Length(eigenvector[1] - expectedEigenvector1);
    double error3 = Length(eigenvector[2] - expectedEigenvector2);

    UTAssert(
        error0 <= maxError &&
        error1 <= maxError &&
        error2 <= maxError &&
        error3 <= maxError,
        "Invalid Gaussian distribution.");

    Matrix<double, 3, 3> R;
    R.SetCol(0, eigenvector[0]);
    R.SetCol(1, eigenvector[1]);
    R.SetCol(2, eigenvector[2]);
    double det = Determinant(R);
    error0 = std::fabs(det - 1.0);
    UTAssert(
        error0 <= 1e-15,
        "Matrix is not a rotation.");
}

void UnitTestApprGaussianDistribution::TestDynamicVector()
{
    std::size_t constexpr DIM = 4;
    std::vector<Vector<double>> points(1024);
    std::ifstream inFile("Mathematics/Approximation/ND/Input/RandomUnitPoints4D_Double_1024.binary", std::ios::binary);
    UTAssert(inFile, "Failed to open input file.");
    for (std::size_t i = 0; i < points.size(); ++i)
    {
        points[i].resize(DIM);
        inFile.read((char*)points[i].data(), points[i].size() * sizeof(points[0][0]));
    }
    inFile.close();

    std::size_t const maxIterations = 1024;
    Vector<double> mean(DIM);
    std::vector<double> eigenvalue;
    std::vector<Vector<double>> eigenvector;
    (void)ApprGaussianDistribution<double>::Fit(points, maxIterations,
        mean, eigenvalue, eigenvector);

    Vector<double> expectedMean{
        -0.0017169845289977748,
        0.024390824842101075,
        0.0035306928398859553,
        -0.0011266538375718841
    };

    Vector<double> expectedEigenvalue{
        0.32097453537476034,
        0.32754679000998321,
        0.34446684397089777,
        0.36673244996083249
    };

    Vector<double> expectedEigenvector0{
        0.49817424197923427,
        0.77603656822932465,
        -0.19579779507850309,
        0.33354593812796690
    };

    Vector<double> expectedEigenvector1{
        -0.85668931483802613,
        0.44606396522865022,
        -0.23817999089132041,
        0.10188546857063238
    };

    Vector<double> expectedEigenvector2{
        0.079144322307068873,
        -0.37823639264727410,
        -0.89085337621886451,
        0.23885909989611601
    };

    Vector<double> expectedEigenvector3{
        0.10789818681202634,
        0.23607501979778503,
        -0.33363719436066780,
        -0.90626309030506991
    };

    double error0 = Length(mean - expectedMean);
    double error1 = Length(eigenvector[0] - expectedEigenvector0);
    double error2 = Length(eigenvector[1] - expectedEigenvector1);
    double error3 = Length(eigenvector[2] - expectedEigenvector2);
    double error4 = Length(eigenvector[3] - expectedEigenvector3);

    UTAssert(
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15 &&
        error4 <= 1e-15,
        "Invalid Gaussian distribution.");

    Matrix<double> R(DIM, DIM);
    R.SetCol(0, eigenvector[0]);
    R.SetCol(1, eigenvector[1]);
    R.SetCol(2, eigenvector[2]);
    R.SetCol(3, eigenvector[3]);
    double det = Determinant(R);
    error0 = std::fabs(det - 1.0);
    UTAssert(
        error0 <= 1e-14,
        "Matrix is not a rotation.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/ND/ApprGaussianDistribution.h>

namespace gtl
{
    template class ApprGaussianDistribution<float, 2>;
    template class ApprGaussianDistribution<float, 3>;
    template class ApprGaussianDistribution<float>;

    template class ApprGaussianDistribution<double, 2>;
    template class ApprGaussianDistribution<double, 3>;
    template class ApprGaussianDistribution<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprGaussianDistribution<Rational, 2>;
    template class ApprGaussianDistribution<Rational, 3>;
    template class ApprGaussianDistribution<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprGaussianDistribution)
