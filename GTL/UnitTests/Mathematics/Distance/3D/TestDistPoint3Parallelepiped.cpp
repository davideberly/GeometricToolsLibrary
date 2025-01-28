#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistPoint3Parallelepiped3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPoint3Parallelepiped3
    {
    public:
        UnitTestDistPoint3Parallelepiped3();

    private:
    };
}

UnitTestDistPoint3Parallelepiped3::UnitTestDistPoint3Parallelepiped3()
{
    UTInformation("Mathematics/Distance/3D/DistPoint3Parallelepiped3 [tested by GTL/VisualTest/Distance/DistancePoint3Parallelepiped3]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistPoint3Parallelepiped3.h>

namespace gtl
{
    template class DCPQuery<float, Vector3<float>, Parallelepiped3<float>>;
    template class DCPQuery<double, Vector3<double>, Parallelepiped3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector3<Rational>, Parallelepiped3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPoint3Parallelepiped3)
