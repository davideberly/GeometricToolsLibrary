#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistLine3Rectangle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLine3Rectangle3
    {
    public:
        UnitTestDistLine3Rectangle3();

    private:
        using LRQuery = DCPQuery<double, Line<double, 3>, Rectangle<double, 3>>;
        using LROutput = LRQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(LROutput const& output,
            double sqrDistance,
            double parameter,
            Vector<double, 2> const& rectangle,
            Vector<double, 3> const& closest0,
            Vector<double, 3> const& closest1);
    };
}

UnitTestDistLine3Rectangle3::UnitTestDistLine3Rectangle3()
{
    UTInformation("Mathematics/Distance/3D/DistLine3Rectangle3");

    Test();
}

void UnitTestDistLine3Rectangle3::Validate(LROutput const& output,
    double sqrDistance,
    double parameter,
    Vector<double, 2> const& cartesian,
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
    error = std::fabs(output.cartesian[0] - cartesian[0]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] - cartesian[1]);
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

void UnitTestDistLine3Rectangle3::Test()
{
    LRQuery query{};
    LROutput output{};
    Line<double, 3> line{};
    Rectangle<double, 3> rectangle{};

    Vector<double, 3> translate{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rotate{};
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, rotate);

    // line intersects rectangle
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = { 0.0625, 0.0125, -1.0 };
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    output = query(line, rectangle);
    Validate(output, 0.0, 0.25, 
        { 0.015625, 0.003125 },
        { 0.015625, 0.003125, 0.0 },
        { 0.015625, 0.003125, 0.0 });

    // line intersects rectangle, arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = { 0.0625, 0.0125, -1.0 };
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(line, rectangle);
    Validate(output, 0.0, 0.25,
        { 0.015625, 0.003125 },
        rotate * Vector<double, 3>{ 0.015625, 0.003125, 0.0 } + translate,
        rotate * Vector<double, 3>{ 0.015625, 0.003125, 0.0 } + translate);

    // line does not intersect rectangle, (s0,s1) = (+,0)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ 3.0, 0.5, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    output = query(line, rectangle);
    Validate(output, 0.0068965517241379283, 0.66896551724137931,
        { 2.0, 0.33448275862068955 },
        { 2.0068965517241377, 0.33448275862068966, 0.082758620689655171 },
        { 2.0, 0.33448275862068955, 0.0 });

    // line does not intersect rectangle, (s0,s1) = (+,0)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ 3.0, 0.5, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(line, rectangle);
    Validate(output, 0.0068965517241379283, 0.66896551724137931,
        { 2.0, 0.33448275862068955 },
        rotate* Vector<double, 3>{ 2.0068965517241377, 0.33448275862068966, 0.082758620689655171 } + translate,
        rotate* Vector<double, 3>{ 2.0, 0.33448275862068955, 0.0 } + translate);

    // line does not intersect rectangle, (s0,s1) = (+,+)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ 3.0, 1.5, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    output = query(line, rectangle);
    Validate(output, 0.0069060773480662981, 0.66850828729281764,
        { 2.0, 1.0 },
        { 2.0055248618784529, 1.0027624309392265, 0.082872928176795591 },
        { 2.0, 1.0, 0.0 });

    // line does not intersect rectangle, (s0,s1) = (+,+)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ 3.0, 1.5, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(line, rectangle);
    Validate(output, 0.0069060773480662981, 0.66850828729281764,
        { 2.0, 1.0 },
        rotate* Vector<double, 3>{ 2.0055248618784529, 1.0027624309392265, 0.082872928176795591 } + translate,
        rotate* Vector<double, 3>{ 2.0, 1.0, 0.0 } + translate);

    // line does not intersect rectangle, (s0,s1) = (0,+)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ 1.0, 2.0, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    output = query(line, rectangle);
    Validate(output, 0.015384615384615384, 0.50769230769230766,
        { 0.50769230769230766, 1.0 },
        { 0.50769230769230766, 1.0153846153846153, 0.12307692307692308 },
        { 0.50769230769230766, 1.0, 0.0 });

    // line does not intersect rectangle, (s0,s1) = (0,+)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ 1.0, 2.0, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(line, rectangle);
    Validate(output, 0.015384615384615384, 0.50769230769230766,
        { 0.50769230769230766, 1.0 },
        rotate* Vector<double, 3>{ 0.50769230769230766, 1.0153846153846153, 0.12307692307692308 } + translate,
        rotate* Vector<double, 3>{ 0.50769230769230766, 1.0, 0.0 } + translate);

    // line does not intersect rectangle, (s0,s1) = (-,+)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -3.0, 1.5, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    output = query(line, rectangle);
    Validate(output, 0.0069060773480662981, 0.66850828729281764,
        { -2.0, 1.0 },
        { -2.0055248618784529, 1.0027624309392265, 0.082872928176795591 },
        { -2.0, 1.0, 0.0 });

    // line does not intersect rectangle, (s0,s1) = (-,+)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -3.0, 1.5, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(line, rectangle);
    Validate(output, 0.0069060773480662981, 0.66850828729281764,
        { -2.0, 1.0 },
        rotate* Vector<double, 3>{ -2.0055248618784529, 1.0027624309392265, 0.082872928176795591 } + translate,
        rotate* Vector<double, 3>{ -2.0, 1.0, 0.0 } + translate);

    // line does not intersect rectangle, (s0,s1) = (-,0)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -3.0, 0.5, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    output = query(line, rectangle);
    Validate(output, 0.0068965517241379283, 0.66896551724137931,
        { -2.0, 0.33448275862068955 },
        { -2.0068965517241377, 0.33448275862068966, 0.082758620689655171 },
        { -2.0, 0.33448275862068955, 0.0 });

    // line does not intersect rectangle, (s0,s1) = (-,0)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -3.0, 0.5, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(line, rectangle);
    Validate(output, 0.0068965517241379283, 0.66896551724137931,
        { -2.0, 0.33448275862068955 },
        rotate* Vector<double, 3>{ -2.0068965517241377, 0.33448275862068966, 0.082758620689655171 } + translate,
        rotate* Vector<double, 3>{ -2.0, 0.33448275862068955, 0.0 } + translate);

    // line does not intersect rectangle, (s0,s1) = (-,-)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -3.0, -1.5, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    output = query(line, rectangle);
    Validate(output, 0.0069060773480662981, 0.66850828729281764,
        { -2.0, -1.0 },
        { -2.0055248618784529, -1.0027624309392265, 0.082872928176795591 },
        { -2.0, -1.0, 0.0 });

    // line does not intersect rectangle, (s0,s1) = (-,-)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -3.0, -1.5, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(line, rectangle);
    Validate(output, 0.0069060773480662981, 0.66850828729281764,
        { -2.0, -1.0 },
        rotate* Vector<double, 3>{ -2.0055248618784529, -1.0027624309392265, 0.082872928176795591 } + translate,
        rotate* Vector<double, 3>{ -2.0, -1.0, 0.0 } + translate);

    // line does not intersect rectangle, (s0,s1) = (0,-)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -1.0, -2.0, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    output = query(line, rectangle);
    Validate(output, 0.015384615384615384, 0.50769230769230766,
        { -0.50769230769230766, -1.0 },
        { -0.50769230769230766, -1.0153846153846153, 0.12307692307692308 },
        { -0.50769230769230766, -1.0, 0.0 });

    // line does not intersect rectangle, (s0,s1) = (0,-)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -1.0, -2.0, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(line, rectangle);
    Validate(output, 0.015384615384615384, 0.50769230769230766,
        { -0.50769230769230766, -1.0 },
        rotate* Vector<double, 3>{ -0.50769230769230766, -1.0153846153846153, 0.12307692307692308 } + translate,
        rotate* Vector<double, 3>{ -0.50769230769230766, -1.0, 0.0 } + translate);

    // line does not intersect rectangle, (s0,s1) = (+,-)
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -3.0, -1.5, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    output = query(line, rectangle);
    Validate(output, 0.0069060773480662981, 0.66850828729281764,
        { -2.0, -1.0 },
        { -2.0055248618784529, -1.0027624309392265, 0.082872928176795591 },
        { -2.0, -1.0, 0.0 });

    // line does not intersect rectangle, (s0,s1) = (+,-)
    // arbitrary rigid motion first
    line.origin = { 0.0, 0.0, 0.25 };
    line.direction = Vector<double, 3>{ -3.0, -1.5, 0.0 } - line.origin;
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    line.origin = rotate * line.origin + translate;
    line.direction = rotate * line.direction;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(line, rectangle);
    Validate(output, 0.0069060773480662981, 0.66850828729281764,
        { -2.0, -1.0 },
        rotate* Vector<double, 3>{ -2.0055248618784529, -1.0027624309392265, 0.082872928176795591 } + translate,
        rotate* Vector<double, 3>{ -2.0, -1.0, 0.0 } + translate);
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistLine3Rectangle3.h>

namespace gtl
{
    template class DCPQuery<float, Line<float, 3>, Rectangle<float, 3>>;
    template class DCPQuery<double, Line<double, 3>, Rectangle<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line<Rational, 3>, Rectangle<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLine3Rectangle3)
