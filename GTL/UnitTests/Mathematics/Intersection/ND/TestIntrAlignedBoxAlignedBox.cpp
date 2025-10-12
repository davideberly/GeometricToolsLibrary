#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/ND/IntrAlignedBoxAlignedBox.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrAlignedBoxAlignedBox
    {
    public:
        UnitTestIntrAlignedBoxAlignedBox();

    private:
        using AATIQuery = TIQuery<double, AlignedBox2<double>, AlignedBox2<double>>;
        using AATIOutput = AATIQuery::Output;
        void TestTIQuery();

        using AAFIQuery = FIQuery<double, AlignedBox2<double>, AlignedBox2<double>>;
        using AAFIOutput = AAFIQuery::Output;
        void TestFIQuery();
    };
}

UnitTestIntrAlignedBoxAlignedBox::UnitTestIntrAlignedBoxAlignedBox()
{
    UTInformation("Mathematics/Intersection/2D/IntrAlignedBoxAlignedBox");

    TestTIQuery();
    TestFIQuery();
}

void UnitTestIntrAlignedBoxAlignedBox::TestTIQuery()
{
    AATIQuery query{};
    AATIOutput output{};
    AlignedBox2<double> box0{}, box1{};

    box0.min = { 0.0, 0.0 };
    box0.max = { 1.0, 1.0 };
    box1.min = { 2.0, 0.5 };
    box1.max = { 3.0, 1.5 };
    output = query(box0, box1);
    UTAssert(output.intersect == false, "Invalid TIQuery result.");

    box1.min = { 1.0, 0.5 };
    box1.max = { 2.0, 1.5 };
    output = query(box0, box1);
    UTAssert(output.intersect == true, "Invalid TIQuery result.");

    box1.min = { 0.5, 0.5 };
    box1.max = { 1.5, 1.5 };
    output = query(box0, box1);
    UTAssert(output.intersect == true, "Invalid TIQuery result.");

    box1.min = { -1.0, 0.5 };
    box1.max = { 0.0, 1.5 };
    output = query(box0, box1);
    UTAssert(output.intersect == true, "Invalid TIQuery result.");

    box1.min = { -1.0, 0.5 };
    box1.max = { 0.0, 1.5 };
    output = query(box0, box1);
    UTAssert(output.intersect == true, "Invalid TIQuery result.");

    box1.min = { -2.0, 0.5 };
    box1.max = { -1.0, 1.5 };
    output = query(box0, box1);
    UTAssert(output.intersect == false, "Invalid TIQuery result.");

    // swap the (x,y) to (y,x) of the previous boxes
    box0.min = { 0.0, 0.0 };
    box0.max = { 1.0, 1.0 };
    box1.min = { 0.5, 2.0 };
    box1.max = { 1.5, 3.0 };
    output = query(box0, box1);
    UTAssert(output.intersect == false, "Invalid TIQuery result.");

    box1.min = { 0.5, 1.0 };
    box1.max = { 1.5, 2.0 };
    output = query(box0, box1);
    UTAssert(output.intersect == true, "Invalid TIQuery result.");

    box1.min = { 0.5, 0.5 };
    box1.max = { 1.5, 1.5 };
    output = query(box0, box1);
    UTAssert(output.intersect == true, "Invalid TIQuery result.");

    box1.min = { 0.5, -1.0 };
    box1.max = { 1.5, 0.0 };
    output = query(box0, box1);
    UTAssert(output.intersect == true, "Invalid TIQuery result.");

    box1.min = { 0.5, -1.0 };
    box1.max = { 1.5, 0.0 };
    output = query(box0, box1);
    UTAssert(output.intersect == true, "Invalid TIQuery result.");

    box1.min = { 0.5 , -2.0 };
    box1.max = { 1.5 , -1.0 };
    output = query(box0, box1);
    UTAssert(output.intersect == false, "Invalid TIQuery result.");
}

void UnitTestIntrAlignedBoxAlignedBox::TestFIQuery()
{
    AAFIQuery query{};
    AAFIOutput output{};
    AlignedBox2<double> box0{}, box1{};

    box0.min = { 0.0, 0.0 };
    box0.max = { 1.0, 1.0 };
    box1.min = { 2.0, 0.5 };
    box1.max = { 3.0, 1.5 };
    output = query(box0, box1);
    UTAssert(output.intersect == false, "Invalid FIQuery result.");

    box1.min = { 1.0, 0.5 };
    box1.max = { 2.0, 1.5 };
    output = query(box0, box1);
    UTAssert(
        output.intersect == true &&
        output.box.min[0] == 1.0 && output.box.min[1] == 0.5 &&
        output.box.max[0] == 1.0 && output.box.max[1] == 1.0,
        "Invalid FIQuery result.");

    box1.min = { 0.5, 0.5 };
    box1.max = { 1.5, 1.5 };
    output = query(box0, box1);
    UTAssert(
        output.intersect == true &&
        output.box.min[0] == 0.5 && output.box.min[1] == 0.5 &&
        output.box.max[0] == 1.0 && output.box.max[1] == 1.0,
        "Invalid FIQuery result.");

    box1.min = { -1.0, 0.5 };
    box1.max = { 0.0, 1.5 };
    output = query(box0, box1);
    UTAssert(
        output.intersect == true &&
        output.box.min[0] == 0.0 && output.box.min[1] == 0.5 &&
        output.box.max[0] == 0.0 && output.box.max[1] == 1.0,
        "Invalid FIQuery result.");

    box1.min = { -2.0, 0.5 };
    box1.max = { -1.0, 1.5 };
    output = query(box0, box1);
    UTAssert(output.intersect == false, "Invalid FIQuery result.");

    // swap the (x,y) to (y,x) of the previous boxes
    box0.min = { 0.0, 0.0 };
    box0.max = { 1.0, 1.0 };
    box1.min = { 0.5, 2.0 };
    box1.max = { 1.5, 3.0 };
    output = query(box0, box1);
    UTAssert(output.intersect == false, "Invalid FIQuery result.");

    box1.min = { 0.5, 1.0 };
    box1.max = { 1.5, 2.0 };
    output = query(box0, box1);
    UTAssert(
        output.intersect == true &&
        output.box.min[0] == 0.5 && output.box.min[1] == 1.0 &&
        output.box.max[0] == 1.0 && output.box.max[1] == 1.0,
        "Invalid FIQuery result.");

    box1.min = { 0.5, 0.5 };
    box1.max = { 1.5, 1.5 };
    output = query(box0, box1);
    UTAssert(
        output.intersect == true &&
        output.box.min[0] == 0.5 && output.box.min[1] == 0.5 &&
        output.box.max[0] == 1.0 && output.box.max[1] == 1.0,
        "Invalid FIQuery result.");

    box1.min = { 0.5, -1.0 };
    box1.max = { 1.5, 0.0 };
    output = query(box0, box1);
    UTAssert(
        output.intersect == true &&
        output.box.min[0] == 0.5 && output.box.min[1] == 0.0 &&
        output.box.max[0] == 1.0 && output.box.max[1] == 0.0,
        "Invalid FIQuery result.");

    box1.min = { 0.5 , -2.0 };
    box1.max = { 1.5 , -1.0 };
    output = query(box0, box1);
    UTAssert(output.intersect == false, "Invalid FIQuery result.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/ND/IntrAlignedBoxAlignedBox.h>

namespace gtl
{
    template class TIQuery<float, AlignedBox2<float>, AlignedBox2<float>>;
    template class FIQuery<float, AlignedBox2<float>, AlignedBox2<float>>;
    template class TIQuery<float, AlignedBox3<float>, AlignedBox3<float>>;
    template class FIQuery<float, AlignedBox3<float>, AlignedBox3<float>>;

    template class TIQuery<double, AlignedBox2<double>, AlignedBox2<double>>;
    template class FIQuery<double, AlignedBox2<double>, AlignedBox2<double>>;
    template class TIQuery<double, AlignedBox3<double>, AlignedBox3<double>>;
    template class FIQuery<double, AlignedBox3<double>, AlignedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, AlignedBox2<Rational>, AlignedBox2<Rational>>;
    template class FIQuery<Rational, AlignedBox2<Rational>, AlignedBox2<Rational>>;
    template class TIQuery<Rational, AlignedBox3<Rational>, AlignedBox3<Rational>>;
    template class FIQuery<Rational, AlignedBox3<Rational>, AlignedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrAlignedBoxAlignedBox)
