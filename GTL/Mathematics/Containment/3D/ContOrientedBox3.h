// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.09.22

#pragma once

#include <GTL/Mathematics/Algebra/RigidMotion.h>
#include <GTL/Mathematics/Approximation/ND/ApprGaussianDistribution.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    // Compute an oriented bounding box of the points. The box center is the
    // average of the points. The box axes are the eigenvectors of the
    // covariance matrix.
    template <typename T>
    void GetContainer(std::vector<Vector3<T>> const& points, OrientedBox3<T>& box)
    {
        // Fit the points with a Gaussian distribution. The covariance matrix
        // is M = sum_j D[j]*U[j]*U[j]^T, where D[j] are the eigenvalues and
        // U[j] are corresponding unit-length eigenvectors.
        Vector3<T> mean{};
        std::array<T, 3> eigenvalues{};
        std::array<Vector3<T>, 3> eigenvectors{};
        (void)ApprGaussianDistribution<T, 3>::Fit(points, mean, eigenvalues, eigenvectors);

        // Let C be the box center and let U0, U1 and U2 be the box axes.
        // Each input point is of the form X = C + y0*U0 + y1*U1 + y2*U2.
        // The following code computes min(y0), max(y0), min(y1), max(y1),
        // min(y2), and max(y2). The box center is then adjusted to be
        //   C' = C + 0.5*(min(y0)+max(y0))*U0 + 0.5*(min(y1)+max(y1))*U1
        //        + 0.5*(min(y2)+max(y2))*U2
        Vector3<T> pmin{}, pmax{};
        for (std::size_t i = 0; i < points.size(); ++i)
        {
            Vector3<T> diff = points[i] - mean;
            for (std::size_t j = 0; j < 3; ++j)
            {
                T dot = Dot(diff, eigenvectors[j]);
                if (dot < pmin[j])
                {
                    pmin[j] = dot;
                }
                else if (dot > pmax[j])
                {
                    pmax[j] = dot;
                }
            }
        }

        box.center = mean;
        for (std::size_t j = 0; j < 3; ++j)
        {
            box.axis[j] = eigenvectors[j];
            box.center += (C_<T>(1, 2) * (pmin[j] + pmax[j])) * box.axis[j];
            box.extent[j] = C_<T>(1, 2) * (pmax[j] - pmin[j]);
        }
    }

    // Test for containment. Let X = C + y0*U0 + y1*U1 + y2*U2 where C is the
    // box center and U0, U1 and U2 are the orthonormal axes of the box. X is
    // in the box if |y_i| <= E_i for all i where E_i are the extents of the
    // box.
    template <typename T>
    bool InContainer(Vector3<T> const& point, OrientedBox3<T> const& box)
    {
        Vector3<T> diff = point - box.center;
        for (std::size_t i = 0; i < 3; ++i)
        {
            T coeff = Dot(diff, box.axis[i]);
            if (std::fabs(coeff) > box.extent[i])
            {
                return false;
            }
        }
        return true;
    }

    // Construct an oriented box that contains two other oriented boxes. The
    // result is not guaranteed to be the minimum volume box containing the
    // input boxes.
    template <typename T>
    void MergeContainers(OrientedBox3<T> const& box0,
        OrientedBox3<T> const& box1, OrientedBox3<T>& merge)
    {
        // The first guess at the box center. This value will be updated
        // later after the input box vertices are projected onto axes
        // determined by an average of box axes.
        merge.center = C_<T>(1, 2) * (box0.center + box1.center);

        // The bounding ellipsoid orientation is the average of the input
        // orientations.
        Matrix3x3<T> rot0{}, rot1{};
        rot0.SetCol(0, box0.axis[0]);
        rot0.SetCol(1, box0.axis[1]);
        rot0.SetCol(2, box0.axis[2]);
        rot1.SetCol(0, box1.axis[0]);
        rot1.SetCol(1, box1.axis[1]);
        rot1.SetCol(2, box1.axis[2]);
        Quaternion<T> q0{}, q1{};
        RigidMotion<T>::Convert(rot0, q0);
        RigidMotion<T>::Convert(rot1, q1);
        if (Dot(q0, q1) < C_<T>(0))
        {
            q1 = -q1;
        }

        Quaternion<T> q = q0 + q1;
        Normalize(q);
        Matrix3x3<T> rot{};
        RigidMotion<T>::Convert(q, rot);
        for (std::size_t j = 0; j < 3; ++j)
        {
            merge.axis[j] = rot.GetCol(j);
        }

        // Project the input box vertices onto the merged-box axes. Each axis
        // D[i] containing the current center C has a minimum projected value
        // min[i] and a maximum projected value max[i]. The corresponding end
        // points on the axes are C+min[i]*D[i] and C+max[i]*D[i]. The point
        // C is not necessarily the midpoint for any of the intervals. The
        // actual box center will be adjusted from C to a point C' that is the
        // midpoint of each interval,
        //   C' = C + sum_{i=0}^2 0.5*(min[i]+max[i])*D[i]
        // The box extents are
        //   e[i] = 0.5*(max[i]-min[i])

        std::array<Vector3<T>, 8> vertex{};
        box0.GetVertices(vertex);
        Vector3<T> pmin{}, pmax{};
        for (std::size_t i = 0; i < 8; ++i)
        {
            Vector3<T> diff = vertex[i] - merge.center;
            for (std::size_t j = 0; j < 3; ++j)
            {
                T dot = Dot(diff, merge.axis[j]);
                if (dot > pmax[j])
                {
                    pmax[j] = dot;
                }
                else if (dot < pmin[j])
                {
                    pmin[j] = dot;
                }
            }
        }

        box1.GetVertices(vertex);
        for (std::size_t i = 0; i < 8; ++i)
        {
            Vector3<T> diff = vertex[i] - merge.center;
            for (std::size_t j = 0; j < 3; ++j)
            {
                T dot = Dot(diff, merge.axis[j]);
                if (dot > pmax[j])
                {
                    pmax[j] = dot;
                }
                else if (dot < pmin[j])
                {
                    pmin[j] = dot;
                }
            }
        }

        // [min,max] is the axis-aligned box in the coordinate system of the
        // merged box axes. Update the current box center to be the center of
        // the new box. Compute the extents based on the new center.
        for (std::size_t j = 0; j < 3; ++j)
        {
            merge.center += (C_<T>(1, 2) * (pmax[j] + pmin[j])) * merge.axis[j];
            merge.extent[j] = C_<T>(1, 2) * (pmax[j] - pmin[j]);
        }
    }
}
