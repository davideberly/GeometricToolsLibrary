// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.02.16

#pragma once

// Test for intersection of two finite cylinders using the method of
// separating axes. The algorithm is described in the document
// https://www.geometrictools.com/Documentation/IntersectionOfCylinders.pdf

#include <GTL/Utility/Exceptions.h>
#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Mathematics/Primitives/ND/Cylinder.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <atomic>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <thread>
#include <vector>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Cylinder3<T>, Cylinder3<T>>
    {
    public:
        // Execute in the main thread when numThreads <= 1; otherwise,
        // execute in the specified number of threads. The potential
        // separating directions are
        //   D(theta[i],phi[j]) = c0*s1 * U + s0*s1 * V + c1 * N
        // where {U,V,N} is a right-handed orthonormal basis with N the
        // north pole of a hemisphere. The parameters are theta[i] =
        // 2 * pi * i / numTheta with 0 <= i < numTheta, phi[j] =
        // pi * j / numPhi with 0 <= j < numPhi, c0 = cos(theta[i]),
        // s0 = sin(theta[i]), c1 = cos(phi[j]), and s1 = sin(phi[j]).
        TIQuery(std::uint32_t numThreads, std::size_t numTheta, std::size_t numPhi)
            :
            mNumThreads(numThreads),
            mNumTheta(numTheta),
            mNumPhi(numPhi),
            mW0{},
            mR0(static_cast<T>(0)),
            mHalfH0(static_cast<T>(0)),
            mW1{},
            mR1(static_cast<T>(0)),
            mHalfH1(static_cast<T>(0)),
            mDelta{},
            mW0xW1{},
            mBasis{}
        {
            GTL_ARGUMENT_ASSERT(
                numTheta > 0 && numPhi > 0,
                "Invalid number of angles.");
        }

        struct Output
        {
            Output()
                :
                separated(false),
                separatingDirection{}
            {
            }

            bool separated;
            Vector3<T> separatingDirection;
        };

        Output operator()(Cylinder3<T> const& cylinder0, Cylinder3<T> const& cylinder1)
        {
            // The constructor sets output.separated to false and
            // output.separatingDirection to (0,0,0).
            Output output{};
            T const zero = static_cast<T>(0);
            T const half = static_cast<T>(0.5);

            mDelta = cylinder1.center - cylinder0.center;
            if (Length(mDelta) == zero)
            {
                return output;
            }

            mW0 = cylinder0.direction;
            mR0 = cylinder0.radius;
            mHalfH0 = half * cylinder0.height;
            mW1 = cylinder1.direction;
            mR1 = cylinder1.radius;
            mHalfH1 = half * cylinder1.height;
            mW0xW1 = Cross(mW0, mW1);
            T lengthW0xW1 = Length(mW0xW1);
            if (lengthW0xW1 > zero)
            {
                // The cylinder directions are not parallel.

                // Test for separation by W0.
                T absDotW0W1 = std::fabs(Dot(mW0, mW1));
                T absDotW0Delta = std::fabs(Dot(mW0, mDelta));
                T test = mR1 * lengthW0xW1 + mHalfH0 + mHalfH1 * absDotW0W1 - absDotW0Delta;
                if (test < zero)
                {
                    output.separated = true;
                    output.separatingDirection = mW0;
                    return output;
                }

                // Test for separation by W1.
                T absDotW1Delta = std::fabs(Dot(mW1, mDelta));
                test = mR0 * lengthW0xW1 + mHalfH0 * absDotW0W1 + mHalfH1 - absDotW1Delta;
                if (test < zero)
                {
                    output.separated = true;
                    output.separatingDirection = mW1;
                    return output;
                }

                // Test for separation by W0xW1.
                T absDotW0xW1Delta = std::fabs(Dot(mW0xW1, mDelta));
                test = (mR0 + mR1) * lengthW0xW1 - absDotW0xW1Delta;
                if (test < zero)
                {
                    output.separated = true;
                    output.separatingDirection = mW0xW1;
                    Normalize(output.separatingDirection);
                    return output;
                }

                // Test for separation by Delta.
                test = mR0 * Length(Cross(mDelta, mW0)) + mR1 * Length(Cross(mDelta, mW1)) +
                    mHalfH0 * absDotW0Delta + mHalfH1 * absDotW1Delta - Dot(mDelta, mDelta);
                if (test < zero)
                {
                    output.separated = true;
                    output.separatingDirection = mDelta;
                    Normalize(output.separatingDirection);
                    return output;
                }

                // Test for separation by other directions.
                if (mNumThreads <= 1)
                {
                    TestForSeparationSingleThreaded(output);
                }
                else
                {
                    TestForSeparationMultithreaded(output);
                }
            }
            else
            {
                // The cylinder directions are parallel.

                // Test for separation by height.
                T dotDeltaW0 = Dot(mDelta, mW0);
                T test = mHalfH0 + mHalfH1 - std::fabs(dotDeltaW0);
                if (test < zero)
                {
                    output.separated = true;
                    output.separatingDirection = mW0;
                    return output;
                }

                // Test for separation radially.
                test = mR0 + mR1 - Length(Cross(mDelta, mW0));
                if (test < zero)
                {
                    output.separated = true;
                    output.separatingDirection = mDelta - dotDeltaW0 * mW0;
                    Normalize(output.separatingDirection);
                    return output;
                }
            }

            return output;
        }

    private:
        void TestForSeparationSingleThreaded(Output& output)
        {
            // Compute a right-handed orthonormal basis {U,V,N} so that N is
            // the north pole of a hemisphere.
            std::array<Vector3<T>, 3> basis{};
            basis[0] = mDelta;
            ComputeOrthogonalComplement(basis[0], basis[1], basis[2]);
            Vector3<T> const& U = basis[1];
            Vector3<T> const& V = basis[2];
            Vector3<T> const& N = basis[0];

            // Sample the hemisphere for potential separating directions.
            T phiMultiplier = C_PI_DIV_2<T> / static_cast<T>(mNumPhi);
            T thetaMultiplier = C_TWO_PI<T> / static_cast<T>(mNumTheta);
            for (std::size_t j = 1; j < mNumPhi; ++j)
            {
                T phi = phiMultiplier * static_cast<T>(j);
                T c1 = std::cos(phi);
                T s1 = std::sin(phi);
                for (std::size_t i = 0; i < mNumTheta; ++i)
                {
                    // Compute the potential separating dimension.
                    T theta = thetaMultiplier * static_cast<T>(i);
                    T c0 = std::cos(theta);
                    T s0 = std::sin(theta);
                    Vector3<T> D = (c0 * s1) * U + (s0 * s1) * V + c1 * N;

                    // Test for separation. If test is negative, the direction
                    // is separating.
                    T test =
                        mR0 * Length(Cross(mW0, D)) + mR1 * Length(Cross(mW1, D)) +
                        mHalfH0 * std::fabs(Dot(mW0, D)) + mHalfH1 * std::fabs(Dot(mW1, D)) -
                        std::fabs(Dot(mDelta, D));
                    if (test < static_cast<T>(0))
                    {
                        output.separated = true;
                        output.separatingDirection = D;
                        return;
                    }
                }
            }
        }

        void TestForSeparationMultithreaded(Output& output)
        {
            // Compute a right-handed orthonormal basis {U,V,N} so that N is
            // the north pole of a hemisphere.
            std::array<Vector3<T>, 3> basis{};
            basis[0] = mDelta;
            ComputeOrthogonalComplement(basis[0], basis[1], basis[2]);
            Vector3<T> const& U = basis[1];
            Vector3<T> const& V = basis[2];
            Vector3<T> const& N = basis[0];

            // Sample the hemisphere for potential separating directions.
            // Distribute the computing across multiple threads.
            T phiMultiplier = C_PI_DIV_2<T> / static_cast<T>(mNumPhi);
            T thetaMultiplier = C_TWO_PI<T> / static_cast<T>(mNumTheta);
            std::vector<std::size_t> jmin(mNumThreads), jsup(mNumThreads);
            size_t numPhiPerThread = mNumPhi / mNumThreads;
            for (size_t t = 0; t < mNumThreads; ++t)
            {
                jmin[t] = numPhiPerThread * t;
                jsup[t] = numPhiPerThread * (t + 1);
            }
            jmin.front() = 1;
            jsup.back() = mNumPhi;

            std::vector<Output> localOutput(mNumThreads);
            std::atomic<std::uint32_t> foundSeparatingDirection(0);

            std::vector<std::thread> process(mNumThreads);
            for (size_t t = 0; t < mNumThreads; ++t)
            {
                process[t] = std::thread
                (
                    [this, phiMultiplier, thetaMultiplier, t, &jmin, &jsup, &U, &V, &N,
                    &localOutput, &foundSeparatingDirection]()
                    {
                        if (foundSeparatingDirection)
                        {
                            return;
                        }

                        for (std::size_t j = jmin[t]; j < jsup[t]; ++j)
                        {
                            T phi = phiMultiplier * static_cast<T>(j);
                            T c1 = std::cos(phi);
                            T s1 = std::sin(phi);
                            for (std::size_t i = 0; i < mNumTheta; ++i)
                            {
                                // Compute the potential separating dimension.
                                T theta = thetaMultiplier * static_cast<T>(i);
                                T c0 = std::cos(theta);
                                T s0 = std::sin(theta);
                                Vector3<T> D = (c0 * s1) * U + (s0 * s1) * V + c1 * N;

                                // Test for separation. If test is negative, the direction
                                // is separating.
                                T test =
                                    mR0 * Length(Cross(mW0, D)) + mR1 * Length(Cross(mW1, D)) +
                                    mHalfH0 * std::fabs(Dot(mW0, D)) + mHalfH1 * std::fabs(Dot(mW1, D)) -
                                    std::fabs(Dot(mDelta, D));
                                if (test < static_cast<T>(0))
                                {
                                    localOutput[t].separated = true;
                                    localOutput[t].separatingDirection = D;
                                    foundSeparatingDirection = true;
                                    return;
                                }
                            }
                        }
                    }
                );
            }

            for (size_t t = 0; t < mNumThreads; ++t)
            {
                process[t].join();

                if (foundSeparatingDirection && localOutput[t].separated)
                {
                    output = localOutput[t];
                    foundSeparatingDirection = false;
                }
            }
        }

        std::uint32_t mNumThreads;
        std::size_t mNumTheta;
        std::size_t mNumPhi;

        // Cylinder 0.
        Vector3<T> mW0; // W0
        T mR0;          // r0
        T mHalfH0;      // h0/2

        // Cylinder 1.
        Vector3<T> mW1; // W1
        T mR1;          // r1
        T mHalfH1;      // h1/2

        // Members dependent on both cylinders.
        Vector3<T> mDelta;  // C1 - C0 (difference of centers)
        Vector3<T> mW0xW1;  // Cross(W0, W1);
        std::array<Vector3<T>, 3> mBasis;  // { U, V, N }

    private:
        friend class UnitTestIntrCylinder3Cylinder3;
    };
}
