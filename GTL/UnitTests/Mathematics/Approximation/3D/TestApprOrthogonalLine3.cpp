#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/3D/ApprOrthogonalLine3.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestApprOrthogonalLine3
    {
    public:
        UnitTestApprOrthogonalLine3();

    private:
        void Test();
    };
}

UnitTestApprOrthogonalLine3::UnitTestApprOrthogonalLine3()
{
    UTInformation("Mathematics/Approximation/3D/ApprOrthogonalLine3");

    Test();
}

void UnitTestApprOrthogonalLine3::Test()
{
    std::vector<Vector<double, 3>> points(1024);
    std::ifstream input("Mathematics/Approximation/3D/RandomUnitPoints3D_Double_1024.binary", std::ios::binary);
    input.read((char*)points.data(), points.size() * sizeof(points[0]));
    input.close();

    Line3<double> line{};
    ApprOrthogonalLine3<double>::Fit(points, line);

    Vector3<double> expectedOrigin{ -0.0060393620597056358, 0.029958720410304059, 0.0061964438903821988 };
    Vector3<double> expectedDirection{ -0.71215867524191701, 0.0014772882574574672, 0.70201697906609228 };
    double diffOrigin = Length(line.origin - expectedOrigin);
    double diffDirection = Length(line.direction - expectedDirection);
    UTAssert(
        diffOrigin <= 1e-16 && diffDirection <= 1e-16,
        "Incorrect fit of point data.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/3D/ApprOrthogonalLine3.h>

namespace gtl
{
    template class ApprOrthogonalLine3<float>;
    template class ApprOrthogonalLine3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprOrthogonalLine3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprOrthogonalLine3)
