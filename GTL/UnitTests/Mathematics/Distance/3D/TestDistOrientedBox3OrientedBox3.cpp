#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistOrientedBox3OrientedBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistOrientedBox3OrientedBox3
    {
    public:
        UnitTestDistOrientedBox3OrientedBox3();

    private:
        using BBQuery = DCPQuery<double, OrientedBox3<double>, OrientedBox3<double>>;
        using BBOutput = BBQuery::Output;

        void Test();

        void Validate(BBOutput const& output,
            double sqrDistance,
            Vector3<double> const& closest0,
            Vector3<double> const& closest1)
        {
            double const maxError = 1e-14;

            double error = std::fabs(output.sqrDistance - sqrDistance);
            UTAssert(
                error <= maxError,
                "Incorrect sqrDistance.");
            error = Length(output.closest[0] - closest0);
            UTAssert(
                error <= maxError,
                "Incorrect closest[0].");
            error = Length(output.closest[1] - closest1);
            UTAssert(
                error <= maxError,
                "Incorrect closest[0].");
        }
    };
}

UnitTestDistOrientedBox3OrientedBox3::UnitTestDistOrientedBox3OrientedBox3()
{
    UTInformation("Mathematics/Distance/3D/DistOrientedBox3OrientedBox3");

    Test();
}

void UnitTestDistOrientedBox3OrientedBox3::Test()
{
    BBQuery query{};
    BBOutput output{};
    OrientedBox3<double> box0{}, box1{};

    box0.center = { -1.0, -2.0, -3.0 };
    box0.axis[0] = { 4.0, 5.0, 6.0 };
    ComputeOrthonormalBasis(1, box0.axis[0], box0.axis[1], box0.axis[2]);
    box0.extent = { 0.7, 0.1, 0.5 };

    box1.center = { 3.0, 2.0, 1.0 };
    box1.axis[0] = { -1.0, 0.0, 1.0 };
    ComputeOrthonormalBasis(1, box1.axis[0], box1.axis[1], box1.axis[2]);
    box1.extent = { 0.5, 0.7, 0.9 };

    output = query(box0, box1);
    Validate(output, 25.712631576842536,
        { -0.23588010823300237, -1.6702274208839925, -2.7604778985789240 },
        { 2.7673241483423769, 1.1000000000000001, 0.24272635799645698 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistOrientedBox3OrientedBox3.h>

namespace gtl
{
    template class DCPQuery<float, OrientedBox3<float>, OrientedBox3<float>>;
    template class DCPQuery<double, OrientedBox3<double>, OrientedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, OrientedBox3<Rational>, OrientedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistOrientedBox3OrientedBox3)
