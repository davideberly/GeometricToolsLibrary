#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/IncrementalDelaunay2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIncrementalDelaunay2
    {
    public:
        UnitTestIncrementalDelaunay2();
    };
}

UnitTestIncrementalDelaunay2::UnitTestIncrementalDelaunay2()
{
    UTInformation("Mathematics/Geometry/2D/IncrementalDelaunay2 [tested by GTL/VisualTests/Geometry/IncrementalDelaunay2]");
}

#else

#include <GTL/Mathematics/Geometry/2D/IncrementalDelaunay2.h>

namespace gtl
{
    template class IncrementalDelaunay2<float>;
    template class IncrementalDelaunay2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IncrementalDelaunay2)
