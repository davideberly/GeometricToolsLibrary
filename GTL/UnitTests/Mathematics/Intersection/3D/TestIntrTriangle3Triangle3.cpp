#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Intersection/3D/IntrTriangle3Triangle3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrTriangle3Triangle3
    {
    public:
        UnitTestIntrTriangle3Triangle3();

    private:
        void ExampleDoubleIntersectionOfTriangles();
        void ExampleRationalIntersectionOfTriangles();
        void TestTIQuery();
        void TestFIQuery();
        void TestFIQueryTranslate();
        void TestFIQueryRigid();
        void TestFIQueryRational();
    };
}

UnitTestIntrTriangle3Triangle3::UnitTestIntrTriangle3Triangle3()
{
    UTInformation("Mathematics/Intersection/3D/IntrTriangle3Triangle3");

    ExampleDoubleIntersectionOfTriangles();
    ExampleRationalIntersectionOfTriangles();
    TestTIQuery();
    TestFIQuery();
    TestFIQueryTranslate();
    TestFIQueryRigid();
    TestFIQueryRational();
}

void UnitTestIntrTriangle3Triangle3::ExampleDoubleIntersectionOfTriangles()
{
    using FIQueryTri3Tri3 = FIQuery<double, Triangle3<double>, Triangle3<double>>;
    using FIOutputTri3Tri3 = FIQueryTri3Tri3::Output;
    Triangle3<double> tri0{}, tri1{};
    FIQueryTri3Tri3 query{};
    FIOutputTri3Tri3 output{};

    // Choose triangles for which it is easy to see that the intersection is a
    // line segment.
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 0.125 };
    tri1.v[1] = { 0.5, 0.75, -1.0 };
    tri1.v[2] = { -0.5, -1.0, -1.0 };
    output = query(tri0, tri1);
    // output.intersect = true
    // output.intersection.size() = 2
    // output.intersection[0] = { 0.48989898989898989, 0.51010101010101006, 0.0 }
    // output.intersection[1] = { 0.38888888888888890, 0.33333333333333337, 0.0 }
    // theoretical intersection[0] = { 485/990, 505/990, 0 }
    // theoretical intersection[1] = { 35/90, 3/9, 0 }
    // The output.intersection[] points are not the theoretical points because
    // of floating-point rounding error.

    // Apply a rigid motion to the triangles.
    Vector3<double> trn{ 0.25, 0.125, 4.0 };
    Matrix3x3<double> rot{};
    rot.SetRow(0, { 0.13333333333333353, -0.66666666666666663, 0.73333333333333317 });
    rot.SetRow(1, { 0.93333333333333324, 0.33333333333333348, 0.13333333333333336 });
    rot.SetRow(2, { -0.33333333333333326, 0.66666666666666652, 0.66666666666666674 });
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    output = query(tri0, tri1);
    // output.intersect = true
    // output.intersection.size() = 2
    // output.intersection[0] = { -0.024747474747474685, 0.75227272727272720, 4.1767676767676765 }
    // output.intersection[1] = { 0.079629629629629717, 0.59907407407407398, 4.0925925925925926 }
    // theoretical intersection[0] = { -245/9900, 74475/99000 , 4135/990  }
    // theoretical intersection[1] = { 7955/99900, 598475/999000, 40885/9990 }
    // The output.intersection[] points are not the theoretical points because
    // of floating-point rounding error.

    // Triangle tri0 lives in the xy-plane (z = 0). Triangle tri1 lives in
    // the halfspace z >= 0 and touches the plane z = 0 only at the vertex
    // tri1.v[1] = { 0.25, 0.25, 0.0 }.
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 1.0 };
    output = query(tri0, tri1);
    // output.intersect = true
    // output.intersection.size() = 1
    // output.intersection[0] = { 0.25, 0.25, 0.0 }
    //
    // Apply a rigid motion to the triangles. Floating-point rounding errors
    // lead to 2 points of intersection, but theoretically there is only 1
    // point of intersection.
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    output = query(tri0, tri1);
    // output.intersect = true
    // output.intersection.size() = 2
    // output.intersection[0] = { 0.11666666666666697, 0.44166666666666671, 4.0833333333333330 }
    // output.intersection[1] = { 0.11666666666666681, 0.44166666666666671, 4.0833333333333330 }
    // theoretical intersection[0] = { 105/900, 3975/9000, 3675/900 }

    // Triangle tri0 lives in the xy-plane (z = 0). Triangle tri1 lives in
    // the halfspace z <= 0 and touches the plane z = 0 only at the vertex
    // tri1.v[1] = { 0.25, 0.25, 0.0 }.
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, -1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, -1.0 };
    // Apply a rigid motion to the triangles. Floating-point rounding errors
    // lead to 0 points of intersection, but theoretically there is 1 point of
    // intersection.
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    output = query(tri0, tri1);
    // output.intersect = false
    // output.intersection.size() = 0
    // theoretical intersection[0] = { 105/900, 3975/9000, 3675/900 }
}

void UnitTestIntrTriangle3Triangle3::ExampleRationalIntersectionOfTriangles()
{
    using Rational = BSRational<UIntegerAP32>;
    using FIQueryTri3Tri3 = FIQuery<Rational, Triangle3<Rational>, Triangle3<Rational>>;
    using FIOutputTri3Tri3 = FIQueryTri3Tri3::Output;
    Triangle3<Rational> tri0{}, tri1{};
    FIQueryTri3Tri3 query{};
    FIOutputTri3Tri3 output{};
    Vector3<Rational> diff{};
    Rational error{};

    // Choose triangles for which it is easy to see that the intersection is a
    // line segment.
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 0.125 };
    tri1.v[1] = { 0.5, 0.75, -1.0 };
    tri1.v[2] = { -0.5, -1.0, -1.0 };
    output = query(tri0, tri1);
    // output.intersect = true
    // output.intersection.size() = 2
    // output.intersection[0] = { 485/990, 505/990, 0 }
    // output.intersection[1] = { 35/90, 3/9, 0 }
    diff = output.intersection[0] - Vector3<Rational>{ Rational(485, 990), Rational(505, 990), Rational(0) };
    error = Length(diff);  // = 0
    diff = output.intersection[1] - Vector3<Rational>{ Rational(35, 90), Rational(3, 9), Rational(0) };
    error = Length(diff);  // = 0

    // Apply a rigid motion to the triangles.
    Vector3<Rational> trn{ 0.25, 0.125, 4.0 };
    Matrix3x3<Rational> rot{};
    rot(0, 0) = Rational(4, 30);
    rot(0, 1) = Rational(-20, 30);
    rot(0, 2) = Rational(22, 30);
    rot(1, 0) = Rational(28, 30);
    rot(1, 1) = Rational(10, 30);
    rot(1, 2) = Rational(4, 30);
    rot(2, 0) = Rational(-10, 30);
    rot(2, 1) = Rational(20, 30);
    rot(2, 2) = Rational(20, 30);
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    output = query(tri0, tri1);
    // output.intersect = true
    // output.intersection.size() = 2
    // output.intersection[0] = { -245/9900, 74475/99000 , 4135/990  }
    // output.intersection[1] = { 7955/99900, 598475/999000, 40885/9990 }
    diff = output.intersection[0] - Vector3<Rational>{ Rational(-245, 9900), Rational(74475, 99000), Rational(4135, 990) };
    error = Length(diff);  // = 0
    diff = output.intersection[1] - Vector3<Rational>{ Rational(7955, 99900), Rational(598475, 999000), Rational(40885, 9990) };
    error = Length(diff);  // = 0

    // Triangle tri0 lives in the xy-plane (z = 0). Triangle tri1 lives in
    // the halfspace z >= 0 and touches the plane z = 0 only at the vertex
    // tri1.v[1] = { 0.25, 0.25, 0.0 }.
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 1.0 };
    output = query(tri0, tri1);
    // output.intersect = true
    // output.intersection.size() = 1
    // output.intersection[0] = { 0.25, 0.25, 0.0 }
    diff = output.intersection[0] - Vector3<Rational>{ 0.25, 0.25, 0.0 };
    error = Length(diff);  // = 0

    // Apply a rigid motion to the triangles.
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    output = query(tri0, tri1);
    // output.intersect = true
    // output.intersection.size() = 1
    // output.intersection[0] = { 105/900, 3975/9000, 3675/900 }
    diff = output.intersection[0] - Vector3<Rational>{ Rational(105, 900), Rational(3975, 9000), Rational(3675, 900) };
    error = Length(diff);  // = 0

    // Triangle tri0 lives in the xy-plane (z = 0). Triangle tri1 lives in
    // the halfspace z <= 0 and touches the plane z = 0 only at the vertex
    // tri1.v[1] = { 0.25, 0.25, 0.0 }.
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, -1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, -1.0 };
    // Apply a rigid motion to the triangles.
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    output = query(tri0, tri1);
    // output.intersect = true
    // output.intersection.size() = 1
    // output.intersection[0] = { 105/900, 3975/9000, 3675/900 }
    diff = output.intersection[0] - Vector3<Rational>{ Rational(105, 900), Rational(3975, 9000), Rational(3675, 900) };
    error = Length(diff);  // = 0

    // Illustration of converting Rational to double. The Rational has
    // arbitrary precision, typically larger than double, so the conversion
    // can have rounding errors.
    Vector3<double> convert
    {
        output.intersection[0][0],
        output.intersection[0][1],
        output.intersection[0][2]
    };
    // convert = { 0.11666666666666667, 0.44166666666666665, 4.0833333333333330 }
    (void)convert;
}

void UnitTestIntrTriangle3Triangle3::TestTIQuery()
{
    using TIQueryTri3Tri3 = TIQuery<double, Triangle3<double>, Triangle3<double>>;
    using TIOutputTri3Tri3 = TIQueryTri3Tri3::Output;
    Triangle3<double> tri0{}, tri1{};
    TIQueryTri3Tri3 tiQuery{};
    TIOutputTri3Tri3 tiOutput{};

    // Separated by plane normal of triangle0.
    tri0.v[0] = { 1.0, 0.0, 0.0 };
    tri0.v[1] = { 0.0, 1.0, 0.0 };
    tri0.v[2] = { 0.0, 0.0, 1.0 };
    tri1.v[0] = { 2.0, 2.0, 2.0 };
    tri1.v[1] = { 2.5, 2.5, 2.5 };
    tri1.v[2] = { 2.0, 2.0, 4.0 };
    tiOutput = tiQuery(tri0, tri1);
    UTAssert(tiOutput.intersect == false, "Invalid TIQuery.");

    // Separated by plane normal of triangle1.
    std::swap(tri0, tri1);
    tiOutput = tiQuery(tri0, tri1);
    UTAssert(tiOutput.intersect == false, "Invalid TIQuery.");

    // Nonparallel triangles, no normal separation, separated by an edge.
    std::array<std::array<std::size_t, 3>, 6> permute
    { {
        { 0, 1, 2 },
        { 2, 0, 1 },
        { 1, 2, 0 },
        { 0, 2, 1 },
        { 1, 0, 2 },
        { 2, 0, 1 }
    } };

    Triangle3<double> rawTri0{}, rawTri1{};
    rawTri0.v[0] = { 0.0, 0.0, 0.0 };
    rawTri0.v[1] = { 1.0, 0.0, 0.0 };
    rawTri0.v[2] = { 0.0, 1.0, 0.0 };
    rawTri1.v[0] = { 1.0, 1.0, -2.0 };
    rawTri1.v[1] = { 1.0, 1.0, 2.0 };
    rawTri1.v[2] = { 2.0, 2.0, 1.0 };

    // sqrLengthN0xN1 > 0 block, separated
    for (auto const& order1 : permute)
    {
        tri1.v[0] = rawTri1.v[order1[0]];
        tri1.v[1] = rawTri1.v[order1[1]];
        tri1.v[2] = rawTri1.v[order1[2]];
        for (auto const& order0 : permute)
        {
            tri0.v[0] = rawTri0.v[order0[0]];
            tri0.v[1] = rawTri0.v[order0[1]];
            tri0.v[2] = rawTri0.v[order0[2]];
        }

        tiOutput = tiQuery(tri0, tri1);
        UTAssert(tiOutput.intersect == false, "Invalid TIQuery.");
    }

    // sqrLengthN0xN1 > 0 block, overlapped
    rawTri1.v[0] = { 0.25, 0.25, -2.0 };
    rawTri1.v[1] = { 0.25, 0.25, 2.0 };
    rawTri1.v[2] = { 2.0, 2.0, 1.0 };
    for (auto const& order1 : permute)
    {
        tri1.v[0] = rawTri1.v[order1[0]];
        tri1.v[1] = rawTri1.v[order1[1]];
        tri1.v[2] = rawTri1.v[order1[2]];
        for (auto const& order0 : permute)
        {
            tri0.v[0] = rawTri0.v[order0[0]];
            tri0.v[1] = rawTri0.v[order0[1]];
            tri0.v[2] = rawTri0.v[order0[2]];
        }

        tiOutput = tiQuery(tri0, tri1);
        UTAssert(tiOutput.intersect == true, "Invalid TIQuery.");
    }

    // coplanar block, separated
    rawTri0.v[0] = { 0.0, 0.0, 0.0 };
    rawTri0.v[1] = { 1.0, 0.0, 0.0 };
    rawTri0.v[2] = { 0.0, 1.0, 0.0 };
    rawTri1.v[0] = { 1.0, 1.0, 0.0 };
    rawTri1.v[1] = { 2.0, 1.0, 0.0 };
    rawTri1.v[2] = { 1.0, 2.0, 0.0 };
    for (auto const& order1 : permute)
    {
        tri1.v[0] = rawTri1.v[order1[0]];
        tri1.v[1] = rawTri1.v[order1[1]];
        tri1.v[2] = rawTri1.v[order1[2]];
        for (auto const& order0 : permute)
        {
            tri0.v[0] = rawTri0.v[order0[0]];
            tri0.v[1] = rawTri0.v[order0[1]];
            tri0.v[2] = rawTri0.v[order0[2]];
        }

        tiOutput = tiQuery(tri0, tri1);
        UTAssert(tiOutput.intersect == false, "Invalid TIQuery.");
    }
    for (auto const& order1 : permute)
    {
        tri1.v[0] = rawTri1.v[order1[0]];
        tri1.v[1] = rawTri1.v[order1[1]];
        tri1.v[2] = rawTri1.v[order1[2]];
        for (auto const& order0 : permute)
        {
            tri0.v[0] = rawTri0.v[order0[0]];
            tri0.v[1] = rawTri0.v[order0[1]];
            tri0.v[2] = rawTri0.v[order0[2]];
        }

        tiOutput = tiQuery(tri1, tri0);
        UTAssert(tiOutput.intersect == false, "Invalid TIQuery.");
    }

    // coplanar block, overlapped
    rawTri0.v[0] = { 0.0, 0.0, 0.0 };
    rawTri0.v[1] = { 1.0, 0.0, 0.0 };
    rawTri0.v[2] = { 0.0, 1.0, 0.0 };
    rawTri1.v[0] = { 0.25, 0.25, 0.0 };
    rawTri1.v[1] = { 2.0, 1.0, 0.0 };
    rawTri1.v[2] = { 1.0, 2.0, 0.0 };
    for (auto const& order1 : permute)
    {
        tri1.v[0] = rawTri1.v[order1[0]];
        tri1.v[1] = rawTri1.v[order1[1]];
        tri1.v[2] = rawTri1.v[order1[2]];
        for (auto const& order0 : permute)
        {
            tri0.v[0] = rawTri0.v[order0[0]];
            tri0.v[1] = rawTri0.v[order0[1]];
            tri0.v[2] = rawTri0.v[order0[2]];
        }

        tiOutput = tiQuery(tri0, tri1);
        UTAssert(tiOutput.intersect == true, "Invalid TIQuery.");
    }
    for (auto const& order1 : permute)
    {
        tri1.v[0] = rawTri1.v[order1[0]];
        tri1.v[1] = rawTri1.v[order1[1]];
        tri1.v[2] = rawTri1.v[order1[2]];
        for (auto const& order0 : permute)
        {
            tri0.v[0] = rawTri0.v[order0[0]];
            tri0.v[1] = rawTri0.v[order0[1]];
            tri0.v[2] = rawTri0.v[order0[2]];
        }

        tiOutput = tiQuery(tri1, tri0);
        UTAssert(tiOutput.intersect == true, "Invalid TIQuery.");
    }
}

void UnitTestIntrTriangle3Triangle3::TestFIQuery()
{
    using FIQueryTri3Tri3 = FIQuery<double, Triangle3<double>, Triangle3<double>>;
    using FIOutputTri3Tri3 = FIQueryTri3Tri3::Output;
    Triangle3<double> tri0{}, tri1{};
    FIQueryTri3Tri3 fiQuery{};
    FIOutputTri3Tri3 fiOutput{};

    // (n,p,z) = (3,0,0)
    tri0.v[0] = { 1.0, 0.0, 0.0 };
    tri0.v[1] = { 0.0, 1.0, 0.0 };
    tri0.v[2] = { 0.0, 0.0, 1.0 };
    tri1.v[0] = { 2.0, 2.0, 2.0 };
    tri1.v[1] = { 2.5, 2.5, 2.5 };
    tri1.v[2] = { 2.0, 2.0, 4.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(fiOutput.intersect == false, "Invalid FIQuery.");

    // (n,p,z) = (0,3,0)
    tri0.v[0] = { 1.0, 0.0, 0.0 };
    tri0.v[1] = { 0.0, 1.0, 0.0 };
    tri0.v[2] = { 0.0, 0.0, 1.0 };
    tri1.v[0] = { -2.0, -2.0, -2.0 };
    tri1.v[1] = { -2.5, -2.5, -2.5 };
    tri1.v[2] = { -2.0, -2.0, -4.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(fiOutput.intersect == false, "Invalid FIQuery.");

    // (n,p,z) = (2,1,0)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 0.125 };
    tri1.v[1] = { 0.5, 0.75, -1.0 };
    tri1.v[2] = { -0.5, -1.0, -1.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    Vector3<double> expected0{ 0.48989898989898989, 0.51010101010101006, 0.0 };
    Vector3<double> expected1{ 0.38888888888888890, 0.33333333333333337, 0.0 };
    double error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (1,2,0)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 0.0, 1.0, 0.0 };
    tri0.v[2] = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    expected0 = Vector3<double>{ 0.48989898989898989, 0.51010101010101006, 0.0 };
    expected1 = Vector3<double>{ 0.38888888888888890, 0.33333333333333337, 0.0 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (1,1,1)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 0.125 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { -0.5, -1.0, -1.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    expected0 = Vector3<double>{ 0.25, 0.25, 0.0 };
    expected1 = Vector3<double>{ 0.38888888888888890, 0.33333333333333337, 0.0 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (0,2,1)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 1.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 1,
        "Invalid FIQuery.");
    expected0 = Vector3<double>{ 0.25, 0.25, 0.0 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (2,0,1)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, -1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, -1.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 1,
        "Invalid FIQuery.");
    expected0 = Vector3<double>{ 0.25, 0.25, 0.0 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (0,1,2)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 0.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    expected0 = Vector3<double>{ 0.25, 0.25, 0.0 };
    expected1 = Vector3<double>{ 0.4, 0.0, 0.0 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (1,0,2)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, -1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 0.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    expected0 = Vector3<double>{ 0.25, 0.25, 0.0 };
    expected1 = Vector3<double>{ 0.4, 0.0, 0.0 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
}

void UnitTestIntrTriangle3Triangle3::TestFIQueryTranslate()
{
    using FIQueryTri3Tri3 = FIQuery<double, Triangle3<double>, Triangle3<double>>;
    using FIOutputTri3Tri3 = FIQueryTri3Tri3::Output;
    Triangle3<double> tri0{}, tri1{};
    FIQueryTri3Tri3 fiQuery{};
    FIOutputTri3Tri3 fiOutput{};

    // (n,p,z) = (3,0,0)
    tri0.v[0] = { 1.0, 0.0, 0.0 };
    tri0.v[1] = { 0.0, 1.0, 0.0 };
    tri0.v[2] = { 0.0, 0.0, 1.0 };
    tri1.v[0] = { 2.0, 2.0, 2.0 };
    tri1.v[1] = { 2.5, 2.5, 2.5 };
    tri1.v[2] = { 2.0, 2.0, 4.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(fiOutput.intersect == false, "Invalid FIQuery.");

    // (n,p,z) = (0,3,0)
    tri0.v[0] = { 1.0, 0.0, 0.0 };
    tri0.v[1] = { 0.0, 1.0, 0.0 };
    tri0.v[2] = { 0.0, 0.0, 1.0 };
    tri1.v[0] = { -2.0, -2.0, -2.0 };
    tri1.v[1] = { -2.5, -2.5, -2.5 };
    tri1.v[2] = { -2.0, -2.0, -4.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(fiOutput.intersect == false, "Invalid FIQuery.");

    Vector3<double> trn{ 0.25, 0.125, 4.0 };

    // (n,p,z) = (2,1,0)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 0.125 };
    tri1.v[1] = { 0.5, 0.75, -1.0 };
    tri1.v[2] = { -0.5, -1.0, -1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] += trn;
        tri1.v[i] += trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    Vector3<double> expected0{ 0.73989898989898983, 0.63510101010101006, 4.0 };
    Vector3<double> expected1{ 0.63888888888888884, 0.45833333333333337, 4.0 };
    double error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (1,2,0)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 0.0, 1.0, 0.0 };
    tri0.v[2] = { 1.0, 0.0, 0.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] += trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    expected0 = Vector3<double>{ 0.73989898989898983, 0.63510101010101006, 4.0 };
    expected1 = Vector3<double>{ 0.63888888888888884, 0.45833333333333337, 4.0 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (1,1,1)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 0.125 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { -0.5, -1.0, -1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] += trn;
        tri1.v[i] += trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    expected0 = Vector3<double>{ 0.5, 0.375, 4.0 };
    expected1 = Vector3<double>{ 0.63888888888888884, 0.45833333333333337, 4.0 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (0,2,1)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] += trn;
        tri1.v[i] += trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 1,
        "Invalid FIQuery.");
    expected0 = Vector3<double>{ 0.5, 0.375, 4.0 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (2,0,1)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, -1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, -1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] += trn;
        tri1.v[i] += trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 1,
        "Invalid FIQuery.");
    expected0 = Vector3<double>{ 0.5, 0.375, 4.0 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (0,1,2)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 0.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] += trn;
        tri1.v[i] += trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    expected0 = Vector3<double>{ 0.5, 0.375, 4.0 };
    expected1 = Vector3<double>{ 0.65, 0.125, 4.0 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (1,0,2)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, -1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 0.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] += trn;
        tri1.v[i] += trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    expected0 = Vector3<double>{ 0.5, 0.375, 4.0 };
    expected1 = Vector3<double>{ 0.65, 0.125, 4.0 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
}

void UnitTestIntrTriangle3Triangle3::TestFIQueryRigid()
{
    using FIQueryTri3Tri3 = FIQuery<double, Triangle3<double>, Triangle3<double>>;
    using FIOutputTri3Tri3 = FIQueryTri3Tri3::Output;
    Triangle3<double> tri0{}, tri1{};
    FIQueryTri3Tri3 fiQuery{};
    FIOutputTri3Tri3 fiOutput{};
    Vector3<double> expected0, expected1;
    double error;

    // (n,p,z) = (3,0,0)
    tri0.v[0] = { 1.0, 0.0, 0.0 };
    tri0.v[1] = { 0.0, 1.0, 0.0 };
    tri0.v[2] = { 0.0, 0.0, 1.0 };
    tri1.v[0] = { 2.0, 2.0, 2.0 };
    tri1.v[1] = { 2.5, 2.5, 2.5 };
    tri1.v[2] = { 2.0, 2.0, 4.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(fiOutput.intersect == false, "Invalid FIQuery.");

    // (n,p,z) = (0,3,0)
    tri0.v[0] = { 1.0, 0.0, 0.0 };
    tri0.v[1] = { 0.0, 1.0, 0.0 };
    tri0.v[2] = { 0.0, 0.0, 1.0 };
    tri1.v[0] = { -2.0, -2.0, -2.0 };
    tri1.v[1] = { -2.5, -2.5, -2.5 };
    tri1.v[2] = { -2.0, -2.0, -4.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(fiOutput.intersect == false, "Invalid FIQuery.");

    Vector3<double> trn{ 0.25, 0.125, 4.0 };
    Matrix3x3<double> rot{};
    rot.SetRow(0, { 0.13333333333333353, -0.66666666666666663, 0.73333333333333317 });
    rot.SetRow(1, { 0.93333333333333324, 0.33333333333333348, 0.13333333333333336 });
    rot.SetRow(2, { -0.33333333333333326, 0.66666666666666652, 0.66666666666666674 });

    // (n,p,z) = (2,1,0)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 0.125 };
    tri1.v[1] = { 0.5, 0.75, -1.0 };
    tri1.v[2] = { -0.5, -1.0, -1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    expected0 = { -0.024747474747474685, 0.75227272727272720, 4.1767676767676765 };
    expected1 = { 0.079629629629629717, 0.59907407407407398, 4.0925925925925926 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (1,2,0)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 0.0, 1.0, 0.0 };
    tri0.v[2] = { 1.0, 0.0, 0.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    expected0 = { -0.024747474747474685, 0.75227272727272720, 4.1767676767676765 };
    expected1 = { 0.079629629629629717, 0.59907407407407398, 4.0925925925925926 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (1,1,1)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 0.125 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { -0.5, -1.0, -1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    expected0 = { 0.11666666666666664, 0.44166666666666710, 4.0833333333333330 };
    expected1 = { 0.079629629629629717, 0.59907407407407398, 4.0925925925925926 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (0,2,1)
    // The theoretical values are distance = (1,0,1), but rounding error leads
    // to the following, which is a misclassification. There should be 1 point
    // of intersection but rounding errors generate 2 points.
    //  distance[0] = 0.99999999999999911
    //  distance[1] = -1.5959455978986625e-16
    //  distance[2] = 0.99999999999999878
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    expected0 = { 0.11666666666666697, 0.44166666666666671, 4.0833333333333330 };
    expected1 = { 0.11666666666666681, 0.44166666666666671, 4.0833333333333330 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (2,0,1)
    // The theoretical values are distance = (1,0,1), but rounding error leads
    // to the following, which is a misclassification. There should be 1 point
    // of intersection but rounding errors generate 0 points.
    //  distance[0] = -0.99999999999999922
    //  distance[1] = -1.5959455978986625e-16
    //  distance[2] = -0.99999999999999956
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, -1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, -1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    // (n,p,z) = (0,1,2)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 0.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    expected0 = { 0.11666666666666681, 0.44166666666666671, 4.0833333333333330 };
    expected1 = { 0.30333333333333340, 0.49833333333333329, 3.8666666666666667 };
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (1,0,2)
    // The theoretical values are distance = (-1,0,0), but rounding error leads
    // to the following, which is a misclassification. There should be 2 points
    // of intersection but rounding errors generate 0 points.
    //  distance[0] = -0.99999999999999922
    //  distance[1] = -1.5959455978986625e-16
    //  distance[2] = -3.3306690738754696e-16
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, -1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 0.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");
}

void UnitTestIntrTriangle3Triangle3::TestFIQueryRational()
{
    using Rational = BSRational<UIntegerAP32>;
    using FIQueryTri3Tri3 = FIQuery<Rational, Triangle3<Rational>, Triangle3<Rational>>;
    using FIOutputTri3Tri3 = FIQueryTri3Tri3::Output;
    Triangle3<Rational> tri0{}, tri1{};
    FIQueryTri3Tri3 fiQuery{};
    FIOutputTri3Tri3 fiOutput{};
    Vector3<Rational> expected0{}, expected1{};
    Rational error;

    // (n,p,z) = (3,0,0)
    tri0.v[0] = { 1.0, 0.0, 0.0 };
    tri0.v[1] = { 0.0, 1.0, 0.0 };
    tri0.v[2] = { 0.0, 0.0, 1.0 };
    tri1.v[0] = { 2.0, 2.0, 2.0 };
    tri1.v[1] = { 2.5, 2.5, 2.5 };
    tri1.v[2] = { 2.0, 2.0, 4.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(fiOutput.intersect == false, "Invalid FIQuery.");

    // (n,p,z) = (0,3,0)
    tri0.v[0] = { 1.0, 0.0, 0.0 };
    tri0.v[1] = { 0.0, 1.0, 0.0 };
    tri0.v[2] = { 0.0, 0.0, 1.0 };
    tri1.v[0] = { -2.0, -2.0, -2.0 };
    tri1.v[1] = { -2.5, -2.5, -2.5 };
    tri1.v[2] = { -2.0, -2.0, -4.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(fiOutput.intersect == false, "Invalid FIQuery.");

    Vector3<Rational> trn{ 0.25, 0.125, 4.0 };
    Matrix3x3<Rational> rot{};
    rot(0, 0) = Rational(4, 30);
    rot(0, 1) = Rational(-20, 30);
    rot(0, 2) = Rational(22, 30);
    rot(1, 0) = Rational(28, 30);
    rot(1, 1) = Rational(10, 30);
    rot(1, 2) = Rational(4, 30);
    rot(2, 0) = Rational(-10, 30);
    rot(2, 1) = Rational(20, 30);
    rot(2, 2) = Rational(20, 30);
    Rational rzero(0);

    // (n,p,z) = (2,1,0)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 0.125 };
    tri1.v[1] = { 0.5, 0.75, -1.0 };
    tri1.v[2] = { -0.5, -1.0, -1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    //expected0 = { -0.024747474747474685, 0.75227272727272720, 4.1767676767676765 };
    //expected1 = { 0.079629629629629717, 0.59907407407407398, 4.0925925925925926 };
    expected0[0] = Rational(-245, 9900);
    expected0[1] = Rational(74475, 99000);
    expected0[2] = Rational(4135, 990);
    expected1[0] = Rational(7955, 99900);
    expected1[1] = Rational(598475, 999000);
    expected1[2] = Rational(40885, 9990);
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error == rzero,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error == rzero,
        "Invalid FIQuery.");

    // (n,p,z) = (1,2,0)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 0.0, 1.0, 0.0 };
    tri0.v[2] = { 1.0, 0.0, 0.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    //expected0 = { -0.024747474747474685, 0.75227272727272720, 4.1767676767676765 };
    //expected1 = { 0.079629629629629717, 0.59907407407407398, 4.0925925925925926 };
    expected0[0] = Rational(-245, 9900);
    expected0[1] = Rational(74475, 99000);
    expected0[2] = Rational(4135, 990);
    expected1[0] = Rational(7955, 99900);
    expected1[1] = Rational(598475, 999000);
    expected1[2] = Rational(40885, 9990);
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error == rzero,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error == rzero,
        "Invalid FIQuery.");

    // (n,p,z) = (1,1,1)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 0.125 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { -0.5, -1.0, -1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    //expected0 = { 0.11666666666666664, 0.44166666666666710, 4.0833333333333330 };
    //expected1 = { 0.079629629629629717, 0.59907407407407398, 4.0925925925925926 };
    expected0[0] = Rational(105, 900);
    expected0[1] = Rational(3975, 9000);
    expected0[2] = Rational(3675, 900);
    expected1[0] = Rational(7955, 99900);
    expected1[1] = Rational(598475, 999000);
    expected1[2] = Rational(40885, 9990);
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error == rzero,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error == rzero,
        "Invalid FIQuery.");

    // (n,p,z) = (0,2,1)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 1,
        "Invalid FIQuery.");
    //expected0 = { 0.11666666666666697, 0.44166666666666671, 4.0833333333333330 };
    expected0[0] = Rational(105, 900);
    expected0[1] = Rational(3975, 9000);
    expected0[2] = Rational(3675, 900);
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error == rzero,
        "Invalid FIQuery.");

    // (n,p,z) = (2,0,1)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, -1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, -1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 1,
        "Invalid FIQuery.");
    //expected0 = { 0.11666666666666697, 0.44166666666666671, 4.0833333333333330 };
    expected0[0] = Rational(105, 900);
    expected0[1] = Rational(3975, 9000);
    expected0[2] = Rational(3675, 900);
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error == rzero,
        "Invalid FIQuery.");

    // (n,p,z) = (0,1,2)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, 1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 0.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    //expected0 = { 0.11666666666666681, 0.44166666666666671, 4.0833333333333330 };
    //expected1 = { 0.30333333333333340, 0.49833333333333329, 3.8666666666666667 };
    expected0[0] = Rational(105, 900);
    expected0[1] = Rational(3975, 9000);
    expected0[2] = Rational(3675, 900);
    expected1[0] = Rational(273, 900);
    expected1[1] = Rational(4485, 9000);
    expected1[2] = Rational(348, 90);
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error == rzero,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error == rzero,
        "Invalid FIQuery.");

    // (n,p,z) = (1,0,2)
    tri0.v[0] = { 0.0, 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0, 0.0 };
    tri1.v[0] = { 0.5, 0.5, -1.0 };
    tri1.v[1] = { 0.25, 0.25, 0.0 };
    tri1.v[2] = { 1.0, -1.0, 0.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        tri0.v[i] = rot * tri0.v[i] + trn;
        tri1.v[i] = rot * tri1.v[i] + trn;
    }
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.intersection.size() == 2,
        "Invalid FIQuery.");
    //expected0 = { 0.11666666666666681, 0.44166666666666671, 4.0833333333333330 };
    //expected1 = { 0.30333333333333340, 0.49833333333333329, 3.8666666666666667 };
    expected0[0] = Rational(105, 900);
    expected0[1] = Rational(3975, 9000);
    expected0[2] = Rational(3675, 900);
    expected1[0] = Rational(273, 900);
    expected1[1] = Rational(4485, 9000);
    expected1[2] = Rational(348, 90);
    error = Length(fiOutput.intersection[0] - expected0);
    UTAssert(
        error == rzero,
        "Invalid FIQuery.");
    error = Length(fiOutput.intersection[1] - expected1);
    UTAssert(
        error == rzero,
        "Invalid FIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrTriangle3Triangle3.h>

namespace gtl
{
    template class TIQuery<float, Triangle3<float>, Triangle3<float>>;
    template class FIQuery<float, Triangle3<float>, Triangle3<float>>;

    template class TIQuery<double, Triangle3<double>, Triangle3<double>>;
    template class FIQuery<double, Triangle3<double>, Triangle3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Triangle3<Rational>, Triangle3<Rational>>;
    template class FIQuery<Rational, Triangle3<Rational>, Triangle3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrTriangle3Triangle3)
