// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.14

#pragma once

// The IntpSphere3 class implements linear or quadratic interpolation of a
// scalar-valued function defined on a set of points on the sphere. The
// input samples are of the form (theta, phi, F(theta, phi)) where
// (theta, phi) are the spherical coordinates for the sample and stored as
// the 2-tuple angles[]. The angle constraints are -pi <= theta <= pi and
// 0 <= phi <= pi.
// 
// To ensure complete coverage of the sphere, the interpolator must have
// domain [-pi,pi]x[0,pi]. To accomplish this, provide function samples at the
// north and south poles. These are named FNorthPole and FSouthPole in the
// constructor. Note that (theta,0,F(theta,0)) = (theta,0,FNorthPole) and
// (theta,pi,F(theta,pi)) = (theta,pi,FSouthPole) for all theta in [-pi,pi].
// The constructor will append samples (-pi,0,FNorthPole), (pi,0,FNorthPole),
// (-pi,pi,FSouthPole), and (pi,pi,FSouthPole). NOTE: Because the constructor
// computes the samples at the north and south poles, the input angles[] must
// not include 2-tuples of the form (theta,0) or (theta,pi).
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
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class IntpSphere3
    {
    public:
        IntpSphere3(std::vector<Vector2<T>> const& angles,
            std::vector<T> const& F, T const& FNorthPole, T const& FSouthPole,
            bool useLinear, std::size_t numThreads)
            :
            mLInterpolator{},
            mQInterpolator{}
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");

            GTL_ARGUMENT_ASSERT(
                angles.size() == F.size(),
                "The angles[] and F[] inputs must have the same size.");

            // Delaunay triangulation will be performed with a unique set of
            // angle pairs. Also, validate the angles by testing that they are
            // in the required domain.
            T const zero = C_<T>(0);
            T const pi = C_PI<T>;
            std::map<Vector2<T>, T> uniqueSamples{};
            for (std::size_t i = 0; i < angles.size(); ++i)
            {
                auto const& angleSample = angles[i];
                GTL_ARGUMENT_ASSERT(
                    -pi <= angleSample[0] && angleSample[0] <= pi &&
                    zero < angleSample[1] && angleSample[1] < pi,
                    "Invalid (theta,phi) pair.");

                uniqueSamples.insert(std::make_pair(angleSample, F[i]));
            }

            // Insert the four corners of the domain.
            uniqueSamples.insert(std::make_pair(Vector2<T>{ -pi, zero }, FNorthPole));
            uniqueSamples.insert(std::make_pair(Vector2<T>{ pi, zero }, FNorthPole));
            uniqueSamples.insert(std::make_pair(Vector2<T>{ -pi, pi }, FSouthPole));
            uniqueSamples.insert(std::make_pair(Vector2<T>{ pi, pi }, FSouthPole));

            // Compute the Delaunay triangulation of the augmented samples.
            std::vector<Vector2<T>> localAngles(uniqueSamples.size());
            std::vector<T> localF(uniqueSamples.size());
            std::size_t i = 0;
            for (auto const& sample : uniqueSamples)
            {
                localAngles[i] = sample.first;
                localF[i] = sample.second;
                ++i;
            }

            Delaunay2<T> delaunay{};
            delaunay(localAngles);
            GTL_RUNTIME_ASSERT(
                delaunay.GetDimension() == 2,
                "The Delaunay triangulation must be 2-dimensional.");

            auto const& indices = delaunay.GetIndices();
            std::vector<std::array<std::size_t, 3>> triangles(indices.size() / 3);
            i = 0;
            for (auto& triangle : triangles)
            {
                triangle[0] = indices[i++];
                triangle[1] = indices[i++];
                triangle[2] = indices[i++];
            }

            // Create the interpolator.
            PlanarMesh<T> mesh(localAngles, triangles, numThreads);
            if (useLinear)
            {
                mLInterpolator = std::make_unique<IntpLinearNonuniform2<T>>(
                    mesh, localF, true, numThreads);
            }
            else
            {
                // TODO: For now, choose a small positive number. Later either
                // make the parameter a constructor input or estimate it from
                // the angles[] data.
                T const spatialDelta = static_cast<T>(0.001);

                mQInterpolator = std::make_unique<IntpQuadraticNonuniform2<T>>(
                    mesh, localF, spatialDelta, true, numThreads);
            }
        }

        ~IntpSphere3() = default;

        // Use this evaluator for either linear or quadratic interpolation.
        // The return value is 'true' if and only if the input point is in the
        // convex hull of the input (theta,phi) array, in which case the
        // interpolation is valid.
        bool Evaluate(T const& theta, T const& phi, T& F) const
        {
            Vector2<T> angles{ theta, phi };
            if (mLInterpolator)
            {
                return mLInterpolator->Evaluate(angles, F);
            }
            else
            {
                // The derivatives values are discarded.
                T thetaDeriv{}, phiDeriv{};
                return mQInterpolator->Evaluate(angles, F, thetaDeriv, phiDeriv);
            }
        }

        // Use this evaluator for quadratic interpolation. The function always
        // returns 'false' if IntpSphere3 creates a linear interpolator.
        bool Evaluate(T const& theta, T const& phi, T& F, T& thetaDeriv, T& phiDeriv)
        {
            Vector2<T> angles{ theta, phi };
            if (mQInterpolator)
            {
                return mQInterpolator->Evaluate(angles, F, thetaDeriv, phiDeriv);
            }
            else
            {
                return false;
            }
        }

        // Spherical coordinates are
        //   x = cos(theta) * sin(phi)
        //   y = sin(theta) * sin(phi)
        //   z = cos(phi)
        // for -pi <= theta <= pi, 0 <= phi <= pi.  The application can use
        // this function to convert unit length vectors (x,y,z) to (theta,phi).
        static Vector2<T> GetSphericalCoordinates(Vector3<T> const& unit)
        {
            // Assumes (x,y,z) is unit length.  Returns -pi <= theta <= pi and
            // 0 <= phiAngle <= pi.
            Vector2<T> angles{};
            if (unit[2] < C_<T>(1))
            {
                if (unit[2] > C_<T>(-1))
                {
                    angles[0] = std::atan2(unit[1], unit[0]);
                    angles[1] = std::acos(unit[2]);
                }
                else
                {
                    angles[0] = -C_PI<T>;
                    angles[1] = C_PI<T>;
                }
            }
            else
            {
                angles[0] = -C_PI<T>;
                angles[1] = C_<T>(0);
            }
            return angles;
        }

    private:
        std::unique_ptr<IntpLinearNonuniform2<T>> mLInterpolator;
        std::unique_ptr<IntpQuadraticNonuniform2<T>> mQInterpolator;

    private:
        friend class UnitTestIntpSphere3;
    };
}
