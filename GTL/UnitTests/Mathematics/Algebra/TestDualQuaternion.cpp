// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Algebra/DualQuaternion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDualQuaternion
    {
    public:
        UnitTestDualQuaternion();

    private:
        void ClassMembers();

        void DoComparisons(DualQuaternion<float> const& d0,
            DualQuaternion<float> const& d1, bool equal, bool lessThan);

        void Comparisons();
        void LinearAlgebraic();
        void Functions();
        void DoRigidTransform();
    };
}

UnitTestDualQuaternion::UnitTestDualQuaternion()
{
    UTInformation("Mathematics/Algebra/DualQuaternion");

    ClassMembers();
    Comparisons();
    LinearAlgebraic();
    Functions();
    DoRigidTransform();
}

void UnitTestDualQuaternion::ClassMembers()
{
    DualQuaternion<float> dqidentity;
    UTAssert(dqidentity[0] == Quaternion<float>::Identity()
        && dqidentity[1] == Quaternion<float>::Zero(),
        "Default constructor failed");

    Quaternion<float> p{ 1.0f, 2.0f, 3.0f, 4.0f };
    Quaternion<float> q{ 1.0f, 1.0f, 1.0f, 1.0f };
    Normalize(p);
    Normalize(q);
    DualQuaternion<float> dqinit(p, q);
    UTAssert(dqinit[0] == p && dqinit[1] == q,
        "Initialize constructor failed");

    DualQuaternion<float> dqzero = DualQuaternion<float>::Zero();
    UTAssert(dqzero[0] == Quaternion<float>::Zero()
        && dqzero[1] == Quaternion<float>::Zero(),
        "static Zero() failed");

    DualQuaternion<float> dq = DualQuaternion<float>::Identity();
    UTAssert(dq == dqidentity, "static Identity() failed");
}
void UnitTestDualQuaternion::DoComparisons(DualQuaternion<float> const& d0,
    DualQuaternion<float> const& d1, bool equal, bool lessThan)
{
    bool result = (d0 == d1);
    UTAssert(result == equal, "Incorrect operator==");
    result = (d0 < d1);
    UTAssert(result == lessThan, "Incorrect operator<");
    UTAssert((d0 != d1) == !equal, "Incorrect operator!=");
    UTAssert((d0 <= d1) == equal || lessThan, "Incorrect operator<=");
    UTAssert((d0 > d1) == (!equal && !lessThan), "Incorrect operator>");
    UTAssert((d0 >= d1) == !lessThan, "Incorrect operator>=");
}

void UnitTestDualQuaternion::Comparisons()
{
    Quaternion<float> q0 = { 1.0f, 2.0f, 3.0f, 4.0f };
    Quaternion<float> q1 = { 5.0f, 6.0f, 7.0f, 8.0f };
    Quaternion<float> q2 = { -1.0f, 1.0f, 1.0f, -1.0f };
    Quaternion<float> q3 = { 1.0f, 0.0f, 1.0f, 0.0f };
    Normalize(q0);
    Normalize(q1);
    Normalize(q2);
    Normalize(q3);
    DualQuaternion<float> d0(q0, q2), d1(q1, q3);

    DoComparisons(d0, d0, true, false);
    DoComparisons(d0, d1, false, true);
    DoComparisons(d1, d0, false, false);
}

void UnitTestDualQuaternion::LinearAlgebraic()
{
    Quaternion<float> q0 = { 1.0f, 2.0f, 3.0f, 4.0f };
    Quaternion<float> q1 = { 5.0f, 6.0f, 7.0f, 8.0f };
    Quaternion<float> q2 = { -1.0f, 1.0f, 1.0f, -1.0f };
    Quaternion<float> q3 = { 1.0f, 0.0f, 1.0f, 0.0f };
    Normalize(q0);
    Normalize(q1);
    Normalize(q2);
    Normalize(q3);
    DualQuaternion<float> d0(q0, q2), d1(q1, q3), d2;

    d2 = +d0;
    UTAssert(d2 == d0, "unary operator+ failed");
    d2 = -d0;
    UTAssert(d2[0] == -d0[0] && d2[1] == -d0[1], "unary operator- failed");

    float const epsilon = 1e-06f;
    std::array<float, 8> diff{};
    d2 = d0 + d1;
    diff[0] = std::fabs(d2[0][0] - 0.561623216f);
    diff[1] = std::fabs(d2[0][1] - 0.820007205f);
    diff[2] = std::fabs(d2[0][2] - 1.07839108f);
    diff[3] = std::fabs(d2[0][3] - 1.33677518f);
    diff[4] = std::fabs(d2[1][0] - 0.207106769f);
    diff[5] = std::fabs(d2[1][1] - 0.500000000f);
    diff[6] = std::fabs(d2[1][2] - 1.20710683f);
    diff[7] = std::fabs(d2[1][3] + 0.500000000f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "operator+(d0,d1) failed");
    }

    d2 = d0 - d1;
    diff[0] = std::fabs(d2[0][0] + 0.196474850f);
    diff[1] = std::fabs(d2[0][1] + 0.0897104740f);
    diff[2] = std::fabs(d2[0][2] - 0.0170539021f);
    diff[3] = std::fabs(d2[0][3] - 0.123818278f);
    diff[4] = std::fabs(d2[1][0] + 1.20710683f);
    diff[5] = std::fabs(d2[1][1] - 0.500000000f);
    diff[6] = std::fabs(d2[1][2] + 0.207106769f);
    diff[7] = std::fabs(d2[1][3] + 0.500000000f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "operator-(d0,d1) failed");
    }

    d2 = d0 * 2.0f;
    diff[0] = std::fabs(d2[0][0] - 0.365148365f);
    diff[1] = std::fabs(d2[0][1] - 0.730296731f);
    diff[2] = std::fabs(d2[0][2] - 1.09544504f);
    diff[3] = std::fabs(d2[0][3] - 1.46059346f);
    diff[4] = std::fabs(d2[1][0] + 1.00000000f);
    diff[5] = std::fabs(d2[1][1] - 1.00000000f);
    diff[6] = std::fabs(d2[1][2] - 1.00000000f);
    diff[7] = std::fabs(d2[1][3] + 1.00000000f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "operator*(d,s) failed");
    }

    d2 = 2.0f * d0;
    diff[0] = std::fabs(d2[0][0] - 0.365148365f);
    diff[1] = std::fabs(d2[0][1] - 0.730296731f);
    diff[2] = std::fabs(d2[0][2] - 1.09544504f);
    diff[3] = std::fabs(d2[0][3] - 1.46059346f);
    diff[4] = std::fabs(d2[1][0] + 1.00000000f);
    diff[5] = std::fabs(d2[1][1] - 1.00000000f);
    diff[6] = std::fabs(d2[1][2] - 1.00000000f);
    diff[7] = std::fabs(d2[1][3] + 1.00000000f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "operator*(s,d) failed");
    }

    d2 = d0 / 2.0f;
    diff[0] = std::fabs(d2[0][0] - 0.0912870914f);
    diff[1] = std::fabs(d2[0][1] - 0.182574183f);
    diff[2] = std::fabs(d2[0][2] - 0.273861259f);
    diff[3] = std::fabs(d2[0][3] - 0.365148365f);
    diff[4] = std::fabs(d2[1][0] + 0.25000000f);
    diff[5] = std::fabs(d2[1][1] - 0.25000000f);
    diff[6] = std::fabs(d2[1][2] - 0.25000000f);
    diff[7] = std::fabs(d2[1][3] + 0.25000000f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "operator/(d,s) failed");
    }

    d2 = d0;
    d2 += d1;
    diff[0] = std::fabs(d2[0][0] - 0.561623216f);
    diff[1] = std::fabs(d2[0][1] - 0.820007205f);
    diff[2] = std::fabs(d2[0][2] - 1.07839108f);
    diff[3] = std::fabs(d2[0][3] - 1.33677518f);
    diff[4] = std::fabs(d2[1][0] - 0.207106769f);
    diff[5] = std::fabs(d2[1][1] - 0.500000000f);
    diff[6] = std::fabs(d2[1][2] - 1.20710683f);
    diff[7] = std::fabs(d2[1][3] + 0.500000000f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "operator+=(d0,d1) failed");
    }

    d2 = d0;
    d2 -= d1;
    diff[0] = std::fabs(d2[0][0] + 0.196474850f);
    diff[1] = std::fabs(d2[0][1] + 0.0897104740f);
    diff[2] = std::fabs(d2[0][2] - 0.0170539021f);
    diff[3] = std::fabs(d2[0][3] - 0.123818278f);
    diff[4] = std::fabs(d2[1][0] + 1.20710683f);
    diff[5] = std::fabs(d2[1][1] - 0.500000000f);
    diff[6] = std::fabs(d2[1][2] + 0.207106769f);
    diff[7] = std::fabs(d2[1][3] + 0.500000000f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "operator-=(d0,d1) failed");
    }

    d2 = d0;
    d2 *= 2.0f;
    diff[0] = std::fabs(d2[0][0] - 0.365148365f);
    diff[1] = std::fabs(d2[0][1] - 0.730296731f);
    diff[2] = std::fabs(d2[0][2] - 1.09544504f);
    diff[3] = std::fabs(d2[0][3] - 1.46059346f);
    diff[4] = std::fabs(d2[1][0] + 1.00000000f);
    diff[5] = std::fabs(d2[1][1] - 1.00000000f);
    diff[6] = std::fabs(d2[1][2] - 1.00000000f);
    diff[7] = std::fabs(d2[1][3] + 1.00000000f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "operator*=(d,s) failed");
    }

    d2 = d0;
    d2 /= 2.0f;
    diff[0] = std::fabs(d2[0][0] - 0.0912870914f);
    diff[1] = std::fabs(d2[0][1] - 0.182574183f);
    diff[2] = std::fabs(d2[0][2] - 0.273861259f);
    diff[3] = std::fabs(d2[0][3] - 0.365148365f);
    diff[4] = std::fabs(d2[1][0] + 0.25000000f);
    diff[5] = std::fabs(d2[1][1] - 0.25000000f);
    diff[6] = std::fabs(d2[1][2] - 0.25000000f);
    diff[7] = std::fabs(d2[1][3] + 0.25000000f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "operator/=(d,s) failed");
    }

    d2 = d0 * d1;
    diff[0] = std::fabs(d2[0][0] - 0.332181931f);
    diff[1] = std::fabs(d2[0][1] - 0.664363861f);
    diff[2] = std::fabs(d2[0][2] - 0.664363801f);
    diff[3] = std::fabs(d2[0][3] + 0.0830454826f);
    diff[4] = std::fabs(d2[1][0] - 0.319737852f);
    diff[5] = std::fabs(d2[1][1] - 0.788867474f);
    diff[6] = std::fabs(d2[1][2] + 0.120850116f);
    diff[7] = std::fabs(d2[1][3] + 1.12287617f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "operator*(d0,d1) failed");
    }
}

void UnitTestDualQuaternion::Functions()
{
    Quaternion<float> q0 = { 1.0f, 2.0f, 3.0f, 4.0f };
    Quaternion<float> q1 = { 5.0f, 6.0f, 7.0f, 8.0f };
    Quaternion<float> q2 = { -1.0f, 1.0f, 1.0f, -1.0f };
    Quaternion<float> q3 = { 1.0f, 0.0f, 1.0f, 0.0f };
    Normalize(q0);
    Normalize(q1);
    Normalize(q2);
    Normalize(q3);
    DualQuaternion<float> d0(q0, q2), d1(q1, q3), d2, p;

    d2 = Conjugate(d0);
    UTAssert(d2[0] == Conjugate(d0[0]) && d2[1] == Conjugate(d0[1]),
        "Conjugate failed");

    float const epsilon = 1e-06f;
    std::array<float, 8> diff{};
    d2 = Inverse(d0);
    p = d2 * d0;
    diff[0] = std::fabs(p[0][0]);
    diff[1] = std::fabs(p[0][1]);
    diff[2] = std::fabs(p[0][2]);
    diff[3] = std::fabs(p[0][3] - 1.0f);
    diff[4] = std::fabs(p[1][0]);
    diff[5] = std::fabs(p[1][1]);
    diff[6] = std::fabs(p[1][2]);
    diff[7] = std::fabs(p[1][3]);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "Inverse failed");
    }

    p = d0 * d2;
    diff[0] = std::fabs(p[0][0]);
    diff[1] = std::fabs(p[0][1]);
    diff[2] = std::fabs(p[0][2]);
    diff[3] = std::fabs(p[0][3] - 1.0f);
    diff[4] = std::fabs(p[1][0]);
    diff[5] = std::fabs(p[1][1]);
    diff[6] = std::fabs(p[1][2]);
    diff[7] = std::fabs(p[1][3]);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "Inverse failed");
    }

    d2 = Dot(d0, d1);
    diff[0] = std::fabs(d2[0][0] - 0.000000000f);
    diff[1] = std::fabs(d2[0][1] - 7.45058060e-09f);
    diff[2] = std::fabs(d2[0][2] - 0.000000000f);
    diff[3] = std::fabs(d2[0][3] - 0.968863964f);
    diff[4] = std::fabs(d2[1][0] - 0.000000000f);
    diff[5] = std::fabs(d2[1][1] - 0.000000000f);
    diff[6] = std::fabs(d2[1][2] - 0.000000000f);
    diff[7] = std::fabs(d2[1][3] - 0.516397715f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "Dot failed");
    }

    d2 = Cross(d0, d1);
    diff[0] = std::fabs(d2[0][0] + 0.0553636551f);
    diff[1] = std::fabs(d2[0][1] - 0.110727310f);
    diff[2] = std::fabs(d2[0][2] + 0.0553636551f);
    diff[3] = std::fabs(d2[0][3] - 0.000000000f);
    diff[4] = std::fabs(d2[1][0] - 0.296103776f);
    diff[5] = std::fabs(d2[1][1] - 0.713057637f);
    diff[6] = std::fabs(d2[1][2] + 0.675152779f);
    diff[7] = std::fabs(d2[1][3] - 0.000000000f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "Cross failed");
    }

    d2 = Norm(d0);
    diff[0] = std::fabs(d2[0][0]);
    diff[1] = std::fabs(d2[0][1]);
    diff[2] = std::fabs(d2[0][2]);
    diff[3] = std::fabs(d2[0][3] - 0.999999881f);
    diff[4] = std::fabs(d2[1][0]);
    diff[5] = std::fabs(d2[1][1]);
    diff[6] = std::fabs(d2[1][2]);
    diff[7] = std::fabs(d2[1][3]);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "Norm failed");
    }

    d2 = Norm(d1);
    diff[0] = std::fabs(d2[0][0]);
    diff[1] = std::fabs(d2[0][1]);
    diff[2] = std::fabs(d2[0][2]);
    diff[3] = std::fabs(d2[0][3] - 1.0f);
    diff[4] = std::fabs(d2[1][0]);
    diff[5] = std::fabs(d2[1][1]);
    diff[6] = std::fabs(d2[1][2]);
    diff[7] = std::fabs(d2[1][3] - 1.28653502f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "Norm failed");
    }
    
    d2 = Length(d0);
    diff[0] = std::fabs(d2[0][0]);
    diff[1] = std::fabs(d2[0][1]);
    diff[2] = std::fabs(d2[0][2]);
    diff[3] = std::fabs(d2[0][3] - 1.0f);
    diff[4] = std::fabs(d2[1][0]);
    diff[5] = std::fabs(d2[1][1]);
    diff[6] = std::fabs(d2[1][2]);
    diff[7] = std::fabs(d2[1][3]);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "Length failed");
    }

    d2 = Length(d1);
    diff[0] = std::fabs(d2[0][0]);
    diff[1] = std::fabs(d2[0][1]);
    diff[2] = std::fabs(d2[0][2]);
    diff[3] = std::fabs(d2[0][3] - 1.0f);
    diff[4] = std::fabs(d2[1][0]);
    diff[5] = std::fabs(d2[1][1]);
    diff[6] = std::fabs(d2[1][2]);
    diff[7] = std::fabs(d2[1][3] - 0.643267512f);
    for (std::size_t i = 0; i < 8; ++i)
    {
        UTAssert(diff[i] <= epsilon, "Length failed");
    }
}

void UnitTestDualQuaternion::DoRigidTransform()
{
    // Construct the quaternion for the rotation.
    Quaternion<float> r(1.0f, 2.0f, 3.0f, 4.0f);
    Normalize(r);

    // Construct the translation.
    Quaternion<float> t(0.1f, -0.2f, 0.3f, 0.0f);

    // Construct the dual quaternion that represents the rigid transform.
    DualQuaternion<float> d(r, 0.5f * t * r);

    // The input point.
    Vector<float, 3> x{ -3.0f, 5.0f, 7.0f };

    // The transformed point.
    Vector<float, 3> yDual = RotateAndTranslate(d, x);

    // Convert the quaternion to a rotation matrix.
    float twoX = 2.0f * r[0];
    float twoY = 2.0f * r[1];
    float twoZ = 2.0f * r[2];
    float twoXX = twoX * r[0];
    float twoXY = twoX * r[1];
    float twoXZ = twoX * r[2];
    float twoXW = twoX * r[3];
    float twoYY = twoY * r[1];
    float twoYZ = twoY * r[2];
    float twoYW = twoY * r[3];
    float twoZZ = twoZ * r[2];
    float twoZW = twoZ * r[3];
    float m00 = 1.0f - twoYY - twoZZ;
    float m01 = twoXY - twoZW;
    float m02 = twoXZ + twoYW;
    float m10 = twoXY + twoZW;
    float m11 = 1.0f - twoXX - twoZZ;
    float m12 = twoYZ - twoXW;
    float m20 = twoXZ - twoYW;
    float m21 = twoYZ + twoXW;
    float m22 = 1.0f - twoXX - twoYY;

    // The transformed point.
    Vector<float, 3> y;
    y[0] = m00 * x[0] + m01 * x[1] + m02 * x[2] + t[0];
    y[1] = m10 * x[0] + m11 * x[1] + m12 * x[2] + t[1];
    y[2] = m20 * x[0] + m21 * x[1] + m22 * x[2] + t[2];

    float const epsilon = 1e-06f;
    std::array<float, 3> diff{};
    for (std::size_t i = 0; i < 3; ++i)
    {
        diff[i] = std::fabs(yDual[i] - y[i]);
        UTAssert(diff[i] <= epsilon, "RigidTransform failed");
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Algebra/DualQuaternion.h>

namespace gtl
{
    template class DualQuaternion<float>;
    using DQF = DualQuaternion<float>;
    using VF3 = Vector<float, 3>;
    using VF4 = Vector<float, 4>;
    template bool operator==(DQF const&, DQF const&);
    template bool operator!=(DQF const&, DQF const&);
    template bool operator<(DQF const&, DQF const&);
    template bool operator<=(DQF const&, DQF const&);
    template bool operator>(DQF const&, DQF const&);
    template bool operator>=(DQF const&, DQF const&);
    template DQF operator+(DQF const&);
    template DQF operator-(DQF const&);
    template DQF operator+(DQF const&, DQF const&);
    template DQF operator-(DQF const&, DQF const&);
    template DQF operator*(DQF const&, float const&);
    template DQF operator*(float const&, DQF const&);
    template DQF operator/(DQF const&, float const&);
    template DQF& operator+=(DQF&, DQF const&);
    template DQF& operator-=(DQF&, DQF const&);
    template DQF& operator*=(DQF&, float const&);
    template DQF& operator/=(DQF&, float const&);
    template DQF operator*(DQF const&, DQF const&);
    template DQF Conjugate(DQF const&);
    template DQF Inverse(DQF const&);
    template DQF Dot(DQF const&, DQF const&);
    template DQF Cross(DQF const&, DQF const&);
    template DQF Norm(DQF const&);
    template DQF Length(DQF const&);
    template VF3 RotateAndTranslate(DQF const&, VF3 const&);
    template VF4 RotateAndTranslate(DQF const&, VF4 const&);

    template class DualQuaternion<double>;
    using DQD = DualQuaternion<double>;
    using VD3 = Vector<double, 3>;
    using VD4 = Vector<double, 4>;
    template bool operator==(DQD const&, DQD const&);
    template bool operator!=(DQD const&, DQD const&);
    template bool operator<(DQD const&, DQD const&);
    template bool operator<=(DQD const&, DQD const&);
    template bool operator>(DQD const&, DQD const&);
    template bool operator>=(DQD const&, DQD const&);
    template DQD operator+(DQD const&);
    template DQD operator-(DQD const&);
    template DQD operator+(DQD const&, DQD const&);
    template DQD operator-(DQD const&, DQD const&);
    template DQD operator*(DQD const&, double const&);
    template DQD operator*(double const&, DQD const&);
    template DQD operator/(DQD const&, double const&);
    template DQD& operator+=(DQD&, DQD const&);
    template DQD& operator-=(DQD&, DQD const&);
    template DQD& operator*=(DQD&, double const&);
    template DQD& operator/=(DQD&, double const&);
    template DQD operator*(DQD const&, DQD const&);
    template DQD Conjugate(DQD const&);
    template DQD Inverse(DQD const&);
    template DQD Dot(DQD const&, DQD const&);
    template DQD Cross(DQD const&, DQD const&);
    template DQD Norm(DQD const&);
    template DQD Length(DQD const&);
    template VD3 RotateAndTranslate(DQD const&, VD3 const&);
    template VD4 RotateAndTranslate(DQD const&, VD4 const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DualQuaternion<Rational>;
    using DQR = DualQuaternion<Rational>;
    using VR3 = Vector<Rational, 3>;
    using VR4 = Vector<Rational, 4>;
    template bool operator==(DQR const&, DQR const&);
    template bool operator!=(DQR const&, DQR const&);
    template bool operator<(DQR const&, DQR const&);
    template bool operator<=(DQR const&, DQR const&);
    template bool operator>(DQR const&, DQR const&);
    template bool operator>=(DQR const&, DQR const&);
    template DQR operator+(DQR const&);
    template DQR operator-(DQR const&);
    template DQR operator+(DQR const&, DQR const&);
    template DQR operator-(DQR const&, DQR const&);
    template DQR operator*(DQR const&, Rational const&);
    template DQR operator*(Rational const&, DQR const&);
    template DQR operator/(DQR const&, Rational const&);
    template DQR& operator+=(DQR&, DQR const&);
    template DQR& operator-=(DQR&, DQR const&);
    template DQR& operator*=(DQR&, Rational const&);
    template DQR& operator/=(DQR&, Rational const&);
    template DQR operator*(DQR const&, DQR const&);
    template DQR Conjugate(DQR const&);
    template DQR Inverse(DQR const&);
    template DQR Dot(DQR const&, DQR const&);
    template DQR Cross(DQR const&, DQR const&);
    template DQR Norm(DQR const&);
    template DQR Length(DQR const&);
    template VR3 RotateAndTranslate(DQR const&, VR3 const&);
    template VR4 RotateAndTranslate(DQR const&, VR4 const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DualQuaternion)
