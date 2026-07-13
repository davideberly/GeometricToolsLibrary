// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// The topic of fast marching methods are discussed in the book
//   Level Set Methods and Fast Marching Methods:
//     Evolving Interfaces in Computational Geometry, Fluid Mechanics,
//     Computer Vision, and Materials Science
//   J.A. Sethian,
//   Cambridge University Press, 1999

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Mathematics/ImageProcessing/FastMarch.h>
#include <cmath>
#include <cstddef>
#include <limits>
#include <vector>

namespace gtl
{
    template <typename T>
    class FastMarch2 : public FastMarch<T>
    {
    public:
        FastMarch2(std::size_t xBound, std::size_t yBound, T const& xSpacing, T const& ySpacing,
            std::vector<std::size_t> const& seeds, std::vector<T> const& speeds)
            :
            FastMarch<T>(xBound * yBound, seeds, speeds)
        {
            Initialize(xBound, yBound, xSpacing, ySpacing);
        }

        FastMarch2(std::size_t xBound, std::size_t yBound, T const& xSpacing, T const& ySpacing,
            std::vector<std::size_t> const& seeds, T const& speed)
            :
            FastMarch<T>(xBound * yBound, seeds, speed)
        {
            Initialize(xBound, yBound, xSpacing, ySpacing);
        }

        virtual ~FastMarch2() = default;

        // Member access.
        inline std::size_t GetXBound() const
        {
            return mXBound;
        }

        inline std::size_t GetYBound() const
        {
            return mYBound;
        }

        inline T const& GetXSpacing() const
        {
            return mXSpacing;
        }

        inline T const& GetYSpacing() const
        {
            return mYSpacing;
        }

        inline std::size_t Index(std::size_t x, std::size_t y) const
        {
            return x + mXBound * y;
        }

        // Pixel classification.
        virtual void GetBoundary(std::vector<std::size_t>& boundary) const override
        {
            for (std::size_t i = 0; i < this->mQuantity; ++i)
            {
                if (this->IsValid(i) && !this->IsTrial(i))
                {
                    if (this->IsTrial(i - 1) ||
                        this->IsTrial(i + 1) ||
                        this->IsTrial(i - mXBound) ||
                        this->IsTrial(i + mXBound))
                    {
                        boundary.push_back(i);
                    }
                }
            }
        }

        virtual bool IsBoundary(std::size_t i) const override
        {
            if (this->IsValid(i) && !this->IsTrial(i))
            {
                if (this->IsTrial(i - 1) ||
                    this->IsTrial(i + 1) ||
                    this->IsTrial(i - mXBound) ||
                    this->IsTrial(i + mXBound))
                {
                    return true;
                }
            }
            return false;
        }

        // Run one step of the fast marching algorithm.
        virtual void Iterate() override
        {
            // Remove the minimum trial value from the heap.
            std::size_t i = 0;
            T value{};
            this->mHeap.Remove(i, value);

            // Promote the trial value to a known value. The value was
            // negative but is now nonnegative (the heap stores only
            // nonnegative numbers).
            this->mTrials[i] = MinHeap<T>::invalid;

            // All trial pixels must be updated. All far neighbors must become
            // trial pixels.
            std::size_t iM1 = i - 1;
            if (this->IsTrial(iM1))
            {
                ComputeTime(iM1);
                this->mHeap.Update(this->mTrials[iM1], this->mTimes[iM1]);
            }
            else if (this->IsFar(iM1))
            {
                ComputeTime(iM1);
                this->mTrials[iM1] = this->mHeap.Insert(iM1, this->mTimes[iM1]);
            }

            std::size_t iP1 = i + 1;
            if (this->IsTrial(iP1))
            {
                ComputeTime(iP1);
                this->mHeap.Update(this->mTrials[iP1], this->mTimes[iP1]);
            }
            else if (this->IsFar(iP1))
            {
                ComputeTime(iP1);
                this->mTrials[iP1] = this->mHeap.Insert(iP1, this->mTimes[iP1]);
            }

            std::size_t iMXB = i - mXBound;
            if (this->IsTrial(iMXB))
            {
                ComputeTime(iMXB);
                this->mHeap.Update(this->mTrials[iMXB], this->mTimes[iMXB]);
            }
            else if (this->IsFar(iMXB))
            {
                ComputeTime(iMXB);
                this->mTrials[iMXB] = this->mHeap.Insert(iMXB, this->mTimes[iMXB]);
            }

            std::size_t iPXB = i + mXBound;
            if (this->IsTrial(iPXB))
            {
                ComputeTime(iPXB);
                this->mHeap.Update(this->mTrials[iPXB], this->mTimes[iPXB]);
            }
            else if (this->IsFar(iPXB))
            {
                ComputeTime(iPXB);
                this->mTrials[iPXB] = this->mHeap.Insert(iPXB, this->mTimes[iPXB]);
            }
        }

    protected:
        // Called by the constructors.
        void Initialize(std::size_t xBound, std::size_t yBound, T const& xSpacing, T const& ySpacing)
        {
            mXBound = xBound;
            mYBound = yBound;
            mXBoundM1 = mXBound - 1;
            mYBoundM1 = mYBound - 1;
            mXSpacing = xSpacing;
            mYSpacing = ySpacing;
            mInvXSpacing = C_<T>(1) / xSpacing;
            mInvYSpacing = C_<T>(1) / ySpacing;

            // Boundary pixels are marked as zero speed to allow us to avoid
            // having to process the boundary pixels separately during the
            // iteration.

            // vertex (0,0)
            std::size_t i = Index(0, 0);
            this->mInvSpeeds[i] = std::numeric_limits<T>::max();
            this->mTimes[i] = -std::numeric_limits<T>::max();

            // vertex (xmax,0)
            i = Index(mXBoundM1, 0);
            this->mInvSpeeds[i] = std::numeric_limits<T>::max();
            this->mTimes[i] = -std::numeric_limits<T>::max();

            // vertex (0,ymax)
            i = Index(0, mYBoundM1);
            this->mInvSpeeds[i] = std::numeric_limits<T>::max();
            this->mTimes[i] = -std::numeric_limits<T>::max();

            // vertex (xmax,ymax)
            i = Index(mXBoundM1, mYBoundM1);
            this->mInvSpeeds[i] = std::numeric_limits<T>::max();
            this->mTimes[i] = -std::numeric_limits<T>::max();

            // edges (x,0) and (x,ymax)
            for (std::size_t x = 0; x < mXBound; ++x)
            {
                i = Index(x, 0);
                this->mInvSpeeds[i] = std::numeric_limits<T>::max();
                this->mTimes[i] = -std::numeric_limits<T>::max();
                i = Index(x, mYBoundM1);
                this->mInvSpeeds[i] = std::numeric_limits<T>::max();
                this->mTimes[i] = -std::numeric_limits<T>::max();
            }

            // edges (0,y) and (xmax,y)
            for (std::size_t y = 0; y < mYBound; ++y)
            {
                i = Index(0, y);
                this->mInvSpeeds[i] = std::numeric_limits<T>::max();
                this->mTimes[i] = -std::numeric_limits<T>::max();
                i = Index(mXBoundM1, y);
                this->mInvSpeeds[i] = std::numeric_limits<T>::max();
                this->mTimes[i] = -std::numeric_limits<T>::max();
            }

            // Compute the first batch of trial pixels.  These are pixels a
            // grid distance of one away from the seed pixels.
            for (std::size_t y = 1; y < mYBoundM1; ++y)
            {
                for (std::size_t x = 1; x < mXBoundM1; ++x)
                {
                    i = Index(x, y);
                    if (this->IsFar(i))
                    {
                        if ((this->IsValid(i - 1) && !this->IsTrial(i - 1)) ||
                            (this->IsValid(i + 1) && !this->IsTrial(i + 1)) ||
                            (this->IsValid(i - mXBound) && !this->IsTrial(i - mXBound)) ||
                            (this->IsValid(i + mXBound) && !this->IsTrial(i + mXBound)))
                        {
                            ComputeTime(i);
                            this->mTrials[i] = this->mHeap.Insert(i, this->mTimes[i]);
                        }
                    }
                }
            }
        }

        // Called by Iterate().
        void ComputeTime(std::size_t i)
        {
            bool hasXTerm{};
            T xConst{};
            if (this->IsValid(i - 1))
            {
                hasXTerm = true;
                xConst = this->mTimes[i - 1];
                if (this->IsValid(i + 1))
                {
                    if (this->mTimes[i + 1] < xConst)
                    {
                        xConst = this->mTimes[i + 1];
                    }
                }
            }
            else if (this->IsValid(i + 1))
            {
                hasXTerm = true;
                xConst = this->mTimes[i + 1];
            }
            else
            {
                hasXTerm = false;
                xConst = C_<T>(0);
            }

            bool hasYTerm{};
            T yConst{};
            if (this->IsValid(i - mXBound))
            {
                hasYTerm = true;
                yConst = this->mTimes[i - mXBound];
                if (this->IsValid(i + mXBound))
                {
                    if (this->mTimes[i + mXBound] < yConst)
                    {
                        yConst = this->mTimes[i + mXBound];
                    }
                }
            }
            else if (this->IsValid(i + mXBound))
            {
                hasYTerm = true;
                yConst = this->mTimes[i + mXBound];
            }
            else
            {
                hasYTerm = false;
                yConst = C_<T>(0);
            }

            if (hasXTerm)
            {
                if (hasYTerm)
                {
                    T sum = xConst + yConst;
                    T diff = xConst - yConst;
                    T discr = C_<T>(2) * this->mInvSpeeds[i] *
                        this->mInvSpeeds[i] - diff * diff;
                    if (discr >= C_<T>(0))
                    {
                        // The quadratic equation has a real-valued solution.
                        // Choose the largest positive root for the crossing
                        // time.
                        this->mTimes[i] = C_<T>(1, 2) * (sum + std::sqrt(discr));
                    }
                    else
                    {
                        // The quadratic equation does not have a real-valued
                        // solution.  This can happen when the speed is so
                        // large that the time gradient has very small length,
                        // which means that the time has not changed
                        // significantly from the neighbors to the current
                        // pixel.  Just choose the maximum time of the
                        // neighbors.  (Is there a better choice?)
                        this->mTimes[i] = (diff >= C_<T>(0) ? xConst : yConst);
                    }
                }
                else
                {
                    // The equation is linear.
                    this->mTimes[i] = this->mInvSpeeds[i] + xConst;
                }
            }
            else if (hasYTerm)
            {
                // The equation is linear.
                this->mTimes[i] = this->mInvSpeeds[i] + yConst;
            }
            else
            {
                // Assert: The pixel must have at least one known neighbor.
            }
        }

        std::size_t mXBound, mYBound, mXBoundM1, mYBoundM1;
        T mXSpacing, mYSpacing, mInvXSpacing, mInvYSpacing;

    private:
        friend class UnitTestFastMarch2;
    };
}
