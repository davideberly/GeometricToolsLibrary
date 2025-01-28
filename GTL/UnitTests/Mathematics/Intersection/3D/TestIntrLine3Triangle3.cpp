#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3Triangle3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrLine3Triangle3
    {
    public:
        UnitTestIntrLine3Triangle3();
    };
}

UnitTestIntrLine3Triangle3::UnitTestIntrLine3Triangle3()
{
    UTInformation("Mathematics/Intersection/3D/IntrLine3Triangle3 [tested by GTL/VisualTests/Graphics/SceneGraphs/Picking]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrLine3Triangle3.h>

namespace gtl
{
    template class TIQuery<float, Line3<float>, Triangle3<float>>;
    template class FIQuery<float, Line3<float>, Triangle3<float>>;

    template class TIQuery<double, Line3<double>, Triangle3<double>>;
    template class FIQuery<double, Line3<double>, Triangle3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Line3<Rational>, Triangle3<Rational>>;
    template class FIQuery<Rational, Line3<Rational>, Triangle3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrLine3Triangle3)
