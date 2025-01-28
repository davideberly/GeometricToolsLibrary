#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistTriangle3Triangle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistTriangle3Triangle3
    {
    public:
        UnitTestDistTriangle3Triangle3();

    private:
        using TTQuery = DCPQuery<double, Triangle<double, 3>, Triangle<double, 3>>;
        using TTOutput = TTQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(TTOutput const& output,
            double sqrDistance,
            Vector<double, 3> const& barycentric0,
            Vector<double, 3> const& barycentric1,
            Vector<double, 3> const& closest0,
            Vector<double, 3> const& closest1);
    };
}

UnitTestDistTriangle3Triangle3::UnitTestDistTriangle3Triangle3()
{
    UTInformation("Mathematics/Distance/3D/DistTriangle3Triangle3");

    Test();
}

void UnitTestDistTriangle3Triangle3::Validate(TTOutput const& output,
    double sqrDistance,
    Vector<double, 3> const& barycentric0,
    Vector<double, 3> const& barycentric1,
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
    error = std::fabs(output.barycentric0[0] - barycentric0[0]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.barycentric0[1] - barycentric0[1]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.barycentric0[2] - barycentric0[2]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.barycentric1[0] - barycentric1[0]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.barycentric1[1] - barycentric1[1]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.barycentric1[2] - barycentric1[2]);
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

void UnitTestDistTriangle3Triangle3::Test()
{
    TTQuery query{};
    TTOutput output{};
    std::array<Triangle<double, 3>, 2> triangle{};

    Vector<double, 3> translate{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rotate{};
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, rotate);

    // Coverage of second loop in DistTriangle3Triangle3 where
    // output.closet[i] = stOutput.closest[1-i];
    triangle[0].v[0] = { 0.0, 0.0, 0.0 };
    triangle[0].v[1] = { 1.0, 0.0, 0.0 };
    triangle[0].v[2] = { 0.0, 1.0, 0.0 };
    triangle[1].v[0] = { 0.45, 0.45, 0.001 };
    triangle[1].v[1] = { 0.5, -0.1, 0.002 };
    triangle[1].v[2] = { 0.5, 0.75, 0.002 };
    output = query(triangle[0], triangle[1]);
    Validate(output, 1.0e-06,
        { 0.1, 0.45, 0.45 },
        { 1.0, 0.0, 0.0 },
        { 0.45, 0.45, 0.0 },
        { 0.45, 0.45, 0.001 });

    // triangles are coincident and overlapping
    triangle[0].v[0] = { 0.0, 0.0, 0.0 };
    triangle[0].v[1] = { 1.0, 0.0, 0.0 };
    triangle[0].v[2] = { 0.0, 1.0, 0.0 };
    triangle[1].v[0] = { 0.25, 0.25, 0.0 };
    triangle[1].v[1] = { 2.0, 0.5, 0.0 };
    triangle[1].v[2] = { 1.5, 1.5, 0.0 };
    output = query(triangle[0], triangle[1]);
    Validate(output, 0.0,
        { 0.0, 0.5, 0.5 },
        { 0.8, 0.0, 0.2 },
        { 0.5, 0.5, 0.0 },
        { 0.5, 0.5, 0.0 });

    // triangles are coincident and overlapping
    // arbitrary rigid motion first
    triangle[0].v[0] = { 0.0, 0.0, 0.0 };
    triangle[0].v[1] = { 1.0, 0.0, 0.0 };
    triangle[0].v[2] = { 0.0, 1.0, 0.0 };
    triangle[1].v[0] = { 0.25, 0.25, 0.0 };
    triangle[1].v[1] = { 2.0, 0.5, 0.0 };
    triangle[1].v[2] = { 1.5, 1.5, 0.0 };
    for (std::size_t j = 0; j < 2; ++j)
    {
        for (std::size_t i = 0; i < 3; ++i)
        {
            triangle[j].v[i] = rotate * triangle[j].v[i] + translate;
        }
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 0.0,
        { 0.5, 0.25, 0.25 },
        { 1.0, 0.0, 0.0 },
        rotate* Vector<double, 3>{ 0.25, 0.25, 0.0 } + translate,
        rotate* Vector<double, 3>{ 0.25, 0.25, 0.0 } + translate);

    // triangles are parallel but not coincident, projections overlap
    triangle[0].v[0] = { 0.0, 0.0, 0.0 };
    triangle[0].v[1] = { 1.0, 0.0, 0.0 };
    triangle[0].v[2] = { 0.0, 1.0, 0.0 };
    triangle[1].v[0] = { 0.25, 0.25, 1.0 };
    triangle[1].v[1] = { 2.0, 0.5, 1.0 };
    triangle[1].v[2] = { 1.5, 1.5, 1.0 };
    output = query(triangle[0], triangle[1]);
    Validate(output, 1.0,
        { 0.0, 0.5, 0.5 },
        { 0.8, 0.0, 0.2 },
        { 0.5, 0.5, 0.0 },
        { 0.5, 0.5, 1.0 });

    // triangles are parallel but not coincident, projections overlap
    // arbitrary rigid motion first
    triangle[0].v[0] = { 0.0, 0.0, 0.0 };
    triangle[0].v[1] = { 1.0, 0.0, 0.0 };
    triangle[0].v[2] = { 0.0, 1.0, 0.0 };
    triangle[1].v[0] = { 0.25, 0.25, 1.0 };
    triangle[1].v[1] = { 2.0, 0.5, 1.0 };
    triangle[1].v[2] = { 1.5, 1.5, 1.0 };
    for (std::size_t j = 0; j < 2; ++j)
    {
        for (std::size_t i = 0; i < 3; ++i)
        {
            triangle[j].v[i] = rotate * triangle[j].v[i] + translate;
        }
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 1.0,
        { 0.0, 0.5, 0.5 },
        { 0.8, 0.0, 0.2 },
        { -0.14326666666666660, 6.3122333333333334, -1.8068333333333335 },
        { 0.59006666666666663, 6.4455666666666662, -1.1401666666666668 });

    Vector<double, 3> trn0{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rot0{};
    Quaternion<double> q0(1.0, 2.0, 3.0, 4.0);
    Normalize(q0);
    RigidMotion<double>::Convert(q0, rot0);
    Vector<double, 3> trn1{ -1.0, -2.0, 3.0 };
    Matrix3x3<double> rot1{};
    Quaternion<double> q1(2.0, 0.0, 5.0, 1.0);
    Normalize(q1);
    RigidMotion<double>::Convert(q1, rot1);

    // random triangles, order {0,1,2}, {0,1,2}
    triangle[0].v[0] = { 0.0, 0.0, 0.0 };
    triangle[0].v[1] = { 1.0, 0.0, 0.0 };
    triangle[0].v[2] = { 0.0, 1.0, 0.0 };
    triangle[1].v[0] = { 0.25, 0.25, 1.0 };
    triangle[1].v[1] = { 2.0, 0.5, 1.0 };
    triangle[1].v[2] = { 1.5, 1.5, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle[0].v[i] = rot0 * triangle[0].v[i] + trn0;
        triangle[1].v[i] = rot1 * triangle[1].v[i] + trn1;
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 96.277378353333319,
        { 0.0, 0.0, 1.0 },
        { 1.0, 0.0, 0.0 },
        { -0.54326666666666668, 6.0122333333333327, -1.3068333333333335 },
        { -0.58333333333333337, -2.2833333333333332, 3.9333333333333336 });

    // random triangles, order {0,1,2}, {1,2,0}
    triangle[0].v[0] = { 0.0, 0.0, 0.0 };
    triangle[0].v[1] = { 1.0, 0.0, 0.0 };
    triangle[0].v[2] = { 0.0, 1.0, 0.0 };
    triangle[1].v[1] = { 0.25, 0.25, 1.0 };
    triangle[1].v[2] = { 2.0, 0.5, 1.0 };
    triangle[1].v[0] = { 1.5, 1.5, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle[0].v[i] = rot0 * triangle[0].v[i] + trn0;
        triangle[1].v[i] = rot1 * triangle[1].v[i] + trn1;
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 96.277378353333319,
        { 0.0, 0.0, 1.0 },
        { 0.0, 1.0, 0.0 },
        { -0.54326666666666668, 6.0122333333333327, -1.3068333333333335 },
        { -0.58333333333333337, -2.2833333333333332, 3.9333333333333336 });

    // random triangles, order {0,1,2}, {2,0,1}
    triangle[0].v[0] = { 0.0, 0.0, 0.0 };
    triangle[0].v[1] = { 1.0, 0.0, 0.0 };
    triangle[0].v[2] = { 0.0, 1.0, 0.0 };
    triangle[1].v[2] = { 0.25, 0.25, 1.0 };
    triangle[1].v[0] = { 2.0, 0.5, 1.0 };
    triangle[1].v[1] = { 1.5, 1.5, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle[0].v[i] = rot0 * triangle[0].v[i] + trn0;
        triangle[1].v[i] = rot1 * triangle[1].v[i] + trn1;
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 96.277378353333319,
        { 0.0, 0.0, 1.0 },
        { 0.0, 0.0, 1.0 },
        { -0.54326666666666668, 6.0122333333333327, -1.3068333333333335 },
        { -0.58333333333333337, -2.2833333333333332, 3.9333333333333336 });

    // random triangles, order {1,2,0}, {0,1,2}
    triangle[0].v[1] = { 0.0, 0.0, 0.0 };
    triangle[0].v[2] = { 1.0, 0.0, 0.0 };
    triangle[0].v[0] = { 0.0, 1.0, 0.0 };
    triangle[1].v[0] = { 0.25, 0.25, 1.0 };
    triangle[1].v[1] = { 2.0, 0.5, 1.0 };
    triangle[1].v[2] = { 1.5, 1.5, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle[0].v[i] = rot0 * triangle[0].v[i] + trn0;
        triangle[1].v[i] = rot1 * triangle[1].v[i] + trn1;
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 96.277378353333319,
        { 1.0, 0.0, 0.0 },
        { 1.0, 0.0, 0.0 },
        { -0.54326666666666668, 6.0122333333333327, -1.3068333333333335 },
        { -0.58333333333333337, -2.2833333333333332, 3.9333333333333336 });

    // random triangles, order {1,2,0}, {1,2,0}
    triangle[0].v[1] = { 0.0, 0.0, 0.0 };
    triangle[0].v[2] = { 1.0, 0.0, 0.0 };
    triangle[0].v[0] = { 0.0, 1.0, 0.0 };
    triangle[1].v[1] = { 0.25, 0.25, 1.0 };
    triangle[1].v[2] = { 2.0, 0.5, 1.0 };
    triangle[1].v[0] = { 1.5, 1.5, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle[0].v[i] = rot0 * triangle[0].v[i] + trn0;
        triangle[1].v[i] = rot1 * triangle[1].v[i] + trn1;
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 96.277378353333319,
        { 1.0, 0.0, 0.0 },
        { 0.0, 1.0, 0.0 },
        { -0.54326666666666668, 6.0122333333333327, -1.3068333333333335 },
        { -0.58333333333333337, -2.2833333333333332, 3.9333333333333336 });

    // random triangles, order {1,2,0}, {2,0,1}
    triangle[0].v[1] = { 0.0, 0.0, 0.0 };
    triangle[0].v[2] = { 1.0, 0.0, 0.0 };
    triangle[0].v[0] = { 0.0, 1.0, 0.0 };
    triangle[1].v[1] = { 0.25, 0.25, 1.0 };
    triangle[1].v[2] = { 2.0, 0.5, 1.0 };
    triangle[1].v[0] = { 1.5, 1.5, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle[0].v[i] = rot0 * triangle[0].v[i] + trn0;
        triangle[1].v[i] = rot1 * triangle[1].v[i] + trn1;
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 96.277378353333319,
        { 1.0, 0.0, 0.0 },
        { 0.0, 1.0, 0.0 },
        { -0.54326666666666668, 6.0122333333333327, -1.3068333333333335 },
        { -0.58333333333333337, -2.2833333333333332, 3.9333333333333336 });

    // random triangles, order {2,0,1}, {0,1,2}
    triangle[0].v[2] = { 0.0, 0.0, 0.0 };
    triangle[0].v[0] = { 1.0, 0.0, 0.0 };
    triangle[0].v[1] = { 0.0, 1.0, 0.0 };
    triangle[1].v[0] = { 0.25, 0.25, 1.0 };
    triangle[1].v[1] = { 2.0, 0.5, 1.0 };
    triangle[1].v[2] = { 1.5, 1.5, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle[0].v[i] = rot0 * triangle[0].v[i] + trn0;
        triangle[1].v[i] = rot1 * triangle[1].v[i] + trn1;
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 96.277378353333319,
        { 0.0, 1.0, 0.0 },
        { 1.0, 0.0, 0.0 },
        { -0.54326666666666668, 6.0122333333333327, -1.3068333333333335 },
        { -0.58333333333333337, -2.2833333333333332, 3.9333333333333336 });

    // random triangles, order {2,0,1}, {1,2,0}
    triangle[0].v[2] = { 0.0, 0.0, 0.0 };
    triangle[0].v[0] = { 1.0, 0.0, 0.0 };
    triangle[0].v[1] = { 0.0, 1.0, 0.0 };
    triangle[1].v[1] = { 0.25, 0.25, 1.0 };
    triangle[1].v[2] = { 2.0, 0.5, 1.0 };
    triangle[1].v[0] = { 1.5, 1.5, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle[0].v[i] = rot0 * triangle[0].v[i] + trn0;
        triangle[1].v[i] = rot1 * triangle[1].v[i] + trn1;
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 96.277378353333319,
        { 0.0, 1.0, 0.0 },
        { 0.0, 1.0, 0.0 },
        { -0.54326666666666668, 6.0122333333333327, -1.3068333333333335 },
        { -0.58333333333333337, -2.2833333333333332, 3.9333333333333336 });

    // random triangles, order {2,0,1}, {2,0,1}
    triangle[0].v[2] = { 0.0, 0.0, 0.0 };
    triangle[0].v[0] = { 1.0, 0.0, 0.0 };
    triangle[0].v[1] = { 0.0, 1.0, 0.0 };
    triangle[1].v[2] = { 0.25, 0.25, 1.0 };
    triangle[1].v[0] = { 2.0, 0.5, 1.0 };
    triangle[1].v[1] = { 1.5, 1.5, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle[0].v[i] = rot0 * triangle[0].v[i] + trn0;
        triangle[1].v[i] = rot1 * triangle[1].v[i] + trn1;
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 96.277378353333319,
        { 0.0, 1.0, 0.0 },
        { 0.0, 0.0, 1.0 },
        { -0.54326666666666668, 6.0122333333333327, -1.3068333333333335 },
        { -0.58333333333333337, -2.2833333333333332, 3.9333333333333336 });

    // random triangles, order {0,1,2}, {0,1,2}
    triangle[0].v[0] = { 0.0, 0.0, 0.0 };
    triangle[0].v[1] = { 1.0, 0.0, 0.0 };
    triangle[0].v[2] = { 0.0, 1.0, 0.0 };
    triangle[1].v[0] = { 0.25, 0.25, 1.0 };
    triangle[1].v[1] = { 2.0, 0.5, -0.1 };
    triangle[1].v[2] = { 1.5, -0.1, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle[0].v[i] = rotate * triangle[0].v[i] + translate;
        triangle[1].v[i] = rotate * triangle[1].v[i] + translate;
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 0.32749221183800648,
        { 0.0, 0.87383177570093462, 0.12616822429906543 },
        { 0.50155763239875406, 0.49844236760124588, 0.0 },
        { 0.15579875389408115, 6.5365323987538941, -2.1806651090342681 },
        { 0.35455264797507813, 6.9114545171339561, -1.7967087227414327 });

    // random triangles, order {0,1,2}, {1,2,0}
    triangle[0].v[0] = { 0.0, 0.0, 0.0 };
    triangle[0].v[1] = { 1.0, 0.0, 0.0 };
    triangle[0].v[2] = { 0.0, 1.0, 0.0 };
    triangle[1].v[1] = { 0.25, 0.25, 1.0 };
    triangle[1].v[2] = { 2.0, 0.5, -0.1 };
    triangle[1].v[0] = { 1.5, -0.1, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle[0].v[i] = rotate * triangle[0].v[i] + translate;
        triangle[1].v[i] = rotate * triangle[1].v[i] + translate;
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 0.32749221183800648,
        { 0.0, 0.87383177570093462, 0.12616822429906543 },
        { 0.0, 0.50155763239875406, 0.49844236760124588 },
        { 0.15579875389408115, 6.5365323987538941, -2.1806651090342681 },
        { 0.35455264797507813, 6.9114545171339561, -1.7967087227414327 });

    // random triangles, order {0,1,2}, {2,0,1}
    triangle[0].v[0] = { 0.0, 0.0, 0.0 };
    triangle[0].v[1] = { 1.0, 0.0, 0.0 };
    triangle[0].v[2] = { 0.0, 1.0, 0.0 };
    triangle[1].v[2] = { 0.25, 0.25, 1.0 };
    triangle[1].v[0] = { 2.0, 0.5, -0.1 };
    triangle[1].v[1] = { 1.5, -0.1, 1.0 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle[0].v[i] = rotate * triangle[0].v[i] + translate;
        triangle[1].v[i] = rotate * triangle[1].v[i] + translate;
    }
    output = query(triangle[0], triangle[1]);
    Validate(output, 0.32749221183800648,
        { 0.0, 0.87383177570093462, 0.12616822429906543 },
        { 0.49844236760124588, 0.0, 0.50155763239875406 },
        { 0.15579875389408115, 6.5365323987538941, -2.1806651090342681 },
        { 0.35455264797507813, 6.9114545171339561, -1.7967087227414327 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistTriangle3Triangle3.h>

namespace gtl
{
    template class DCPQuery<float, Triangle<float, 3>, Triangle<float, 3>>;
    template class DCPQuery<double, Triangle<double, 3>, Triangle<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Triangle<Rational, 3>, Triangle<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistTriangle3Triangle3)
