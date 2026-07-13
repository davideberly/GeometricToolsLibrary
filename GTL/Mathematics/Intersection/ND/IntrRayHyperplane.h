// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.13

#pragma once

#include <GTL/Mathematics/Intersection/ND/IntrLineHyperplane.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>
#include <cstddef>

namespace gtl
{
    template <typename T, std::size_t N>
    class TIQuery<T, Ray<T, N>, Hyperplane<T, N>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false)
            {
            }

            bool intersect;
        };

        // The ray is P + t * D for t >= 0, where P is the origin and D is the
        // ray direction. It is not necessary that D have unit length. It is
        // also not necessary that the hyperplane normal be unit length.
        Output operator()(Ray<T, N> const& ray, Hyperplane<T, N> const& hyperplane)
        {
            Output output{};

            // Compute the signed distance from the ray origin to the
            // hyperplane.
            DCPQuery<T, Vector<T, N>, Hyperplane<T, N>> vpQuery{};
            auto vpOutput = vpQuery(ray.origin, hyperplane);

            T DdN = Dot(ray.direction, hyperplane.normal);
            if (DdN > C_<T>(0))
            {
                // The ray is not parallel to the hyperplane and is directed
                // toward the +normal side of the hyperplane.
                output.intersect = (vpOutput.signedDistance <= C_<T>(0));
            }
            else if (DdN < C_<T>(0))
            {
                // The ray is not parallel to the hyperplane and is directed
                // toward the -normal side of the plane.
                output.intersect = (vpOutput.signedDistance >= C_<T>(0));
            }
            else
            {
                // The ray and plane are parallel.
                output.intersect = (vpOutput.distance == C_<T>(0));
            }

            return output;
        }

    private:
        friend class UnitTestIntrRayHyperplane;
    };

    template <typename T, std::size_t N>
    class FIQuery<T, Ray<T, N>, Hyperplane<T, N>>
        :
        public FIQuery<T, Line<T, N>, Hyperplane<T, N>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line<T, N>, Hyperplane<T, N>>::Output
        {
            // No additional information to compute.
        };

        // The ray is P + t * D for t >= 0, where P is the origin and D is the
        // ray direction. It is not necessary that D have unit length. It is
        // also not necessary that the hyperplane normal be unit length.
        Output operator()(Ray<T, N> const& ray, Hyperplane<T, N> const& hyperplane)
        {
            Output output{};
            DoQuery(ray.origin, ray.direction, hyperplane, output);
            if (output.intersect)
            {
                output.point = ray.origin + output.parameter * ray.direction;
            }
            return output;
        }

    protected:
        // The caller must ensure that on entry, 'output' is default
        // constructed as if there is no intersection. If an intersection is
        // found, the 'output' values will be modified accordingly.
        void DoQuery(Vector<T, N> const& rayOrigin, Vector<T, N> const& rayDirection,
            Hyperplane<T, N> const& hyperplane, Output& output)
        {
            FIQuery<T, Line<T, N>, Hyperplane<T, N>>::DoQuery(
                rayOrigin, rayDirection, hyperplane, output);

            if (output.intersect)
            {
                // The line intersects the plane in a point that might not be
                // on the ray.
                if (output.parameter < C_<T>(0))
                {
                    output = Output{};
                }
            }
        }

    private:
        friend class UnitTestIntrRayHyperplane;
    };
}
