#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrPlane3Plane3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrPlane3Plane3
    {
    public:
        UnitTestIntrPlane3Plane3();
    };
}

UnitTestIntrPlane3Plane3::UnitTestIntrPlane3Plane3()
{
    UTInformation("Mathematics/Intersection/3D/IntrPlane3Plane3 (tested by TestIntrPlane3Circle3");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrPlane3Plane3.h>

namespace gtl
{
    template class TIQuery<float, Plane3<float>, Plane3<float>>;
    template class FIQuery<float, Plane3<float>, Plane3<float>>;

    template class TIQuery<double, Plane3<double>, Plane3<double>>;
    template class FIQuery<double, Plane3<double>, Plane3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Plane3<Rational>, Plane3<Rational>>;
    template class FIQuery<Rational, Plane3<Rational>, Plane3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrPlane3Plane3)
