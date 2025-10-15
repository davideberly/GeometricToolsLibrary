// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.15

#pragma once

#include <GTL/Mathematics/Algebra/Vector.h>
#include <cstddef>
#include <limits>
#include <vector>

namespace gtl
{
    template <typename T, std::size_t N>
    class ParticleSystem
    {
    public:
        // Construction and destruction. If a particle is to be immovable,
        // set its mass to std::numeric_limits<T>::max().
        virtual ~ParticleSystem() = default;

        ParticleSystem(std::size_t numParticles, T const& step)
            :
            mNumParticles(numParticles),
            mMass(numParticles, C_<T>(0)),
            mInvMass(numParticles, C_<T>(0)),
            mPosition(numParticles, Vector<T, N>::Zero()),
            mVelocity(numParticles, Vector<T, N>::Zero()),
            mStep(step),
            mHalfStep(C_<T>(1, 2) * step),
            mSixthStep(C_<T>(1, 6) * step),
            mPTmp(numParticles),
            mVTmp(numParticles),
            mPAllTmp(numParticles),
            mVAllTmp(numParticles)
        {
        }

        // Member access.
        inline std::size_t GetNumParticles() const
        {
            return mNumParticles;
        }

        void SetMass(std::size_t i, T const& mass)
        {
            if (C_<T>(0) < mass && mass < std::numeric_limits<T>::max())
            {
                mMass[i] = mass;
                mInvMass[i] = C_<T>(1) / mass;
            }
            else
            {
                mMass[i] = std::numeric_limits<T>::max();
                mInvMass[i] = C_<T>(0);
            }
        }

        inline void SetPosition(std::size_t i, Vector<T, N> const& position)
        {
            mPosition[i] = position;
        }

        inline void SetVelocity(std::size_t i, Vector<T, N> const& velocity)
        {
            mVelocity[i] = velocity;
        }

        void SetStep(T const& step)
        {
            mStep = step;
            mHalfStep = mStep / (T)2;
            mSixthStep = mStep / (T)6;
        }

        inline T const& GetMass(std::size_t i) const
        {
            return mMass[i];
        }

        inline Vector<T, N> const& GetPosition(std::size_t i) const
        {
            return mPosition[i];
        }

        inline Vector<T, N> const& GetVelocity(std::size_t i) const
        {
            return mVelocity[i];
        }

        inline T GetStep() const
        {
            return mStep;
        }

        // Update the particle positions based on current time and particle
        // state. The Acceleration(...) function is called in this update
        // for each particle. This function is virtual so that derived
        // classes can perform pre-update and/or post-update semantics.
        virtual void Update(T const& time)
        {
            // Runge-Kutta fourth-order solver.
            T halfTime = time + mHalfStep;
            T fullTime = time + mStep;

            // Compute the first step.
            for (std::size_t i = 0; i < mNumParticles; ++i)
            {
                if (mInvMass[i] > C_<T>(0))
                {
                    mPAllTmp[i].d1 = mVelocity[i];
                    mVAllTmp[i].d1 = Acceleration(i, time, mPosition, mVelocity);
                }
            }
            for (std::size_t i = 0; i < mNumParticles; ++i)
            {
                if (mInvMass[i] > C_<T>(0))
                {
                    mPTmp[i] = mPosition[i] + mHalfStep * mPAllTmp[i].d1;
                    mVTmp[i] = mVelocity[i] + mHalfStep * mVAllTmp[i].d1;
                }
                else
                {
                    mPTmp[i] = mPosition[i];
                    MakeZero(mVTmp[i]);
                }
            }

            // Compute the second step.
            for (std::size_t i = 0; i < mNumParticles; ++i)
            {
                if (mInvMass[i] > C_<T>(0))
                {
                    mPAllTmp[i].d2 = mVTmp[i];
                    mVAllTmp[i].d2 = Acceleration(i, halfTime, mPTmp, mVTmp);
                }
            }
            for (std::size_t i = 0; i < mNumParticles; ++i)
            {
                if (mInvMass[i] > C_<T>(0))
                {
                    mPTmp[i] = mPosition[i] + mHalfStep * mPAllTmp[i].d2;
                    mVTmp[i] = mVelocity[i] + mHalfStep * mVAllTmp[i].d2;
                }
                else
                {
                    mPTmp[i] = mPosition[i];
                    MakeZero(mVTmp[i]);
                }
            }

            // Compute the third step.
            for (std::size_t i = 0; i < mNumParticles; ++i)
            {
                if (mInvMass[i] > C_<T>(0))
                {
                    mPAllTmp[i].d3 = mVTmp[i];
                    mVAllTmp[i].d3 = Acceleration(i, halfTime, mPTmp, mVTmp);
                }
            }
            for (std::size_t i = 0; i < mNumParticles; ++i)
            {
                if (mInvMass[i] > C_<T>(0))
                {
                    mPTmp[i] = mPosition[i] + mStep * mPAllTmp[i].d3;
                    mVTmp[i] = mVelocity[i] + mStep * mVAllTmp[i].d3;
                }
                else
                {
                    mPTmp[i] = mPosition[i];
                    MakeZero(mVTmp[i]);
                }
            }

            // Compute the fourth step.
            for (std::size_t i = 0; i < mNumParticles; ++i)
            {
                if (mInvMass[i] > C_<T>(0))
                {
                    mPAllTmp[i].d4 = mVTmp[i];
                    mVAllTmp[i].d4 = Acceleration(i, fullTime, mPTmp, mVTmp);
                }
            }
            for (std::size_t i = 0; i < mNumParticles; ++i)
            {
                if (mInvMass[i] > C_<T>(0))
                {
                    mPosition[i] += mSixthStep * (mPAllTmp[i].d1 +
                        C_<T>(2) * (mPAllTmp[i].d2 + mPAllTmp[i].d3) + mPAllTmp[i].d4);

                    mVelocity[i] += mSixthStep * (mVAllTmp[i].d1 +
                        C_<T>(2) * (mVAllTmp[i].d2 + mVAllTmp[i].d3) + mVAllTmp[i].d4);
                }
            }
        }

    protected:
        // Callback for acceleration (ODE solver uses x" = F/m) applied to
        // particle i.  The positions and velocities are not necessarily
        // mPosition and mVelocity, because the ODE solver evaluates the
        // impulse function at intermediate positions.
        virtual Vector<T, N> Acceleration(std::size_t i, T const& time,
            std::vector<Vector<T, N>> const& position,
            std::vector<Vector<T, N>> const& velocity) = 0;

        std::size_t mNumParticles;
        std::vector<T> mMass, mInvMass;
        std::vector<Vector<T, N>> mPosition, mVelocity;
        T mStep, mHalfStep, mSixthStep;

        // Temporary storage for the Runge-Kutta differential equation solver.
        struct Temporary
        {
            Temporary()
                :
                d1{},
                d2{},
                d3{},
                d4{}
            {
            }

            Vector<T, N> d1, d2, d3, d4;
        };

        std::vector<Vector<T, N>> mPTmp, mVTmp;
        std::vector<Temporary> mPAllTmp, mVAllTmp;

    private:
        friend class UnitTestParticleSystem;
    };
}
