// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Functions/RotationEstimate.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestRotationEstimate
    {
    public:
        UnitTestRotationEstimate();

    private:
        void TestRotationEstimate();
        void TestRotationDerivativeEstimate();
        void TestRotationAndDerivativeEstimate();

        std::default_random_engine mDRE;
        std::uniform_real_distribution<double> mURD;
    };
}

UnitTestRotationEstimate::UnitTestRotationEstimate()
    :
    mURD(-1.0, 1.0)
{
    UTInformation("Mathematics/Functions/RotationEstimate");

    TestRotationEstimate();
    TestRotationDerivativeEstimate();
    TestRotationAndDerivativeEstimate();
}

void UnitTestRotationEstimate::TestRotationEstimate()
{
    Matrix<double, 3, 3> R, estR, diff;
    AxisAngle<double> aa;
    Vector<double, 3> p;
    double error, maxError = 0.0;
    RigidMotion<double> rt;

    for (std::size_t i = 0; i < 1024; ++i)
    {
        for (std::size_t j = 0; j < 3; ++j)
        {
            p[j] = mURD(mDRE);
        }

        RotationEstimate<double, 16>(p, estR);

        aa.angle = Length(p);
        aa.axis = p / aa.angle;
        rt.Convert(aa, R);

        diff = R - estR;
        error = L1Norm(diff);
        if (error > maxError)
        {
            maxError = error;
        }
    }

    UTAssert(maxError <= 1.64e-13, "Incorrect maximum error.");
}

void UnitTestRotationEstimate::TestRotationDerivativeEstimate()
{
    // Transpose(R)*R = I
    // Transpose(R)*dR/ds[k] + R*Transpose(dR/ds[k]) = 0
    std::array<Matrix<double, 3, 3>, 3> estRDer;
    Matrix<double, 3, 3> R, term;
    AxisAngle<double> aa;
    Vector<double, 3> p;
    double error, maxError = 0.0;
    RigidMotion<double> rt;

    for (std::size_t i = 0; i < 1024; ++i)
    {
        for (std::size_t j = 0; j < 3; ++j)
        {
            p[j] = mURD(mDRE);
        }

        RotationDerivativeEstimate<double, 16>(p, estRDer);

        aa.angle = Length(p);
        aa.axis = p / aa.angle;
        rt.Convert(aa, R);

        for (std::size_t k = 0; k < 3; ++k)
        {
            term = MultiplyATB(R, estRDer[k]) + MultiplyATB(estRDer[k], R);
            error = L1Norm(term);
            if (error > maxError)
            {
                maxError = error;
            }
        }
    }

    UTAssert(maxError <= 1e-12, "Incorrect maximum error.");
}

void UnitTestRotationEstimate::TestRotationAndDerivativeEstimate()
{
    // Transpose(R)*R = I
    // Transpose(R)*dR/ds[k] + R*Transpose(dR/ds[k]) = 0
    std::array<Matrix<double, 3, 3>, 3> estRDer;
    Matrix<double, 3, 3> R, estR, diff, term;
    AxisAngle<double> aa;
    Vector<double, 3> p;
    double error, maxErrorR = 0.0, maxErrorRDer = 0.0;
    RigidMotion<double> rt;

    for (std::size_t i = 0; i < 1024; ++i)
    {
        for (std::size_t j = 0; j < 3; ++j)
        {
            p[j] = mURD(mDRE);
        }

        RotationAndDerivativeEstimate<double, 16>(p, estR, estRDer);

        aa.angle = Length(p);
        aa.axis = p / aa.angle;

        rt.Convert(aa, R);
        diff = R - estR;
        error = L1Norm(diff);
        if (error > maxErrorR)
        {
            maxErrorR = error;
        }

        for (std::size_t k = 0; k < 3; ++k)
        {
            term = MultiplyATB(R, estRDer[k]) + MultiplyATB(estRDer[k], R);
            error = L1Norm(term);
            if (error > maxErrorRDer)
            {
                maxErrorRDer = error;
            }
        }
    }

    UTAssert(maxErrorR <= 1e-12, "Incorrect maximum error.");
    UTAssert(maxErrorRDer <= 1e-12, "Incorrect maximum error.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Functions/RotationEstimate.h>

namespace gtl
{
    using MF33 = Matrix<float, 3, 3>;
    using VF3 = Vector<float, 3>;

    template float RotC0Estimate<float, 4>(float);
    template float RotC0Estimate<float, 6>(float);
    template float RotC0Estimate<float, 8>(float);
    template float RotC0Estimate<float, 10>(float);
    template float RotC0Estimate<float, 12>(float);
    template float RotC0Estimate<float, 14>(float);
    template float RotC0Estimate<float, 16>(float);
    template float GetRotC0EstimateMaxError<float, 4>();
    template float GetRotC0EstimateMaxError<float, 6>();
    template float GetRotC0EstimateMaxError<float, 8>();
    template float GetRotC0EstimateMaxError<float, 10>();
    template float GetRotC0EstimateMaxError<float, 12>();
    template float GetRotC0EstimateMaxError<float, 14>();
    template float GetRotC0EstimateMaxError<float, 16>();

    template float RotC1Estimate<float, 4>(float);
    template float RotC1Estimate<float, 6>(float);
    template float RotC1Estimate<float, 8>(float);
    template float RotC1Estimate<float, 10>(float);
    template float RotC1Estimate<float, 12>(float);
    template float RotC1Estimate<float, 14>(float);
    template float RotC1Estimate<float, 16>(float);
    template float GetRotC1EstimateMaxError<float, 4>();
    template float GetRotC1EstimateMaxError<float, 6>();
    template float GetRotC1EstimateMaxError<float, 8>();
    template float GetRotC1EstimateMaxError<float, 10>();
    template float GetRotC1EstimateMaxError<float, 12>();
    template float GetRotC1EstimateMaxError<float, 14>();
    template float GetRotC1EstimateMaxError<float, 16>();

    template float RotC2Estimate<float, 4>(float);
    template float RotC2Estimate<float, 6>(float);
    template float RotC2Estimate<float, 8>(float);
    template float RotC2Estimate<float, 10>(float);
    template float RotC2Estimate<float, 12>(float);
    template float RotC2Estimate<float, 14>(float);
    template float RotC2Estimate<float, 16>(float);
    template float GetRotC2EstimateMaxError<float, 4>();
    template float GetRotC2EstimateMaxError<float, 6>();
    template float GetRotC2EstimateMaxError<float, 8>();
    template float GetRotC2EstimateMaxError<float, 10>();
    template float GetRotC2EstimateMaxError<float, 12>();
    template float GetRotC2EstimateMaxError<float, 14>();
    template float GetRotC2EstimateMaxError<float, 16>();

    template float RotC3Estimate<float, 4>(float);
    template float RotC3Estimate<float, 6>(float);
    template float RotC3Estimate<float, 8>(float);
    template float RotC3Estimate<float, 10>(float);
    template float RotC3Estimate<float, 12>(float);
    template float RotC3Estimate<float, 14>(float);
    template float RotC3Estimate<float, 16>(float);
    template float GetRotC3EstimateMaxError<float, 4>();
    template float GetRotC3EstimateMaxError<float, 6>();
    template float GetRotC3EstimateMaxError<float, 8>();
    template float GetRotC3EstimateMaxError<float, 10>();
    template float GetRotC3EstimateMaxError<float, 12>();
    template float GetRotC3EstimateMaxError<float, 14>();
    template float GetRotC3EstimateMaxError<float, 16>();

    template float RotC4Estimate<float, 4>(float);
    template float RotC4Estimate<float, 6>(float);
    template float RotC4Estimate<float, 8>(float);
    template float RotC4Estimate<float, 10>(float);
    template float RotC4Estimate<float, 12>(float);
    template float RotC4Estimate<float, 14>(float);
    template float RotC4Estimate<float, 16>(float);
    template float GetRotC4EstimateMaxError<float, 4>();
    template float GetRotC4EstimateMaxError<float, 6>();
    template float GetRotC4EstimateMaxError<float, 8>();
    template float GetRotC4EstimateMaxError<float, 10>();
    template float GetRotC4EstimateMaxError<float, 12>();
    template float GetRotC4EstimateMaxError<float, 14>();
    template float GetRotC4EstimateMaxError<float, 16>();

    template void RotationEstimate<float, 4>(VF3 const&, MF33&);
    template void RotationEstimate<float, 6>(VF3 const&, MF33&);
    template void RotationEstimate<float, 8>(VF3 const&, MF33&);
    template void RotationEstimate<float, 10>(VF3 const&, MF33&);
    template void RotationEstimate<float, 12>(VF3 const&, MF33&);
    template void RotationEstimate<float, 14>(VF3 const&, MF33&);
    template void RotationEstimate<float, 16>(VF3 const&, MF33&);

    template void RotationDerivativeEstimate<float, 4>(VF3 const&, std::array<MF33, 3>&);
    template void RotationDerivativeEstimate<float, 6>(VF3 const&, std::array<MF33, 3>&);
    template void RotationDerivativeEstimate<float, 8>(VF3 const&, std::array<MF33, 3>&);
    template void RotationDerivativeEstimate<float, 10>(VF3 const&, std::array<MF33, 3>&);
    template void RotationDerivativeEstimate<float, 12>(VF3 const&, std::array<MF33, 3>&);
    template void RotationDerivativeEstimate<float, 14>(VF3 const&, std::array<MF33, 3>&);
    template void RotationDerivativeEstimate<float, 16>(VF3 const&, std::array<MF33, 3>&);

    template void RotationAndDerivativeEstimate<float, 4>(VF3 const&, MF33&, std::array<MF33, 3>&);
    template void RotationAndDerivativeEstimate<float, 6>(VF3 const&, MF33&, std::array<MF33, 3>&);
    template void RotationAndDerivativeEstimate<float, 8>(VF3 const&, MF33&, std::array<MF33, 3>&);
    template void RotationAndDerivativeEstimate<float, 10>(VF3 const&, MF33&, std::array<MF33, 3>&);
    template void RotationAndDerivativeEstimate<float, 12>(VF3 const&, MF33&, std::array<MF33, 3>&);
    template void RotationAndDerivativeEstimate<float, 14>(VF3 const&, MF33&, std::array<MF33, 3>&);
    template void RotationAndDerivativeEstimate<float, 16>(VF3 const&, MF33&, std::array<MF33, 3>&);

    using MD33 = Matrix<double, 3, 3>;
    using VD3 = Vector<double, 3>;

    template double RotC0Estimate<double, 4>(double);
    template double RotC0Estimate<double, 6>(double);
    template double RotC0Estimate<double, 8>(double);
    template double RotC0Estimate<double, 10>(double);
    template double RotC0Estimate<double, 12>(double);
    template double RotC0Estimate<double, 14>(double);
    template double RotC0Estimate<double, 16>(double);
    template double GetRotC0EstimateMaxError<double, 4>();
    template double GetRotC0EstimateMaxError<double, 6>();
    template double GetRotC0EstimateMaxError<double, 8>();
    template double GetRotC0EstimateMaxError<double, 10>();
    template double GetRotC0EstimateMaxError<double, 12>();
    template double GetRotC0EstimateMaxError<double, 14>();
    template double GetRotC0EstimateMaxError<double, 16>();

    template double RotC1Estimate<double, 4>(double);
    template double RotC1Estimate<double, 6>(double);
    template double RotC1Estimate<double, 8>(double);
    template double RotC1Estimate<double, 10>(double);
    template double RotC1Estimate<double, 12>(double);
    template double RotC1Estimate<double, 14>(double);
    template double RotC1Estimate<double, 16>(double);
    template double GetRotC1EstimateMaxError<double, 4>();
    template double GetRotC1EstimateMaxError<double, 6>();
    template double GetRotC1EstimateMaxError<double, 8>();
    template double GetRotC1EstimateMaxError<double, 10>();
    template double GetRotC1EstimateMaxError<double, 12>();
    template double GetRotC1EstimateMaxError<double, 14>();
    template double GetRotC1EstimateMaxError<double, 16>();

    template double RotC2Estimate<double, 4>(double);
    template double RotC2Estimate<double, 6>(double);
    template double RotC2Estimate<double, 8>(double);
    template double RotC2Estimate<double, 10>(double);
    template double RotC2Estimate<double, 12>(double);
    template double RotC2Estimate<double, 14>(double);
    template double RotC2Estimate<double, 16>(double);
    template double GetRotC2EstimateMaxError<double, 4>();
    template double GetRotC2EstimateMaxError<double, 6>();
    template double GetRotC2EstimateMaxError<double, 8>();
    template double GetRotC2EstimateMaxError<double, 10>();
    template double GetRotC2EstimateMaxError<double, 12>();
    template double GetRotC2EstimateMaxError<double, 14>();
    template double GetRotC2EstimateMaxError<double, 16>();

    template double RotC3Estimate<double, 4>(double);
    template double RotC3Estimate<double, 6>(double);
    template double RotC3Estimate<double, 8>(double);
    template double RotC3Estimate<double, 10>(double);
    template double RotC3Estimate<double, 12>(double);
    template double RotC3Estimate<double, 14>(double);
    template double RotC3Estimate<double, 16>(double);
    template double GetRotC3EstimateMaxError<double, 4>();
    template double GetRotC3EstimateMaxError<double, 6>();
    template double GetRotC3EstimateMaxError<double, 8>();
    template double GetRotC3EstimateMaxError<double, 10>();
    template double GetRotC3EstimateMaxError<double, 12>();
    template double GetRotC3EstimateMaxError<double, 14>();
    template double GetRotC3EstimateMaxError<double, 16>();

    template void RotationEstimate<double, 4>(VD3 const&, MD33&);
    template void RotationEstimate<double, 6>(VD3 const&, MD33&);
    template void RotationEstimate<double, 8>(VD3 const&, MD33&);
    template void RotationEstimate<double, 10>(VD3 const&, MD33&);
    template void RotationEstimate<double, 12>(VD3 const&, MD33&);
    template void RotationEstimate<double, 14>(VD3 const&, MD33&);
    template void RotationEstimate<double, 16>(VD3 const&, MD33&);

    template void RotationDerivativeEstimate<double, 4>(VD3 const&, std::array<MD33, 3>&);
    template void RotationDerivativeEstimate<double, 6>(VD3 const&, std::array<MD33, 3>&);
    template void RotationDerivativeEstimate<double, 8>(VD3 const&, std::array<MD33, 3>&);
    template void RotationDerivativeEstimate<double, 10>(VD3 const&, std::array<MD33, 3>&);
    template void RotationDerivativeEstimate<double, 12>(VD3 const&, std::array<MD33, 3>&);
    template void RotationDerivativeEstimate<double, 14>(VD3 const&, std::array<MD33, 3>&);
    template void RotationDerivativeEstimate<double, 16>(VD3 const&, std::array<MD33, 3>&);

    template void RotationAndDerivativeEstimate<double, 4>(VD3 const&, MD33&, std::array<MD33, 3>&);
    template void RotationAndDerivativeEstimate<double, 6>(VD3 const&, MD33&, std::array<MD33, 3>&);
    template void RotationAndDerivativeEstimate<double, 8>(VD3 const&, MD33&, std::array<MD33, 3>&);
    template void RotationAndDerivativeEstimate<double, 10>(VD3 const&, MD33&, std::array<MD33, 3>&);
    template void RotationAndDerivativeEstimate<double, 12>(VD3 const&, MD33&, std::array<MD33, 3>&);
    template void RotationAndDerivativeEstimate<double, 14>(VD3 const&, MD33&, std::array<MD33, 3>&);
    template void RotationAndDerivativeEstimate<double, 16>(VD3 const&, MD33&, std::array<MD33, 3>&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    using MR33 = Matrix<Rational, 3, 3>;
    using VR3 = Vector<Rational, 3>;

    template Rational RotC0Estimate<Rational, 4>(Rational);
    template Rational RotC0Estimate<Rational, 6>(Rational);
    template Rational RotC0Estimate<Rational, 8>(Rational);
    template Rational RotC0Estimate<Rational, 10>(Rational);
    template Rational RotC0Estimate<Rational, 12>(Rational);
    template Rational RotC0Estimate<Rational, 14>(Rational);
    template Rational RotC0Estimate<Rational, 16>(Rational);
    template Rational GetRotC0EstimateMaxError<Rational, 4>();
    template Rational GetRotC0EstimateMaxError<Rational, 6>();
    template Rational GetRotC0EstimateMaxError<Rational, 8>();
    template Rational GetRotC0EstimateMaxError<Rational, 10>();
    template Rational GetRotC0EstimateMaxError<Rational, 12>();
    template Rational GetRotC0EstimateMaxError<Rational, 14>();
    template Rational GetRotC0EstimateMaxError<Rational, 16>();

    template Rational RotC1Estimate<Rational, 4>(Rational);
    template Rational RotC1Estimate<Rational, 6>(Rational);
    template Rational RotC1Estimate<Rational, 8>(Rational);
    template Rational RotC1Estimate<Rational, 10>(Rational);
    template Rational RotC1Estimate<Rational, 12>(Rational);
    template Rational RotC1Estimate<Rational, 14>(Rational);
    template Rational RotC1Estimate<Rational, 16>(Rational);
    template Rational GetRotC1EstimateMaxError<Rational, 4>();
    template Rational GetRotC1EstimateMaxError<Rational, 6>();
    template Rational GetRotC1EstimateMaxError<Rational, 8>();
    template Rational GetRotC1EstimateMaxError<Rational, 10>();
    template Rational GetRotC1EstimateMaxError<Rational, 12>();
    template Rational GetRotC1EstimateMaxError<Rational, 14>();
    template Rational GetRotC1EstimateMaxError<Rational, 16>();

    template Rational RotC2Estimate<Rational, 4>(Rational);
    template Rational RotC2Estimate<Rational, 6>(Rational);
    template Rational RotC2Estimate<Rational, 8>(Rational);
    template Rational RotC2Estimate<Rational, 10>(Rational);
    template Rational RotC2Estimate<Rational, 12>(Rational);
    template Rational RotC2Estimate<Rational, 14>(Rational);
    template Rational RotC2Estimate<Rational, 16>(Rational);
    template Rational GetRotC2EstimateMaxError<Rational, 4>();
    template Rational GetRotC2EstimateMaxError<Rational, 6>();
    template Rational GetRotC2EstimateMaxError<Rational, 8>();
    template Rational GetRotC2EstimateMaxError<Rational, 10>();
    template Rational GetRotC2EstimateMaxError<Rational, 12>();
    template Rational GetRotC2EstimateMaxError<Rational, 14>();
    template Rational GetRotC2EstimateMaxError<Rational, 16>();

    template Rational RotC3Estimate<Rational, 4>(Rational);
    template Rational RotC3Estimate<Rational, 6>(Rational);
    template Rational RotC3Estimate<Rational, 8>(Rational);
    template Rational RotC3Estimate<Rational, 10>(Rational);
    template Rational RotC3Estimate<Rational, 12>(Rational);
    template Rational RotC3Estimate<Rational, 14>(Rational);
    template Rational RotC3Estimate<Rational, 16>(Rational);
    template Rational GetRotC3EstimateMaxError<Rational, 4>();
    template Rational GetRotC3EstimateMaxError<Rational, 6>();
    template Rational GetRotC3EstimateMaxError<Rational, 8>();
    template Rational GetRotC3EstimateMaxError<Rational, 10>();
    template Rational GetRotC3EstimateMaxError<Rational, 12>();
    template Rational GetRotC3EstimateMaxError<Rational, 14>();
    template Rational GetRotC3EstimateMaxError<Rational, 16>();

    template void RotationEstimate<Rational, 4>(VR3 const&, MR33&);
    template void RotationEstimate<Rational, 6>(VR3 const&, MR33&);
    template void RotationEstimate<Rational, 8>(VR3 const&, MR33&);
    template void RotationEstimate<Rational, 10>(VR3 const&, MR33&);
    template void RotationEstimate<Rational, 12>(VR3 const&, MR33&);
    template void RotationEstimate<Rational, 14>(VR3 const&, MR33&);
    template void RotationEstimate<Rational, 16>(VR3 const&, MR33&);

    template void RotationDerivativeEstimate<Rational, 4>(VR3 const&, std::array<MR33, 3>&);
    template void RotationDerivativeEstimate<Rational, 6>(VR3 const&, std::array<MR33, 3>&);
    template void RotationDerivativeEstimate<Rational, 8>(VR3 const&, std::array<MR33, 3>&);
    template void RotationDerivativeEstimate<Rational, 10>(VR3 const&, std::array<MR33, 3>&);
    template void RotationDerivativeEstimate<Rational, 12>(VR3 const&, std::array<MR33, 3>&);
    template void RotationDerivativeEstimate<Rational, 14>(VR3 const&, std::array<MR33, 3>&);
    template void RotationDerivativeEstimate<Rational, 16>(VR3 const&, std::array<MR33, 3>&);

    template void RotationAndDerivativeEstimate<Rational, 4>(VR3 const&, MR33&, std::array<MR33, 3>&);
    template void RotationAndDerivativeEstimate<Rational, 6>(VR3 const&, MR33&, std::array<MR33, 3>&);
    template void RotationAndDerivativeEstimate<Rational, 8>(VR3 const&, MR33&, std::array<MR33, 3>&);
    template void RotationAndDerivativeEstimate<Rational, 10>(VR3 const&, MR33&, std::array<MR33, 3>&);
    template void RotationAndDerivativeEstimate<Rational, 12>(VR3 const&, MR33&, std::array<MR33, 3>&);
    template void RotationAndDerivativeEstimate<Rational, 14>(VR3 const&, MR33&, std::array<MR33, 3>&);
    template void RotationAndDerivativeEstimate<Rational, 16>(VR3 const&, MR33&, std::array<MR33, 3>&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RotationEstimate)
