#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/2D/IntrAreaEllipse2Ellipse2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrAreaEllipse2Ellipse2
    {
    public:
        UnitTestIntrAreaEllipse2Ellipse2();

    private:
        template <typename T>
        using AreaEllipses = AreaEllipse2Ellipse2<T>;

        using Rational =  BSRational<UIntegerAP32>;

        void TestAreaIntersection();
    };
}

UnitTestIntrAreaEllipse2Ellipse2::UnitTestIntrAreaEllipse2Ellipse2()
{
    UTInformation("Mathematics/Intersection/2D/IntrAreaEllipse2Ellipse2");

    TestAreaIntersection();
}

void UnitTestIntrAreaEllipse2Ellipse2::TestAreaIntersection()
{
    double constexpr maxError = 1e-13;

    Ellipse2<double> ellipse0{}, ellipse1{};
    AreaEllipses<double> query{};
    AreaEllipses<double>::Output output{};

    Ellipse2<Rational> rEllipse0{}, rEllipse1{};
    AreaEllipses<Rational> rQuery{};
    AreaEllipses<Rational>::Output rOutput{};

    // Mathematica 10.1 was used to numerically integrate the region of
    // intersection to compute the area.
    //   p = <polynomial of x and y, ellipse is p = 0>
    //   q = <polynomial of x and y, ellipse is q = 0>
    //  NumberForm[NIntegrate[Boole[p <= 0 && q <= 0], {x,xmin,xmax},{y,ymin,ymax}]

    // two transverse intersections
    // p = x^2 + y^2 - 1
    // q = 4*x^2 + (y-1)^2 - 1
    // Mathematica: xmin = -1, xmax = 1, ymin = -1, ymax = 1, area = 0.7424002627363588
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 1.0, 1.0 };
    ellipse1.center = { 0.0, 1.0 };
    ellipse1.axis[0] = { 1.0, 0.0 };
    ellipse1.axis[1] = { 0.0, 1.0 };
    ellipse1.extent = { 0.5, 1.0 };
    output = query(ellipse0, ellipse1);
    UTAssert(std::fabs(output.area - 0.74240026509663459) <= maxError, "invalid area");

    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 1.0, 1.0 };
    rEllipse1.center = { 0.0, 1.0 };
    rEllipse1.axis[0] = { 1.0, 0.0 };
    rEllipse1.axis[1] = { 0.0, 1.0 };
    rEllipse1.extent = { 0.5, 1.0 };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(std::fabs((double)rOutput.area - 0.74240026509663448) <= maxError, "invalid area");

    // two transverse intersections
    // p = x^2 + y^2 - 1
    // q = -x - y + 0.75*x^2 - 0.5*x*y + 0.75*y^2
    // Mathematica: xmin = -4, xmax = 4, ymin = -4, ymax = 4, area = 1.135130849622308
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 1.0, 1.0 };
    ellipse1.center = { 1.0, 1.0 };
    ellipse1.axis[0] = { 1.0, -1.0 };
    ellipse1.axis[1] = { 1.0, 1.0 };
    ellipse1.extent = { 1.0, std::sqrt(2.0) };
    output = query(ellipse0, ellipse1);
    UTAssert(std::fabs(output.area - 1.1351308320462075) <= maxError, "invalid area");

    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 1.0, 1.0 };
    rEllipse1.center = { 1.0, 1.0 };
    rEllipse1.axis[0] = { 1.0, -1.0 };
    rEllipse1.axis[1] = { 1.0, 1.0 };
    rEllipse1.extent = { 1.0, std::sqrt(2.0) };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(std::fabs((double)rOutput.area - 1.1351308320462072) <= maxError, "invalid area");

    // two transverse intersections (f3 = f4 = 0)
    // p = x^2 + y^2 - 1
    // q = (x-1)^2 + (y-1)^2 - 1/4
    // Mathematica: xmin = -4, xmax = 4, ymin = -4, ymax = 4, area = 0.0269941175853503
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 1.0, 1.0 };
    ellipse1.center = { 1.0, 1.0 };
    ellipse1.axis[0] = { 1.0, 0.0 };
    ellipse1.axis[1] = { 0.0, 1.0 };
    ellipse1.extent = { 0.5, 0.5 };
    output = query(ellipse0, ellipse1);
    UTAssert(std::fabs(output.area - 0.026994117624261821) <= maxError, "invalid area");

    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 1.0, 1.0 };
    rEllipse1.center = { 1.0, 1.0 };
    rEllipse1.axis[0] = { 1.0, 0.0 };
    rEllipse1.axis[1] = { 0.0, 1.0 };
    rEllipse1.extent = { 0.5, 0.5 };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(std::fabs((double)rOutput.area - 0.026994117624261821) <= maxError, "invalid area");

    // two transverse intersections
    // d2 = d4 = 0 (L = 0, so w^2 = * equation must be solved).  The query
    // avoids the biquadratic misclassification by detecting d2 = d4 = 0 before
    // calling the root finder.
    // p = x^2 + y^2 - 1
    // q = (x-1)^2 + 4*y^2 - 1
    // Mathematica: xmin = -4, xmax = 4, ymin = -4, ymax = 4, area = 0.7424002643161881
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 1.0, 1.0 };
    ellipse1.center = { 1.0, 0.0 };
    ellipse1.axis[0] = { 1.0, 0.0 };
    ellipse1.axis[1] = { 0.0, 1.0 };
    ellipse1.extent = { 1.0, 0.5 };
    output = query(ellipse0, ellipse1);
    UTAssert(std::fabs(output.area - 0.74240026509663448) <= maxError, "invalid area");

    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 1.0, 1.0 };
    rEllipse1.center = { 1.0, 0.0 };
    rEllipse1.axis[0] = { 1.0, 0.0 };
    rEllipse1.axis[1] = { 0.0, 1.0 };
    rEllipse1.extent = { 1.0, 0.5 };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(std::fabs((double)rOutput.area - 0.74240026509663437) <= maxError, "invalid area");

    // one tangential intersection, ellipses separated
    // p = x^2 + y^2 - 1
    // q = (x-2)^2 + 4*y^2 - 1
    // The double-precision version misses the intersection due to rounding errors.
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 1.0, 1.0 };
    ellipse1.center = { 2.0, 0.0 };
    ellipse1.axis[0] = { 1.0, 0.0 };
    ellipse1.axis[1] = { 0.0, 1.0 };
    ellipse1.extent = { 1.0, 0.5 };
    output = query(ellipse0, ellipse1);
    UTAssert(output.area == 0.0, "invalid area");

    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 1.0, 1.0 };
    rEllipse1.center = { 2.0, 0.0 };
    rEllipse1.axis[0] = { 1.0, 0.0 };
    rEllipse1.axis[1] = { 0.0, 1.0 };
    rEllipse1.extent = { 1.0, 0.5 };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert((double)rOutput.area == 0.0, "invalid area");

    // three intersections, one tangential
    // p = x^2 + y^2 - 1
    // q = 4*x^2 + (y-1)^2/4 = 1
    // Mathematica: xmin = -4, xmax = 4, ymin = -4, ymax = 4, area = 1.527507346872982
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 1.0, 1.0 };
    ellipse1.center = { 0.0, 1.0 };
    ellipse1.axis[0] = { 1.0, 0.0 };
    ellipse1.axis[1] = { 0.0, 1.0 };
    ellipse1.extent = { 0.5, 2.0 };
    output = query(ellipse0, ellipse1);
    UTAssert(std::fabs(output.area - 1.5275073486208612) <= maxError, "invalid area");

    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 1.0, 1.0 };
    rEllipse1.center = { 0.0, 1.0 };
    rEllipse1.axis[0] = { 1.0, 0.0 };
    rEllipse1.axis[1] = { 0.0, 1.0 };
    rEllipse1.extent = { 0.5, 2.0 };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(std::fabs((double)rOutput.area - 1.5275073486208612) <= maxError, "invalid area");

    // four intersections
    // p = x^2/4 + y^2 - 1
    // q = (x-1,y)^T*(U0*U0^T/16 + U1*U1^T*(x-1,y)/(1/9) = 1
    //   U0 = (1,1)/sqrt(2), U1 = (-1,1)/sqrt(2)
    // Mathematica: xmin = -4, xmax = 4, ymin = -4, ymax = 4, area = 1.46541651628303
    ellipse0.center = { 0.0, 0.0 };
    ellipse0.axis[0] = { 1.0, 0.0 };
    ellipse0.axis[1] = { 0.0, 1.0 };
    ellipse0.extent = { 2.0, 1.0 };
    ellipse1.center = { 1.0, 0.0 };
    ellipse1.axis[0] = { 1.0, 1.0 };
    ellipse1.axis[1] = { -1.0, 1.0 };
    ellipse1.extent = { 4.0, 1.0 / 3.0 };
    output = query(ellipse0, ellipse1);
    UTAssert(std::fabs(output.area - 1.4654165246075777) <= maxError, "invalid area");

    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 2.0, 1.0 };
    rEllipse1.center = { 1.0, 0.0 };
    rEllipse1.axis[0] = { 1.0, 1.0 };
    rEllipse1.axis[1] = { -1.0, 1.0 };
    rEllipse1.extent = { 4.0, Rational(1.0, 3.0) };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(std::fabs((double)rOutput.area - 1.4654165246075779) <= maxError, "invalid area");

    // one tangential intersection, ellipse contains other ellipse
    // p = x^2 + y^1 - 1
    // q = (x-1/2)^2 + y^2 - 1/4
    // Mathematica: xmin = -4, xmax = 4, ymin = -4, ymax = 4, area = 0.785398162398547
    rEllipse0.center = { 0.0, 0.0 };
    rEllipse0.axis[0] = { 1.0, 0.0 };
    rEllipse0.axis[1] = { 0.0, 1.0 };
    rEllipse0.extent = { 1.0, 1.0 };
    rEllipse1.center = { 0.5, 0.0 };
    rEllipse1.axis[0] = { 1.0, 0.0 };
    rEllipse1.axis[1] = { 0.0, 1.0 };
    rEllipse1.extent = { 0.5, 0.5 };
    rOutput = rQuery(rEllipse0, rEllipse1);
    UTAssert(std::fabs((double)rOutput.area - 0.78539816339744828) <= maxError, "invalid area");

    // Reverse p and q.
    rOutput = rQuery(rEllipse1, rEllipse0);
    UTAssert(std::fabs((double)rOutput.area - 0.78539816339744828) <= maxError, "invalid area");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/2D/IntrAreaEllipse2Ellipse2.h>

namespace gtl
{
    template class AreaEllipse2Ellipse2<float>;
    template class AreaEllipse2Ellipse2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class AreaEllipse2Ellipse2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrAreaEllipse2Ellipse2)
