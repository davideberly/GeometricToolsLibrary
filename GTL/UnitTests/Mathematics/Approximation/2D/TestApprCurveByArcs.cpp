#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/2D/ApprCurveByArcs.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestApprCurveByArcs
    {
    public:
        UnitTestApprCurveByArcs();

    private:
    };
}

UnitTestApprCurveByArcs::UnitTestApprCurveByArcs()
{
    UTInformation("Mathematics/Approximation/2D/ApprCurveByArcs [tested by GTL/VisualTests/Approximation/ApproximationBezierCurveByArcs]");
}

#else

#include <GTL/Mathematics/Approximation/2D/ApprCurveByArcs.h>

namespace gtl
{
    template void ApproximateCurveByArcs(std::shared_ptr<ParametricCurve<float, 2>> const&,
        std::size_t, std::vector<float>&, std::vector<Vector2<float>>&, std::vector<Arc2<float>>&, float);

    template void ApproximateCurveByArcs(std::shared_ptr<ParametricCurve<double, 2>> const&,
        std::size_t, std::vector<double>&, std::vector<Vector2<double>>&, std::vector<Arc2<double>>&, double);
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprCurveByArcs)
