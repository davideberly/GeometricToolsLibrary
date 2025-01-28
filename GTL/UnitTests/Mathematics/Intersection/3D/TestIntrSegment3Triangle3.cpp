#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrSegment3Triangle3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrSegment3Triangle3
    {
    public:
        UnitTestIntrSegment3Triangle3();
    };
}

UnitTestIntrSegment3Triangle3::UnitTestIntrSegment3Triangle3()
{
    UTInformation("Mathematics/Intersection/3D/IntrSegment3Triangle3 [tested by GTL/VisualTests/Graphics/SceneGraphs/Picking]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrSegment3Triangle3.h>

namespace gtl
{
    template class TIQuery<float, Segment3<float>, Triangle3<float>>;
    template class FIQuery<float, Segment3<float>, Triangle3<float>>;

    template class TIQuery<double, Segment3<double>, Triangle3<double>>;
    template class FIQuery<double, Segment3<double>, Triangle3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Segment3<Rational>, Triangle3<Rational>>;
    template class FIQuery<Rational, Segment3<Rational>, Triangle3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrSegment3Triangle3)
