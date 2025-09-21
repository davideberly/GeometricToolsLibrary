#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Approximation/2D/ApprParallelLines2.h>
#include <GTL/Mathematics/Distance/ND/DistPointLine.h>
#include <fstream>
#include <random>
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
    // Generate points that are perturbed from two parallel lines.
    std::vector<Vector<double, 2>> points(1024);
    Vector2<double> D{ 3.0, 4.0 };
    Normalize(D);
    Vector2<double> perpD = Perp(D);
    Vector2<double> origin0{ 0.1, 0.2 };
    Vector2<double> origin1 = origin0 + 5.0 * perpD;
    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0, 1.0);
    double amplitude = 10.0;
    double perturb = 1e-02;
    for (std::size_t i = 0; i < points.size(); ++i)
    {
        if ((i & 1) == 0)
        {
            points[i] = origin0 + amplitude * urd(dre) * D + perturb * perpD;
        }
        else
        {
            points[i] = origin1 + amplitude * urd(dre) * D + perturb * perpD;
        }
    }

    Vector2<double> estimateC{}, estimateD{};
    double estimateRadius{};
    ApprParallelLines2<double>{}.Fit(points, estimateC, estimateD, estimateRadius);
    // estimateC = (1.9760369059017922, -1.4819606396224556)
    // estimateD = (0.59998262926267121, 0.80001302775832839)
    // estimateRadius = 2.4999985868412766

    Vector2<double> actualC = 0.5 * (origin0 + origin1);
    actualC = actualC - Dot(D, actualC) * D;
    Vector2<double> const& actualD = D;
    auto output = DCPQuery<double, Vector2<double>, Line2<double>>{}(actualC, Line2<double>{origin0, D});
    double actualRadius = output.distance;
    // actualC = (1.9679999999999997, -1.4759999999999998)
    // actualD = (0.59999999999999998, 0.80000000000000004)
    // actualRadius = 2.4999999999999996

    double diffDistC = Length(estimateC - actualC);
    double diffDistD = Length(estimateD - actualD);
    double diffRadius = std::fabs(estimateRadius - actualRadius);
    UTAssert(diffDistC <= 0.02, "Inaccurate center estimate.");
    UTAssert(diffDistD <= 3e-05, "Inaccurate direction estimate.");
    UTAssert(diffRadius <= 2e-06, "Inaccurate radius estimate.");
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
