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
    std::ifstream input("Mathematics/Approximation/3D/Input/RandomUnitPoints3D_Double_1024.binary", std::ios::binary);
    input.read((char*)points.data(), points.size() * sizeof(points[0]));
    input.close();

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
