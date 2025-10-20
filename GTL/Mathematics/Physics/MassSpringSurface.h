// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.20

#pragma once

// MassSpringSurface represents an RxC array of masses lying on a surface and
// connected by an array of springs. The masses are indexed by mass[r][c] for
// 0 <= r < R and 0 <= c < C. The mass at interior position X[r][c] is
// connected by springs to the masses at positions X[r-1][c], X[r+1][c],
// X[r][c-1] and X[r][c+1]. Boundary masses have springs connecting them to
// the obvious neighbors ("edge" mass has 3 neighbors, "corner" mass has 2
// neighbors). The masses are arranged in row-major order: position[c+C*r] =
// X[r][c] for 0 <= r < R and 0 <= c < C. The other arrays are stored
// similarly.

#include <GTL/Mathematics/Physics/ParticleSystem.h>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T, std::size_t N>
    class MassSpringSurface : public ParticleSystem<T, N>
    {
    public:
        virtual ~MassSpringSurface() = default;

        MassSpringSurface(std::size_t numRows, std::size_t numCols, T const& step)
            :
            ParticleSystem<T, N>(numRows * numCols, step),
            mNumRows(numRows),
            mNumCols(numCols),
            mConstantR(numRows * numCols, C_<T>(0)),
            mLengthR(numRows * numCols, C_<T>(0)),
            mConstantC(numRows * numCols, C_<T>(0)),
            mLengthC(numRows * numCols, C_<T>(0))
        {
        }

        // Member access.
        inline std::size_t GetNumRows() const
        {
            return mNumRows;
        }

        inline std::size_t GetNumCols() const
        {
            return mNumCols;
        }

        inline void SetMass(std::size_t r, std::size_t c, T const& mass)
        {
            ParticleSystem<T, N>::SetMass(GetIndex(r, c), mass);
        }

        inline void SetPosition(std::size_t r, std::size_t c, Vector<T, N> const& position)
        {
            ParticleSystem<T, N>::SetPosition(GetIndex(r, c), position);
        }

        inline void SetVelocity(std::size_t r, std::size_t c, Vector<T, N> const& velocity)
        {
            ParticleSystem<T, N>::SetVelocity(GetIndex(r, c), velocity);
        }

        inline T const& GetMass(std::size_t r, std::size_t c) const
        {
            return ParticleSystem<T, N>::GetMass(GetIndex(r, c));
        }

        inline Vector<T, N> const& GetPosition(std::size_t r, std::size_t c) const
        {
            return ParticleSystem<T, N>::GetPosition(GetIndex(r, c));
        }

        inline Vector<T, N> const& GetVelocity(std::size_t r, std::size_t c) const
        {
            return ParticleSystem<T, N>::GetVelocity(GetIndex(r, c));
        }

        // The interior mass at (r,c) has springs to the left, right, bottom,
        // and top. Edge masses have only three neighbors and corner masses
        // have only two neighbors. The mass at (r,c) provides access to the
        // springs connecting to locations (r,c+1) and (r+1,c) assuming that
        // that r+1 < numRows and c+1 < mNumCols. If a location is not valid,
        // the std::vector objects have extra storage for the information but
        // that information is not used in the simulation.

        // Set constant of spring from (r,c) to (r+1,c).
        inline void SetConstantR(std::size_t r, std::size_t c, T const& constant)
        {
            mConstantR[GetIndex(r, c)] = constant;
        }

        // Set length of spring from (r,c) to (r+1,c).
        inline void SetLengthR(std::size_t r, std::size_t c, T const& length)
        {
            mLengthR[GetIndex(r, c)] = length;
        }

        // Set constant of spring from (r,c) to (r,c+1).
        inline void SetConstantC(std::size_t r, std::size_t c, T const& constant)
        {
            mConstantC[GetIndex(r, c)] = constant;
        }

        // Set length of spring from (r,c) to (r,c+1).
        inline void SetLengthC(std::size_t r, std::size_t c, T const& length)
        {
            mLengthC[GetIndex(r, c)] = length;
        }

        // Get constant of spring from (r,c) to (r+1,c).
        inline T const& GetConstantR(std::size_t r, std::size_t c) const
        {
            return mConstantR[GetIndex(r, c)];
        }

        // Get length of spring from (r,c) to (r+1,c).
        inline T const& GetLengthR(std::size_t r, std::size_t c) const
        {
            return mLengthR[GetIndex(r, c)];
        }

        // Get constant of spring from (r,c) to (r,c+1).
        inline T const& GetConstantC(std::size_t r, std::size_t c) const
        {
            return mConstantC[GetIndex(r, c)];
        }

        // Get length of spring from (r,c) to (r,c+1).
        inline T const& GetLengthC(std::size_t r, std::size_t c) const
        {
            return mLengthC[GetIndex(r, c)];
        }

        // The default external force is zero. Derive a class from this one
        // to provide nonzero external forces such as gravity, wind, friction,
        // and so on. This function is called by Acceleration(...) to compute
        // the impulse F/m generated by the external force F.
        virtual Vector<T, N> ExternalAcceleration(std::size_t, T const&,
            std::vector<Vector<T, N>> const&,
            std::vector<Vector<T, N>> const&)
        {
            return Vector<T, N>::Zero();
        }

    protected:
        // Callback for acceleration (ODE solver uses x" = F/m) applied to
        // particle i. The positions and velocities are not necessarily
        // mPosition and mVelocity, because the ODE solver evaluates the
        // impulse function at intermediate positions.
        virtual Vector<T, N> Acceleration(std::size_t i, T const& time,
            std::vector<Vector<T, N>> const& position,
            std::vector<Vector<T, N>> const& velocity)
        {
            // Compute spring forces on position X[i]. The positions are not
            // necessarily mPosition, because the RK4 solver in ParticleSystem
            // evaluates the acceleration function at intermediate positions.
            // The edge and corner points of the surface of masses must be
            // handled separately, because each has fewer than four springs
            // attached to it.

            Vector<T, N> acceleration = ExternalAcceleration(i, time, position, velocity);
            Vector<T, N> diff{}, force{};
            T ratio{};

            std::size_t r{}, c{}, prev{}, next{};
            GetCoordinates(i, r, c);

            if (r > 0)
            {
                prev = i - mNumCols;  // index to previous row-neighbor
                diff = position[prev] - position[i];
                ratio = GetLengthR(r - 1, c) / Length(diff);
                force = GetConstantR(r - 1, c) * (C_<T>(1) - ratio) * diff;
                acceleration += this->mInvMass[i] * force;
            }

            if (r < mNumRows - 1)
            {
                next = i + mNumCols;  // index to next row-neighbor
                diff = position[next] - position[i];
                ratio = GetLengthR(r, c) / Length(diff);
                force = GetConstantR(r, c) * (C_<T>(1) - ratio) * diff;
                acceleration += this->mInvMass[i] * force;
            }

            if (c > 0)
            {
                prev = i - 1;  // index to previous col-neighbor
                diff = position[prev] - position[i];
                ratio = GetLengthC(r, c - 1) / Length(diff);
                force = GetConstantC(r, c - 1) * (C_<T>(1) - ratio) * diff;
                acceleration += this->mInvMass[i] * force;
            }

            if (c < mNumCols - 1)
            {
                next = i + 1;  // index to next col-neighbor
                diff = position[next] - position[i];
                ratio = GetLengthC(r, c) / Length(diff);
                force = GetConstantC(r, c) * (C_<T>(1) - ratio) * diff;
                acceleration += this->mInvMass[i] * force;
            }

            return acceleration;
        }

        inline std::size_t GetIndex(std::size_t r, std::size_t c) const
        {
            return c + mNumCols * r;
        }

        void GetCoordinates(std::size_t i, std::size_t& r, std::size_t& c) const
        {
            c = i % mNumCols;
            r = i / mNumCols;
        }

        std::size_t mNumRows, mNumCols;
        std::vector<T> mConstantR, mLengthR;
        std::vector<T> mConstantC, mLengthC;

    private:
        friend class UnitTestMassSpringSurface;
    };
}
