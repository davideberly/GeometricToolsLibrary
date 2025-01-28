#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistPointHyperplane.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPointHyperplane
    {
    public:
        UnitTestDistPointHyperplane();

    private:
        void Test2D();
        void Test3D();
    };
}

UnitTestDistPointHyperplane::UnitTestDistPointHyperplane()
{
    UTInformation("Mathematics/Distance/ND/DistPointHyperplane");

    Test2D();
    Test3D();
}

void UnitTestDistPointHyperplane::Test2D()
{
    using DCPPointHyperplaneQuery = DCPQuery<double, Vector<double, 2>, Hyperplane<double, 2>>;
    DCPPointHyperplaneQuery query{};
    DCPPointHyperplaneQuery::Output output{};
    Vector<double, 2> expectedClosest{};
    double error{};

    Hyperplane<double, 2> line{};
    line.normal = { 2.0, -1.0 };
    Normalize(line.normal);
    Vector<double, 2> origin{ 1.0, 1.0 };
    line.constant = Dot(line.normal, origin);

    Vector<double, 2> point = { 2.0, 1.0 };
    output = query(point, line);
    error = std::fabs(output.distance - std::sqrt(0.8));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.signedDistance - std::sqrt(0.8));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 1.2, 1.4 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    line.normal = -line.normal;
    line.constant = -line.constant;
    output = query(point, line);
    error = std::fabs(output.distance - std::sqrt(0.8));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.signedDistance + std::sqrt(0.8));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 1.2, 1.4 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
}

void UnitTestDistPointHyperplane::Test3D()
{
    using DCPPointHyperplaneQuery = DCPQuery<double, Vector<double, 3>, Hyperplane<double, 3>>;
    DCPPointHyperplaneQuery query{};
    DCPPointHyperplaneQuery::Output output{};
    Vector<double, 3> expectedClosest{};
    double error{};

    Hyperplane<double, 3> plane{};
    plane.normal = { 2.0, -1.0, 3.0 };
    Normalize(plane.normal);
    Vector<double, 3> origin{ 1.0, 1.0, 1.0 };
    plane.constant = Dot(plane.normal, origin);

    Vector<double, 3> point = { 4.0, 4.0, 4.0 };
    output = query(point, plane);
    error = std::fabs(output.distance - 3.2071349029490928);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.signedDistance - 3.2071349029490928);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 2.2857142857142856, 4.8571428571428577, 1.4285714285714284 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(Cross(plane.normal, output.closest[0] - output.closest[1]));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    plane.normal = -plane.normal;
    plane.constant = -plane.constant;
    output = query(point, plane);
    error = std::fabs(output.distance - 3.2071349029490928);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.signedDistance + 3.2071349029490928);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 2.2857142857142856, 4.8571428571428577, 1.4285714285714284 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(Cross(plane.normal, output.closest[0] - output.closest[1]));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistPointHyperplane.h>

namespace gtl
{
    template class DCPQuery<float, Vector<float, 2>, Hyperplane<float, 2>>;
    template class DCPQuery<double, Vector<double, 3>, Hyperplane<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector<Rational, 4>, Hyperplane<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPointHyperplane)
