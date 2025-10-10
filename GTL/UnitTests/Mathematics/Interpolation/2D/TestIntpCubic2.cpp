#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/2D/IntpCubic2.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpCubic2
    {
    public:
        UnitTestIntpCubic2();

    private:
        void Test();
    };
}

UnitTestIntpCubic2::UnitTestIntpCubic2()
{
    UTInformation("Mathematics/Interpolation/2D/IntpCubic2 [tested by GTL/VisualTests/Mathematics/Interpolation/Interpolation2D]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/2D/IntpCubic2.h>

namespace gtl
{
    template class IntpCubic2<float>;
    template class IntpCubic2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpCubic2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpCubic2)
