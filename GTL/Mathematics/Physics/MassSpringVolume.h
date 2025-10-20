// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.20

#pragma once

// MassSpringVolume represents an SxRxC array of masses lying on in a volume
// and connected by an array of springs. The masses are indexed by
// mass[s][r][c] for 0 <= s < S, 0 <= r < R, and 0 <= c < C. The mass at
// interior position X[s][r][c] is connected by springs to the masses at
// positions X[s][r-1][c], X[s][r+1][c], X[s][r][c-1], X[s][r][c+1],
// X[s-1][r][c] and X[s+1][r][c]. Boundary masses have springs connecting
// them to the obvious neighbors: "face" mass has 5 neighbors, "edge" mass
// has 4 neighbors, "corner" mass has 3 neighbors. The masses are arranged
// in lexicographical order: position[c+C*(r+R*s)] = X[s][r][c] for
// 0 <= s < S, 0 <= r < R, and 0 <= c < C. The other arrays are stored
// similarly.

#include <GTL/Mathematics/Physics/ParticleSystem.h>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T, std::size_t N>
    class MassSpringVolume : public ParticleSystem<T, N>
    {
    public:
        virtual ~MassSpringVolume() = default;

        MassSpringVolume(std::size_t numSlices, std::size_t numRows, std::size_t numCols, T const& step)
            :
            ParticleSystem<T, N>(numSlices * numRows* numCols, step),
            mNumSlices(numSlices),
            mNumRows(numRows),
            mNumCols(numCols),
            mConstantS(numSlices * numRows * numCols, C_<T>(0)),
            mLengthS(numSlices* numRows* numCols, C_<T>(0)),
            mConstantR(numSlices* numRows* numCols, C_<T>(0)),
            mLengthR(numSlices* numRows* numCols, C_<T>(0)),
            mConstantC(numSlices* numRows* numCols, C_<T>(0)),
            mLengthC(numSlices* numRows* numCols, C_<T>(0))
        {
        }

        // Member access.
        inline std::size_t GetNumSlices() const
        {
            return mNumSlices;
        }

        inline std::size_t GetNumRows() const
        {
            return mNumRows;
        }

        inline std::size_t GetNumCols() const
        {
            return mNumCols;
        }

        inline void SetMass(std::size_t s, std::size_t r, std::size_t c, T const& mass)
        {
            ParticleSystem<T, N>::SetMass(GetIndex(s, r, c), mass);
        }

        inline void SetPosition(std::size_t s, std::size_t r, std::size_t c, Vector<T, N> const& position)
        {
            ParticleSystem<T, N>::SetPosition(GetIndex(s, r, c), position);
        }

        inline void SetVelocity(std::size_t s, std::size_t r, std::size_t c, Vector<T, N> const& velocity)
        {
            ParticleSystem<T, N>::SetVelocity(GetIndex(s, r, c), velocity);
        }

        T const& GetMass(std::size_t s, std::size_t r, std::size_t c) const
        {
            return ParticleSystem<T, N>::GetMass(GetIndex(s, r, c));
        }

        inline Vector<T, N> const& GetPosition(std::size_t s, std::size_t r, std::size_t c) const
        {
            return ParticleSystem<T, N>::GetPosition(GetIndex(s, r, c));
        }

        inline Vector<T, N> const& GetVelocity(std::size_t s, std::size_t r, std::size_t c) const
        {
            return ParticleSystem<T, N>::GetVelocity(GetIndex(s, r, c));
        }

        // Each interior mass at (s,r,c) has 6 adjacent springs. Face masses
        // have only 5 neighbors, edge masses have only 4 neighbors, and corner
        // masses have only 3 neighbors. Each mass provides access to 3 adjacent
        // springs at (s,r,c+1), (s,r+1,c), and (s+1,r,c). The face, edge and
        // corner masses provide access to only an appropriate subset of these.
        // The caller is responsible for ensuring the validity of the (s,r,c)
        // inputs.

        // Set constant of spring from (s,r,c) to (s+1,r,c).
        inline void SetConstantS(std::size_t s, std::size_t r, std::size_t c, T const& constant)
        {
            mConstantS[GetIndex(s, r, c)] = constant;
        }

        // Set length of spring from (s,r,c) to (s+1,r,c).
        inline void SetLengthS(std::size_t s, std::size_t r, std::size_t c, T const& length)
        {
            mLengthS[GetIndex(s, r, c)] = length;
        }

        // Set constant of spring from (s,r,c) to (s,r+1,c).
        inline void SetConstantR(std::size_t s, std::size_t r, std::size_t c, T const& constant)
        {
            mConstantR[GetIndex(s, r, c)] = constant;
        }

        // Set length of spring from (s,r,c) to (s,r+1,c).
        inline void SetLengthR(std::size_t s, std::size_t r, std::size_t c, T const& length)
        {
            mLengthR[GetIndex(s, r, c)] = length;
        }

        // Set constant of spring from (s,r,c) to (s,r,c+1).
        inline void SetConstantC(std::size_t s, std::size_t r, std::size_t c, T const& constant)
        {
            mConstantC[GetIndex(s, r, c)] = constant;
        }

        // Set length of spring from (s,r,c) to (s,r,c+1).
        inline void SetLengthC(std::size_t s, std::size_t r, std::size_t c, T const& length)
        {
            mLengthC[GetIndex(s, r, c)] = length;
        }

        // Get constant of spring from (s,r,c) to (s+1,r,c).
        inline T const& GetConstantS(std::size_t s, std::size_t r, std::size_t c) const
        {
            return mConstantS[GetIndex(s, r, c)];
        }

        // Get length of spring from (s,r,c) to (s+1,r,c).
        inline T const& GetLengthS(std::size_t s, std::size_t r, std::size_t c) const
        {
            return mLengthS[GetIndex(s, r, c)];
        }

        // Get constant of spring from (s,r,c) to (s,r+1,c).
        inline T const& GetConstantR(std::size_t s, std::size_t r, std::size_t c) const
        {
            return mConstantR[GetIndex(s, r, c)];
        }

        // Get length of spring from (s,r,c) to (s,r+1,c).
        inline T const& GetLengthR(std::size_t s, std::size_t r, std::size_t c) const
        {
            return mLengthR[GetIndex(s, r, c)];
        }

        // Get constant of spring from (s,r,c) to (s,r,c+1).
        inline T const& GetConstantC(std::size_t s, std::size_t r, std::size_t c) const
        {
            return mConstantC[GetIndex(s, r, c)];
        }

        // Get length of spring from (s,r,c) to (s,r,c+1).
        inline T const& GetLengthC(std::size_t s, std::size_t r, std::size_t c) const
        {
            return mLengthC[GetIndex(s, r, c)];
        }

        // The default external force is zero. Derive a class from this one to
        // provide nonzero external forces such as gravity, wind, friction and
        // so on. This function is called by Acceleration(...) to compute the
        // impulse F/m generated by the external force F.
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
            // The face, edge and corner points of the volume of masses must
            // be handled separately, because each has fewer than eight
            // springs attached to it.

            Vector<T, N> acceleration = ExternalAcceleration(i, time, position, velocity);
            Vector<T, N> diff{}, force{};
            T ratio{};

            std::size_t s{}, r{}, c{}, prev{}, next{};
            GetCoordinates(i, s, r, c);

            if (s > 0)
            {
                prev = i - mNumRows * mNumCols;  // index to previous s-neighbor
                diff = position[prev] - position[i];
                ratio = GetLengthS(s - 1, r, c) / Length(diff);
                force = GetConstantS(s - 1, r, c) * (C_<T>(1) - ratio) * diff;
                acceleration += this->mInvMass[i] * force;
            }

            if (s < mNumSlices - 1)
            {
                next = i + mNumRows * mNumCols;  // index to next s-neighbor
                diff = position[next] - position[i];
                ratio = GetLengthS(s, r, c) / Length(diff);
                force = GetConstantS(s, r, c) * (C_<T>(1) - ratio) * diff;
                acceleration += this->mInvMass[i] * force;
            }

            if (r > 0)
            {
                prev = i - mNumCols;  // index to previous r-neighbor
                diff = position[prev] - position[i];
                ratio = GetLengthR(s, r - 1, c) / Length(diff);
                force = GetConstantR(s, r - 1, c) * (C_<T>(1) - ratio) * diff;
                acceleration += this->mInvMass[i] * force;
            }

            if (r < mNumRows - 1)
            {
                next = i + mNumCols;  // index to next r-neighbor
                diff = position[next] - position[i];
                ratio = GetLengthR(s, r, c) / Length(diff);
                force = GetConstantR(s, r, c) * (C_<T>(1) - ratio) * diff;
                acceleration += this->mInvMass[i] * force;
            }

            if (c > 0)
            {
                prev = i - 1;  // index to previous c-neighbor
                diff = position[prev] - position[i];
                ratio = GetLengthC(s, r, c - 1) / Length(diff);
                force = GetConstantC(s, r, c - 1) * (C_<T>(1) - ratio) * diff;
                acceleration += this->mInvMass[i] * force;
            }

            if (c < mNumCols - 1)
            {
                next = i + 1;  // index to next c-neighbor
                diff = position[next] - position[i];
                ratio = GetLengthC(s, r, c) / Length(diff);
                force = GetConstantC(s, r, c) * (C_<T>(1) - ratio) * diff;
                acceleration += this->mInvMass[i] * force;
            }

            return acceleration;
        }

        inline std::size_t GetIndex(std::size_t s, std::size_t r, std::size_t c) const
        {
            return c + mNumCols * (r + mNumRows * s);
        }

        void GetCoordinates(std::size_t i, std::size_t& s, std::size_t& r, std::size_t& c) const
        {
            c = i % mNumCols;
            i = (i - c) / mNumCols;
            r = i % mNumRows;
            s = i / mNumRows;
        }

        std::size_t mNumSlices, mNumRows, mNumCols;
        std::vector<T> mConstantS, mLengthS;
        std::vector<T> mConstantR, mLengthR;
        std::vector<T> mConstantC, mLengthC;

    private:
        friend class UnitTestMassSpringVolume;
    };
}
