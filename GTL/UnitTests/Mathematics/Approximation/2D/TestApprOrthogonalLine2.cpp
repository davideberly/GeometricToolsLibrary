#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/2D/ApprOrthogonalLine2.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestApprOrthogonalLine2
    {
    public:
        UnitTestApprOrthogonalLine2();

    private:
        void Test();
    };
}

UnitTestApprOrthogonalLine2::UnitTestApprOrthogonalLine2()
{
    UTInformation("Mathematics/Approximation/2D/ApprOrthogonalLine2");

    Test();
}

void UnitTestApprOrthogonalLine2::Test()
{
    std::vector<Vector<double, 2>> points(1024);
    std::ifstream input("Mathematics/Approximation/2D/Input/RandomUnitPoints2D_Double_1024.binary", std::ios::binary);
    input.read((char*)points.data(), points.size() * sizeof(points[0]));
    input.close();

    Line2<double> line{};
    ApprOrthogonalLine2<double>::Fit(points, line);

    Vector2<double> expectedOrigin{ 0.98697055157543789, 1.8354747720667344 };
    Vector2<double> expectedDirection{ -0.71157432210023208, 0.70261083405224767 };
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
#include <GTL/Mathematics/Approximation/2D/ApprOrthogonalLine2.h>

namespace gtl
{
    template class ApprOrthogonalLine2<float>;
    template class ApprOrthogonalLine2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprOrthogonalLine2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprOrthogonalLine2)
