#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/3D/ApprHeightPlane3.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestApprHeightPlane3
    {
    public:
        UnitTestApprHeightPlane3();

    private:
        void Test();
    };
}

UnitTestApprHeightPlane3::UnitTestApprHeightPlane3()
{
    UTInformation("Mathematics/Approximation/3D/ApprHeightPlane3");

    Test();
}

void UnitTestApprHeightPlane3::Test()
{
    std::vector<Vector<double, 3>> points(1024);
    std::ifstream input("Mathematics/Approximation/3D/Input/RandomUnitPoints3D_Double_1024.binary", std::ios::binary);
    input.read((char*)points.data(), points.size() * sizeof(points[0]));
    input.close();

    Vector3<double> average{};
    Vector2<double> slopes{};
    ApprHeightPlane3<double>::Fit(points, average, slopes);

    Vector3<double> expectedAverage{ -0.0060393620597056358, 0.029958720410304059, 0.0061964438903821988 };
    Vector2<double> expectedSlopes = { -0.0029783800047525509, 0.0064724177057415471 };
    double diffAverageLength = Length(average - expectedAverage);
    double diffAbsSlope = Length(slopes - expectedSlopes);
    UTAssert(
        diffAverageLength <= 1e-16 && diffAbsSlope <= 1e-16,
        "Incorrect fit of height data.");

    // Mathematica:
    // data = BinaryReadList["RandomUnitPoints3D_Double_1024.binary",
    //   {"Real64","Real64","Real64"}]
    // Fit[data, {1, x, y}, {x, y}]
    // 0.00598455 - 0.00297838 x + 0.00647242 y
    // 
    // double c = average[2] - slopes[0] * average[0] - slopes[1] * average[1];
    // (void)c;  // 0.0059845510227570968
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/3D/ApprHeightPlane3.h>

namespace gtl
{
    template class ApprHeightPlane3<float>;
    template class ApprHeightPlane3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprHeightPlane3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprHeightPlane3)
