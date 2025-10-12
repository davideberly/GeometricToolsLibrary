#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Physics/BoxManager.h>
using namespace gtl;

namespace gtl
{
    class UnitTestBoxManager
    {
    public:
        UnitTestBoxManager();

    private:
    };
}

UnitTestBoxManager::UnitTestBoxManager()
{
    UTInformation("Mathematics/Physics/BoxManager [tested by GTL/VisualTests/Mathematics/Physics/IntersectingBoxes]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Physics/BoxManager.h>

namespace gtl
{
    template class BoxManager<float>;
    template class BoxManager<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class BoxManager<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BoxManager)
