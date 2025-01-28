#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Miscellaneous/InscribedFixedAspectRectInQuad.h>
using namespace gtl;

namespace gtl
{
    class UnitTestInscribedFixedAspectRectInQuad
    {
    public:
        UnitTestInscribedFixedAspectRectInQuad();

    private:
        void Test();
    };
}

UnitTestInscribedFixedAspectRectInQuad::UnitTestInscribedFixedAspectRectInQuad()
{
    UTInformation("Mathematics/Miscellaneous/InscribedFixedAspectRectInQuad");

    Test();
}

void UnitTestInscribedFixedAspectRectInQuad::Test()
{
    double const maxError = 1e-15;
    Vector2<double> rectOrigin{};
    double rectWidth{}, rectHeight{};
    bool isUnique{};

    std::array<Vector2<double>, 4> quad =
    {
        Vector2<double>{ 1.0, 1.0 },
        Vector2<double>{ 2.0, 3.0 },
        Vector2<double>{ 3.0, 6.0 },
        Vector2<double>{ 0.0, 10.0 }
    };

    double r = 2.0;

    isUnique = InscribedFixedAspectRectInQuad<double>::Execute(quad, r, rectOrigin, rectWidth, rectHeight);
    UTAssert(isUnique, "The maximum should be unique.");
    UTAssert(std::fabs(rectOrigin[0] - 5291.0 / 9990.0) <= maxError, "Invalid rectOrigin[0].");
    UTAssert(std::fabs(rectOrigin[1] - 52281.0 / 9990.0) <= maxError, "Invalid rectOrigin[1].");
    UTAssert(std::fabs(rectWidth - 22126.0 / 9990.0) <= maxError, "Invalid rectWidth.");
    UTAssert(std::fabs(rectHeight - 11063.0 / 9990.0) <= maxError, "Invalid rectHeight.");
    // isUnique = true
    // rectOrigin = (0.52962962962962945, 5.2333333333333343) = (5291/9990, 52281/9990)
    // rectWidth = 2.2148148148148152 = 22126/9990
    // rectHeight = 1.1074074074074076 = 11063/9990

    quad[0] = { 1.0, 1.0 };
    quad[1] = { 2.0, 3.0 };
    quad[2] = { 2.0, 6.0 };
    quad[3] = { 1.0, 10.0 };
    isUnique = InscribedFixedAspectRectInQuad<double>::Execute(quad, r, rectOrigin, rectWidth, rectHeight);
    // isUnique = false
    // rectOrigin = (1, 3)
    // rectWidth = 1
    // rectHeight = 0.5
    UTAssert(!isUnique, "The maximum is attained on a line segment.");
    UTAssert(rectOrigin[0] == 1.0, "Invalid rectOrigin[0].");
    UTAssert(rectOrigin[1] == 3.0, "Invalid rectOrigin[1].");
    UTAssert(rectWidth == 1.0, "Invalid rectWidth.");
    UTAssert(rectHeight == 0.5, "Invalid rectHeight.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Miscellaneous/InscribedFixedAspectRectInQuad.h>

namespace gtl
{
    template class InscribedFixedAspectRectInQuad<float>;
    template class InscribedFixedAspectRectInQuad<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class InscribedFixedAspectRectInQuad<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(InscribedFixedAspectRectInQuad)
