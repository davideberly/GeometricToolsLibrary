#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3AlignedBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrLine3AlignedBox3
    {
    public:
        UnitTestIntrLine3AlignedBox3();

    private:
        void Test();
    };
}

UnitTestIntrLine3AlignedBox3::UnitTestIntrLine3AlignedBox3()
{
    UTInformation("Mathematics/Intersection/3D/IntrLine3AlignedBox3");

    Test();
}

void UnitTestIntrLine3AlignedBox3::Test()
{
    Line3<double> line{};
    AlignedBox3<double> box{};
    box.min = { 0.0, 0.0, 0.0 };
    box.max = { 1.0, 1.0, 1.0 };

    line.origin = { -0.1, -0.2, -0.3 };
    line.direction = { 1.0, 1.0, 1.0 };
    Normalize(line.direction);

    FIQuery<double, Line3<double>, AlignedBox3<double>> fiQuery{};
    TIQuery<double, Line3<double>, AlignedBox3<double>> tiQuery{};

    // 2 intersections
    auto fiOutput = fiQuery(line, box);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2,
        "Invalid FIQuery.");
    Vector3<double> diff = fiOutput.point[0] - Vector3<double>{0.2, 0.1, 0.0};
    double error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    diff = fiOutput.point[1] - Vector3<double>{1.0, 0.9, 0.8};
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    auto tiOutput = tiQuery(line, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // no intersection
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, box);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, box);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // tangential intersection
    line.origin = { -1.0, 0.0, 0.3 };
    fiOutput = fiQuery(line, box);
    
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        (fiOutput.point[0] == Vector3<double>{0.0, 0.0, 0.3}) &&
        (fiOutput.point[1] == Vector3<double>{1.0, 0.0, 0.3}),
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // 1 intersection
    box.max = { 2.0, 1.0, 1.5 };
    line.origin = { 4.0, 0.5, 0.0 };
    line.direction = { -4.0, 0.0, 3.0 };
    fiOutput = fiQuery(line, box);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1,
        "Invalid FIQuery.");
    diff = fiOutput.point[0] - Vector3<double>{ 2.0, 0.5, 1.5 };
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrLine3AlignedBox3.h>

namespace gtl
{
    template class TIQuery<float, Line3<float>, AlignedBox3<float>>;
    template class FIQuery<float, Line3<float>, AlignedBox3<float>>;

    template class TIQuery<double, Line3<double>, AlignedBox3<double>>;
    template class FIQuery<double, Line3<double>, AlignedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Line3<Rational>, AlignedBox3<Rational>>;
    template class FIQuery<Rational, Line3<Rational>, AlignedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrLine3AlignedBox3)
