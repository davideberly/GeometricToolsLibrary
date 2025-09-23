#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrAlignedBox3Sphere3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrAlignedBox3Sphere3
    {
    public:
        UnitTestIntrAlignedBox3Sphere3();

    private:
        void Test();
    };
}

UnitTestIntrAlignedBox3Sphere3::UnitTestIntrAlignedBox3Sphere3()
{
    UTInformation("Mathematics/Intersection/3D/IntrAlignedBox3Sphere3 [tested by GTL/VisualTests/Intersection/IntersectMovingBoxSphere]");

    Test();
}

void UnitTestIntrAlignedBox3Sphere3::Test()
{
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrAlignedBox3Sphere3.h>

namespace gtl
{
    template class TIQuery<float, AlignedBox3<float>, Sphere3<float>>;
    template class FIQuery<float, AlignedBox3<float>, Sphere3<float>>;

    template class TIQuery<double, AlignedBox3<double>, Sphere3<double>>;
    template class FIQuery<double, AlignedBox3<double>, Sphere3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, AlignedBox3<Rational>, Sphere3<Rational>>;
    template class FIQuery<Rational, AlignedBox3<Rational>, Sphere3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrAlignedBox3Sphere3)
