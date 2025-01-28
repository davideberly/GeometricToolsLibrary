#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/DisjointRectangles.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDisjointRectangles
    {
    public:
        UnitTestDisjointRectangles();

    private:
        void Test();
    };
}

UnitTestDisjointRectangles::UnitTestDisjointRectangles()
{
    UTInformation("Mathematics/Geometry/2D/DisjointRectangles");

    Test();
}

void UnitTestDisjointRectangles::Test()
{
    DisjointRectangles<float> R1{}, R2{};
    R1.Insert(0.0f, 2.0f, 0.0f, 2.0f);
    R2.Insert(1.0f, 3.0f, 1.0f, 3.0f);
    DisjointRectangles<float> R1uR2 = R1 | R2;  // union
    DisjointRectangles<float> R1iR2 = R1 & R2;  // intersection
    DisjointRectangles<float> R1dR2 = R1 - R2;  // difference
    DisjointRectangles<float> R1xR2 = R1 ^ R2;  // exclusive-or

    float xmin{}, xmax{}, ymin{}, ymax{};

    // R1uR2:
    UTAssert(R1uR2.GetNumStrips() == 3, "Invalid number of strips.");
    UTAssert(R1uR2.GetNumRectangles() == 3, "Invalid number of rectangles.");
    R1uR2.GetRectangle(0, xmin, xmax, ymin, ymax);
    UTAssert(xmin == 0.0f && xmax == 2.0f && ymin == 0.0f && ymax == 1.0f, "Invalid rectangle.");
    R1uR2.GetRectangle(1, xmin, xmax, ymin, ymax);
    UTAssert(xmin == 0.0f && xmax == 3.0f && ymin == 1.0f && ymax == 2.0f, "Invalid rectangle.");
    R1uR2.GetRectangle(2, xmin, xmax, ymin, ymax);
    UTAssert(xmin == 1.0f && xmax == 3.0f && ymin == 2.0f && ymax == 3.0f, "Invalid rectangle.");

    // R1iR2:
    UTAssert(R1iR2.GetNumStrips() == 1, "Invalid number of strips.");
    UTAssert(R1iR2.GetNumRectangles() == 1, "Invalid number of rectangles.");
    R1iR2.GetRectangle(0, xmin, xmax, ymin, ymax);
    UTAssert(xmin == 1.0f && xmax == 2.0f && ymin == 1.0f && ymax == 2.0f, "Invalid rectangle.");

    // R1dR2:
    UTAssert(R1dR2.GetNumStrips() == 2, "Invalid number of strips.");
    UTAssert(R1dR2.GetNumRectangles() == 2, "Invalid number of rectangles.");
    R1dR2.GetRectangle(0, xmin, xmax, ymin, ymax);
    UTAssert(xmin == 0.0f && xmax == 2.0f && ymin == 0.0f && ymax == 1.0f, "Invalid rectangle.");
    R1dR2.GetRectangle(1, xmin, xmax, ymin, ymax);
    UTAssert(xmin == 0.0f && xmax == 1.0f && ymin == 1.0f && ymax == 2.0f, "Invalid rectangle.");

    // R1xR2:
    UTAssert(R1xR2.GetNumStrips() == 3, "Invalid number of strips.");
    UTAssert(R1xR2.GetNumRectangles() == 4, "Invalid number of rectangles.");
    R1xR2.GetRectangle(0, xmin, xmax, ymin, ymax);
    UTAssert(xmin == 0.0f && xmax == 2.0f && ymin == 0.0f && ymax == 1.0f, "Invalid rectangle.");
    R1xR2.GetRectangle(1, xmin, xmax, ymin, ymax);
    UTAssert(xmin == 0.0f && xmax == 1.0f && ymin == 1.0f && ymax == 2.0f, "Invalid rectangle.");
    R1xR2.GetRectangle(2, xmin, xmax, ymin, ymax);
    UTAssert(xmin == 2.0f && xmax == 3.0f && ymin == 1.0f && ymax == 2.0f, "Invalid rectangle.");
    R1xR2.GetRectangle(3, xmin, xmax, ymin, ymax);
    UTAssert(xmin == 1.0f && xmax == 3.0f && ymin == 2.0f && ymax == 3.0f, "Invalid rectangle.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Geometry/2D/DisjointRectangles.h>

namespace gtl
{
    template class DisjointRectangles<std::int32_t>;
    template class DisjointRectangles<std::uint32_t>;
    template class DisjointRectangles<float>;
    template class DisjointRectangles<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSNumber<UIntegerAP32>;
    template class DisjointRectangles<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DisjointRectangles)
