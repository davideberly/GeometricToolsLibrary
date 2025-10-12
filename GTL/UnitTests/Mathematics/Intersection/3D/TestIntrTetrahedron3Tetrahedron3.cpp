#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Intersection/3D/IntrTetrahedron3Tetrahedron3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrTetrahedron3Tetrahedron3
    {
    public:
        UnitTestIntrTetrahedron3Tetrahedron3();

    private:
    };
}

UnitTestIntrTetrahedron3Tetrahedron3::UnitTestIntrTetrahedron3Tetrahedron3()
{
    UTInformation("Mathematics/Intersection/3D/IntrTetrahedron3Tetrahedron3 [tested by GTL/VisualTests/Mathematics/Physics/BouncingTetrahedron]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrTetrahedron3Tetrahedron3.h>

namespace gtl
{
    template class TIQuery<float, Tetrahedron3<float>, Tetrahedron3<float>>;
    template class TIQuery<double, Tetrahedron3<double>, Tetrahedron3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Tetrahedron3<Rational>, Tetrahedron3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrTetrahedron3Tetrahedron3)
