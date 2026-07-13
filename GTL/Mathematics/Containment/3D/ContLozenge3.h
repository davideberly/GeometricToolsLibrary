// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.13

#pragma once

#include <GTL/Mathematics/Approximation/ND/ApprGaussianDistribution.h>
#include <GTL/Mathematics/Distance/ND/DistPointRectangle.h>
#include <GTL/Mathematics/Primitives/ND/Lozenge.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <vector>

namespace gtl
{
    template <typename T>
    class ContLozenge3
    {
    public:
        // Compute the plane of the lozenge rectangle using least-squares fit.
        // Parallel planes are chosen close enough together so that all the
        // data points lie between them. The radius is half the distance
        // between the two planes. The half-cylinder and quarter-cylinder side
        // pieces are chosen using a method similar to that used for fitting
        // by capsules.
        static void GetContainer(std::vector<Vector3<T>> const& points, Lozenge3<T>& lozenge)
        {
            Vector3<T> mean{};
            std::array<T, 3> eigenvalues{};
            std::array<Vector3<T>, 3> eigenvectors{};
            ApprGaussianDistribution<T, 3>::Fit(points, mean, eigenvalues, eigenvectors);

            Vector3<T> diff = points[0] - mean;
            T wMin = Dot(eigenvectors[0], diff);
            T wMax = wMin;
            for (std::size_t i = 1; i < points.size(); ++i)
            {
                diff = points[i] - mean;
                T w = Dot(eigenvectors[0], diff);
                if (w < wMin)
                {
                    wMin = w;
                }
                else if (w > wMax)
                {
                    wMax = w;
                }
            }

            T radius = C_<T>(1, 2) * (wMax - wMin);
            T rSqr = radius * radius;
            mean += (C_<T>(1, 2) * (wMax + wMin)) * eigenvectors[0];

            T aMin = std::numeric_limits<T>::max();
            T aMax = -aMin;
            T bMin = std::numeric_limits<T>::max();
            T bMax = -bMin;
            for (auto const& point : points)
            {
                diff = point - mean;
                T u = Dot(eigenvectors[2], diff);
                T v = Dot(eigenvectors[1], diff);
                T w = Dot(eigenvectors[0], diff);
                T discr = rSqr - w * w;
                T radical = std::sqrt(std::max(discr, C_<T>(0)));

                T test = u + radical;
                if (test < aMin)
                {
                    aMin = test;
                }

                test = u - radical;
                if (test > aMax)
                {
                    aMax = test;
                }

                test = v + radical;
                if (test < bMin)
                {
                    bMin = test;
                }

                test = v - radical;
                if (test > bMax)
                {
                    bMax = test;
                }
            }

            // The enclosing region might be a capsule or a sphere.
            if (aMin >= aMax)
            {
                T test = C_<T>(1, 2) * (aMin + aMax);
                aMin = test;
                aMax = test;
            }
            if (bMin >= bMax)
            {
                T test = C_<T>(1, 2) * (bMin + bMax);
                bMin = test;
                bMax = test;
            }

            // Make correction for points inside mitered corner but outside
            // quarter sphere.
            for (auto const& point : points)
            {
                diff = point - mean;
                T u = Dot(eigenvectors[2], diff);
                T v = Dot(eigenvectors[1], diff);

                T* aExtreme = nullptr;
                T* bExtreme = nullptr;

                if (u > aMax)
                {
                    if (v > bMax)
                    {
                        aExtreme = &aMax;
                        bExtreme = &bMax;
                    }
                    else if (v < bMin)
                    {
                        aExtreme = &aMax;
                        bExtreme = &bMin;
                    }
                }
                else if (u < aMin)
                {
                    if (v > bMax)
                    {
                        aExtreme = &aMin;
                        bExtreme = &bMax;
                    }
                    else if (v < bMin)
                    {
                        aExtreme = &aMin;
                        bExtreme = &bMin;
                    }
                }

                if (aExtreme)
                {
                    T deltaU = u - *aExtreme;
                    T deltaV = v - *bExtreme;
                    T deltaSumSqr = deltaU * deltaU + deltaV * deltaV;
                    T w = Dot(eigenvectors[0], diff);
                    T wSqr = w * w;
                    T test = deltaSumSqr + wSqr;
                    if (test > rSqr)
                    {
                        T discr = (rSqr - wSqr) / deltaSumSqr;
                        T t = -std::sqrt(std::max(discr, C_<T>(0)));
                        *aExtreme = u + t * deltaU;
                        *bExtreme = v + t * deltaV;
                    }
                }
            }

            lozenge.radius = radius;
            lozenge.rectangle.axis[0] = eigenvectors[2];
            lozenge.rectangle.axis[1] = eigenvectors[1];

            if (aMin < aMax)
            {
                if (bMin < bMax)
                {
                    // Container is a lozenge.
                    lozenge.rectangle.center =
                        mean + aMin * eigenvectors[2] + bMin * eigenvectors[1];
                    lozenge.rectangle.extent[0] = C_<T>(1, 2) * (aMax - aMin);
                    lozenge.rectangle.extent[1] = C_<T>(1, 2) * (bMax - bMin);
                }
                else
                {
                    // Container is a capsule.
                    lozenge.rectangle.center = mean + aMin * eigenvectors[2] +
                        (C_<T>(1, 2) * (bMin + bMax)) * eigenvectors[1];
                    lozenge.rectangle.extent[0] = C_<T>(1, 2) * (aMax - aMin);
                    lozenge.rectangle.extent[1] = C_<T>(0);
                }
            }
            else
            {
                if (bMin < bMax)
                {
                    // Container is a capsule.
                    lozenge.rectangle.center = mean + bMin * eigenvectors[1] +
                        (C_<T>(1, 2) * (aMin + aMax)) * eigenvectors[2];
                    lozenge.rectangle.extent[0] = C_<T>(0);
                    lozenge.rectangle.extent[1] = C_<T>(1, 2) * (bMax - bMin);
                }
                else
                {
                    // Container is a sphere.
                    lozenge.rectangle.center = mean +
                        (C_<T>(1, 2) * (aMin + aMax)) * eigenvectors[2] +
                        (C_<T>(1, 2) * (bMin + bMax)) * eigenvectors[1];
                    lozenge.rectangle.extent[0] = C_<T>(0);
                    lozenge.rectangle.extent[1] = C_<T>(0);
                }
            }
        }

        // Test for containment of a point by a lozenge.
        static bool InContainer(Vector3<T> const& point, Lozenge3<T> const& lozenge)
        {
            DCPQuery<T, Vector3<T>, Rectangle3<T>> prQuery{};
            auto result = prQuery(point, lozenge.rectangle);
            return result.distance <= lozenge.radius;
        }

    private:
        friend class UnitTestContLozenge3;
    };
}
