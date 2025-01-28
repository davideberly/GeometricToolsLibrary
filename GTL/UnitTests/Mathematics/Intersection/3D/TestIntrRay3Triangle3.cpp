#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrRay3Triangle3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrRay3Triangle3
    {
    public:
        UnitTestIntrRay3Triangle3();
    };
}

UnitTestIntrRay3Triangle3::UnitTestIntrRay3Triangle3()
{
    UTInformation("Mathematics/Intersection/3D/IntrRay3Triangle3 [tested by GTL/VisualTests/Graphics/SceneGraphs/Picking]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrRay3Triangle3.h>

namespace gtl
{
    template class TIQuery<float, Ray3<float>, Triangle3<float>>;
    template class FIQuery<float, Ray3<float>, Triangle3<float>>;

    template class TIQuery<double, Ray3<double>, Triangle3<double>>;
    template class FIQuery<double, Ray3<double>, Triangle3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Ray3<Rational>, Triangle3<Rational>>;
    template class FIQuery<Rational, Ray3<Rational>, Triangle3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrRay3Triangle3)
