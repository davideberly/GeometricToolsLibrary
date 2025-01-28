// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

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
    void GetContainer(std::vector<Vector2<T>> const& points, OrientedBox2<T>& box)
    {
        // Fit the points with a Gaussian distribution. The covariance matrix
        // is M = sum_j D[j]*U[j]*U[j]^T, where D[j] are the eigenvalues and
        // U[j] are corresponding unit-length eigenvectors.
        Vector2<T> mean{};
        std::array<T, 2> eigenvalues{};
        std::array<Vector2<T>, 2> eigenvectors{};
        (void)ApprGaussianDistribution<T, 2>::Fit(points, mean, eigenvalues, eigenvectors);

        // Let C be the box center and let U0 and U1 be the box axes. Each
        // input point is of the form X = C + y0*U0 + y1*U1. The following
        // code computes min(y0), max(y0), min(y1) and max(y1). The box
        // center is then adjusted to be
        //   C' = C + 0.5 * (min(y0) + max(y0)) * U0
        //          + 0.5 * (min(y1) + max(y1)) * U1
        Vector2<T> pmin{}, pmax{};
        for (std::size_t i = 0; i < points.size(); ++i)
        {
            Vector2<T> diff = points[i] - mean;
            for (std::size_t j = 0; j < 2; ++j)
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
        for (std::size_t j = 0; j < 2; ++j)
        {
            box.axis[j] = eigenvectors[j];
            box.center += (C_<T>(1, 2) * (pmin[j] + pmax[j])) * eigenvectors[j];
            box.extent[j] = C_<T>(1, 2) * (pmax[j] - pmin[j]);
        }
    }

    // Test for containment. Let X = C + y0*U0 + y1*U1 where C is the box
    // center and U0 and U1 are the orthonormal axes of the box. X is in the
    // box when |y_i| <= E_i for all i, where E_i are the extents of the box.
    template <typename T>
    bool InContainer(Vector2<T> const& point, OrientedBox2<T> const& box)
    {
        Vector2<T> diff = point - box.center;
        for (std::size_t i = 0; i < 2; ++i)
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
    // result is not guaranteed to be the minimum area box containing the
    // input boxes.
    template <typename T>
    void MergeContainers(OrientedBox2<T> const& box0,
        OrientedBox2<T> const& box1, OrientedBox2<T>& merge)
    {
        // The first guess at the box center. This value will be updated
        // later after the input box vertices are projected onto axes
        // determined by an average of box axes.
        merge.center = C_<T>(1, 2) * (box0.center + box1.center);

        // The merged box axes are the averages of the input box axes. The
        // axes of the second box are negated, if necessary, so they form
        // acute angles with the axes of the first box.
        if (Dot(box0.axis[0], box1.axis[0]) >= C_<T>(0))
        {
            merge.axis[0] = C_<T>(1, 2) * (box0.axis[0] + box1.axis[0]);
        }
        else
        {
            merge.axis[0] = C_<T>(1, 2) * (box0.axis[0] - box1.axis[0]);
        }
        Normalize(merge.axis[0]);
        merge.axis[1] = -Perp(merge.axis[0]);

        // Project the input box vertices onto the merged-box axes. Each
        // axis D[i] containing the current center C has a minimum projected
        // value min[i] and a maximum projected value max[i]. The
        // corresponding endpoints on the axes are C+min[i]*D[i] and
        // C+max[i]*D[i]. The point C is not necessarily the midpoint for
        // any of the intervals. The actual box center will be adjusted from
        // C to a point C' that is the midpoint of each interval,
        //   C' = C + sum_{i=0}^1 0.5*(min[i]+max[i])*D[i]
        // The box extents are
        //   e[i] = 0.5*(max[i]-min[i])

        std::array<Vector2<T>, 4> vertex{};
        box0.GetVertices(vertex);
        Vector2<T> pmin{}, pmax{};
        for (std::size_t i = 0; i < 4; ++i)
        {
            Vector2<T> diff = vertex[i] - merge.center;
            for (std::size_t j = 0; j < 2; ++j)
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
        for (std::size_t i = 0; i < 4; ++i)
        {
            Vector2<T> diff = vertex[i] - merge.center;
            for (std::size_t j = 0; j < 2; ++j)
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
        for (std::size_t j = 0; j < 2; ++j)
        {
            merge.center += (C_<T>(1, 2) * (pmax[j] + pmin[j])) * merge.axis[j];
            merge.extent[j] = C_<T>(1, 2) * (pmax[j] - pmin[j]);
        }
    }
}
