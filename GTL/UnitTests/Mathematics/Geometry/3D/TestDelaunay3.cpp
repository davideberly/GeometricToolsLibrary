#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/3D/Delaunay3.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestDelaunay3
    {
    public:
        UnitTestDelaunay3();

    private:
    };
}

UnitTestDelaunay3::UnitTestDelaunay3()
{
    UTInformation("Mathematics/Geometry/3D/Delaunay3 [tested by GTL/VisualTests/Geometry/Delaunay3]");
}

#else

#include <GTL/Mathematics/Geometry/3D/Delaunay3.h>

namespace gtl
{
    template class Delaunay3<float>;
    template class Delaunay3<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Delaunay3)
