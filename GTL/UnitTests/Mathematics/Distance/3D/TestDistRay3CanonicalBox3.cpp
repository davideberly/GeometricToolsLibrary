#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistRay3CanonicalBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRay3CanonicalBox3
    {
    public:
        UnitTestDistRay3CanonicalBox3();

    private:
        using RBQuery = DCPQuery<double, Ray3<double>, CanonicalBox3<double>>;
        using RBOutput = RBQuery::Output;

        void Test();
    };
}

UnitTestDistRay3CanonicalBox3::UnitTestDistRay3CanonicalBox3()
{
    UTInformation("Mathematics/Distance/3D/DistRay3CanonicalBox3");

    Test();
}

void UnitTestDistRay3CanonicalBox3::Test()
{
    RBQuery query{};
    RBOutput output{};
    Ray3<double> ray{};
    CanonicalBox3<double> box{};
    double const maxError = 1e-14;

    box.extent = { 1.5, 1.0, 0.5 };
    ray.origin = { -2.5, -2.0, -1.5 };
    ray.direction = { 0.55793191403459019, 0.81406026771105011, 0.16130052645888099 };
    output = query(ray, box);
    UTAssert(std::fabs(output.distance - 0.39783584635892300) <= maxError,
        "Invalid distance");
    UTAssert(output.parameter > 0.0, "Invalid parameter.");

    // Verified with Mathematica.
    ray.origin += 6.0 * ray.direction;
    output = query(ray, box);
    UTAssert(std::fabs(output.distance - 1.8846366492660536) <= maxError,
        "Invalid distance");
    UTAssert(output.parameter == 0.0, "Invalid parameter.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistRay3CanonicalBox3.h>

namespace gtl
{
    template class DCPQuery<float, Ray3<float>, CanonicalBox3<float>>;
    template class DCPQuery<double, Ray3<double>, CanonicalBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Ray3<Rational>, CanonicalBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistRay3CanonicalBox3)
