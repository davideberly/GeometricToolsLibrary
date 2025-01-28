#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistAlignedBoxAlignedBox.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistAlignedBoxAlignedBox
    {
    public:
        UnitTestDistAlignedBoxAlignedBox();

    private:
        void Test2D();
        void Test3D();
    };
}

UnitTestDistAlignedBoxAlignedBox::UnitTestDistAlignedBoxAlignedBox()
{
    UTInformation("Mathematics/Distance/ND/DistAlignedBoxAlignedBox");

    Test2D();
    Test3D();
}

void UnitTestDistAlignedBoxAlignedBox::Test2D()
{
    using BBQuery = DCPQuery<double, AlignedBox2<double>, AlignedBox2<double>>;
    BBQuery query{};
    BBQuery::Output output{};
    Vector2<double> expectedClosest{};
    std::array<AlignedBox2<double>, 2> box{};

    // overlap has positive area
    box[0].min = { 0.0, 0.0 };
    box[0].max = { 1.0, 1.0 };
    box[1].min = { 0.5, 0.75 };
    box[1].max = { 1.5, 1.25 };
    output = query(box[0], box[1]);
    UTAssert(
        output.distance == 0.0,
        "Incorrect squared distance.");
    UTAssert(
        (output.closest[0].min == Vector2<double>{ 0.5, 0.75 }) &&
        (output.closest[0].max == Vector2<double>{ 1.0, 1.0 }),
        "Incorrect closest[0].");
    UTAssert(
        output.closest[1] == output.closest[0],
        "Incorrect closest[1].");

    // overlap is a segment on y = 1
    box[0].min = { 0.0, 0.0 };
    box[0].max = { 1.0, 1.0 };
    box[1].min = { 0.5, 1.0 };
    box[1].max = { 1.5, 1.25 };
    output = query(box[0], box[1]);
    UTAssert(
        output.distance == 0.0,
        "Incorrect squared distance.");
    UTAssert(
        (output.closest[0].min == Vector2<double>{ 0.5, 1.0 }) &&
        (output.closest[0].max == Vector2<double>{ 1.0, 1.0 }),
        "Incorrect closest[0].");
    UTAssert(
        output.closest[1] == output.closest[0],
        "Incorrect closest[1].");

    // overlap is a segment on x = 0
    box[0].min = { 0.0, 0.0 };
    box[0].max = { 1.0, 1.0 };
    box[1].min = { -0.5, 0.75 };
    box[1].max = { 0.0, 1.25 };
    output = query(box[0], box[1]);
    UTAssert(
        output.distance == 0.0,
        "Incorrect squared distance.");
    UTAssert(
        (output.closest[0].min == Vector2<double>{ 0.0, 0.75 }) &&
        (output.closest[0].max == Vector2<double>{ 0.0, 1.0 }),
        "Incorrect closest[0].");
    UTAssert(
        output.closest[1] == output.closest[0],
        "Incorrect closest[1].");

    // separated, closest sets are segments
    box[0].min = { 0.0, 0.0 };
    box[0].max = { 1.0, 1.0 };
    box[1].min = { -0.5, 0.75 };
    box[1].max = { -0.25, 1.25 };
    output = query(box[0], box[1]);
    UTAssert(
        output.distance == 0.25,
        "Incorrect squared distance.");
    UTAssert(
        (output.closest[0].min == Vector2<double>{ 0.0, 0.75 }) &&
        (output.closest[0].max == Vector2<double>{ 0.0, 1.0 }),
        "Incorrect closest[0].");
    UTAssert(
        (output.closest[1].min == Vector2<double>{ -0.25, 0.75 }) &&
        (output.closest[1].max == Vector2<double>{ -0.25, 1.0 }),
        "Incorrect closest[1].");

    // separated, closest sets are points
    box[0].min = { 0.0, 0.0 };
    box[0].max = { 1.0, 1.0 };
    box[1].min = { -0.5, 1.25 };
    box[1].max = { -0.25, 1.5 };
    output = query(box[0], box[1]);
    UTAssert(
        output.sqrDistance == 0.125,
        "Incorrect squared distance.");
    UTAssert(
        (output.closest[0].min == Vector2<double>{ 0.0, 1.0 }) &&
        (output.closest[0].max == Vector2<double>{ 0.0, 1.0 }),
        "Incorrect closest[0].");
    UTAssert(
        (output.closest[1].min == Vector2<double>{ -0.25, 1.25 }) &&
        (output.closest[1].max == Vector2<double>{ -0.25, 1.25 }),
        "Incorrect closest[1].");
}

void UnitTestDistAlignedBoxAlignedBox::Test3D()
{
    using BBQuery = DCPQuery<double, AlignedBox3<double>, AlignedBox3<double>>;
    BBQuery query{};
    BBQuery::Output output{};
    Vector3<double> expectedClosest{};
    std::array<AlignedBox3<double>, 2> box{};

    // overlap has positive volume
    box[0].min = { 0.0, 0.0, 0.0 };
    box[0].max = { 1.0, 1.0, 1.0 };
    box[1].min = { 0.5, 0.75, 0.25 };
    box[1].max = { 1.5, 1.25, 0.75 };
    output = query(box[0], box[1]);
    UTAssert(
        output.distance == 0.0,
        "Incorrect squared distance.");
    UTAssert(
        (output.closest[0].min == Vector3<double>{ 0.5, 0.75, 0.25 }) &&
        (output.closest[0].max == Vector3<double>{ 1.0, 1.0, 0.75 }),
        "Incorrect closest[0].");
    UTAssert(
        output.closest[1] == output.closest[0],
        "Incorrect closest[1].");

    // overlap is a rectangle on z = 1
    box[0].min = { 0.0, 0.0, 0.0 };
    box[0].max = { 1.0, 1.0, 1.0 };
    box[1].min = { 0.5, 0.75, 1.0 };
    box[1].max = { 1.5, 1.25, 1.5 };
    output = query(box[0], box[1]);
    UTAssert(
        output.distance == 0.0,
        "Incorrect squared distance.");
    UTAssert(
        (output.closest[0].min == Vector3<double>{ 0.5, 0.75, 1.0 }) &&
        (output.closest[0].max == Vector3<double>{ 1.0, 1.0, 1.0 }),
        "Incorrect closest[0].");
    UTAssert(
        output.closest[1] == output.closest[0],
        "Incorrect closest[1].");

    // overlap is a segment on (y,z) = (1,1)
    box[0].min = { 0.0, 0.0, 0.0 };
    box[0].max = { 1.0, 1.0, 1.0 };
    box[1].min = { 0.25, 1.0, 1.0 };
    box[1].max = { 0.75, 1.25, 1.5 };
    output = query(box[0], box[1]);
    UTAssert(
        output.distance == 0.0,
        "Incorrect squared distance.");
    UTAssert(
        (output.closest[0].min == Vector3<double>{ 0.25, 1.0, 1.0 }) &&
        (output.closest[0].max == Vector3<double>{ 0.75, 1.0, 1.0 }),
        "Incorrect closest[0].");
    UTAssert(
        output.closest[1] == output.closest[0],
        "Incorrect closest[1].");

    // closests sets are rectangles
    box[0].min = { 0.0, 0.0, 0.0 };
    box[0].max = { 1.0, 1.0, 1.0 };
    box[1].min = { 0.5, 0.75, 1.25 };
    box[1].max = { 1.5, 1.25, 1.5 };
    output = query(box[0], box[1]);
    UTAssert(
        output.distance == 0.25,
        "Incorrect squared distance.");
    UTAssert(
        (output.closest[0].min == Vector3<double>{ 0.5, 0.75, 1.0 }) &&
        (output.closest[0].max == Vector3<double>{ 1.0, 1.0, 1.0 }),
        "Incorrect closest[0].");
    UTAssert(
        (output.closest[1].min == Vector3<double>{ 0.5, 0.75, 1.25 }) &&
        (output.closest[1].max == Vector3<double>{ 1.0, 1.0, 1.25 }),
        "Incorrect closest[1].");

    // closests sets are segments
    box[0].min = { 0.0, 0.0, 0.0 };
    box[0].max = { 1.0, 1.0, 1.0 };
    box[1].min = { 0.5, 1.25, 1.25 };
    box[1].max = { 1.5, 1.75, 1.5 };
    output = query(box[0], box[1]);
    UTAssert(
        output.sqrDistance == 0.125,
        "Incorrect squared distance.");
    UTAssert(
        (output.closest[0].min == Vector3<double>{ 0.5, 1.0, 1.0 }) &&
        (output.closest[0].max == Vector3<double>{ 1.0, 1.0, 1.0 }),
        "Incorrect closest[0].");
    UTAssert(
        (output.closest[1].min == Vector3<double>{ 0.5, 1.25, 1.25 }) &&
        (output.closest[1].max == Vector3<double>{ 1.0, 1.25, 1.25 }),
        "Incorrect closest[1].");

    // closests sets are points
    box[0].min = { 0.0, 0.0, 0.0 };
    box[0].max = { 1.0, 1.0, 1.0 };
    box[1].min = { 1.25, 1.25, 1.25 };
    box[1].max = { 1.75, 1.75, 1.75 };
    output = query(box[0], box[1]);
    UTAssert(
        output.sqrDistance == 0.1875,
        "Incorrect squared distance.");
    UTAssert(
        (output.closest[0].min == Vector3<double>{ 1.0, 1.0, 1.0 }) &&
        (output.closest[0].max == Vector3<double>{ 1.0, 1.0, 1.0 }),
        "Incorrect closest[0].");
    UTAssert(
        (output.closest[1].min == Vector3<double>{ 1.25, 1.25, 1.25 }) &&
        (output.closest[1].max == Vector3<double>{ 1.25, 1.25, 1.25 }),
        "Incorrect closest[1].");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistAlignedBoxAlignedBox.h>

namespace gtl
{
    template class DCPQuery<float, AlignedBox<float, 2>, AlignedBox<float, 2>>;
    template class DCPQuery<double, AlignedBox<double, 3>, AlignedBox<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, AlignedBox<Rational, 4>, AlignedBox<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistAlignedBoxAlignedBox)
