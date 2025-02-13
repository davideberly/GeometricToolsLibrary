#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/3D/ApprOrthogonalPlane3.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestApprOrthogonalPlane3
    {
    public:
        UnitTestApprOrthogonalPlane3();

    private:
        void Test();
    };
}

UnitTestApprOrthogonalPlane3::UnitTestApprOrthogonalPlane3()
{
    UTInformation("Mathematics/Approximation/3D/ApprOrthogonalPlane3");

    Test();
}

void UnitTestApprOrthogonalPlane3::Test()
{
    std::vector<Vector<double, 3>> points(1024);
    std::ifstream input("Mathematics/Approximation/3D/RandomUnitPoints3D_Double_1024.binary", std::ios::binary);
    input.read((char*)points.data(), points.size() * sizeof(points[0]));
    input.close();

    //std::ofstream output("Mathematics/Approximation/RandomUnitPoints3D_Double_1024.txt");
    //for (auto const& point : points)
    //{
    //    output << point[0] << ", " << point[1] << ", " << point[2] << std::endl;
    //}
    //output.close();
    //
    // Mathematica 12.3 code that verifies correctness.
    // x = Import["C:\\Users\\deber\\GeometricTools\\GTL\\UnitTests\\Mathematics\\Approximation\\RandomUnitPoints2D_Double_1024.txt", "CSV"]
    // mean = Sum[x[[i]],{i,Length[x]}]/Length[x]
    //    {-0.00603936, 0.0299587, 0.00619644}
    // covar = Sum[Outer[Times,x[[i]]-mean,x[[i]]-mean],{i,Length[x]}]/Length[x]
    //    {{0.35956, 0.015791, -0.000968697},
    //     {0.015791, 0.335441, 0.00212409},
    //     {-0.000968697, 0.00212409, 0.331099}}
    // Eigensystem[covar]
    // {{0.36737, 0.332272, 0.326458},
    //    {{0.896313, 0.443417, 0.00202922},
    //     {0.200802, -0.401808, -0.893437},
    //     {0.39535, -0.801206, 0.449185} }}

    Vector3<double> origin{}, normal{};
    ApprOrthogonalPlane3<double>::Fit(points, origin, normal);

    Vector3<double> expectedOrigin{ -0.0060393620597056358, 0.029958720410304059, 0.0061964438903821988 };
    Vector3<double> expectedNormal{ -0.39534966792327458, 0.80120644304402122, -0.44918467883233038 };
    double diffOrigin = Length(origin - expectedOrigin);
    double diffNormal = Length(normal - expectedNormal);
    UTAssert(
        diffOrigin <= 1e-16 && diffNormal <= 1e-16,
        "Incorrect fit of point data.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/3D/ApprOrthogonalPlane3.h>

namespace gtl
{
    template class ApprOrthogonalPlane3<float>;
    template class ApprOrthogonalPlane3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprOrthogonalPlane3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprOrthogonalPlane3)
