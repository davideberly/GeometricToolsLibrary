#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrCylinder3Cylinder3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrCylinder3Cylinder3
    {
    public:
        UnitTestIntrCylinder3Cylinder3();
    };
}

UnitTestIntrCylinder3Cylinder3::UnitTestIntrCylinder3Cylinder3()
{
    UTInformation("Mathematics/Intersection/3D/IntrCylinder3Cylinder3 [tested by GTL/VisualTests/Intersection/IntersectCylinders]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrCylinder3Cylinder3.h>

namespace gtl
{
    template class TIQuery<float, Cylinder3<float>, Cylinder3<float>>;
    template class TIQuery<double, Cylinder3<double>, Cylinder3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Cylinder3<Rational>, Cylinder3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrCylinder3Cylinder3)
