#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/2D/ApprHeightLine2.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestApprHeightLine2
    {
    public:
        UnitTestApprHeightLine2();

    private:
        void Test();
    };
}

UnitTestApprHeightLine2::UnitTestApprHeightLine2()
{
    UTInformation("Mathematics/Approximation/2D/ApprHeightLine2");

    Test();
}

void UnitTestApprHeightLine2::Test()
{
    std::vector<Vector<double, 2>> points(1024);
    std::ifstream input("Mathematics/Approximation/2D/Support/RandomUnitPoints2D_Double_1024.binary", std::ios::binary);
    input.read((char*)points.data(), points.size() * sizeof(points[0]));
    input.close();

    Vector2<double> average{};
    double slope{};
    ApprHeightLine2<double>::Fit(points, average, slope);

    Vector2<double> expectedAverage{ 0.98697055157543789, 1.8354747720667344 };
    double expectedSlope = -0.71474244016920296;
    double diffAverageLength = Length(average - expectedAverage);
    double diffAbsSlope = std::fabs(slope - expectedSlope);
    UTAssert(
        diffAverageLength <= 1e-16 && diffAbsSlope <= 1e-16,
        "Incorrect fit of height data.");

    // Mathematica:
    // data = BinaryReadList["RandomUnitPoints2D_Double_1024.binary",
    //   {"Real64","Real64"}]
    // Fit[data, {1, x}, x]
    // 2.5409 - 0.714742 x
    // 
    // double b = average[1] - slope * average[0];
    // (void)b;  // 2.5409045124749072
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/2D/ApprHeightLine2.h>

namespace gtl
{
    template class ApprHeightLine2<float>;
    template class ApprHeightLine2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprHeightLine2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprHeightLine2)
