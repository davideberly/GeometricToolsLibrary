// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Algebra/Quaternion.h>
#include <GTL/Mathematics/Algebra/Vector.h>
using namespace gtl;

namespace gtl
{
    class UnitTestQuaternion
    {
    public:
        UnitTestQuaternion();

    private:
        void ClassMembers();
        void Operations();
        void TestSlerp();
    };
}

UnitTestQuaternion::UnitTestQuaternion()
{
    UTInformation("Mathematics/Algebra/Quaternion");

    ClassMembers();
    Operations();
    TestSlerp();
}

void UnitTestQuaternion::ClassMembers()
{
    Quaternion<float> qdef;
    UTAssert(qdef[0] == 0.0f && qdef[1] == 0.0f && qdef[2] == 0.0f && qdef[3] == 0.0f,
        "Default constructor failed");

    Vector<float, 4> v{ 1.0f, 2.0f, 3.0f, 4.0f };
    Normalize(v);

    Quaternion<float> q(v[0], v[1], v[2], v[3]);
    float const epsilon = 1e-06f;
    float diff[4];
    diff[0] = std::fabs(q[0] - 0.182574183f);
    diff[1] = std::fabs(q[1] - 0.365148365f);
    diff[2] = std::fabs(q[2] - 0.547722518f);
    diff[3] = std::fabs(q[3] - 0.730296731f);
    UTAssert(diff[0] <= epsilon && diff[1] <= epsilon && diff[2] <= epsilon
        && diff[3] <= epsilon, "Element constructor failed");

    Quaternion<float> zero = Quaternion<float>::Zero();
    UTAssert(zero[0] == 0.0f && zero[1] == 0.0f && zero[2] == 0.0f && zero[3] == 0.0f,
        "static Zero() failed");

    Quaternion<float> I = Quaternion<float>::I();
    UTAssert(I[0] == 1.0f && I[1] == 0.0f && I[2] == 0.0f && I[3] == 0.0f,
        "static I() failed");

    Quaternion<float> J = Quaternion<float>::J();
    UTAssert(J[0] == 0.0f && J[1] == 1.0f && J[2] == 0.0f && J[3] == 0.0f,
        "static J() failed");

    Quaternion<float> K = Quaternion<float>::K();
    UTAssert(K[0] == 0.0f && K[1] == 0.0f && K[2] == 1.0f && K[3] == 0.0f,
        "static K() failed");

    Quaternion<float> one = Quaternion<float>::Identity();
    UTAssert(one[0] == 0.0f && one[1] == 0.0f && one[2] == 0.0f && one[3] == 1.0f,
        "static Identity() failed");
}

void UnitTestQuaternion::Operations()
{
    Quaternion<float> I = Quaternion<float>::I();
    Quaternion<float> J = Quaternion<float>::J();
    Quaternion<float> K = Quaternion<float>::K();
    Quaternion<float> one = Quaternion<float>::Identity();
    Quaternion<float> result;

    result = one * one;
    UTAssert(result == one, "operator* failed");
    result = one * I;
    UTAssert(result == I, "operator* failed");
    result = one * J;
    UTAssert(result == J, "operator* failed");
    result = one * K;
    UTAssert(result == K, "operator* failed");

    result = I * one;
    UTAssert(result == I, "operator* failed");
    result = I * I;
    UTAssert(result == -one, "operator* failed");
    result = I * J;
    UTAssert(result == K, "operator* failed");
    result = I * K;
    UTAssert(result == -J, "operator* failed");

    result = J * one;
    UTAssert(result == J, "operator* failed");
    result = J * I;
    UTAssert(result == -K, "operator* failed");
    result = J * J;
    UTAssert(result == -one, "operator* failed");
    result = J * K;
    UTAssert(result == I, "operator* failed");

    result = K * one;
    UTAssert(result == K, "operator* failed");
    result = K * I;
    UTAssert(result == J, "operator* failed");
    result = K * J;
    UTAssert(result == -I, "operator* failed");
    result = K * K;
    UTAssert(result == -one, "operator* failed");

    Quaternion<float> invQ = Inverse(Quaternion<float>::Zero());
    UTAssert(invQ == Quaternion<float>::Zero(), "Inverse failed");

    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    Normalize(q);
    float const epsilon = 1e-06f;
    float diff[4];
    invQ = Inverse(q);
    Quaternion<float> p = invQ * q - Quaternion<float>::Identity();
    for (std::size_t i = 0; i < 4; ++i)
    {
        diff[i] = std::fabs(p[i]);
    }
    UTAssert(p[0] <= epsilon && p[1] <= epsilon && p[2] <= epsilon
        && p[3] <= epsilon, "Inverse failed");

    Quaternion<float> conjQ = Conjugate(q);
    UTAssert(conjQ[0] == -q[0] && conjQ[1] == -q[1] && conjQ[2] == -q[2]
        && conjQ[3] == q[3], "Conjugate failed");

    float angle = C_PI_DIV_4<float>;
    float sn = std::sin(angle), cs = std::cos(angle);
    float x = 1.0f, y = 2.0f, z = 3.0f;
    float length = std::sqrt(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;
    q[0] = angle * x;
    q[1] = angle * y;
    q[2] = angle * z;
    q[3] = 0.0f;
    Quaternion<float> expQ = Exp(q);
    diff[0] = std::fabs(expQ[0] - sn * x);
    diff[1] = std::fabs(expQ[1] - sn * y);
    diff[2] = std::fabs(expQ[2] - sn * z);
    diff[3] = std::fabs(expQ[3] - cs);
    UTAssert(diff[0] <= epsilon && diff[1] <= epsilon && diff[2] <= epsilon
        && diff[3] <= epsilon, "Exp failed");

    q[0] = sn * x;
    q[1] = sn * y;
    q[2] = sn * z;
    q[3] = cs;
    Quaternion<float> logQ = Log(q);
    diff[0] = std::fabs(logQ[0] - angle * x);
    diff[1] = std::fabs(logQ[1] - angle * y);
    diff[2] = std::fabs(logQ[2] - angle * z);
    diff[3] = 0.0f;
    UTAssert(diff[0] <= epsilon && diff[1] <= epsilon && diff[2] <= epsilon
        && diff[3] <= epsilon, "Log failed");

    float twoX = 2.0f * q[0];
    float twoY = 2.0f * q[1];
    float twoZ = 2.0f * q[2];
    float twoXX = twoX * q[0];
    float twoXY = twoX * q[1];
    float twoXZ = twoX * q[2];
    float twoXW = twoX * q[3];
    float twoYY = twoY * q[1];
    float twoYZ = twoY * q[2];
    float twoYW = twoY * q[3];
    float twoZZ = twoZ * q[2];
    float twoZW = twoZ * q[3];
    float r00 = 1.0f - twoYY - twoZZ;
    float r01 = twoXY - twoZW;
    float r02 = twoXZ + twoYW;
    float r10 = twoXY + twoZW;
    float r11 = 1.0f - twoXX - twoZZ;
    float r12 = twoYZ - twoXW;
    float r20 = twoXZ - twoYW;
    float r21 = twoYZ + twoXW;
    float r22 = 1.0f - twoXX - twoYY;
    Vector<float, 3> v = { 3.0f, 2.0f, 1.0f };
    Vector<float, 3> uq = Rotate(q, v);
    Vector<float, 3> um;
    um[0] = r00 * v[0] + r01 * v[1] + r02 * v[2];
    um[1] = r10 * v[0] + r11 * v[1] + r12 * v[2];
    um[2] = r20 * v[0] + r21 * v[1] + r22 * v[2];
    for (std::size_t i = 0; i < 3; ++i)
    {
        diff[i] = std::fabs(uq[i] - um[i]);
    }
    UTAssert(diff[0] <= epsilon && diff[1] <= epsilon && diff[2] <= epsilon,
        "Rotate failed");
}

void UnitTestQuaternion::TestSlerp()
{
    Quaternion<float> q0(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion<float> q1(1.0f, 1.0f, 1.0f, 1.0f);

    Normalize(q0);
    Normalize(q1);

    float const epsilon0 = 1e-06f;
    float const epsilon1 = 1e-05f;

    // Compute slerp without any restrictions on the angle.
    float t = 0.25f;
    Quaternion<float> qslerp = Slerp(t, q0, q1);
    // qslerp = (0.267249286, 0.405973792, 0.544698238, 0.683422804)
    float diff[4];
    diff[0] = std::fabs(qslerp[0] - 0.267249286f);
    diff[1] = std::fabs(qslerp[1] - 0.405973792f);
    diff[2] = std::fabs(qslerp[2] - 0.544698238f);
    diff[3] = std::fabs(qslerp[3] - 0.683422804f);
    UTAssert(diff[0] <= epsilon0 && diff[1] <= epsilon0 && diff[2] <= epsilon0
        && diff[3] <= epsilon0, "Slerp failed");

    Quaternion<float> negQ1 = -q1;
    qslerp = Slerp(t, q0, negQ1);
    diff[0] = std::fabs(qslerp[0] - 0.267249286f);
    diff[1] = std::fabs(qslerp[1] - 0.405973792f);
    diff[2] = std::fabs(qslerp[2] - 0.544698238f);
    diff[3] = std::fabs(qslerp[3] - 0.683422804f);
    UTAssert(diff[0] <= epsilon0 && diff[1] <= epsilon0 && diff[2] <= epsilon0
        && diff[3] <= epsilon0, "Slerp failed");

    // The quaternions q0, q1 and qslerp must lie in the same hyperplane
    // because they are all on the same great hypercircle.
    qslerp = Slerp(t, q0, q1);
    Vector<float, 4> v0 = { q0[0], q0[1], q0[2], q0[3] };
    Vector<float, 4> v1 = { q1[0], q1[1], q1[2], q1[3] };
    Vector<float, 4> v2 = { qslerp[0], qslerp[1], qslerp[2], qslerp[3] };
    Vector<float, 4> hcross = HyperCross(v0, v1, v2);
    // hcross = (-3.72529030e-09, 7.45058060e-09, 7.45058060e-09, -3.72529030e-09)
    for (std::size_t i = 0; i < 4; ++i)
    {
        diff[i] = std::fabs(hcross[i]);
    }
    UTAssert(diff[0] <= epsilon0 && diff[1] <= epsilon0 && diff[2] <= epsilon0
        && diff[3] <= epsilon0, "HyperCross unexpected, Slerp failed");

    // Let A be the acute angle between q0 and q1.  The angle between q0 and
    // qslerp must be t*A and the angle between qslerp and q1 must be (1-t)*A.
    float angle01 = std::acos(Dot(v0, v1));  // 0.420534313
    float angle02 = std::acos(Dot(v0, v2));  // 0.105135001
    float angle12 = std::acos(Dot(v1, v2));  // 0.315401107
    float result0 = angle02 + angle12 - angle01;  // 1.78813934e-06
    float result1 = angle02 - t * angle01;  // 1.42306089e-06
    diff[0] = std::fabs(result0);
    diff[1] = std::fabs(result1);
    UTAssert(diff[0] <= epsilon1 && diff[1] <= epsilon1,
        "Angles unexpected, Slerp failed");

    Quaternion<float> qslerpR = SlerpR(t, q0, q1);
    UTAssert(qslerpR == qslerp, "SlerpR failed");

    float cosA = Dot(q0, q1);
    Quaternion<float> qslerpRP = SlerpRP(t, q0, q1, cosA);
    UTAssert(qslerpRP == qslerp, "SlerpRP failed");

    float cosAH = std::sqrt(0.5f * (1.0f + cosA));
    Quaternion<float> qh = (q0 + q1) / (2.0f * cosAH);
    Quaternion<float> qslerpRPH = SlerpRPH(t, q0, q1, qh, cosAH);
    qslerp = Slerp(t, q0, q1);
    for (std::size_t i = 0; i < 4; ++i)
    {
        diff[i] = std::fabs(qslerp[i] - qslerpRPH[i]);
    }
    UTAssert(diff[0] <= epsilon0 && diff[1] <= epsilon0 && diff[2] <= epsilon0
        && diff[3] <= epsilon0, "SlerpRPH failed");

    t = 0.75f;
    qslerp = Slerp(t, q0, q1);
    qslerpRP = SlerpRP(t, q0, q1, cosA);
    qslerpRPH = SlerpRPH(t, q0, q1, qh, cosAH);
    for (std::size_t i = 0; i < 4; ++i)
    {
        diff[i] = std::fabs(qslerp[i] - qslerpRPH[i]);
    }
    UTAssert(diff[0] <= epsilon0 && diff[1] <= epsilon0 && diff[2] <= epsilon0
        && diff[3] <= epsilon0, "SlerpRPH failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Algebra/Quaternion.h>

namespace gtl
{
    template class Quaternion<float>;
    using QF = Quaternion<float>;
    using VF3 = Vector<float, 3>;
    using VF4 = Vector<float, 4>;

    template bool operator==(QF const&, QF const&);
    template bool operator!=(QF const&, QF const&);
    template bool operator<(QF const&, QF const&);
    template bool operator<=(QF const&, QF const&);
    template bool operator>(QF const&, QF const&);
    template bool operator>=(QF const&, QF const&);
    template QF operator+(QF const&);
    template QF operator-(QF const&);
    template QF operator+(QF const&, QF const&);
    template QF operator-(QF const&, QF const&);
    template QF operator*(QF const&, float const&);
    template QF operator*(float const&, QF const&);
    template QF operator/(QF const&, float const&);
    template QF& operator+=(QF&, QF const&);
    template QF& operator-=(QF&, QF const&);
    template QF& operator*=(QF&, float const&);
    template QF& operator/=(QF&, float const&);
    template float Dot(QF const&, QF const&);
    template float Length(QF const&);
    template float Normalize(QF&);
    template QF operator*(QF const&, QF const&);
    QF Inverse(QF const&);
    QF Conjugate(QF const&);
    QF Exp(QF const&);
    QF Log(QF const&);
    VF3 Rotate(QF const&, VF3 const&);
    VF4 Rotate(QF const&, VF4 const&);
    QF Slerp(float const&, QF const&, QF const&);
    QF SlerpR(float const&, QF const&, QF const&);
    QF SlerpRP(float const&, QF const&, QF const&, float const&);
    QF SlerpRPH(float const&, QF const&, QF const&, QF const&, float const&);

    template class Quaternion<double>;
    using QD = Quaternion<double>;
    using VD3 = Vector<double, 3>;
    using VD4 = Vector<double, 4>;
    template bool operator==(QD const&, QD const&);
    template bool operator!=(QD const&, QD const&);
    template bool operator<(QD const&, QD const&);
    template bool operator<=(QD const&, QD const&);
    template bool operator>(QD const&, QD const&);
    template bool operator>=(QD const&, QD const&);
    template QD operator+(QD const&);
    template QD operator-(QD const&);
    template QD operator+(QD const&, QD const&);
    template QD operator-(QD const&, QD const&);
    template QD operator*(QD const&, double const&);
    template QD operator*(double const&, QD const&);
    template QD operator/(QD const&, double const&);
    template QD& operator+=(QD&, QD const&);
    template QD& operator-=(QD&, QD const&);
    template QD& operator*=(QD&, double const&);
    template QD& operator/=(QD&, double const&);
    template double Dot(QD const&, QD const&);
    template double Length(QD const&);
    template double Normalize(QD&);
    template QD operator*(QD const&, QD const&);
    QD Inverse(QD const&);
    QD Conjugate(QD const&);
    QD Exp(QD const&);
    QD Log(QD const&);
    VD3 Rotate(QD const&, VD3 const&);
    VD4 Rotate(QD const&, VD4 const&);
    QD Slerp(double const&, QD const&, QD const&);
    QD SlerpR(double const&, QD const&, QD const&);
    QD SlerpRP(double const&, QD const&, QD const&, double const&);
    QD SlerpRPH(double const&, QD const&, QD const&, QD const&, double const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    using QR = Quaternion<Rational>;
    using VR3 = Vector<Rational, 3>;
    using VR4 = Vector<Rational, 4>;
    template bool operator==(QR const&, QR const&);
    template bool operator!=(QR const&, QR const&);
    template bool operator<(QR const&, QR const&);
    template bool operator<=(QR const&, QR const&);
    template bool operator>(QR const&, QR const&);
    template bool operator>=(QR const&, QR const&);
    template QR operator+(QR const&);
    template QR operator-(QR const&);
    template QR operator+(QR const&, QR const&);
    template QR operator-(QR const&, QR const&);
    template QR operator*(QR const&, Rational const&);
    template QR operator*(Rational const&, QR const&);
    template QR operator/(QR const&, Rational const&);
    template QR& operator+=(QR&, QR const&);
    template QR& operator-=(QR&, QR const&);
    template QR& operator*=(QR&, Rational const&);
    template QR& operator/=(QR&, Rational const&);
    template Rational Dot(QR const&, QR const&);
    template Rational Length(QR const&);
    template Rational Normalize(QR&);
    template QR operator*(QR const&, QR const&);
    QR Inverse(QR const&);
    QR Conjugate(QR const&);
    QR Exp(QR const&);
    QR Log(QR const&);
    VR3 Rotate(QR const&, VR3 const&);
    VR3 Rotate(VR3 const&, QR const&);
    VR4 Rotate(QR const&, VR4 const&);
    VR4 Rotate(VR4 const&, QR const&);
    QR Slerp(Rational const&, QR const&, QR const&);
    QR SlerpR(Rational const&, QR const&, QR const&);
    QR SlerpRP(Rational const&, QR const&, QR const&, Rational const&);
    QR SlerpRPH(Rational const&, QR const&, QR const&, QR const&, Rational const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Quaternion)
