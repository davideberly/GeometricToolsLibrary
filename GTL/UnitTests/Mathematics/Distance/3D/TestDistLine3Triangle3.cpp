#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistLine3Triangle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLine3Triangle3
    {
    public:
        UnitTestDistLine3Triangle3();

    private:
        using LTQuery = DCPQuery<double, Line<double, 3>, Triangle<double, 3>>;
        using LTOutput = LTQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(LTOutput const& output,
            double sqrDistance,
            double parameter,
            Vector<double, 3> const& barycentric,
            Vector<double, 3> const& closest0,
            Vector<double, 3> const& closest1);
    };
}

UnitTestDistLine3Triangle3::UnitTestDistLine3Triangle3()
{
    UTInformation("Mathematics/Distance/3D/DistLine3Triangle3");

    Test();
}

void UnitTestDistLine3Triangle3::Validate(LTOutput const& output,
    double sqrDistance,
    double parameter,
    Vector<double, 3> const& barycentric,
    Vector<double, 3> const& closest0,
    Vector<double, 3> const& closest1)
{
    double constexpr maxError = 1e-14;

    double error = std::fabs(output.sqrDistance - sqrDistance);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(sqrDistance));
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter - parameter);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.barycentric[0] - barycentric[0]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.barycentric[1] - barycentric[1]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.barycentric[2] - barycentric[2]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - closest0);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[1] - closest1);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
}

void UnitTestDistLine3Triangle3::Test()
{
    LTQuery query{};
    LTOutput output{};
    Line<double, 3> line{};
    Triangle<double, 3> triangle{};

    Vector<double, 3> translate{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rotate{};
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, rotate);

    // line intersects triangle
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = { 0.0625, 0.0125, -1.0 };
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    output = query(line, triangle);
    Validate(output, 0.0, 0.25, 
        { 0.98125, 0.015625, 0.003125 },
        { 0.015625, 0.003125, 0.0 },
        { 0.015625, 0.003125, 0.0 });

    // line intersects triangle, arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = { 0.0625, 0.0125, -1.0 };
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    triangle.v[0] = rotate * triangle.v[0] + translate;
    triangle.v[1] = rotate * triangle.v[1] + translate;
    triangle.v[2] = rotate * triangle.v[2] + translate;
    output = query(line, triangle);
    Validate(output, 0.0, 0.25,
        { 0.98125, 0.015625, 0.003125 },
        rotate * Vector<double, 3>{ 0.015625, 0.003125, 0.0 } + translate,
        rotate * Vector<double, 3>{ 0.015625, 0.003125, 0.0 } + translate);

    // line does not intersect triangle, in (b0,b1,b2) = (-,+,+)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ 2.0, 3.0, 0.0 } - line.origin;
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    output = query(line, triangle);
    Validate(output, 0.039800995024875621, 0.20398009950248755,
        { 0.0, 0.39800995024875618, 0.60199004975124382 },
        { 0.40796019900497510, 0.61194029850746268, 0.19900497512437812 },
        { 0.39800995024875618, 0.60199004975124382, 0.0 });

    // line does not intersect triangle, in (b0,b1,b2) = (-,+,+)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ 2.0, 3.0, 0.0 } - line.origin;
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    triangle.v[0] = rotate * triangle.v[0] + translate;
    triangle.v[1] = rotate * triangle.v[1] + translate;
    triangle.v[2] = rotate * triangle.v[2] + translate;
    output = query(line, triangle);
    Validate(output, 0.039800995024875621, 0.20398009950248755,
        { 0.0, 0.39800995024875618, 0.60199004975124382 },
        rotate* Vector<double, 3>{ 0.40796019900497510, 0.61194029850746268, 0.19900497512437812 } + translate,
        rotate* Vector<double, 3>{ 0.39800995024875618, 0.60199004975124382, 0.0 } + translate);

    // line does not intersect triangle, in (b0,b1,b2) = (-,-,+)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -1.0, 3.0, 0.0 } - line.origin;
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    output = query(line, triangle);
    Validate(output, 0.058823529411764705, 0.058823529411764705,
        { 0.82352941176470584, 0.0, 0.17647058823529416 },
        { -0.058823529411764705, 0.17647058823529410, 0.23529411764705882 },
        { 0.0, 0.17647058823529416, 0.0 });

    // line does not intersect triangle, in (b0,b1,b2) = (-,-,+)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -1.0, 3.0, 0.0 } - line.origin;
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    triangle.v[0] = rotate * triangle.v[0] + translate;
    triangle.v[1] = rotate * triangle.v[1] + translate;
    triangle.v[2] = rotate * triangle.v[2] + translate;
    output = query(line, triangle);
    Validate(output, 0.058823529411764705, 0.058823529411764705,
        { 0.82352941176470584, 0.0, 0.17647058823529416 },
        rotate* Vector<double, 3>{ -0.058823529411764705, 0.17647058823529410, 0.23529411764705882 } + translate,
        rotate* Vector<double, 3>{ 0.0, 0.17647058823529416, 0.0 } + translate);

    // line does not intersect triangle, in (b0,b1,b2) = (+,-,+)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -1.0, 1.0, 0.0 } - line.origin;
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    output = query(line, triangle);
    Validate(output, 0.058823529411764705, 0.058823529411764705,
        { 0.94117647058823528, 0.0, 0.058823529411764719 },
        { -0.058823529411764705, 0.058823529411764705, 0.23529411764705882 },
        { 0.0, 0.058823529411764719, 0.0 });

    // line does not intersect triangle, in (b0,b1,b2) = (+,-,+)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -1.0, 1.0, 0.0 } - line.origin;
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    triangle.v[0] = rotate * triangle.v[0] + translate;
    triangle.v[1] = rotate * triangle.v[1] + translate;
    triangle.v[2] = rotate * triangle.v[2] + translate;
    output = query(line, triangle);
    Validate(output, 0.058823529411764705, 0.058823529411764705,
        { 0.94117647058823528, 0.0, 0.058823529411764719 },
        rotate* Vector<double, 3>{ -0.058823529411764705, 0.058823529411764705, 0.23529411764705882 } + translate,
        rotate* Vector<double, 3>{ 0.0, 0.058823529411764719, 0.0 } + translate);

    // line does not intersect triangle, in (b0,b1,b2) = (+,-,-)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -1.0, -2.0, 0.0 } - line.origin;
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    output = query(line, triangle);
    Validate(output, 0.061728395061728392, 0.012345679012345678,
        { 1.0, 0.0, 0.0 },
        { -0.012345679012345678, -0.024691358024691357, 0.24691358024691357 },
        { 0.0, 0.0, 0.0 });

    // line does not intersect triangle, in (b0,b1,b2) = (+,-,-)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -1.0, -2.0, 0.0 } - line.origin;
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    triangle.v[0] = rotate * triangle.v[0] + translate;
    triangle.v[1] = rotate * triangle.v[1] + translate;
    triangle.v[2] = rotate * triangle.v[2] + translate;
    output = query(line, triangle);
    Validate(output, 0.061728395061728392, 0.012345679012345678,
        { 1.0, 0.0, 0.0 },
        rotate* Vector<double, 3>{ -0.012345679012345678, -0.024691358024691357, 0.24691358024691357 } + translate,
        rotate* Vector<double, 3>{ 0.0, 0.0, 0.0 } + translate);

    // line does not intersect triangle, in (b0,b1,b2) = (+,+,-)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ 1.0, -1.0, 0.0 } - line.origin;
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    output = query(line, triangle);
    Validate(output, 0.058823529411764705, 0.058823529411764705,
        { 0.94117647058823528, 0.058823529411764705, 0.0 },
        { 0.058823529411764705, -0.058823529411764705, 0.23529411764705882 },
        { 0.058823529411764705, 0.0, 0.0 });

    // line does not intersect triangle, in (b0,b1,b2) = (+,+,-)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ 1.0, -1.0, 0.0 } - line.origin;
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    triangle.v[0] = rotate * triangle.v[0] + translate;
    triangle.v[1] = rotate * triangle.v[1] + translate;
    triangle.v[2] = rotate * triangle.v[2] + translate;
    output = query(line, triangle);
    Validate(output, 0.058823529411764705, 0.058823529411764705,
        { 0.94117647058823528, 0.058823529411764705, 0.0 },
        rotate* Vector<double, 3>{ 0.058823529411764705, -0.058823529411764705, 0.23529411764705882 } + translate,
        rotate* Vector<double, 3>{ 0.058823529411764705, 0.0, 0.0 } + translate);

    // line does not intersect triangle, in (b0,b1,b2) = (-,+,-)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ 3.0, -1.0, 0.0 } - line.origin;
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    output = query(line, triangle);
    Validate(output, 0.058823529411764705, 0.058823529411764705,
        { 0.82352941176470584, 0.17647058823529413, 0.0 },
        { 0.17647058823529410, -0.058823529411764705, 0.23529411764705882 },
        { 0.17647058823529413, 0.0, 0.0 });

    // line does not intersect triangle, in (b0,b1,b2) = (-,+,-)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ 3.0, -1.0, 0.0 } - line.origin;
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    triangle.v[0] = rotate * triangle.v[0] + translate;
    triangle.v[1] = rotate * triangle.v[1] + translate;
    triangle.v[2] = rotate * triangle.v[2] + translate;
    output = query(line, triangle);
    Validate(output, 0.058823529411764705, 0.058823529411764705,
        { 0.82352941176470584, 0.17647058823529413, 0.0 },
        rotate* Vector<double, 3>{ 0.17647058823529410, -0.058823529411764705, 0.23529411764705882 } + translate,
        rotate* Vector<double, 3>{ 0.17647058823529413, 0.0, 0.0 } + translate);

    // choose triangle with non-unit-length normal, (b0,b1,b2) = (+,+,+)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = { 0.0625, 0.0125, -1.0 };
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 2.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 3.0, 0.0 };
    output = query(line, triangle);
    Validate(output, 0.0, 0.25,
        { 0.99114583333333328, 0.0078125000000000000, 0.0010416666666666669 },
        { 0.015625, 0.003125, 0.0 },
        { 0.015625, 0.003125, 0.0 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistLine3Triangle3.h>

namespace gtl
{
    template class DCPQuery<float, Line<float, 3>, Triangle<float, 3>>;
    template class DCPQuery<double, Line<double, 3>, Triangle<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line<Rational, 3>, Triangle<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLine3Triangle3)
