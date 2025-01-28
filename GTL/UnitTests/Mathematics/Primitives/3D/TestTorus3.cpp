#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/3D/Torus3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestTorus3
    {
    public:
        UnitTestTorus3();

    private:
        void Test();
    };
}

UnitTestTorus3::UnitTestTorus3()
{
    UTInformation("Mathematics/Primitives/3D/Torus3");

    Test();
}

void UnitTestTorus3::Test()
{
    double maxError = 1e-14;
    Vector3<double> center{ 0.0, 0.0, 0.0 };
    Vector3<double> direction0{ 1.0, 0.0, 0.0 };
    Vector3<double> direction1{ 0.0, 1.0, 0.0 };
    Vector3<double> normal{ 0.0, 0.0, 1.0 };
    double radius0 = 2.0;
    double radius1 = 1.0;

    double u = 0.01234;
    double v = 0.56789;
    std::array<Vector3<double>, 6> jet{};
    bool success{};

    Torus3<double> torus(center, direction0, direction1, normal, radius0, radius1);
    UTAssert(Length(torus.center - center) <= maxError, "Incorrect center.");
    UTAssert(Length(torus.direction0 - direction0) <= maxError, "Incorrect direction0.");
    UTAssert(Length(torus.direction1 - direction1) <= maxError, "Incorrect direction1.");
    UTAssert(Length(torus.normal - normal) <= maxError, "Incorrect normal.");
    UTAssert(std::fabs(torus.radius0 - radius0) <= maxError, "Incorrect radius0.");
    UTAssert(std::fabs(torus.radius1 - radius1) <= maxError, "Incorrect radius1.");

    success = torus.Evaluate(u, v, 2, jet);
    UTAssert(success, "The maximum order was exceeded.");

    // The torus is F(u,v) = 0 where
    // F(u,v) = [|P-C|^2 + r0^2 - r1^2]^2 - 4*r0^2*[|P-C|^2 - (Dot(N,P-C))^2]
    // for all (u,v) in the domain. Verify that F(u,v) = 0, dF(u,v)/du = 0,
    // dF(u,v)/dv = 0, d2F(u,v)/du^2 = 0, d2F(u,v)/dudv = 0 and
    // d2F(u,v)/dv^2 = 0.
    Vector3<double> delta = jet[0] - torus.center;  // P-C
    double r0Sqr = torus.radius0 * torus.radius0;   // r0^2
    double r1Sqr = torus.radius1 * torus.radius1;   // r1^2
    double sqrLenDelta = Dot(delta, delta);         // |P-C|^2
    double t0 = sqrLenDelta + r0Sqr - r1Sqr;        // |P-C|^2 + r0^2 - r1^2
    double t1 = Dot(torus.normal, delta);           // Dot(N,P-C)
    double t2 = sqrLenDelta - t1 * t1;              // |P-C|^2 - Dot(N,P-C)^2
    double F = t0 * t0 - 4.0 * r0Sqr * t2;
    UTAssert(std::fabs(F) <= maxError, "F is not close enough to zero.");

    double t3 = sqrLenDelta - r0Sqr - r1Sqr;    // |P-C|^2 - r0^2 - r1^2
    double t4 = Dot(delta, jet[1]);             // Dot(P-C,dP/du)
    double t5 = Dot(torus.normal, jet[1]);      // Dot(N,dP/du)
    double dFdu = t3 * t4 + 2.0 * r0Sqr * t1 * t5;
    UTAssert(std::fabs(dFdu) <= maxError, "dFdu is not close enough to zero.");

    double t6 = Dot(delta, jet[2]);             // Dot(P-C,dP/dv)
    double t7 = Dot(torus.normal, jet[2]);      // Dot(N,dP/dv)
    double dFdv = t3 * t6 + 2.0 * r0Sqr * t1 * t7;
    UTAssert(std::fabs(dFdv) <= maxError, "dFdv is not close enough to zero.");

    double t8 = Dot(delta, jet[3]);             // Dot(P-C,d2P/du^2)
    double t9 = Dot(jet[1], jet[1]);            // Dot(dP/du,dP/du)
    double t10 = Dot(torus.normal, jet[3]);     // Dot(N,d2P/du^2)
    double d2Fdu2 = t3 * (t8 + t9) + 2.0 * (t4 * t4 + r0Sqr * (t1 * t10 + t5 * t5));
    UTAssert(std::fabs(d2Fdu2) <= maxError, "d2Fdu2 is not close enough to zero.");

    double t12 = Dot(delta, jet[5]);            // Dot(P-C,d2P/dv^2)
    double t13 = Dot(jet[2], jet[2]);           // Dot(dP/dv,dP/dv)
    double t14 = Dot(torus.normal, jet[5]);     // Dot(N,d2P/dv^2)
    double d2Fdv2 = t3 * (t12 + t13) + 2.0 * (t6 * t6 + r0Sqr * (t1 * t14 + t7 * t7));
    UTAssert(std::fabs(d2Fdv2) <= maxError, "d2Fdv2 is not close enough to zero.");

    double t15 = Dot(delta, jet[4]);            // Dot(P-C,d2P/dudv)
    double t16 = Dot(jet[1], jet[2]);           // Dot(dP/du,dP/dv)
    double t17 = Dot(torus.normal, jet[4]);     // Dot(N,d2P/dv^2)
    double d2Fdudv = t3 * (t15 + t16) + 2.0 * (t4 * t6 + r0Sqr * (t1 * t17 + t5 * t7));
    UTAssert(std::fabs(d2Fdudv) <= maxError, "d2Fdudv is not close enough to zero.");

    center = { 0.1, 0.2, 0.3 };
    Quaternion<double> q{ 1.0, 2.0, 3.0, 4.0 };
    Normalize(q);
    Matrix3x3<double> rotate{};
    RigidMotion<double>::Convert(q, rotate);
    direction0 = rotate.GetCol(0);
    direction1 = rotate.GetCol(1);
    normal = rotate.GetCol(2);

    torus = Torus3<double>(center, direction0, direction1, normal, radius0, radius1);
    UTAssert(Length(torus.center - center) <= maxError, "Incorrect center.");
    UTAssert(Length(torus.direction0 - direction0) <= maxError, "Incorrect direction0.");
    UTAssert(Length(torus.direction1 - direction1) <= maxError, "Incorrect direction1.");
    UTAssert(Length(torus.normal - normal) <= maxError, "Incorrect normal.");
    UTAssert(std::fabs(torus.radius0 - radius0) <= maxError, "Incorrect radius0.");
    UTAssert(std::fabs(torus.radius1 - radius1) <= maxError, "Incorrect radius1.");

    success = torus.Evaluate(u, v, 2, jet);
    UTAssert(success, "The maximum order was exceeded.");

    delta = jet[0] - torus.center;  // P-C
    r0Sqr = torus.radius0 * torus.radius0;   // r0^2
    r1Sqr = torus.radius1 * torus.radius1;   // r1^2
    sqrLenDelta = Dot(delta, delta);         // |P-C|^2
    t0 = sqrLenDelta + r0Sqr - r1Sqr;        // |P-C|^2 + r0^2 - r1^2
    t1 = Dot(torus.normal, delta);           // Dot(N,P-C)
    t2 = sqrLenDelta - t1 * t1;              // |P-C|^2 - Dot(N,P-C)^2
    F = t0 * t0 - 4.0 * r0Sqr * t2;
    UTAssert(std::fabs(F) <= maxError, "F is not close enough to zero.");

    t3 = sqrLenDelta - r0Sqr - r1Sqr;    // |P-C|^2 - r0^2 - r1^2
    t4 = Dot(delta, jet[1]);             // Dot(P-C,dP/du)
    t5 = Dot(torus.normal, jet[1]);      // Dot(N,dP/du)
    dFdu = t3 * t4 + 2.0 * r0Sqr * t1 * t5;
    UTAssert(std::fabs(dFdu) <= maxError, "dFdu is not close enough to zero.");

    t6 = Dot(delta, jet[2]);             // Dot(P-C,dP/dv)
    t7 = Dot(torus.normal, jet[2]);      // Dot(N,dP/dv)
    dFdv = t3 * t6 + 2.0 * r0Sqr * t1 * t7;
    UTAssert(std::fabs(dFdv) <= maxError, "dFdv is not close enough to zero.");

    t8 = Dot(delta, jet[3]);             // Dot(P-C,d2P/du^2)
    t9 = Dot(jet[1], jet[1]);            // Dot(dP/du,dP/du)
    t10 = Dot(torus.normal, jet[3]);     // Dot(N,d2P/du^2)
    d2Fdu2 = t3 * (t8 + t9) + 2.0 * (t4 * t4 + r0Sqr * (t1 * t10 + t5 * t5));
    UTAssert(std::fabs(d2Fdu2) <= maxError, "d2Fdu2 is not close enough to zero.");

    t12 = Dot(delta, jet[5]);            // Dot(P-C,d2P/dv^2)
    t13 = Dot(jet[2], jet[2]);           // Dot(dP/dv,dP/dv)
    t14 = Dot(torus.normal, jet[5]);     // Dot(N,d2P/dv^2)
    d2Fdv2 = t3 * (t12 + t13) + 2.0 * (t6 * t6 + r0Sqr * (t1 * t14 + t7 * t7));
    UTAssert(std::fabs(d2Fdv2) <= maxError, "d2Fdv2 is not close enough to zero.");

    t15 = Dot(delta, jet[4]);            // Dot(P-C,d2P/dudv)
    t16 = Dot(jet[1], jet[2]);           // Dot(dP/du,dP/dv)
    t17 = Dot(torus.normal, jet[4]);     // Dot(N,d2P/dv^2)
    d2Fdudv = t3 * (t15 + t16) + 2.0 * (t4 * t6 + r0Sqr * (t1 * t17 + t5 * t7));
    UTAssert(std::fabs(d2Fdudv) <= maxError, "d2Fdudv is not close enough to zero.");

    double uRecover = 0.0, vRecover = 0.0;
    torus.GetParameters(jet[0], uRecover, vRecover);
    UTAssert(std::fabs(u - uRecover) <= maxError, "Invalid recovered u.");
    UTAssert(std::fabs(v - vRecover) <= maxError, "Invalid recovered v.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/3D/Torus3.h>

namespace gtl
{
    template class Torus3<float>;
    template bool operator==(Torus3<float> const&, Torus3<float> const&);
    template bool operator!=(Torus3<float> const&, Torus3<float> const&);
    template bool operator< (Torus3<float> const&, Torus3<float> const&);
    template bool operator<=(Torus3<float> const&, Torus3<float> const&);
    template bool operator> (Torus3<float> const&, Torus3<float> const&);
    template bool operator>=(Torus3<float> const&, Torus3<float> const&);

    template class Torus3<double>;
    template bool operator==(Torus3<double> const&, Torus3<double> const&);
    template bool operator!=(Torus3<double> const&, Torus3<double> const&);
    template bool operator< (Torus3<double> const&, Torus3<double> const&);
    template bool operator<=(Torus3<double> const&, Torus3<double> const&);
    template bool operator> (Torus3<double> const&, Torus3<double> const&);
    template bool operator>=(Torus3<double> const&, Torus3<double> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Torus3<Rational>;
    template bool operator==(Torus3<Rational> const&, Torus3<Rational> const&);
    template bool operator!=(Torus3<Rational> const&, Torus3<Rational> const&);
    template bool operator< (Torus3<Rational> const&, Torus3<Rational> const&);
    template bool operator<=(Torus3<Rational> const&, Torus3<Rational> const&);
    template bool operator> (Torus3<Rational> const&, Torus3<Rational> const&);
    template bool operator>=(Torus3<Rational> const&, Torus3<Rational> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Torus3)
