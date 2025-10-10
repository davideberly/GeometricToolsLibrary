#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/2D/IntpAkimaUniform2.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpAkimaUniform2
    {
    public:
        UnitTestIntpAkimaUniform2();

    private:
        void Test();
    };
}

UnitTestIntpAkimaUniform2::UnitTestIntpAkimaUniform2()
{
    UTInformation("Mathematics/Interpolation/2D/IntpAkimaUniform2 [tested by GTL/VisualTests/Mathematics/Interpolation/Interpolation2D]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/2D/IntpAkimaUniform2.h>

namespace gtl
{
    template class IntpAkimaUniform2<float>;
    template class IntpAkimaUniform2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpAkimaUniform2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpAkimaUniform2)
