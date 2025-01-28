#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/BSPPolygon2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestBSPPolygon2
    {
    public:
        UnitTestBSPPolygon2();

    private:
    };
}

UnitTestBSPPolygon2::UnitTestBSPPolygon2()
{
    UTInformation("Mathematics/Geometry/2D/BSPPolygon2 [tested by GTL/VisualTests/Geometry/PolygonBooleanOperations]");
}

#else

#include <GTL/Mathematics/Geometry/2D/BSPPolygon2.h>

namespace gtl
{
    template class BSPPolygon2<float>;
    template class BSPPolygon2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BSPPolygon2)
