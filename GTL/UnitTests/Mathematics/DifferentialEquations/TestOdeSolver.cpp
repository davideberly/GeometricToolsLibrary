#if !defined(GTL_UNIT_TESTS)

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/DifferentialEquations/OdeSolver.h>

namespace gtl
{
    template class OdeSolver<float, 1>;
    template class OdeSolver<float, 2>;
    template class OdeSolver<float>;

    template class OdeSolver<double, 1>;
    template class OdeSolver<double, 2>;
    template class OdeSolver<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class OdeSolver<Rational, 1>;
    template class OdeSolver<Rational, 2>;
    template class OdeSolver<Rational>;
#endif
}

#endif
