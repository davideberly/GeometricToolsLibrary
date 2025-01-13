// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Arithmetic/QFNumber.h>
using namespace gtl;

namespace gtl
{
    class UnitTestQFNumber
    {
    public:
        UnitTestQFNumber();

    private:
        using BSR = BSRational<UIntegerAP32>;

        using QFN1f = QFNumber<float, 1>;
        using QFN1d = QFNumber<double, 1>;
        using QFN1r = QFNumber<BSR, 1>;

        using QFN2f = QFNumber<float, 2>;
        using QFN2d = QFNumber<double, 2>;
        using QFN2r = QFNumber<BSR, 2>;

        void Constructors();
        void Comparisons();
        void Arithmetic();
    };
}

UnitTestQFNumber::UnitTestQFNumber()
{
    UTInformation("Mathematics/Arithmetic/QFNumber");

    Constructors();
    Comparisons();
    Arithmetic();
}

void UnitTestQFNumber::Constructors()
{
    QFN1f u0;
    UTAssert(
        u0.x[0] == 0.0f && u0.x[1] == 0.0f &&
        u0.d == 0.0f,
        "Default constructor failed.");

    QFN1d u1(2.0);
    UTAssert(
        u1.x[0] == 0.0 && u1.x[1] == 0.0 &&
        u1.d == 2.0,
        "Discriminant constructor failed.");

    QFN1r u2(BSR(1), BSR(2), BSR(3));
    UTAssert(
        u2.x[0] == BSR(1) && u2.x[1] == BSR(2) &&
        u2.d == BSR(3),
        "Tuple constructor failed.");

    std::array<BSR, 2> x = { BSR(1), BSR(2) };
    QFN1r u3(x, BSR(3));
    UTAssert(
        u3.x[0] == BSR(1) && u3.x[1] == BSR(2) &&
        u3.d == BSR(3),
        "Array constructor failed.");

    QFN2f v0;
    UTAssert(
        v0.x[0].x[0] == 0.0f && v0.x[0].x[1] == 0.0f &&
        v0.x[1].x[0] == 0.0f && v0.x[1].x[1] == 0.0f &&
        v0.d == 0.0f,
        "Default constructor failed.");

    QFN2d v1(2.0);
    UTAssert(
        v1.x[0].x[0] == 0.0 && v1.x[0].x[1] == 0.0 &&
        v1.x[1].x[0] == 0.0 && v1.x[1].x[1] == 0.0 &&
        v1.d == 2.0,
        "Discriminant constructor failed.");

    QFN1r w0(1, 2, 3), w1(4, 5, 3);
    QFN2r v2(w0, w1, 7);
    UTAssert(
        v2.x[0].x[0] == BSR(1) && v2.x[0].x[1] == BSR(2) &&
        v2.x[1].x[0] == BSR(4) && v2.x[1].x[1] == BSR(5) &&
        v2.x[0].d == BSR(3) && v2.d == BSR(7),
        "Tuple constructor failed.");

    std::array<QFN1r, 2> warray = { w0, w1 };
    QFN2r v3(warray, 7);
    UTAssert(
        v3.x[0].x[0] == BSR(1) && v3.x[0].x[1] == BSR(2) &&
        v3.x[1].x[0] == BSR(4) && v3.x[1].x[1] == BSR(5) &&
        v3.x[0].d == BSR(3) && v3.d == BSR(7),
        "Tuple constructor failed.");
}

void UnitTestQFNumber::Comparisons()
{
    BSR const d(2);
    QFN1r e0(1, 2, d), e1(3, 4, d), zero(d);
    bool success;

    success = (e0 == zero);
    UTAssert(!success, "operator== failed");

    success = (e0 != zero);
    UTAssert(success, "operator!= failed");

    success = (e0 < zero);
    UTAssert(!success, "operator< failed");

    success = (e0 <= zero);
    UTAssert(!success, "operator<= failed");

    success = (e0 > zero);
    UTAssert(success, "operator> failed");

    success = (e0 >= zero);
    UTAssert(success, "operator>= failed");

    success = (e0 == e1);
    UTAssert(!success, "operator== failed");

    success = (e0 != e1);
    UTAssert(success, "operator!= failed");

    success = (e0 < e1);
    UTAssert(success, "operator< failed");

    success = (e0 <= e1);
    UTAssert(success, "operator<= failed");

    success = (e0 > e1);
    UTAssert(!success, "operator> failed");

    success = (e0 >= e1);
    UTAssert(!success, "operator>= failed");
}

void UnitTestQFNumber::Arithmetic()
{
    BSR const d(2), s(3);
    QFN1r e0(1, 2, d), e1(3, 4, d), zero(d);
    QFN1r e2;
    bool success;

    e2 = +e1;
    success = (e2 == e1);
    UTAssert(success, "unary+ failed");

    e2 = -e1;
    success = (e2.x[0] == -e1.x[0] && e2.x[1] == -e1.x[1]);
    UTAssert(success, "unary- failed");

    e2 = e0 + e1;  // e2 = { 4, 6 }
    UTAssert(e2.x[0] == BSR(4) && e2.x[1] == BSR(6), "ADDrr failed");

    e2 = e0 - e1;  // e2 = { -2, -2 }
    UTAssert(e2.x[0] == BSR(-2) && e2.x[1] == BSR(-2), "SUBrr failed");

    e2 = e0 * e1;  // e2 = { 19, 10 }
    UTAssert(e2.x[0] == BSR(19) && e2.x[1] == BSR(10), "MULrr failed");

    e2 = e0 / e1;  // e2 = { 13/23, -2/23 }
    UTAssert(e2.x[0] == BSR(13, 23) && e2.x[1] == BSR(-2, 23), "DIVrr failed");

    e2 = e0 + s;   // e2 = { 4, 2 }
    UTAssert(e2.x[0] == BSR(4) && e2.x[1] == BSR(2), "ADDrs failed");

    e2 = s + e0;   // e2 = { 4, 2 }
    UTAssert(e2.x[0] == BSR(4) && e2.x[1] == BSR(2), "ADDsr failed");

    e2 = e0 - s;   // e2 = { -2, 2 }
    UTAssert(e2.x[0] == BSR(-2) && e2.x[1] == BSR(2), "SUBrs failed");

    e2 = s - e0;   // e2 = { 2, -2 }
    UTAssert(e2.x[0] == BSR(2) && e2.x[1] == BSR(-2), "SUBsr failed");

    e2 = e0 * s;   // e2 = { 3, 6 }
    UTAssert(e2.x[0] == BSR(3) && e2.x[1] == BSR(6), "MULrs failed");

    e2 = s * e0;   // e2 = { 3, 6 }
    UTAssert(e2.x[0] == BSR(3) && e2.x[1] == BSR(6), "MULsr failed");

    e2 = e0 / s;   // e2 = { 1/3, 2/3 }
    UTAssert(e2.x[0] == BSR(1, 3) && e2.x[1] == BSR(2, 3), "DIVrs failed");

    e2 = s / e0;   // e2 = { -3/7, 6/7 }
    UTAssert(e2.x[0] == BSR(-3, 7) && e2.x[1] == BSR(6, 7), "DIVsr failed");

    e0 = QFN1r(1, 2, d);
    e1 = QFN1r(3, 4, d);
    e0 += e1;
    UTAssert(e0.x[0] == BSR(4) && e0.x[1] == BSR(6), "ADDUrr failed");

    e0 = QFN1r(1, 2, d);
    e0 += s;
    UTAssert(e0.x[0] == BSR(4) && e0.x[1] == BSR(2), "ADDUrs failed");

    e0 = QFN1r(1, 2, d);
    e0 -= e1;
    UTAssert(e0.x[0] == BSR(-2) && e0.x[1] == BSR(-2), "SUBUrr failed");

    e0 = QFN1r(1, 2, d);
    e0 -= s;
    UTAssert(e0.x[0] == BSR(-2) && e0.x[1] == BSR(2), "SUBUrs failed");

    e0 = QFN1r(1, 2, d);
    e0 *= e1;
    UTAssert(e0.x[0] == BSR(19) && e0.x[1] == BSR(10), "MULUrr failed");

    e0 = QFN1r(1, 2, d);
    e0 *= s;
    UTAssert(e0.x[0] == BSR(3) && e0.x[1] == BSR(6), "MULUrs failed");

    e0 = QFN1r(1, 2, d);
    e0 /= e1;
    UTAssert(e0.x[0] == BSR(13, 23) && e0.x[1] == BSR(-2, 23), "DIVUrr failed");

    e0 = QFN1r(1, 2, d);
    e0 /= s;
    UTAssert(e0.x[0] == BSR(1, 3) && e0.x[1] == BSR(2, 3), "DIVUrs failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Arithmetic/QFNumber.h>

namespace gtl
{
    template class QFNumber<float, 1>;
    using QFN1f = QFNumber<float, 1>;
    template QFN1f operator+(QFN1f const&);
    template QFN1f operator-(QFN1f const&);
    template QFN1f operator+(QFN1f const&, QFN1f const&);
    template QFN1f operator+(QFN1f const&, float const&);
    template QFN1f operator+(float const&, QFN1f const&);
    template QFN1f operator-(QFN1f const&, QFN1f const&);
    template QFN1f operator-(QFN1f const&, float const&);
    template QFN1f operator-(float const&, QFN1f const&);
    template QFN1f operator*(QFN1f const&, QFN1f const&);
    template QFN1f operator*(QFN1f const&, float const&);
    template QFN1f operator*(float const&, QFN1f const&);
    template QFN1f operator/(QFN1f const&, QFN1f const&);
    template QFN1f operator/(QFN1f const&, float const&);
    template QFN1f operator/(float const&, QFN1f const&);
    template QFN1f& operator+=(QFN1f&, QFN1f const&);
    template QFN1f& operator+=(QFN1f&, float const&);
    template QFN1f& operator-=(QFN1f&, QFN1f const&);
    template QFN1f& operator-=(QFN1f&, float const&);
    template QFN1f& operator*=(QFN1f&, QFN1f const&);
    template QFN1f& operator*=(QFN1f&, float const&);
    template QFN1f& operator/=(QFN1f&, QFN1f const&);
    template QFN1f& operator/=(QFN1f&, float const&);
    template bool operator==(QFN1f const&, QFN1f const&);
    template bool operator!=(QFN1f const&, QFN1f const&);
    template bool operator< (QFN1f const&, QFN1f const&);
    template bool operator<=(QFN1f const&, QFN1f const&);
    template bool operator> (QFN1f const&, QFN1f const&);
    template bool operator>=(QFN1f const&, QFN1f const&);

    template class QFNumber<double, 1>;
    using QFN1d = QFNumber<double, 1>;
    template QFN1d operator+(QFN1d const&);
    template QFN1d operator-(QFN1d const&);
    template QFN1d operator+(QFN1d const&, QFN1d const&);
    template QFN1d operator+(QFN1d const&, double const&);
    template QFN1d operator+(double const&, QFN1d const&);
    template QFN1d operator-(QFN1d const&, QFN1d const&);
    template QFN1d operator-(QFN1d const&, double const&);
    template QFN1d operator-(double const&, QFN1d const&);
    template QFN1d operator*(QFN1d const&, QFN1d const&);
    template QFN1d operator*(QFN1d const&, double const&);
    template QFN1d operator*(double const&, QFN1d const&);
    template QFN1d operator/(QFN1d const&, QFN1d const&);
    template QFN1d operator/(QFN1d const&, double const&);
    template QFN1d operator/(double const&, QFN1d const&);
    template QFN1d& operator+=(QFN1d&, QFN1d const&);
    template QFN1d& operator+=(QFN1d&, double const&);
    template QFN1d& operator-=(QFN1d&, QFN1d const&);
    template QFN1d& operator-=(QFN1d&, double const&);
    template QFN1d& operator*=(QFN1d&, QFN1d const&);
    template QFN1d& operator*=(QFN1d&, double const&);
    template QFN1d& operator/=(QFN1d&, QFN1d const&);
    template QFN1d& operator/=(QFN1d&, double const&);
    template bool operator==(QFN1d const&, QFN1d const&);
    template bool operator!=(QFN1d const&, QFN1d const&);
    template bool operator< (QFN1d const&, QFN1d const&);
    template bool operator<=(QFN1d const&, QFN1d const&);
    template bool operator> (QFN1d const&, QFN1d const&);
    template bool operator>=(QFN1d const&, QFN1d const&);

    template class QFNumber<float, 2>;
    using QFN2f = QFNumber<float, 2>;
    template QFN2f operator+(QFN2f const&);
    template QFN2f operator-(QFN2f const&);
    template QFN2f operator+(QFN2f const&, QFN2f const&);
    template QFN2f operator+(QFN2f const&, float const&);
    template QFN2f operator+(float const&, QFN2f const&);
    template QFN2f operator-(QFN2f const&, QFN2f const&);
    template QFN2f operator-(QFN2f const&, float const&);
    template QFN2f operator-(float const&, QFN2f const&);
    template QFN2f operator*(QFN2f const&, QFN2f const&);
    template QFN2f operator*(QFN2f const&, float const&);
    template QFN2f operator*(float const&, QFN2f const&);
    template QFN2f operator/(QFN2f const&, QFN2f const&);
    template QFN2f operator/(QFN2f const&, float const&);
    template QFN2f operator/(float const&, QFN2f const&);
    template QFN2f& operator+=(QFN2f&, QFN2f const&);
    template QFN2f& operator+=(QFN2f&, float const&);
    template QFN2f& operator-=(QFN2f&, QFN2f const&);
    template QFN2f& operator-=(QFN2f&, float const&);
    template QFN2f& operator*=(QFN2f&, QFN2f const&);
    template QFN2f& operator*=(QFN2f&, float const&);
    template QFN2f& operator/=(QFN2f&, QFN2f const&);
    template QFN2f& operator/=(QFN2f&, float const&);
    template bool operator==(QFN2f const&, QFN2f const&);
    template bool operator!=(QFN2f const&, QFN2f const&);
    template bool operator< (QFN2f const&, QFN2f const&);
    template bool operator<=(QFN2f const&, QFN2f const&);
    template bool operator> (QFN2f const&, QFN2f const&);
    template bool operator>=(QFN2f const&, QFN2f const&);

    template class QFNumber<double, 2>;
    using QFN2d = QFNumber<double, 2>;
    template QFN2d operator+(QFN2d const&);
    template QFN2d operator-(QFN2d const&);
    template QFN2d operator+(QFN2d const&, QFN2d const&);
    template QFN2d operator+(QFN2d const&, double const&);
    template QFN2d operator+(double const&, QFN2d const&);
    template QFN2d operator-(QFN2d const&, QFN2d const&);
    template QFN2d operator-(QFN2d const&, double const&);
    template QFN2d operator-(double const&, QFN2d const&);
    template QFN2d operator*(QFN2d const&, QFN2d const&);
    template QFN2d operator*(QFN2d const&, double const&);
    template QFN2d operator*(double const&, QFN2d const&);
    template QFN2d operator/(QFN2d const&, QFN2d const&);
    template QFN2d operator/(QFN2d const&, double const&);
    template QFN2d operator/(double const&, QFN2d const&);
    template QFN2d& operator+=(QFN2d&, QFN2d const&);
    template QFN2d& operator+=(QFN2d&, double const&);
    template QFN2d& operator-=(QFN2d&, QFN2d const&);
    template QFN2d& operator-=(QFN2d&, double const&);
    template QFN2d& operator*=(QFN2d&, QFN2d const&);
    template QFN2d& operator*=(QFN2d&, double const&);
    template QFN2d& operator/=(QFN2d&, QFN2d const&);
    template QFN2d& operator/=(QFN2d&, double const&);
    template bool operator==(QFN2d const&, QFN2d const&);
    template bool operator!=(QFN2d const&, QFN2d const&);
    template bool operator< (QFN2d const&, QFN2d const&);
    template bool operator<=(QFN2d const&, QFN2d const&);
    template bool operator> (QFN2d const&, QFN2d const&);
    template bool operator>=(QFN2d const&, QFN2d const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using BSRap = BSRational<UIntegerAP32>;
    using BSRfp = BSRational<UIntegerFP32<4>>;

    template class QFNumber<BSRap, 1>;
    using QFN1ap = QFNumber<BSRap, 1>;
    template QFN1ap operator+(QFN1ap const&);
    template QFN1ap operator-(QFN1ap const&);
    template QFN1ap operator+(QFN1ap const&, QFN1ap const&);
    template QFN1ap operator+(QFN1ap const&, BSRap const&);
    template QFN1ap operator+(BSRap const&, QFN1ap const&);
    template QFN1ap operator-(QFN1ap const&, QFN1ap const&);
    template QFN1ap operator-(QFN1ap const&, BSRap const&);
    template QFN1ap operator-(BSRap const&, QFN1ap const&);
    template QFN1ap operator*(QFN1ap const&, QFN1ap const&);
    template QFN1ap operator*(QFN1ap const&, BSRap const&);
    template QFN1ap operator*(BSRap const&, QFN1ap const&);
    template QFN1ap operator/(QFN1ap const&, QFN1ap const&);
    template QFN1ap operator/(QFN1ap const&, BSRap const&);
    template QFN1ap operator/(BSRap const&, QFN1ap const&);
    template QFN1ap& operator+=(QFN1ap&, QFN1ap const&);
    template QFN1ap& operator+=(QFN1ap&, BSRap const&);
    template QFN1ap& operator-=(QFN1ap&, QFN1ap const&);
    template QFN1ap& operator-=(QFN1ap&, BSRap const&);
    template QFN1ap& operator*=(QFN1ap&, QFN1ap const&);
    template QFN1ap& operator*=(QFN1ap&, BSRap const&);
    template QFN1ap& operator/=(QFN1ap&, QFN1ap const&);
    template QFN1ap& operator/=(QFN1ap&, BSRap const&);
    template bool operator==(QFN1ap const&, QFN1ap const&);
    template bool operator!=(QFN1ap const&, QFN1ap const&);
    template bool operator< (QFN1ap const&, QFN1ap const&);
    template bool operator<=(QFN1ap const&, QFN1ap const&);
    template bool operator> (QFN1ap const&, QFN1ap const&);
    template bool operator>=(QFN1ap const&, QFN1ap const&);

    template class QFNumber<BSRfp, 1>;
    using QFN1fp = QFNumber<BSRfp, 1>;
    template QFN1fp operator+(QFN1fp const&);
    template QFN1fp operator-(QFN1fp const&);
    template QFN1fp operator+(QFN1fp const&, QFN1fp const&);
    template QFN1fp operator+(QFN1fp const&, BSRfp const&);
    template QFN1fp operator+(BSRfp const&, QFN1fp const&);
    template QFN1fp operator-(QFN1fp const&, QFN1fp const&);
    template QFN1fp operator-(QFN1fp const&, BSRfp const&);
    template QFN1fp operator-(BSRfp const&, QFN1fp const&);
    template QFN1fp operator*(QFN1fp const&, QFN1fp const&);
    template QFN1fp operator*(QFN1fp const&, BSRfp const&);
    template QFN1fp operator*(BSRfp const&, QFN1fp const&);
    template QFN1fp operator/(QFN1fp const&, QFN1fp const&);
    template QFN1fp operator/(QFN1fp const&, BSRfp const&);
    template QFN1fp operator/(BSRfp const&, QFN1fp const&);
    template QFN1fp& operator+=(QFN1fp&, QFN1fp const&);
    template QFN1fp& operator+=(QFN1fp&, BSRfp const&);
    template QFN1fp& operator-=(QFN1fp&, QFN1fp const&);
    template QFN1fp& operator-=(QFN1fp&, BSRfp const&);
    template QFN1fp& operator*=(QFN1fp&, QFN1fp const&);
    template QFN1fp& operator*=(QFN1fp&, BSRfp const&);
    template QFN1fp& operator/=(QFN1fp&, QFN1fp const&);
    template QFN1fp& operator/=(QFN1fp&, BSRfp const&);
    template bool operator==(QFN1fp const&, QFN1fp const&);
    template bool operator!=(QFN1fp const&, QFN1fp const&);
    template bool operator< (QFN1fp const&, QFN1fp const&);
    template bool operator<=(QFN1fp const&, QFN1fp const&);
    template bool operator> (QFN1fp const&, QFN1fp const&);
    template bool operator>=(QFN1fp const&, QFN1fp const&);

    template class QFNumber<BSRap, 2>;
    using QFN2ap = QFNumber<BSRap, 2>;
    template QFN2ap operator+(QFN2ap const&);
    template QFN2ap operator-(QFN2ap const&);
    template QFN2ap operator+(QFN2ap const&, QFN2ap const&);
    template QFN2ap operator+(QFN2ap const&, BSRap const&);
    template QFN2ap operator+(BSRap const&, QFN2ap const&);
    template QFN2ap operator-(QFN2ap const&, QFN2ap const&);
    template QFN2ap operator-(QFN2ap const&, BSRap const&);
    template QFN2ap operator-(BSRap const&, QFN2ap const&);
    template QFN2ap operator*(QFN2ap const&, QFN2ap const&);
    template QFN2ap operator*(QFN2ap const&, BSRap const&);
    template QFN2ap operator*(BSRap const&, QFN2ap const&);
    template QFN2ap operator/(QFN2ap const&, QFN2ap const&);
    template QFN2ap operator/(QFN2ap const&, BSRap const&);
    template QFN2ap operator/(BSRap const&, QFN2ap const&);
    template QFN2ap& operator+=(QFN2ap&, QFN2ap const&);
    template QFN2ap& operator+=(QFN2ap&, BSRap const&);
    template QFN2ap& operator-=(QFN2ap&, QFN2ap const&);
    template QFN2ap& operator-=(QFN2ap&, BSRap const&);
    template QFN2ap& operator*=(QFN2ap&, QFN2ap const&);
    template QFN2ap& operator*=(QFN2ap&, BSRap const&);
    template QFN2ap& operator/=(QFN2ap&, QFN2ap const&);
    template QFN2ap& operator/=(QFN2ap&, BSRap const&);
    template bool operator==(QFN2ap const&, QFN2ap const&);
    template bool operator!=(QFN2ap const&, QFN2ap const&);
    template bool operator< (QFN2ap const&, QFN2ap const&);
    template bool operator<=(QFN2ap const&, QFN2ap const&);
    template bool operator> (QFN2ap const&, QFN2ap const&);
    template bool operator>=(QFN2ap const&, QFN2ap const&);

    template class QFNumber<BSRfp, 2>;
    using QFN2fp = QFNumber<BSRfp, 2>;
    template QFN2fp operator+(QFN2fp const&);
    template QFN2fp operator-(QFN2fp const&);
    template QFN2fp operator+(QFN2fp const&, QFN2fp const&);
    template QFN2fp operator+(QFN2fp const&, BSRfp const&);
    template QFN2fp operator+(BSRfp const&, QFN2fp const&);
    template QFN2fp operator-(QFN2fp const&, QFN2fp const&);
    template QFN2fp operator-(QFN2fp const&, BSRfp const&);
    template QFN2fp operator-(BSRfp const&, QFN2fp const&);
    template QFN2fp operator*(QFN2fp const&, QFN2fp const&);
    template QFN2fp operator*(QFN2fp const&, BSRfp const&);
    template QFN2fp operator*(BSRfp const&, QFN2fp const&);
    template QFN2fp operator/(QFN2fp const&, QFN2fp const&);
    template QFN2fp operator/(QFN2fp const&, BSRfp const&);
    template QFN2fp operator/(BSRfp const&, QFN2fp const&);
    template QFN2fp& operator+=(QFN2fp&, QFN2fp const&);
    template QFN2fp& operator+=(QFN2fp&, BSRfp const&);
    template QFN2fp& operator-=(QFN2fp&, QFN2fp const&);
    template QFN2fp& operator-=(QFN2fp&, BSRfp const&);
    template QFN2fp& operator*=(QFN2fp&, QFN2fp const&);
    template QFN2fp& operator*=(QFN2fp&, BSRfp const&);
    template QFN2fp& operator/=(QFN2fp&, QFN2fp const&);
    template QFN2fp& operator/=(QFN2fp&, BSRfp const&);
    template bool operator==(QFN2fp const&, QFN2fp const&);
    template bool operator!=(QFN2fp const&, QFN2fp const&);
    template bool operator< (QFN2fp const&, QFN2fp const&);
    template bool operator<=(QFN2fp const&, QFN2fp const&);
    template bool operator> (QFN2fp const&, QFN2fp const&);
    template bool operator>=(QFN2fp const&, QFN2fp const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(QFNumber)
