#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Approximation/2D/ApprParallelLines2.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestApprParallelLines2
    {
    public:
        UnitTestApprParallelLines2();

    private:
        using Rational = BSRational<UIntegerAP32>;
        void Test();
    };
}

UnitTestApprParallelLines2::UnitTestApprParallelLines2()
{
    UTInformation("Mathematics/Approximation/2D/ApprParallelLines2");

    Test();
}

void UnitTestApprParallelLines2::Test()
{
    // TODO: Need to create point set that is nearly on two parallel lines.
    std::vector<Vector<double, 2>> points(1024);
    Vector2<double> C{}, V{};
    double radius{};
    ApprParallelLines2<double>{}.Fit(points, C, V, radius);
}

#else

#include <GTL/Mathematics/Approximation/2D/ApprParallelLines2.h>

namespace gtl
{
    template class ApprParallelLines2<float>;
    template class ApprParallelLines2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprParallelLines2)
