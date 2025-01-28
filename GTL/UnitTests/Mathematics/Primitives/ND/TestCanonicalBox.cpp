#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/CanonicalBox.h>
using namespace gtl;

namespace gtl
{
    class UnitTestCanonicalBox
    {
    public:
        UnitTestCanonicalBox();

    private:
        void Test();
    };
}

UnitTestCanonicalBox::UnitTestCanonicalBox()
{
    UTInformation("Mathematics/Primitives/ND/CanonicalBox");

    Test();
}

void UnitTestCanonicalBox::Test()
{
    CanonicalBox<double, 2> box0({ 1.0, 2.0 });
    std::array<Vector2<double>, 4> vertices0{};
    box0.GetVertices(vertices0);
    UTAssert(
        (vertices0[0] == Vector2<double>{ -1.0, -2.0 }) &&
        (vertices0[1] == Vector2<double>{ +1.0, -2.0 }) &&
        (vertices0[2] == Vector2<double>{ -1.0, +2.0 }) &&
        (vertices0[3] == Vector2<double>{ +1.0, +2.0 }),
        "Incorrect box vertex.");

    CanonicalBox<double, 3> box1({ 1.0, 2.0, 3.0 });
    std::array<Vector3<double>, 8> vertices1{};
    box1.GetVertices(vertices1);
    UTAssert(
        (vertices1[0] == Vector3<double>{ -1.0, -2.0, -3.0 }) &&
        (vertices1[1] == Vector3<double>{ +1.0, -2.0, -3.0 }) &&
        (vertices1[2] == Vector3<double>{ -1.0, +2.0, -3.0 }) &&
        (vertices1[3] == Vector3<double>{ +1.0, +2.0, -3.0 }) &&
        (vertices1[4] == Vector3<double>{ -1.0, -2.0, +3.0 }) &&
        (vertices1[5] == Vector3<double>{ +1.0, -2.0, +3.0 }) &&
        (vertices1[6] == Vector3<double>{ -1.0, +2.0, +3.0 }) &&
        (vertices1[7] == Vector3<double>{ +1.0, +2.0, +3.0 }),
        "Incorrect box vertex.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/CanonicalBox.h>

namespace gtl
{
    template class CanonicalBox<float, 2>;
    template bool operator==(CanonicalBox<float, 2> const&, CanonicalBox<float, 2> const&);
    template bool operator!=(CanonicalBox<float, 2> const&, CanonicalBox<float, 2> const&);
    template bool operator< (CanonicalBox<float, 2> const&, CanonicalBox<float, 2> const&);
    template bool operator<=(CanonicalBox<float, 2> const&, CanonicalBox<float, 2> const&);
    template bool operator> (CanonicalBox<float, 2> const&, CanonicalBox<float, 2> const&);
    template bool operator>=(CanonicalBox<float, 2> const&, CanonicalBox<float, 2> const&);

    template class CanonicalBox<double, 3>;
    template bool operator==(CanonicalBox<double, 3> const&, CanonicalBox<double, 3> const&);
    template bool operator!=(CanonicalBox<double, 3> const&, CanonicalBox<double, 3> const&);
    template bool operator< (CanonicalBox<double, 3> const&, CanonicalBox<double, 3> const&);
    template bool operator<=(CanonicalBox<double, 3> const&, CanonicalBox<double, 3> const&);
    template bool operator> (CanonicalBox<double, 3> const&, CanonicalBox<double, 3> const&);
    template bool operator>=(CanonicalBox<double, 3> const&, CanonicalBox<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class CanonicalBox<Rational, 4>;
    template bool operator==(CanonicalBox<Rational, 4> const&, CanonicalBox<Rational, 4> const&);
    template bool operator!=(CanonicalBox<Rational, 4> const&, CanonicalBox<Rational, 4> const&);
    template bool operator< (CanonicalBox<Rational, 4> const&, CanonicalBox<Rational, 4> const&);
    template bool operator<=(CanonicalBox<Rational, 4> const&, CanonicalBox<Rational, 4> const&);
    template bool operator> (CanonicalBox<Rational, 4> const&, CanonicalBox<Rational, 4> const&);
    template bool operator>=(CanonicalBox<Rational, 4> const&, CanonicalBox<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(CanonicalBox)
