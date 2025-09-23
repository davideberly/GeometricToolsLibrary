#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrOrientedBox3Sphere3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrOrientedBox3Sphere3
    {
    public:
        UnitTestIntrOrientedBox3Sphere3();

    private:
        void Test();
    };
}

UnitTestIntrOrientedBox3Sphere3::UnitTestIntrOrientedBox3Sphere3()
{
    UTInformation("Mathematics/Intersection/3D/IntrOrientedBox3Sphere3 [tested by GTL/VisualTests/Intersection/IntersectMovingBoxSphere]");

    Test();
}

void UnitTestIntrOrientedBox3Sphere3::Test()
{
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrOrientedBox3Sphere3.h>

namespace gtl
{
    template class TIQuery<float, OrientedBox3<float>, Sphere3<float>>;
    template class FIQuery<float, OrientedBox3<float>, Sphere3<float>>;

    template class TIQuery<double, OrientedBox3<double>, Sphere3<double>>;
    template class FIQuery<double, OrientedBox3<double>, Sphere3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, OrientedBox3<Rational>, Sphere3<Rational>>;
    template class FIQuery<Rational, OrientedBox3<Rational>, Sphere3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrOrientedBox3Sphere3)
