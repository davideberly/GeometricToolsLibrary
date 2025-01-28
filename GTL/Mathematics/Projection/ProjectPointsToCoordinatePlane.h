// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Project a collection of 3D points to a collection of 2D points using a
// specified normal vector N to select the coordinate plane of projection.
// The normal vector does not have to be unit length. Let X = (x[0],x[1],x[2])
// be a 3D point and yet Y = (y[0],y[1) be the projected 2D point. Let
// N = (n[0],n[1],n[2]) and maxIndex in {0,1,2} is chosen so that
// |n[maxIndex]| = max{|n[0]|, |n[1]|, |n[2]|}. The coordinate plane is
// selected as follows:
//   maxIndex = 0, Y = (x[1],x[2]), permute = {1,2,0}
//   maxIndex = 1, Y = (x[0],x[2]), permute = {0,2,1}
//   maxIndex = 2, Y = (x[0],x[1]), permute = {0,1,2}

#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class ProjectPointsToCoordinatePlane
    {
    public:
        // Compute maxIndex and permute as define in the comments at the
        // beginning of this file.
        static void Select(Vector3<T> const& normal,
            std::size_t& maxIndex, std::array<std::size_t, 3>& permute)
        {
            maxIndex = 0;
            T cmax = std::fabs(normal[0]);
            T cvalue = std::fabs(normal[1]);
            if (cvalue > cmax)
            {
                maxIndex = 1;
                cmax = cvalue;
            }
            cvalue = std::fabs(normal[2]);
            if (cvalue > cmax)
            {
                maxIndex = 2;
            }

            if (maxIndex == 0)
            {
                // Project onto the yz-plane.
                permute = { 1, 2, 0 };
            }
            else if (maxIndex == 1)
            {
                // Project onto the xz-plane.
                permute = { 0, 2, 1 };
            }
            else // maxIndex = 2
            {
                // Project onto the xy-plane.
                permute = { 0, 1, 2 };
            }
        }

        // Project a collection of 3D points to the coordinate plane that
        // was used to generate permute[] in the Select function.
        static void Project(std::size_t numPoints, Vector3<T> const* points,
            std::array<std::size_t, 3> const& permute, Vector2<T>* projectedPoints)
        {
            for (std::size_t i = 0; i < numPoints; ++i)
            {
                projectedPoints[i][0] = points[i][permute[0]];
                projectedPoints[i][1] = points[i][permute[1]];
            }
        }

        // Combine the coordinate plane selection and projection into a single
        // step.
        static void SelectAndProject(std::size_t numPoints, Vector3<T> const* points,
            Vector3<T> const& normal, Vector2<T>* projectedPoints,
            std::size_t& maxIndex, std::array<std::size_t, 3>& permute)
        {
            Select(normal, maxIndex, permute);
            for (std::size_t i = 0; i < numPoints; ++i)
            {
                projectedPoints[i][0] = points[i][permute[0]];
                projectedPoints[i][1] = points[i][permute[1]];
            }
        }

        // Unproject the 2D points onto a plane Dot(N, X - P) = 0, where P is
        // the plane origin.
        static void Lift(std::size_t numPoints, Vector2<T> const* projectedPoints,
            Vector3<T> const& origin, Vector3<T> const& normal,
            std::array<std::size_t, 3> const& permute, Vector3<T>* points)
        {
            for (std::size_t i = 0; i < numPoints; ++i)
            {
                points[i][permute[0]] = projectedPoints[i][0];
                points[i][permute[1]] = projectedPoints[i][1];
                points[i][permute[2]] = origin[permute[2]] -
                    (normal[permute[0]] * projectedPoints[i][0] +
                    normal[permute[1]] * projectedPoints[i][1])
                    / normal[permute[2]];
            }
        }

    private:
        friend class UnitTestProjectPointsToCoordinatePlane;
    };
}
