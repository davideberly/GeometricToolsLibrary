#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/1D/IntpBSplineUniform1.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpBSplineUniform1
    {
    public:
        UnitTestIntpBSplineUniform1();

    private:
        class Controls
        {
        public:
            using Type = double;

            Controls(std::size_t numElements)
                :
                signal(numElements)
            {
            }

            std::size_t GetSize(std::size_t) const
            {
                return signal.size();
            }

            Type operator() (std::size_t const* tuple) const
            {
                return signal[tuple[0]];
            }

            Type operator() (std::size_t x) const
            {
                return signal[x];
            }

            std::vector<double> signal;
        };

        using Interpolator = IntpBSplineUniform1<double, Controls>;

        void Test();
    };
}

UnitTestIntpBSplineUniform1::UnitTestIntpBSplineUniform1()
{
    UTInformation("Mathematics/Interpolation/1D/IntpBSplineUniform1");

    Test();
}

void UnitTestIntpBSplineUniform1::Test()
{
    // Compare the results to the values on the graph in the PDF.
    Controls controls(6);
    controls.signal[0] = 1.0;
    controls.signal[1] = 2.0;
    controls.signal[2] = 1.5;
    controls.signal[3] = 0.25;
    controls.signal[4] = 1.25;
    controls.signal[5] = 1.25;

    Interpolator interp(2, controls, 0.0, Interpolator::NO_CACHING);

    double const epsilon = 1e-08;
    double result = interp.Evaluate(0, -0.5);  // 1.5000000000000000
    UTAssert(std::fabs(result - 1.5) <= epsilon, "Unexpected result.");

    result = interp.Evaluate(0, 0.0);   // 1.7499999999999998
    UTAssert(std::fabs(result - 1.75) <= epsilon, "Unexpected result.");

    result = interp.Evaluate( 0, 1.0);   // 1.7500000000000000
    UTAssert(std::fabs(result - 1.75) <= epsilon, "Unexpected result.");

    result = interp.Evaluate( 0, 2.0);   // 1.2500000000000002
    UTAssert(std::fabs(result - 1.25) <= epsilon, "Unexpected result.");

    result = interp.Evaluate( 0, 3.0);   // 0.58333333333333348
    UTAssert(std::fabs(result - 7.0 / 12.0) <= epsilon, "Unexpected result.");

    result = interp.Evaluate( 0, 4.0);   // 0.75000000000000000
    UTAssert(std::fabs(result - 0.75) <= epsilon, "Unexpected result.");

    result = interp.Evaluate( 0, 5.0);   // 1.1944444444444444
    UTAssert(std::fabs(result - 43.0 / 36.0) <= epsilon, "Unexpected result.");

    result = interp.Evaluate( 0, 5.5);   // 1.2500000000000000
    UTAssert(std::fabs(result - 1.25) <= epsilon, "Unexpected result.");

    // Compare derivative computations with finite difference estimates
    // to verify correctness of derivative computation.  Let's use t = 2.5
    // for the test case.
    double dfdt = interp.Evaluate(1, 2.5);  // -0.83333333333333326
    UTAssert(std::fabs(dfdt + 5.0 / 6.0) <= epsilon, "Unexpected result.");
    double h = 1e-06;
    double f0 = interp.Evaluate(0, 2.5 - h);  // 0.87500083333316681
    double f1 = interp.Evaluate(0, 2.5 + h);  // 0.87499916666716648
    double dfdtEstimate = (f1 - f0) / (2.0 * h);  // -0.83333300016086298
    UTAssert(std::fabs(dfdt - dfdtEstimate) <= 1e-06, "Unexpected result.");

    // The spline is piecewise quadratic, so the 2nd derivative is
    // piecewise constant.  The subdomains are [i,i+1), so the 2nd
    // derivative at i-epsilon is different from the 2nd derivative
    // at i.
    double d2fdt2 = interp.Evaluate(2, 2.49999); // -0.33333333333333331
    UTAssert(std::fabs(d2fdt2 + 1.0 / 3.0) <= epsilon, "Unexpected result.");

    d2fdt2 = interp.Evaluate(2, 2.499999); // -0.33333333333333331
    UTAssert(std::fabs(d2fdt2 + 1.0 / 3.0) <= epsilon, "Unexpected result.");

    d2fdt2 = interp.Evaluate(2, 2.5); // 1.0000000000000000
    UTAssert(std::fabs(d2fdt2 - 1.0) <= epsilon, "Unexpected result.");

    d2fdt2 = interp.Evaluate(2, 2.500001); // 1.0000000000000000
    UTAssert(std::fabs(d2fdt2 - 1.0) <= epsilon, "Unexpected result.");

    d2fdt2 = interp.Evaluate(2, 2.50001); // 1.0000000000000000
    UTAssert(std::fabs(d2fdt2 - 1.0) <= epsilon, "Unexpected result.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/1D/IntpBSplineUniform1.h>

namespace gtl
{
    template <typename T>
    class Controls
    {
    public:
        // The type of the control points.
        using Type = T;

        Controls()
            :
            mSignal{}
        {
        }

        std::size_t GetSize(std::size_t) const
        {
            return mSignal.size();
        }

        Type operator() (std::size_t const* tuple) const
        {
            return mSignal[tuple[0]];
        }

        Type operator() (std::size_t x) const
        {
            return mSignal[x];
        }

    private:
        std::vector<T> mSignal;
    };

    template class IntpBSplineUniform1<float, Controls<float>>;
    template class IntpBSplineUniform1<double, Controls<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpBSplineUniform1<Rational, Controls<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpBSplineUniform1)
