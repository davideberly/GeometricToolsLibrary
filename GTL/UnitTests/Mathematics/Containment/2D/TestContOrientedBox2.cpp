#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Containment/2D/ContOrientedBox2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestContOrientedBox2
    {
    public:
        UnitTestContOrientedBox2();
    };
}

UnitTestContOrientedBox2::UnitTestContOrientedBox2()
{
    UTInformation("Mathematics/Containment/2D/ContOrientedBox2 [tested by GTL/VisualTests/Mathematics/Approximation/2D/ApproximationEllipse2]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Containment/2D/ContOrientedBox2.h>

namespace gtl
{
    template void GetContainer(std::vector<Vector2<float>> const&, OrientedBox2<float>&);
    template bool InContainer(Vector2<float> const&, OrientedBox2<float> const&);
    template void MergeContainers(OrientedBox2<float> const&, OrientedBox2<float> const&, OrientedBox2<float>&);

    template void GetContainer(std::vector<Vector2<double>> const&, OrientedBox2<double>&);
    template bool InContainer(Vector2<double> const&, OrientedBox2<double> const&);
    template void MergeContainers(OrientedBox2<double> const&, OrientedBox2<double> const&, OrientedBox2<double>&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template void GetContainer(std::vector<Vector2<Rational>> const&, OrientedBox2<Rational>&);
    template bool InContainer(Vector2<Rational> const&, OrientedBox2<Rational> const&);
    template void MergeContainers(OrientedBox2<Rational> const&, OrientedBox2<Rational> const&, OrientedBox2<Rational>&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ContOrientedBox2)
