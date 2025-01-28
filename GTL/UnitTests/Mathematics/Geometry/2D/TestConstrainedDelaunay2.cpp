#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/ConstrainedDelaunay2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestConstrainedDelaunay2
    {
    public:
        UnitTestConstrainedDelaunay2();

    private:
    };
}

UnitTestConstrainedDelaunay2::UnitTestConstrainedDelaunay2()
{
    UTInformation("Mathematics/Geometry/2D/ConstrainedDelaunay2 [tested by GTL/VisualTests/Geometry/ConstrainedDelaunay2D]");
}

#else

#include <GTL/Mathematics/Geometry/2D/ConstrainedDelaunay2.h>

namespace gtl
{
    template class ConstrainedDelaunay2<float>;
    template class ConstrainedDelaunay2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ConstrainedDelaunay2)
