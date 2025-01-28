#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Minimizers/LCPSolver.h>
using namespace gtl;

namespace gtl
{
    class UnitTestLCPSolver
    {
    public:
        UnitTestLCPSolver();
    };
}

UnitTestLCPSolver::UnitTestLCPSolver()
{
    UTInformation("Mathematics/Minimizers/LCPSolver [GTE/VisualTests/Mathematcs/Distance/DistanceOrientedBoxCone]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Arithmetic/QFNumber.h>
#endif
#include <GTL/Mathematics/Minimizers/LCPSolver.h>

namespace gtl
{
    template class LCPSolverShared<float>;
    template class LCPSolver<float, 5>;
    template class LCPSolver<float>;

    template class LCPSolverShared<double>;
    template class LCPSolver<double, 5>;
    template class LCPSolver<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class LCPSolverShared<Rational>;
    template class LCPSolver<Rational, 5>;
    template class LCPSolver<Rational>;

    using QFRational1 = QFNumber<Rational, 1>;
    template class LCPSolverShared<QFRational1>;
    template class LCPSolver<QFRational1, 5>;
    template class LCPSolver<QFRational1>;

    using QFRational2 = QFNumber<Rational, 2>;
    template class LCPSolverShared<QFRational2>;
    template class LCPSolver<QFRational2, 5>;
    template class LCPSolver<QFRational2>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(LCPSolver)
