// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.14

#pragma once

// Given points (x0[i],y0[i]) which are mapped to (x1[i],y1[i]) for
// 0 <= i < N, interpolate positions (xIn,yIn) to (xOut,yOut).
//
// The useLinear constructor input is set to 'true' when you want linear
// interpolation or 'false' when you want quadratic interpolation.
//
// The underlying StaticVETManifoldMesh of the PlanarMesh object allows for
// multithreading (to compute adjacency information for shared edges). If so
// desired, set numThreads in the constructor to a positive value when you
// have a large number of samples.

#include <GTL/Mathematics/Geometry/2D/Delaunay2.h>
#include <GTL/Mathematics/Interpolation/2D/IntpLinearNonuniform2.h>
#include <GTL/Mathematics/Interpolation/2D/IntpQuadraticNonuniform2.h>
#include <array>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <vector>

namespace gtl
{
    template <typename T>
    class IntpVectorField2
    {
    public:
        IntpVectorField2(std::vector<Vector2<T>> const& domain,
            std::vector<Vector2<T>> const& range, bool useLinear,
            std::size_t numThreads)
            :
            mLXInterpolator{},
            mLYInterpolator{},
            mQXInterpolator{},
            mQYInterpolator{}
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");

            GTL_ARGUMENT_ASSERT(
                domain.size() == range.size(),
                "The domain[] and range[] inputs must have the same size.");

            // Repackage the output vectors into individual components. This
            // is required because of the format that the interpolators expect
            // for their input data.
            std::vector<T> xRange(range.size());
            std::vector<T> yRange(range.size());
            for (std::size_t i = 0; i < range.size(); ++i)
            {
                xRange[i] = range[i][0];
                yRange[i] = range[i][1];
            }

            // Compute the Delaunay triangulation of the samples.
            Delaunay2<T> delaunay{};
            delaunay(domain);
            GTL_RUNTIME_ASSERT(
                delaunay.GetDimension() == 2,
                "The Delaunay triangulation must be 2-dimensional.");

            auto const& indices = delaunay.GetIndices();
            std::vector<std::array<std::size_t, 3>> triangles(indices.size() / 3);
            std::size_t i = 0;
            for (auto& triangle : triangles)
            {
                triangle[0] = indices[i++];
                triangle[1] = indices[i++];
                triangle[2] = indices[i++];
            }

            // Create the interpolator.
            PlanarMesh<T> mesh(domain, triangles, numThreads);
            if (useLinear)
            {
                mLXInterpolator = std::make_unique<IntpLinearNonuniform2<T>>(
                    mesh, xRange, true, numThreads);

                mLYInterpolator = std::make_unique<IntpLinearNonuniform2<T>>(
                    mesh, yRange, true, numThreads);
            }
            else
            {
                // TODO: For now, choose a small positive number. Later either
                // make the parameter a constructor input or estimate it from
                // the angles[] data.
                T const spatialDelta = static_cast<T>(0.001);

                mQXInterpolator = std::make_unique<IntpQuadraticNonuniform2<T>>(
                    mesh, xRange, spatialDelta, true, numThreads);

                mQYInterpolator = std::make_unique<IntpQuadraticNonuniform2<T>>(
                    mesh, yRange, spatialDelta, true, numThreads);
            }
        }

        ~IntpVectorField2() = default;

        // Use this evaluator for either linear or quadratic interpolation.
        // The return value is 'true' if and only if the input point is in the
        // convex hull of the input (theta,phi) array, in which case the
        // interpolation is valid.
        bool Evaluate(Vector2<T> const& input, Vector2<T>& output) const
        {
            if (mLXInterpolator)
            {
                return mLXInterpolator->Evaluate(input, output[0])
                    && mLYInterpolator->Evaluate(input, output[1]);

            }
            else
            {
                // The derivatives values are discarded.
                Vector2<T> xDeriv{}, yDeriv{};
                return mQXInterpolator->Evaluate(input, output[0], xDeriv[0], xDeriv[1])
                    && mQYInterpolator->Evaluate(input, output[1], yDeriv[0], yDeriv[1]);
            }
        }

        // Use this evaluator for quadratic interpolation. The function always
        // returns 'false' if IntpVectorField2 creates a linear interpolator.
        bool Evaluate(Vector2<T> const& input, Vector2<T>& output,
            Vector2<T>& xDeriv, Vector2<T>& yDeriv)
        {
            if (mQXInterpolator)
            {
                return mQXInterpolator->Evaluate(input, output[0], xDeriv[0], xDeriv[1])
                    && mQYInterpolator->Evaluate(input, output[1], yDeriv[0], yDeriv[1]);
            }
            else
            {
                return false;
            }
        }

    protected:
        std::unique_ptr<IntpLinearNonuniform2<T>> mLXInterpolator;
        std::unique_ptr<IntpLinearNonuniform2<T>> mLYInterpolator;
        std::unique_ptr<IntpQuadraticNonuniform2<T>> mQXInterpolator;
        std::unique_ptr<IntpQuadraticNonuniform2<T>> mQYInterpolator;

    private:
        friend class UnitTestIntpVectorField2;
    };
}
