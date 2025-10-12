#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Physics/RectangleManager.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRectangleManager
    {
    public:
        UnitTestRectangleManager();

    private:
    };
}

UnitTestRectangleManager::UnitTestRectangleManager()
{
    UTInformation("Mathematics/Physics/RectangleManager [tested by GTL/VisualTests/Mathematics/Physics/IntersectingRectangles]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Physics/RectangleManager.h>

namespace gtl
{
    template class RectangleManager<float>;
    template class RectangleManager<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class RectangleManager<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RectangleManager)
