// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/RemezAlgorithm.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRemezAlgorithm
    {
    public:
        UnitTestRemezAlgorithm();

    private:
        void ApproximateSinDegree5();
    };
}

UnitTestRemezAlgorithm::UnitTestRemezAlgorithm()
{
    UTInformation("Mathematics/Functions/RemezAlgorithm");

    ApproximateSinDegree5();
}

void UnitTestRemezAlgorithm::ApproximateSinDegree5()
{
    auto F = [](double const& x)
    {
        return std::sin(x);
    };

    auto FDer = [](double const& x)
    {
        return std::cos(x);
    };

    double const maxError = 1e-12;
    double const xMin = 0.0;
    double const xMax = C_PI_DIV_2<double>;
    std::size_t const degree = 5;
    std::size_t const maxRemezIterations = 16;
    std::size_t const maxBisectionIterations = 1048;
    std::size_t const maxBracketIterations = 128;
    RemezAlgorithm<double> remez;

    auto iterations = remez.Execute(F, FDer, xMin, xMax, degree,
        maxRemezIterations, maxBisectionIterations, maxBracketIterations);

    UTAssert(iterations == maxRemezIterations, "Unexpected number of iterations.");

    auto const& p = remez.GetCoefficients();
    UTAssert(std::fabs(p[0] - 7.0685186758729533e-06) <= maxError, "Incorrect coefficient 0.");
    UTAssert(std::fabs(p[1] - 0.99968986443393670) <= maxError, "Incorrect coefficient 1.");
    UTAssert(std::fabs(p[2] - 0.0021937161709613094) <= maxError, "Incorrect coefficient 2.");
    UTAssert(std::fabs(p[3] + 0.17223886508803649) <= maxError, "Incorrect coefficient 3.");
    UTAssert(std::fabs(p[4] - 0.0060973836732878166) <= maxError, "Incorrect coefficient 4.");
    UTAssert(std::fabs(p[5] - 0.0057217240548524534) <= maxError, "Incorrect coefficient 5.");

    double estimatedMaximumError = remez.GetEstimatedMaxError();
    UTAssert(std::fabs(estimatedMaximumError + 7.0685186758729533e-06) <= maxError, "Incorrect estimated maximum error.");

    auto const& x = remez.GetXNodes();
    UTAssert(std::fabs(x[0] - 0.0000000000000000) <= maxError, "Incorrect x-node 0.");
    UTAssert(std::fabs(x[1] - 0.10950063957513409) <= maxError, "Incorrect x-node 1.");
    UTAssert(std::fabs(x[2] - 0.40467937702524381) <= maxError, "Incorrect x-node 2.");
    UTAssert(std::fabs(x[3] - 0.79996961817349699) <= maxError, "Incorrect x-node 3.");
    UTAssert(std::fabs(x[4] - 1.1880777522163142) <= maxError, "Incorrect x-node 4.");
    UTAssert(std::fabs(x[5] - 1.4686862883722980) <= maxError, "Incorrect x-node 5.");
    UTAssert(std::fabs(x[6] - 1.5707963267948966) <= maxError, "Incorrect x-node 6.");

    auto const& error = remez.GetErrors();
    UTAssert(std::fabs(error[0] + 7.0685186758729533e-06) <= maxError, "Incorrect error 0.");
    UTAssert(std::fabs(error[1] - 7.0685186758651097e-06) <= maxError, "Incorrect error 1.");
    UTAssert(std::fabs(error[2] + 7.0685186758789875e-06) <= maxError, "Incorrect error 2.");
    UTAssert(std::fabs(error[3] - 7.0685186759344987e-06) <= maxError, "Incorrect error 3.");
    UTAssert(std::fabs(error[4] + 7.0685186757124541e-06) <= maxError, "Incorrect error 4.");
    UTAssert(std::fabs(error[5] - 7.0685186759344987e-06) <= maxError, "Incorrect error 5.");
    UTAssert(std::fabs(error[6] + 7.0685186761565433e-06) <= maxError, "Incorrect error 6.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/RemezAlgorithm.h>

namespace gtl
{
    template class RemezAlgorithm<float>;
    template class RemezAlgorithm<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class RemezAlgorithm<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RemezAlgorithm)
