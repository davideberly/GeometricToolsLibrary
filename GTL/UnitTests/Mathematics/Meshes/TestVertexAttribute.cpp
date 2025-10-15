#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/VertexAttribute.h>
using namespace gtl;

namespace gtl
{
    class UnitTestVertexAttribute
    {
    public:
        UnitTestVertexAttribute();

    private:
    };
}

UnitTestVertexAttribute::UnitTestVertexAttribute()
{
    UTInformation("Mathematics/Meshes/VertexAttribute [tested by GTL/VisualTests/Mathematics/Physics/Rope]");
}

#else

#include <GTL/Mathematics/Meshes/VertexAttribute.h>

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(VertexAttribute)
