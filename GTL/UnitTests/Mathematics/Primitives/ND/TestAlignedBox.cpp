#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/AlignedBox.h>
using namespace gtl;

namespace gtl
{
    class UnitTestAlignedBox
    {
    public:
        UnitTestAlignedBox();

    private:
        void Test();
    };
}

UnitTestAlignedBox::UnitTestAlignedBox()
{
    UTInformation("Mathematics/Primitives/ND/AlignedBox");

    Test();
}

void UnitTestAlignedBox::Test()
{
    AlignedBox<double, 2> box0{};
    UTAssert(
        box0.min[0] == 0.0 && box0.min[1] == 0.0 && box0.max[0] == 0.0 && box0.max[1] == 0.0,
        "Expecting default degenerate box0.");

    box0 = AlignedBox<double, 2>({ 1.0f, 2.0f }, { 3.0f, 4.0f });
    UTAssert(
        box0.min[0] == 1.0 && box0.min[1] == 2.0 && box0.max[0] == 3.0 && box0.max[1] == 4.0,
        "Box construction failed.");

    Vector<double, 2> center{}, extent{};
    box0.GetCenteredForm(center, extent);
    UTAssert(
        center[0] == 2.0 && center[1] == 3.0 && extent[0] == 1.0 && extent[1] == 1.0,
        "The center or extent is incorrect.");

    std::array<Vector2<double>, 4> vertices0{};
    box0.GetVertices(vertices0);
    UTAssert(
        (vertices0[0] == Vector2<double>{ 1.0, 2.0 }) &&
        (vertices0[1] == Vector2<double>{ 3.0, 2.0 }) &&
        (vertices0[2] == Vector2<double>{ 1.0, 4.0 }) &&
        (vertices0[3] == Vector2<double>{ 3.0, 4.0 }),
        "Incorrect box vertex.");

    AlignedBox<double, 2> box1({ 5.0f, 6.0f }, { 7.0f, 8.0f });

    bool equal = (box0 == box0);
    UTAssert(
        equal,
        "operator== failed");

    bool notEqual = (box0 != box1);
    UTAssert(
        notEqual,
        "operato!== failed");

    bool lessThan = (box0 < box1);
    UTAssert(
        lessThan,
        "operator< failed");

    bool lessThanOrEqual = (box0 <= box1);
    UTAssert(
        lessThanOrEqual,
        "operator< failed");

    lessThanOrEqual = (box1 <= box0);
    UTAssert(
        !lessThanOrEqual,
        "operator< failed");

    bool greaterThan = (box1 > box0);
    UTAssert(
        greaterThan,
        "operator> failed");

    bool greaterThanOrEqual = (box1 >= box0);
    UTAssert(
        greaterThanOrEqual,
        "operator>= failed");

    greaterThanOrEqual = (box0 >= box1);
    UTAssert(
        !greaterThanOrEqual,
        "operator>= failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/AlignedBox.h>

namespace gtl
{
    template class AlignedBox<float, 2>;
    template bool operator==(AlignedBox<float, 2> const&, AlignedBox<float, 2> const&);
    template bool operator!=(AlignedBox<float, 2> const&, AlignedBox<float, 2> const&);
    template bool operator< (AlignedBox<float, 2> const&, AlignedBox<float, 2> const&);
    template bool operator<=(AlignedBox<float, 2> const&, AlignedBox<float, 2> const&);
    template bool operator> (AlignedBox<float, 2> const&, AlignedBox<float, 2> const&);
    template bool operator>=(AlignedBox<float, 2> const&, AlignedBox<float, 2> const&);

    template class AlignedBox<double, 3>;
    template bool operator==(AlignedBox<double, 3> const&, AlignedBox<double, 3> const&);
    template bool operator!=(AlignedBox<double, 3> const&, AlignedBox<double, 3> const&);
    template bool operator< (AlignedBox<double, 3> const&, AlignedBox<double, 3> const&);
    template bool operator<=(AlignedBox<double, 3> const&, AlignedBox<double, 3> const&);
    template bool operator> (AlignedBox<double, 3> const&, AlignedBox<double, 3> const&);
    template bool operator>=(AlignedBox<double, 3> const&, AlignedBox<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class AlignedBox<Rational, 4>;
    template bool operator==(AlignedBox<Rational, 4> const&, AlignedBox<Rational, 4> const&);
    template bool operator!=(AlignedBox<Rational, 4> const&, AlignedBox<Rational, 4> const&);
    template bool operator< (AlignedBox<Rational, 4> const&, AlignedBox<Rational, 4> const&);
    template bool operator<=(AlignedBox<Rational, 4> const&, AlignedBox<Rational, 4> const&);
    template bool operator> (AlignedBox<Rational, 4> const&, AlignedBox<Rational, 4> const&);
    template bool operator>=(AlignedBox<Rational, 4> const&, AlignedBox<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(AlignedBox)
