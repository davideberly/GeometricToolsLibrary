#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistLine3AlignedBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLine3AlignedBox3
    {
    public:
        UnitTestDistLine3AlignedBox3();

    private:
        using LBQuery = DCPQuery<double, Line3<double>, AlignedBox3<double>>;
        using LBOutput = LBQuery::Output;

        void Test();
    };
}

UnitTestDistLine3AlignedBox3::UnitTestDistLine3AlignedBox3()
{
    UTInformation("Mathematics/Distance/3D/DistLine3AlignedBox3");

    Test();
}

void UnitTestDistLine3AlignedBox3::Test()
{
    LBQuery query{};
    LBOutput output{};
    Line3<double> line{};
    AlignedBox3<double> box{};
    double const maxError = 1e-14;

    box.min = { 1.0, 2.0, 3.0 };
    box.max = { 3.0, 5.0, 7.0 };
    line.origin = { -1.0, -1.0, -1.0 };
    line.direction = { -0.13375998748853216, -0.49589068532333880, 0.85802138315814536 };
    output = query(line, box);
    UTAssert(std::fabs(output.distance - 5.1174239793088221) <= maxError,
        "Invalid distance");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistLine3AlignedBox3.h>

namespace gtl
{
    template class DCPQuery<float, Line3<float>, AlignedBox3<float>>;
    template class DCPQuery<double, Line3<double>, AlignedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line3<Rational>, AlignedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLine3AlignedBox3)
