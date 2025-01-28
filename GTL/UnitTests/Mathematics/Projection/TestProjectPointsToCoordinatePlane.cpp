#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Projection/ProjectPointsToCoordinatePlane.h>
using namespace gtl;

namespace gtl
{
    class UnitTestProjectPointsToCoordinatePlane
    {
    public:
        UnitTestProjectPointsToCoordinatePlane();

    private:
        void Test();
    };
}

UnitTestProjectPointsToCoordinatePlane::UnitTestProjectPointsToCoordinatePlane()
{
    UTInformation("Mathematics/Projection/ProjectPointsToCoordinatePlane");

    Test();
}

void UnitTestProjectPointsToCoordinatePlane::Test()
{
    Vector3<double> normal{};
    std::size_t maxIndex{};
    std::array<std::size_t, 3> permute{};
    Vector3<double> point{ 1.0, 2.0, 3.0 };
    Vector2<double> projectedPoint{};
    Vector3<double> origin{}, lifted{};

    normal = { 0.0, 0.0, 1.0 };
    ProjectPointsToCoordinatePlane<double>::SelectAndProject(1, &point, normal, &projectedPoint, maxIndex, permute);
    UTAssert(maxIndex == 2, "Invalid max index.");
    UTAssert(permute[0] == 0 && permute[1] == 1 && permute[2] == 2, "Invalid permute.");
    UTAssert(projectedPoint[0] == 1.0 && projectedPoint[1] == 2.0, "Invalid projected point.");
    origin = { 0.0, 0.0, point[2]};
    ProjectPointsToCoordinatePlane<double>::Lift(1, &projectedPoint, origin, normal, permute, &lifted);
    UTAssert(lifted == point, "Invalid lift.");

    normal = { 0.0, 1.0, 0.0 };
    ProjectPointsToCoordinatePlane<double>::SelectAndProject(1, &point, normal, &projectedPoint, maxIndex, permute);
    UTAssert(maxIndex == 1, "Invalid max index.");
    UTAssert(permute[0] == 0 && permute[1] == 2 && permute[2] == 1, "Invalid permute.");
    UTAssert(projectedPoint[0] == 1.0 && projectedPoint[1] == 3.0, "Invalid projected point.");
    origin = { 0.0, point[1], 0.0 };
    ProjectPointsToCoordinatePlane<double>::Lift(1, &projectedPoint, origin, normal, permute, &lifted);
    UTAssert(lifted == point, "Invalid lift.");

    normal = { 1.0, 0.0, 0.0 };
    ProjectPointsToCoordinatePlane<double>::SelectAndProject(1, &point, normal, &projectedPoint, maxIndex, permute);
    UTAssert(maxIndex == 0, "Invalid max index.");
    UTAssert(permute[0] == 1 && permute[1] == 2 && permute[2] == 0, "Invalid permute.");
    UTAssert(projectedPoint[0] == 2.0 && projectedPoint[1] == 3.0, "Invalid projected point.");
    origin = { point[0], 0.0, 0.0 };
    ProjectPointsToCoordinatePlane<double>::Lift(1, &projectedPoint, origin, normal, permute, &lifted);
    UTAssert(lifted == point, "Invalid lift.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Projection/ProjectPointsToCoordinatePlane.h>

namespace gtl
{
    template class ProjectPointsToCoordinatePlane<float>;
    template class ProjectPointsToCoordinatePlane<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ProjectPointsToCoordinatePlane<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ProjectPointsToCoordinatePlane)
