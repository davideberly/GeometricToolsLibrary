// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/UIntegerAP32.h>
#include <GTL/Mathematics/Arithmetic/BSNumber.h>
#include <GTL/Mathematics/Arithmetic/SWInterval.h>
#include <GTL/Mathematics/Arithmetic/IEEEBinary.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
using namespace gtl;

namespace gtl
{
    class UnitTestSWInterval
    {
    public:
        UnitTestSWInterval();

    private:
        using BSN = BSNumber<UIntegerAP32>;

        // These are used in Section 4.2 "Signs of Determinants" of RAEFGC.
        template <typename FPType, typename OutputType>
        void ComputeDeterminant(Matrix2x2<FPType> const& A, OutputType& det)
        {
            OutputType a00(A(0, 0)), a01(A(0, 1)), a10(A(1, 0)), a11(A(1, 1));
            det = a00 * a11 - a01 * a10;
        }

        template <typename FPType, typename OutputType>
        void ComputeDeterminant(Matrix3x3<FPType> const& A, OutputType& det)
        {
            OutputType a00(A(0, 0)), a01(A(0, 1)), a02(A(0, 2));
            OutputType a10(A(1, 0)), a11(A(1, 1)), a12(A(1, 2));
            OutputType a20(A(2, 0)), a21(A(2, 1)), a22(A(2, 2));
            det =
                a00 * (a11 * a22 - a12 * a21) -
                a01 * (a10 * a22 - a12 * a20) +
                a02 * (a10 * a21 - a11 * a20);
        }

        template <typename FPType, typename OutputType>
        void ComputeDeterminant(Matrix4x4<FPType> const& A, OutputType& det)
        {
            OutputType a00(A(0, 0)), a01(A(0, 1)), a02(A(0, 2)), a03(A(0, 3));
            OutputType a10(A(1, 0)), a11(A(1, 1)), a12(A(1, 2)), a13(A(1, 3));
            OutputType a20(A(2, 0)), a21(A(2, 1)), a22(A(2, 2)), a23(A(2, 3));
            OutputType a30(A(3, 0)), a31(A(3, 1)), a32(A(3, 2)), a33(A(3, 3));
            OutputType u0 = a00 * a11 - a01 * a10, v0 = a20 * a31 - a21 * a30;
            OutputType u1 = a00 * a12 - a02 * a10, v1 = a20 * a32 - a22 * a30;
            OutputType u2 = a00 * a13 - a02 * a10, v2 = a20 * a33 - a23 * a30;
            OutputType u3 = a01 * a12 - a02 * a11, v3 = a21 * a32 - a22 * a31;
            OutputType u4 = a01 * a13 - a03 * a11, v4 = a21 * a33 - a23 * a31;
            OutputType u5 = a02 * a13 - a03 * a12, v5 = a22 * a33 - a23 * a32;
            det = u0 * v5 - u1 * v4 + u2 * v3 + u3 * v2 - u4 * v1 + u5 * v0;
        }

        template <typename FPType, typename MatrixType>
        std::int32_t GetSignDeterminant(MatrixType const& A, SWInterval<FPType>& idet)
        {
            ComputeDeterminant(A, idet);
            FPType constexpr zero = 0;
            if (idet[0] > zero)
            {
                return +1;
            }
            else if (idet[1] < zero)
            {
                return -1;
            }
            else
            {
                // The interval contains 0, so compute the determinant using
                // arbitrary-precision arithmetic.
                BSN rdet;
                ComputeDeterminant(A, rdet);
                return rdet.GetSign();
            }
        }

        void SignDeterminant2x2();
        void SignDeterminant3x3();
        void SignDeterminant4x4();

        // The example of Listing 4.5 in RAEFGC for the expression whose tree
        // is in Figure 4.1.
        template <typename T>
        void IntervalExampleCompute(T x, T y, T z, T w, T t, T u, T v, T& e, SWInterval<T>& ie)
        {
            SWInterval<T> ix(x), iy(y), iz(z), iw(w), it(t), iu(u), iv(v);
            ie = ix * iy + iz * iw - T(2) / (T(3) + it * iu * iv);
            e = x * y + z * w - T(2) / (T(3) + t * u * v);
        }

        void IntervalExampleDifferentTypes();
    };
}

UnitTestSWInterval::UnitTestSWInterval()
{
    UTInformation("Mathematics/Arithmetic/SWInterval");

    SignDeterminant2x2();
    SignDeterminant3x3();
    SignDeterminant4x4();
    IntervalExampleDifferentTypes();
}

void UnitTestSWInterval::SignDeterminant2x2()
{
    float const cs = std::cos(0.1f), sn = std::sin(0.1f);
    Matrix2x2<float> R = { { { cs, -sn }, { sn, cs } } };
    Matrix2x2<float> D = { { { 1.0f, 0.0f }, { 0.0f, 0.0f } } };
    Matrix2x2<float> DR{}, A{};
    SWInterval<float> idet;
    std::int32_t exactSign;
    float fdet, convert;
    BSN rdet;

    // Compute D*R.
    D(1, 1) = 1e-8f;
    DR = D * R;

    // Compute A = R^T*D*R.
    A = MultiplyATB(R, DR);

    // Compute the exact sign of the determinant using interval arithmetic.
    // The interval does not contain zero, so no rational arithmetic was
    // used in the computation.
    exactSign = GetSignDeterminant<float>(A, idet);
    UTAssert(
        exactSign == 1 &&
        idet[0] == 9.31322486e-09f &&
        idet[1] == 1.30385169e-08f, "Expecting positive interval.");

    // Compute floating-point determinant (has rounding errors). The
    // result could change depending on the code the compiler generates
    // for the floating-point arithmetic.
    ComputeDeterminant(A, fdet);
    UTAssert(fdet == 1.11758709e-08f, "Invalid determinant (floating-point issue)");

    // Compute rational determinant (exact). The compiled expressions for
    // the entries of A(r,c) are different between Debug and Release builds.
    ComputeDeterminant(A, rdet);
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -54 &&
        rdet.GetUInteger().GetNumBits() == 28 &&
        rdet.GetUInteger().GetBits()[0] == 0x0b5670f1,
        "Invalid exact determinant.");

    convert = rdet;
    UTAssert(convert == 1.05590194e-08f, "Invalid convert (floating-point issue)");


    // Compute D*R.
    D(1, 1) = 1e-10f;
    DR = D * R;

    // Compute A = R^T*D*R.
    A = MultiplyATB(R, DR);

    // Compute the exact sign of the determinant using interval arithmetic.
    // The interval contains zero, so rational arithmetic was used in the
    // computation.
    exactSign = GetSignDeterminant<float>(A, idet);
    UTAssert(
        exactSign == 1 &&
        idet[0] == -9.31322686e-10f &&
        idet[1] == 2.79396795e-09f, "Expecting interval to contain zero.");

    // Compute floating-point determinant (has rounding errors). The
    // result could change depending on the code the compiler generates
    // for the floating-point arithmetic. For MSVS 2019 16.8.3, the
    // fdet value is the same in Debug or Release.
    ComputeDeterminant(A, fdet);
    UTAssert(fdet == 9.31322575e-10f, "Invalid determinant (floating-point issue)");

    // Compute rational determinant (exact).
    ComputeDeterminant(A, rdet);
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -49 &&
        rdet.GetUInteger().GetNumBits() == 18 &&
        rdet.GetUInteger().GetBits()[0] == 0x0003940f,
        "Invalid exact determinant.");

    convert = rdet;
    UTAssert(convert == 4.16575219e-10f, "Invalid convert (floating-point issue)");
}

void UnitTestSWInterval::SignDeterminant3x3()
{
    Vector3<float> U = { 1.0f, 2.0f, 3.0f };
    Vector3<float> V = { 0.0f, -3.0f, 2.0f };
    Vector3<float> W = { -13.0f, 2.0f, 3.0f };
    float ulength = std::sqrt(14.0f);
    float vlength = std::sqrt(13.0f);
    float wlength = std::sqrt(182.0f);
    for (std::size_t r = 0; r < 3; ++r)
    {
        U[r] /= ulength;
        V[r] /= vlength;
        W[r] /= wlength;
    }

    Matrix3x3<float> R =
    {{
        { U[0], V[0], W[0] },
        { U[1], V[1], W[1] },
        { U[2], V[2], W[2] }
    }};

    Matrix3x3<float> D =
    {{
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f }
    }};

    Matrix3x3<float> DR{}, A{};
    SWInterval<float> idet;
    std::int32_t exactSign;
    float fdet, convert;
    BSN rdet;

    // Compute D*R.
    D(2, 2) = 1e-4f;
    DR = D * R;

    // Compute A = R^T*D*R.
    A = MultiplyATB(R, DR);

    // Compute the exact sign of the determinant using interval arithmetic.
    // The interval does not contain zero, so no rational arithmetic was
    // used in the computation.
    exactSign = GetSignDeterminant<float>(A, idet);
    UTAssert(
        exactSign == 1 &&
        idet[0] == 9.98750256e-05f &&
        idet[1] == 0.000100091107f, "Expecting positive interval.");

    // Compute floating-point determinant (has rounding errors). The
    // result could change depending on the code the compiler generates
    // for the floating-point arithmetic.
    ComputeDeterminant(A, fdet);
    UTAssert(fdet == 9.99793410e-05f, "Invalid determinant (floating-point issue)");

    // Compute rational determinant (exact).
    ComputeDeterminant(A, rdet);
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -75 &&
        rdet.GetUInteger().GetNumBits() == 62 &&
        rdet.GetUInteger().GetBits()[0] == 0x91f12fa3u &&
        rdet.GetUInteger().GetBits()[1] == 0x346d2194u,
        "Invalid exact determinant.");

    convert = rdet;
    UTAssert(convert == 9.99952172e-05f, "Invalid convert (floating-point issue)");


    // Compute D*R.
    D(2, 2) = 1e-8f;
    DR = D * R;

    // Compute A = R^T*D*R.
    A = MultiplyATB(R, DR);

    // Compute the exact sign of the determinant using interval arithmetic.
    // The interval contains zero, so rational arithmetic was used in the
    // computation.
    exactSign = GetSignDeterminant<float>(A, idet);
    UTAssert(
        exactSign == 1 &&
        idet[0] == -1.08033426e-07f &&
        idet[1] == 1.26659884e-07f, "Expecting interval to contain zero.");

    // Compute floating-point determinant (has rounding errors). The
    // result could change depending on the code the compiler generates
    // for the floating-point arithmetic. NOTE: Floating-point rounding
    // errors leads to a negative determinant, but rational arithmetic leads
    // to a positive determinant.
    ComputeDeterminant(A, fdet);
    UTAssert(fdet == 1.11758709e-08f, "Invalid determinant (floating-point issue)");

    // Compute rational determinant (exact).
    ComputeDeterminant(A, rdet);
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -77 &&
        rdet.GetUInteger().GetNumBits() == 50 &&
        rdet.GetUInteger().GetBits()[0] == 0x7164216b &&
        rdet.GetUInteger().GetBits()[1] == 0x0003447f,
        "Invalid exact determinant.");

    convert = rdet;
    UTAssert(convert == 6.08632256e-09f, "Invalid convert (floating-point issue)");
}

void UnitTestSWInterval::SignDeterminant4x4()
{
    Vector4<float> U = { 0.182574183f, 0.365148365f, 0.547722578f, 0.730296731f };
    Vector4<float> V = { 0.0f, 0.832050323f, -0.554700196f, 0.0f };
    Vector4<float> W = { 0.0f, -0.412020892f, -0.618031383f, 0.669534087f };
    Vector4<float> X = { -0.983192086f, 0.0678063408f, 0.101709522f, 0.135612682f };

    Matrix4x4<float> R =
    { {
        { U[0], V[0], W[0], X[0] },
        { U[1], V[1], W[1], X[1] },
        { U[2], V[2], W[2], X[2] },
        { U[3], V[3], W[3], X[3] }
    } };

    Matrix4x4<float> D =
    { {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f }
    } };

    Matrix4x4<float> DR{}, A{};
    SWInterval<float> idet;
    std::int32_t exactSign;
    float fdet, convert;
    BSN rdet;

    // Compute D*R.
    D(3, 3) = 1e-4f;
    DR = D * R;

    // Compute A = R^T*D*R.
    A = MultiplyATB(R, DR);

    // Compute the exact sign of the determinant using interval arithmetic.
    // The interval does not contain zero, so no rational arithmetic was
    // used in the computation.
    exactSign = GetSignDeterminant<float>(A, idet);
    UTAssert(
        exactSign == 1 &&
        idet[0] == 9.97911993e-05f &&
        idet[1] == 0.000100232675f, "Expecting positive interval.");

    // Compute floating-point determinant (has rounding errors). The
    // result could change depending on the code the compiler generates
    // for the floating-point arithmetic.
    ComputeDeterminant(A, fdet);
    UTAssert(fdet == 0.000100005418f, "Invalid determinant (floating-point issue)");

    // Compute rational determinant (exact).
    ComputeDeterminant(A, rdet);
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -106 &&
        rdet.GetUInteger().GetNumBits() == 93 &&
        rdet.GetUInteger().GetBits()[0] == 0xcde6f799 &&
        rdet.GetUInteger().GetBits()[1] == 0x1fda7663 &&
        rdet.GetUInteger().GetBits()[2] == 0x1a3735b4,
        "Invalid exact determinant.");

    convert = rdet;
    UTAssert(convert == 0.000100004821f, "Invalid convert (floating-point issue)");


    // Compute D*R.
    D(3, 3) = 1e-8f;
    DR = D * R;

    // Compute A = R^T*D*R.
    A = MultiplyATB(R, DR);

    // Compute the exact sign of the determinant using interval arithmetic.
    // The interval contains zero, so rational arithmetic was used in the
    // computation. NOTE: In this example, the compiled code for the entries
    // of A(r,c) 
    exactSign = GetSignDeterminant<float>(A, idet);
    UTAssert(
        exactSign == -1 &&
        idet[0] == -2.19792156e-07f &&
        idet[1] == 1.91852479e-07f, "Expecting interval to contain zero.");

    // Compute floating-point determinant (has rounding errors). The
    // result could change depending on the code the compiler generates
    // for the floating-point arithmetic.
    ComputeDeterminant(A, fdet);
    UTAssert(fdet == -2.14204192e-08f, "Invalid determinant (floating-point issue)");

    // Compute rational determinant (exact).
    ComputeDeterminant(A, rdet);
    UTAssert(
        rdet.GetSign() == -1 &&
        rdet.GetBiasedExponent() == -102 &&
        rdet.GetUInteger().GetNumBits() == 76 &&
        rdet.GetUInteger().GetBits()[0] == 0x2020a847 &&
        rdet.GetUInteger().GetBits()[1] == 0x5ac184cd &&
        rdet.GetUInteger().GetBits()[2] == 0x00000a4f,
        "Invalid exact determinant.");

    convert = rdet;
    UTAssert(convert == -9.60191571e-09f, "Invalid convert (floating-point issue)");
}

void UnitTestSWInterval::IntervalExampleDifferentTypes()
{
    float fe;
    SWInterval<float> intervalFE;
    IntervalExampleCompute(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, fe, intervalFE);
    UTAssert(fe == 13.9906101f &&
        intervalFE[0] == 13.9906073f && intervalFE[1] == 13.9906130f,
        "Invalid interval result.");

    double de;
    SWInterval<double> intervalDE;
    IntervalExampleCompute(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, de, intervalDE);
    UTAssert(de == 13.990610328638498 &&
        intervalDE[0] == 13.990610328638493 && intervalDE[1] == 13.990610328638503,
        "Invalid interval result.");
}

#else

#include <GTL/Mathematics/Arithmetic/SWInterval.h>

namespace gtl
{
    template class SWInterval<float>;
    template class SWInterval<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(SWInterval)
