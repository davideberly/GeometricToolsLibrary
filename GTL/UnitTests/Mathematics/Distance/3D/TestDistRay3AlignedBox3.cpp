#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistRay3AlignedBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRay3AlignedBox3
    {
    public:
        UnitTestDistRay3AlignedBox3();

    private:
        using RBQuery = DCPQuery<double, Ray3<double>, AlignedBox3<double>>;
        using RBOutput = RBQuery::Output;

        void Test();
    };
}

UnitTestDistRay3AlignedBox3::UnitTestDistRay3AlignedBox3()
{
    UTInformation("Mathematics/Distance/3D/DistRay3AlignedBox3");

    Test();
}

void UnitTestDistRay3AlignedBox3::Test()
{
    RBQuery query{};
    RBOutput output{};
    Ray3<double> ray{};
    AlignedBox3<double> box{};
    double const maxError = 1e-14;

    box.min = { 1.0, 2.0, 3.0 };
    box.max = { 3.0, 5.0, 7.0 };
    ray.origin = { -1.0, -1.0, -1.0 };
    ray.direction = { -0.13375998748853216, -0.49589068532333880, 0.85802138315814536 };
    output = query(ray, box);
    UTAssert(std::fabs(output.distance - 5.1174239793088221) <= maxError,
        "Invalid distance");
    UTAssert(output.parameter > 0.0, "Invalid parameter.");

    // Verified with Mathematica.
    ray.origin += 6.0 * ray.direction;
    output = query(ray, box);
    UTAssert(std::fabs(output.distance - 6.5999302563683777) <= maxError,
        "Invalid distance");
    UTAssert(output.parameter == 0.0, "Invalid parameter.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistRay3AlignedBox3.h>

namespace gtl
{
    template class DCPQuery<float, Ray3<float>, AlignedBox3<float>>;
    template class DCPQuery<double, Ray3<double>, AlignedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Ray3<Rational>, AlignedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistRay3AlignedBox3)
