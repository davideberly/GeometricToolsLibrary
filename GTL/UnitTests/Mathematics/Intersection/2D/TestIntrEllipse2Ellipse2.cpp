#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/2D/IntrEllipse2Ellipse2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrEllipse2Ellipse2
    {
    public:
        UnitTestIntrEllipse2Ellipse2();

    private:
        template <typename T>
        using TIEllipses2 = TIQuery<T, Ellipse2<T>, Ellipse2<T>>;

        template <typename T>
        using FIEllipses2 = FIQuery<T, Ellipse2<T>, Ellipse2<T>>;

        using Rational =  BSRational<UIntegerAP32>;

        void TestTIQuery();
        void TestFIQuery();
    };
}

UnitTestIntrEllipse2Ellipse2::UnitTestIntrEllipse2Ellipse2()
{
    UTInformation("Mathematics/Intersection/2D/IntrEllipse2Ellipse2");

    TestTIQuery();
    TestFIQuery();
}

void UnitTestIntrEllipse2Ellipse2::TestTIQuery()
{
    Ellipse2<double> E0{}, E1{};
    TIEllipses2<double> query{};
    TIEllipses2<double>::Output output = TIEllipses2<double>::Output::ELLIPSES_UNKNOWN;

    E0.center = { 0.0, 0.0 };
    E0.axis[0] = { 1.0, 0.0 };
    E0.axis[1] = { 0.0, 1.0 };
    E0.extent = { 1.0, 1.0 };
    E1.center = { 2.0, 3.12345 };
    E1.axis[0] = { 1.0, 0.0 };
    E1.axis[1] = { 0.0, 1.0 };
    E1.extent = { 1.0, 0.25 };
    output = query(E0, E1);
    UTAssert(output == TIEllipses2<double>::Output::ELLIPSES_SEPARATED, "invalid");

    E0.center = { 0.0, 0.0 };
    E0.axis[0] = { 1.0, 0.0 };
    E0.axis[1] = { 0.0, 1.0 };
    E0.extent = { 1.0, 1.0 };
    E1.center = { 2.0, 3.12345 };
    Normalize(E1.center);
    E1.center *= 0.85;
    E1.axis[0] = { 1.0, 0.0 };
    E1.axis[1] = { 0.0, 1.0 };
    E1.extent = { 1.0, 0.25 };
    output = query(E0, E1);
    UTAssert(output == TIEllipses2<double>::Output::ELLIPSES_OVERLAP, "invalid");

    E0.center = { 0.0, 0.0 };
    E0.axis[0] = { 1.0, 0.0 };
    E0.axis[1] = { 0.0, 1.0 };
    E0.extent = { 1.0, 1.0 };
    E1.center = { 0.0, 0.0 };
    E1.axis[0] = { 1.0, 0.0 };
    E1.axis[1] = { 0.0, 1.0 };
    E1.extent = { 1.0, 1.0 };
    output = query(E0, E1);
    UTAssert(output == TIEllipses2<double>::Output::ELLIPSES_EQUAL, "invalid");

    E0.center = { 0.0, 0.0 };
    E0.axis[0] = { 1.0, 0.0 };
    E0.axis[1] = { 0.0, 1.0 };
    E0.extent = { 1.0, 1.0 };
    E1.center = { 2.0, 0.0 };
    E1.axis[0] = { 1.0, 0.0 };
    E1.axis[1] = { 0.0, 1.0 };
    E1.extent = { 1.0, 4.0 };
    output = query(E0, E1);
    UTAssert(output == TIEllipses2<double>::Output::ELLIPSE0_OUTSIDE_ELLIPSE1_BUT_TANGENT, "invalid");

    E0.center = { 0.0, 0.0 };
    E0.axis[0] = { 1.0, 0.0 };
    E0.axis[1] = { 0.0, 1.0 };
    E0.extent = { 1.0, 1.0 };
    E1.center = { 0.01, 0.02 };
    E1.axis[0] = { 1.0, 1.0 };
    Normalize(E1.axis[0]);
    E1.axis[1] = { -1.0, 1.0 };
    Normalize(E1.axis[1]);
    E1.extent = { 0.2, 0.1 };
    output = query(E0, E1);
    UTAssert(output == TIEllipses2<double>::Output::ELLIPSE0_STRICTLY_CONTAINS_ELLIPSE1, "invalid");

    // Concentric ellipses, one inside the other.
    E0.center = { 0.0, 0.0 };
    E0.axis[0] = { 1.0, 0.0 };
    E0.axis[1] = { 0.0, 1.0 };
    E0.extent = { 1.0, 1.0 };
    E1.center = { 0.0, 0.0 };
    E1.axis[0] = { 1.0, 1.0 };
    Normalize(E1.axis[0]);
    E1.axis[1] = { -1.0, 1.0 };
    Normalize(E1.axis[1]);
    E1.extent = { 0.5, 0.5 };
    output = query(E0, E1);
    UTAssert(output == TIEllipses2<double>::Output::ELLIPSE0_STRICTLY_CONTAINS_ELLIPSE1, "invalid");

    E0.center = { 0.0, 0.0 };
    E0.axis[0] = { 1.0, 0.0 };
    E0.axis[1] = { 0.0, 1.0 };
    E0.extent = { 1.0, 1.0 };
    E1.center = { 0.5, 0.0 };
    E1.axis[0] = { 1.0, 0.0 };
    E1.axis[1] = { 0.0, 1.0 };
    E1.extent = { 0.5, 0.25 };
    output = query(E0, E1);
    UTAssert(output == TIEllipses2<double>::Output::ELLIPSE0_CONTAINS_ELLIPSE1_BUT_TANGENT, "invalid");

    E0.center = { 0.0, 0.0 };
    E0.axis[0] = { 1.0, 0.0 };
    E0.axis[1] = { 0.0, 1.0 };
    E0.extent = { 1.0, 1.0 };
    E1.center = { 2.0, 0.0 };
    E1.axis[0] = { 1.0, 0.0 };
    E1.axis[1] = { 0.0, 1.0 };
    E1.extent = { 4.0, 5.0 };
    output = query(E0, E1);
    UTAssert(output == TIEllipses2<double>::Output::ELLIPSE1_STRICTLY_CONTAINS_ELLIPSE0, "invalid");

    E0.center = { 0.5, 0.0 };
    E0.axis[0] = { 1.0, 0.0 };
    E0.axis[1] = { 0.0, 1.0 };
    E0.extent = { 0.5, 0.5 };
    E1.center = { 2.0, 0.0 };
    E1.axis[0] = { 1.0, 0.0 };
    E1.axis[1] = { 0.0, 1.0 };
    E1.extent = { 2.0, 4.0 };
    output = query(E0, E1);
    UTAssert(output == TIEllipses2<double>::Output::ELLIPSE1_CONTAINS_ELLIPSE0_BUT_TANGENT, "invalid");
}

void UnitTestIntrEllipse2Ellipse2::TestFIQuery()
{
    double constexpr maxError = 1e-13;
    Matrix2x2<double> M{};
    Vector2<double> diff{};
    double adiff0{}, adiff1{};
    double Q = 0.0;

    Ellipse2<double> ellipse0{}, ellipse1{};
    FIEllipses2<double> query{};
    FIEllipses2<double>::Output output{};

    Ellipse2<Rational> rEllipse0{}, rEllipse1{};
    FIEllipses2<Rational> rQuery{};
    FIEllipses2<Rational>::Output rOutput{};

    // x^2 + y^2 = 1, 4*x^2 + (y-1)^2 = 1
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 1.0, 1.0 };
    ellipse1.center = { 0.0, 1.0 };
    ellipse1.axis[0] = { 1.0, 0.0 };
    ellipse1.axis[1] = { 0.0, 1.0 };
    ellipse1.extent = { 0.5, 1.0 };
    output = query(ellipse0, ellipse1);
    UTAssert(output.intersect && output.numPoints == 2, "Invalid num points.");
    adiff0 = std::fabs(output.points[0][0] + 0.49565921883308067);
    adiff1 = std::fabs(output.points[0][1] - 0.86851709182132963);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs(output.points[1][0] - 0.49565921883308067);
    adiff1 = std::fabs(output.points[1][1] - 0.86851709182132963);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    for (std::size_t i = 0; i < output.numPoints; ++i)
    {
        Q = std::fabs(ellipse0.GetLevelValue(output.points[i]));
        UTAssert(Q <= maxError, "Invalid Q.");
        diff = output.points[i] - ellipse1.center;
        Q = std::fabs(ellipse1.GetLevelValue(output.points[i]));
        UTAssert(Q <= maxError, "Invalid Q.");
    }

    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 1.0, 1.0 };
    rEllipse1.center = { 0.0, 1.0 };
    rEllipse1.axis[0] = { 1.0, 0.0 };
    rEllipse1.axis[1] = { 0.0, 1.0 };
    rEllipse1.extent = { 0.5, 1.0 };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(rOutput.intersect && rOutput.numPoints == 2, "Invalid num points.");
    adiff0 = std::fabs((double)rOutput.points[0][0] + 0.49565921883308067);
    adiff1 = std::fabs((double)rOutput.points[0][1] - 0.86851709182132963);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs((double)rOutput.points[1][0] - 0.49565921883308067);
    adiff1 = std::fabs((double)rOutput.points[1][1] - 0.86851709182132963);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");

    // x^2 + y^2 = 1, -x - y + 0.75*x^2 - 0.5*x*y + 0.75*y^2 = 0
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 1.0, 1.0 };
    ellipse1.center = { 1.0, 1.0 };
    ellipse1.axis[0] = { 1.0, -1.0 };
    ellipse1.axis[1] = { 1.0, 1.0 };
    ellipse1.extent = { 1.0, std::sqrt(2.0) };
    output = query(ellipse0, ellipse1);
    UTAssert(output.intersect && output.numPoints == 2, "Invalid num points.");
    adiff0 = std::fabs(output.points[0][0] + 0.15887205491823964);
    adiff1 = std::fabs(output.points[0][1] - 0.98729917966442982);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs(output.points[1][0] - 0.98729917966442993);
    adiff1 = std::fabs(output.points[1][1] + 0.15887205491823972);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    for (std::size_t i = 0; i < output.numPoints; ++i)
    {
        Q = std::fabs(ellipse0.GetLevelValue(output.points[i]));
        UTAssert(Q <= maxError, "Invalid Q.");
        diff = output.points[i] - ellipse1.center;
        Q = std::fabs(ellipse1.GetLevelValue(output.points[i]));
        UTAssert(Q <= maxError, "Invalid Q.");
    }

    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 1.0, 1.0 };
    rEllipse1.center = { 1.0, 1.0 };
    rEllipse1.axis[0] = { 1.0, -1.0 };
    rEllipse1.axis[1] = { 1.0, 1.0 };
    rEllipse1.extent = { 1.0, std::sqrt(2.0) };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(rOutput.intersect && rOutput.numPoints == 2, "Invalid num points.");
    adiff0 = std::fabs((double)rOutput.points[0][0] + 0.15887205491823964);
    adiff1 = std::fabs((double)rOutput.points[0][1] - 0.98729917966442970);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs((double)rOutput.points[1][0] - 0.98729917966442948);
    adiff1 = std::fabs((double)rOutput.points[1][1] + 0.15887205491823947);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");

    // f3 = f4 = 0
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 1.0, 1.0 };
    ellipse1.center = { 1.0, 1.0 };
    ellipse1.axis[0] = { 1.0, 0.0 };
    ellipse1.axis[1] = { 0.0, 1.0 };
    ellipse1.extent = { 0.5, 0.5 };
    output = query(ellipse0, ellipse1);
    UTAssert(output.intersect && output.numPoints == 2, "Invalid num points.");
    adiff0 = std::fabs(output.points[0][0] - 0.52214054305846314);
    adiff1 = std::fabs(output.points[0][1] - 0.85285945694153686);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs(output.points[1][0] - 0.85285945694153686);
    adiff1 = std::fabs(output.points[1][1] - 0.52214054305846314);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    for (std::size_t i = 0; i < output.numPoints; ++i)
    {
        Q = std::fabs(ellipse0.GetLevelValue(output.points[i]));
        UTAssert(Q <= maxError, "Invalid Q.");
        diff = output.points[i] - ellipse1.center;
        Q = std::fabs(ellipse1.GetLevelValue(output.points[i]));
        UTAssert(Q <= maxError, "Invalid Q.");
    }

    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 1.0, 1.0 };
    rEllipse1.center = { 1.0, 1.0 };
    rEllipse1.axis[0] = { 1.0, 0.0 };
    rEllipse1.axis[1] = { 0.0, 1.0 };
    rEllipse1.extent = { 0.5, 0.5 };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(rOutput.intersect && rOutput.numPoints == 2, "Invalid num points.");
    adiff0 = std::fabs((double)rOutput.points[0][0] - 0.52214054305846314);
    adiff1 = std::fabs((double)rOutput.points[0][1] - 0.85285945694153686);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs((double)rOutput.points[1][0] - 0.85285945694153686);
    adiff1 = std::fabs((double)rOutput.points[1][1] - 0.52214054305846314);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");

    // d2 = d4 = 0 (L = 0, so w^2 = * equation must be solved).  The query
    // avoids the biquadratic misclassification by detecting d2 = d4 = 0 before
    // calling the root finder.
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 1.0, 1.0 };
    ellipse1.center = { 1.0, 0.0 };
    ellipse1.axis[0] = { 1.0, 0.0 };
    ellipse1.axis[1] = { 0.0, 1.0 };
    ellipse1.extent = { 1.0, 0.5 };
    output = query(ellipse0, ellipse1);
    UTAssert(output.intersect && output.numPoints == 2, "Invalid num points.");
    adiff0 = std::fabs(output.points[0][0] - 0.86851709182132986);
    adiff1 = std::fabs(output.points[0][1] + 0.49565921883308056);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs(output.points[1][0] - 0.86851709182132986);
    adiff1 = std::fabs(output.points[1][1] - 0.49565921883308056);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    for (std::size_t i = 0; i < output.numPoints; ++i)
    {
        Q = std::fabs(ellipse0.GetLevelValue(output.points[i]));
        UTAssert(Q <= maxError, "Invalid Q.");
        diff = output.points[i] - ellipse1.center;
        Q = std::fabs(ellipse1.GetLevelValue(output.points[i]));
        UTAssert(Q <= maxError, "Invalid Q.");
    }

    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 1.0, 1.0 };
    rEllipse1.center = { 1.0, 0.0 };
    rEllipse1.axis[0] = { 1.0, 0.0 };
    rEllipse1.axis[1] = { 0.0, 1.0 };
    rEllipse1.extent = { 1.0, 0.5 };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(rOutput.intersect && rOutput.numPoints == 2, "Invalid num points.");
    adiff0 = std::fabs((double)rOutput.points[0][0] - 0.86851709182132975);
    adiff1 = std::fabs((double)rOutput.points[0][1] + 0.49565921883308073);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs((double)rOutput.points[1][0] - 0.86851709182132975);
    adiff1 = std::fabs((double)rOutput.points[1][1] - 0.49565921883308073);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");

    // x^2 + y^2 = 1, (x-2)^2 + 4*y^2 = 1 (one tangential intersection)
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 1.0, 1.0 };
    ellipse1.center = { 2.0, 0.0 };
    ellipse1.axis[0] = { 1.0, 0.0 };
    ellipse1.axis[1] = { 0.0, 1.0 };
    ellipse1.extent = { 1.0, 0.5 };
    output = query(ellipse0, ellipse1);
    UTAssert(output.intersect && output.numPoints == 1, "Invalid num points.");
    adiff0 = std::fabs(output.points[0][0] - 1.0);
    adiff1 = std::fabs(output.points[0][1]);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");

    // The rational evaluation misses the tangential intersection because of
    // rounding errors when computing roots of a quadratic equation using the
    // closed formulas. The roots are theoretically 1 and -7/3, but the
    // conversion of rational to double during the root solving leads to
    // a double that is slightly smaller than -7/3.
    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 1.0, 1.0 };
    rEllipse1.center = { 2.0, 0.0 };
    rEllipse1.axis[0] = { 1.0, 0.0 };
    rEllipse1.axis[1] = { 0.0, 1.0 };
    rEllipse1.extent = { 1.0, 0.5 };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(!rOutput.intersect && rOutput.numPoints == 0, "Invalid num points.");

    // x^2 + y^2 = 1, 4*x^2 + (y-1)^2/4 = 1 (three intersections, one of which is tangential)
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 1.0, 1.0 };
    ellipse1.center = { 0.0, 1.0 };
    ellipse1.axis[0] = { 1.0, 0.0 };
    ellipse1.axis[1] = { 0.0, 1.0 };
    ellipse1.extent = { 0.5, 2.0 };
    output = query(ellipse0, ellipse1);
    UTAssert(output.intersect && output.numPoints == 3, "Invalid num points.");
    adiff0 = std::fabs(output.points[0][0] + 0.49888765156985887);
    adiff1 = std::fabs(output.points[0][1] - 0.86666666666666670);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    UTAssert(
        output.points[1][0] == 0.0 &&
        output.points[1][1] == -1.0,
        "invalid point");
    adiff0 = std::fabs(output.points[2][0] - 0.49888765156985887);
    adiff1 = std::fabs(output.points[2][1] - 0.86666666666666670);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    for (std::size_t i = 0; i < output.numPoints; ++i)
    {
        Q = std::fabs(ellipse0.GetLevelValue(output.points[i]));
        UTAssert(Q <= maxError, "Invalid Q.");
        diff = output.points[i] - ellipse1.center;
        Q = std::fabs(ellipse1.GetLevelValue(output.points[i]));
        UTAssert(Q <= maxError, "Invalid Q.");
    }

    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 1.0, 1.0 };
    rEllipse1.center = { 0.0, 1.0 };
    rEllipse1.axis[0] = { 1.0, 0.0 };
    rEllipse1.axis[1] = { 0.0, 1.0 };
    rEllipse1.extent = { 0.5, 2.0 };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(rOutput.intersect && rOutput.numPoints == 3, "Invalid num points.");
    adiff0 = std::fabs((double)rOutput.points[0][0] + 0.49888765156985887);
    adiff1 = std::fabs((double)rOutput.points[0][1] - 0.86666666666666681);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    UTAssert(
        (double)rOutput.points[1][0] == 0.0 &&
        (double)rOutput.points[1][1] == -1.0,
        "invalid point");
    adiff0 = std::fabs((double)rOutput.points[2][0] - 0.49888765156985887);
    adiff1 = std::fabs((double)rOutput.points[2][1] - 0.86666666666666681);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");

    // x^2/4 + y^2 = 1,
    // (x-1,y)^T*(U0*U0^T/16 + U1*U1^T*(x-1,y)/(1/9) = 1
    //   U0 = (1,1)/sqrt(2), U1 = (1,-1)/sqrt(2)
    // 4 intersections
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 2.0, 1.0 };
    ellipse1.center = { 1.0, 0.0 };
    ellipse1.axis[0] = { 1.0, 1.0 };
    ellipse1.axis[1] = { 1.0, -1.0 };
    ellipse1.extent = { 4.0, 1.0 / 3.0 };
    output = query(ellipse0, ellipse1);
    UTAssert(output.intersect && output.numPoints == 4, "Invalid num points.");
    adiff0 = std::fabs(output.points[0][0] + 0.40655353716372078);
    adiff1 = std::fabs(output.points[0][1] + 0.97912131799635371);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs(output.points[1][0] - 0.48466658732681495);
    adiff1 = std::fabs(output.points[1][1] + 0.97019306057209298);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs(output.points[2][0] - 1.2977987010596941);
    adiff1 = std::fabs(output.points[2][1] - 0.76087425563094158);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs(output.points[3][0] - 1.8455038321616994);
    adiff1 = std::fabs(output.points[3][1] - 0.38539447501114965);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    for (std::size_t i = 0; i < output.numPoints; ++i)
    {
        Q = std::fabs(ellipse0.GetLevelValue(output.points[i]));
        UTAssert(Q <= maxError, "Invalid Q.");
        diff = output.points[i] - ellipse1.center;
        Q = std::fabs(ellipse1.GetLevelValue(output.points[i]));
        UTAssert(Q <= maxError, "Invalid Q.");
    }

    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 2.0, 1.0 };
    rEllipse1.center = { 1.0, 0.0 };
    rEllipse1.axis[0] = { 1.0, 1.0 };
    rEllipse1.axis[1] = { -1.0, 1.0 };
    rEllipse1.extent = { 4.0, Rational(1.0, 3.0) };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(rOutput.intersect && rOutput.numPoints == 4, "Invalid num points.");
    adiff0 = std::fabs((double)rOutput.points[0][0] + 0.40655353716372128);
    adiff1 = std::fabs((double)rOutput.points[0][1] + 0.97912131799635382);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs((double)rOutput.points[1][0] - 0.48466658732681561);
    adiff1 = std::fabs((double)rOutput.points[1][1] + 0.97019306057209331);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs((double)rOutput.points[2][0] - 1.2977987010596945);
    adiff1 = std::fabs((double)rOutput.points[2][1] - 0.76087425563094080);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
    adiff0 = std::fabs((double)rOutput.points[3][0] - 1.8455038321616992);
    adiff1 = std::fabs((double)rOutput.points[3][1] - 0.38539447501114954);
    UTAssert(adiff0 <= maxError && adiff1 <= maxError,
        "invalid point");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/2D/IntrEllipse2Ellipse2.h>

namespace gtl
{
    template class TIQuery<float, Ellipse2<float>, Ellipse2<float>>;
    template class FIQuery<float, Ellipse2<float>, Ellipse2<float>>;

    template class TIQuery<double, Ellipse2<double>, Ellipse2<double>>;
    template class FIQuery<double, Ellipse2<double>, Ellipse2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Ellipse2<Rational>, Ellipse2<Rational>>;
    template class FIQuery<Rational, Ellipse2<Rational>, Ellipse2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrEllipse2Ellipse2)
