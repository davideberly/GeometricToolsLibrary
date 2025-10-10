#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/2D/IntpLinear2.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpLinear2
    {
    public:
        UnitTestIntpLinear2();

    private:
        void Test();
    };
}

UnitTestIntpLinear2::UnitTestIntpLinear2()
{
    UTInformation("Mathematics/Interpolation/2D/IntpLinear2 [tested by GTL/VisualTests/Mathematics/Interpolation/Interpolation2D]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/2D/IntpLinear2.h>

namespace gtl
{
    template class IntpLinear2<float>;
    template class IntpLinear2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpLinear2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpLinear2)
