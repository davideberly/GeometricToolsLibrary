#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Physics/RigidBody.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRigidBody
    {
    public:
        UnitTestRigidBody();

    private:
    };
}

UnitTestRigidBody::UnitTestRigidBody()
{
    UTInformation("Mathematics/Physics/RigidBody [tested by GTL/VisualTests/Mathematics/Physics/BouncingSpheres]");
}

#else

#include <GTL/Mathematics/Physics/RigidBody.h>

namespace gtl
{
    template class RigidBody<float>;
    template class RigidBody<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RigidBody)
