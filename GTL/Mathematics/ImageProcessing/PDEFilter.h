// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// Support for image processing using finite differences for partial
// differential equations. PDEFilter is the abstract base class for
// PDEFilter1, PDEFilter2 and PDEFilter3.

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class PDEFilter
    {
    public:
        enum class ScaleType
        {
            // The data is processed as is.
            ST_NONE,

            // The data range is d in [min,max].  The scaled values are d'.

            // d' = (d-min)/(max-min) in [0,1]
            ST_UNIT,

            // d' = -1 + 2*(d-min)/(max-min) in [-1,1]
            ST_SYMMETRIC,

            // max > -min:  d' = d/max in [min/max,1]
            // max < -min:  d' = -d/min in [-1,-max/min]
            ST_PRESERVE_ZERO
        };

        // The abstract base class for all PDE-based filters.
        virtual ~PDEFilter() = default;

        // Member access.
        inline std::size_t GetQuantity() const
        {
            return mQuantity;
        }

        inline T const& GetBorderValue() const
        {
            return mBorderValue;
        }

        inline ScaleType GetScaleType() const
        {
            return mScaleType;
        }

        // Access to the time step for the PDE solver.
        inline void SetTimeStep(T const& timeStep)
        {
            mTimeStep = timeStep;
        }

        inline T const& GetTimeStep() const
        {
            return mTimeStep;
        }

        // This function executes one iteration of the filter.  It calls
        // OnPreUpdate, OnUpdate and OnPostUpdate, in that order.
        void Update()
        {
            OnPreUpdate();
            OnUpdate();
            OnPostUpdate();
        }

    protected:
        PDEFilter(std::size_t quantity, T const* data, T const& borderValue, ScaleType scaleType)
            :
            mQuantity(quantity),
            mBorderValue(borderValue),
            mScaleType(scaleType),
            mMin(C_<T>(0)),
            mOffset(C_<T>(0)),
            mScale(C_<T>(0)),
            mTimeStep(C_<T>(0))
        {
            T maxValue = data[0];
            mMin = maxValue;
            for (std::size_t i = 1; i < mQuantity; i++)
            {
                T value = data[i];
                if (value < mMin)
                {
                    mMin = value;
                }
                else if (value > maxValue)
                {
                    maxValue = value;
                }
            }

            if (mMin != maxValue)
            {
                switch (mScaleType)
                {
                case ScaleType::ST_NONE:
                    mOffset = C_<T>(0);
                    mScale = C_<T>(1);
                    break;
                case ScaleType::ST_UNIT:
                    mOffset = C_<T>(0);
                    mScale = C_<T>(1) / (maxValue - mMin);
                    break;
                case ScaleType::ST_SYMMETRIC:
                    mOffset = -C_<T>(1);
                    mScale = C_<T>(2) / (maxValue - mMin);
                    break;
                case ScaleType::ST_PRESERVE_ZERO:
                    mOffset = C_<T>(0);
                    mScale = (maxValue >= -mMin ? C_<T>(1) / maxValue : -C_<T>(1) / mMin);
                    mMin = C_<T>(0);
                    break;
                }
            }
            else
            {
                mOffset = C_<T>(0);
                mScale = C_<T>(1);
            }
        }

        // The derived classes for 2D and 3D implement this to recompute the
        // boundary values when Neumann conditions are used. If derived
        // classes built on top of the 2D or 3D classes implement this also,
        // they must call the base-class OnPreUpdate first.
        virtual void OnPreUpdate() = 0;

        // The derived classes for 2D and 3D implement this to iterate over
        // the image elements, updating an element only if it is not masked
        // out.
        virtual void OnUpdate() = 0;

        // The derived classes for 2D and 3D implement this to swap the
        // buffers for the next pass. If derived classes built on top of the
        // 2D or 3D classes implement this also, they must call the base-class
        // OnPostUpdate last. 
        virtual void OnPostUpdate() = 0;

        // The number of image elements.
        std::size_t mQuantity;

        // When set to std::numeric_limits<T>::max(), Neumann conditions
        // are in use (zero-valued derivatives on the image border).
        // Dirichlet conditions are used, otherwise (image is constant on the
        // border).
        T mBorderValue;

        // This member stores how the image data was transformed during the
        // constructor call.
        ScaleType mScaleType;
        T mMin, mOffset, mScale;

        // The time step for the PDE solver. The stability of an algorithm
        // depends on the magnitude of the time step, but the magnitude itself
        // depends on the algorithm.
        T mTimeStep;

    private:
        friend class UnitTestPDEFilter;
    };
}
