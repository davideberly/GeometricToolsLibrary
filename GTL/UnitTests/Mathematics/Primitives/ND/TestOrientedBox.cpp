#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
using namespace gtl;

namespace gtl
{
    class UnitTestOrientedBox
    {
    public:
        UnitTestOrientedBox();

    private:
        void Test();
    };
}

UnitTestOrientedBox::UnitTestOrientedBox()
{
    UTInformation("Mathematics/Primitives/ND/OrientedBox");

    Test();
}

void UnitTestOrientedBox::Test()
{
    OrientedBox<double, 2> box0{};
    UTAssert(
        IsZero(box0.center) &&
        IsZero(box0.axis[0]) &&
        IsZero(box0.axis[1]) &&
        IsZero(box0.extent),
        "Expecting default degenerate box0.");

    box0.center = { 1.0, 2.0 };
    box0.axis[0] = { 3.0 / 5.0, 4.0 / 5.0 };
    box0.axis[1] = -Perp(box0.axis[0]);
    box0.extent = { 5.0, 7.0 };
    UTAssert(
        (box0.center == Vector2<double>{ 1.0, 2.0 }) &&
        (box0.axis[0] == Vector2<double>{ 3.0 / 5.0, 4.0 / 5.0 }) &&
        (box0.axis[1] == Vector2<double>{ -4.0 / 5.0, 3.0 / 5.0 }) &&
        (box0.extent == Vector2<double>{ 5.0, 7.0 }),
        "Box construction failed.");

    std::array<Vector2<double>, 4> vertices0{};
    box0.GetVertices(vertices0);
    std::array<Vector2<double>, 4> expectedVertex{};
    expectedVertex[0] = { 3.6, -6.2 };
    expectedVertex[1] = { 9.6, 1.8 };
    expectedVertex[2] = { -7.6, 2.2 };
    expectedVertex[3] = { -1.6, 10.2 };
    for (std::size_t i = 0; i < 4; ++i)
    {
        double error = Length(vertices0[i] - expectedVertex[i]);
        UTAssert(
            error <= 1e-14,
            "Box vertex " + std::to_string(i) + " is incorrect.");
    }

    // TODO: Add tests for comparison operators.
}


#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>

namespace gtl
{
    template class OrientedBox<float, 2>;
    template bool operator==(OrientedBox<float, 2> const&, OrientedBox<float, 2> const&);
    template bool operator!=(OrientedBox<float, 2> const&, OrientedBox<float, 2> const&);
    template bool operator< (OrientedBox<float, 2> const&, OrientedBox<float, 2> const&);
    template bool operator<=(OrientedBox<float, 2> const&, OrientedBox<float, 2> const&);
    template bool operator> (OrientedBox<float, 2> const&, OrientedBox<float, 2> const&);
    template bool operator>=(OrientedBox<float, 2> const&, OrientedBox<float, 2> const&);

    template class OrientedBox<double, 3>;
    template bool operator==(OrientedBox<double, 3> const&, OrientedBox<double, 3> const&);
    template bool operator!=(OrientedBox<double, 3> const&, OrientedBox<double, 3> const&);
    template bool operator< (OrientedBox<double, 3> const&, OrientedBox<double, 3> const&);
    template bool operator<=(OrientedBox<double, 3> const&, OrientedBox<double, 3> const&);
    template bool operator> (OrientedBox<double, 3> const&, OrientedBox<double, 3> const&);
    template bool operator>=(OrientedBox<double, 3> const&, OrientedBox<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class OrientedBox<Rational, 4>;
    template bool operator==(OrientedBox<Rational, 4> const&, OrientedBox<Rational, 4> const&);
    template bool operator!=(OrientedBox<Rational, 4> const&, OrientedBox<Rational, 4> const&);
    template bool operator< (OrientedBox<Rational, 4> const&, OrientedBox<Rational, 4> const&);
    template bool operator<=(OrientedBox<Rational, 4> const&, OrientedBox<Rational, 4> const&);
    template bool operator> (OrientedBox<Rational, 4> const&, OrientedBox<Rational, 4> const&);
    template bool operator>=(OrientedBox<Rational, 4> const&, OrientedBox<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(OrientedBox)
