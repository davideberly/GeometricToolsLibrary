// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The ellipse is (x/a)^2 + (y/b)^2 = 1, but only the portion in the first
// quadrant (x >= 0 and y >= 0) is approximated. Generate numArcs >= 2 arcs
// by constructing points corresponding to the weighted averages of the
// curvatures at the ellipse points (a,0) and (0,b). The returned input point
// array has numArcs+1 elements and the returned input center and radius
// arrays each have numArc elements. The arc associated with points[i] and
// points[i+1] has center centers[i] and radius radii[i].  The algorithm
// is described in
//   https://www.geometrictools.com/Documentation/ApproximateEllipse.pdf

#include <GTL/Mathematics/Containment/2D/ContScribeCircle2.h>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprEllipseByArcs
    {
    public:
        // The function returns 'true' when the approximation succeeded, in
        // which case the output arrays are nonempty. If the number of arcs
        // is smaller than 2 or a == b or one of the calls to Circumscribe
        // fails, the function returns 'false'.
        static bool Fit(T const& a, T const& b, std::size_t numArcs,
            std::vector<Vector2<T>>& points, std::vector<Vector2<T>>& centers,
            std::vector<T>& radii)
        {
            if (numArcs < 2 || a == b)
            {
                // At least 2 arcs are required and the ellipse cannot be a
                // circle already.
                points.clear();
                centers.clear();
                radii.clear();
                return false;
            }

            points.resize(numArcs + 1);
            centers.resize(numArcs);
            radii.resize(numArcs);

            // Compute intermediate ellipse quantities.
            T a2 = a * a, b2 = b * b, ab = a * b, b2ma2 = b2 - a2;

            // Compute the endpoints of the ellipse in the first quadrant. The
            // points are generated in counterclockwise order.
            points[0] = { a, C_<T>(0) };
            points[numArcs] = { C_<T>(0), b };

            // Compute the curvature at the endpoints. These are used when
            // computing the arcs.
            T curv0 = a / b2;
            T curv1 = b / a2;

            // Select the ellipse points based on curvature properties.
            T tNumArcs = static_cast<T>(numArcs);
            for (std::size_t i = 1; i < numArcs; ++i)
            {
                // The curvature at a new point is a weighted average of
                // curvature at the endpoints.
                T weight1 = static_cast<T>(i) / tNumArcs;
                T weight0 = C_<T>(1) - weight1;
                T curv = weight0 * curv0 + weight1 * curv1;

                // Compute point having this curvature.
                T tmp = std::pow(ab / curv, C_<T>(2, 3));
                points[i][0] = a * std::sqrt(std::fabs((tmp - a2) / b2ma2));
                points[i][1] = b * std::sqrt(std::fabs((tmp - b2) / b2ma2));
            }

            // Compute the arc at (a,0).
            Circle2<T> circle{};
            Vector2<T> const& p0 = points[0];
            Vector2<T> const& p1 = points[1];
            if (!Circumscribe(Vector2<T>{ p1[0], -p1[1] }, p0, p1, circle))
            {
                // This should not happen for the arc-fitting algorithm.
                points.clear();
                centers.clear();
                radii.clear();
                return false;
            }
            centers[0] = circle.center;
            radii[0] = circle.radius;

            // Compute arc at (0,b).
            std::size_t last = numArcs - 1;
            Vector2<T> const& pNm1 = points[last];
            Vector2<T> const& pN = points[numArcs];
            if (!Circumscribe(Vector2<T>{ -pNm1[0], pNm1[1] }, pN, pNm1, circle))
            {
                // This should not happen for the arc-fitting algorithm.
                points.clear();
                centers.clear();
                radii.clear();
                return false;
            }

            centers[last] = circle.center;
            radii[last] = circle.radius;

            // Compute arcs at intermediate points between (a,0) and (0,b).
            for (std::size_t iM = 0, i = 1, iP = 2; i < last; ++iM, ++i, ++iP)
            {
                Circumscribe(points[iM], points[i], points[iP], circle);
                centers[i] = circle.center;
                radii[i] = circle.radius;
            }
            return true;
        }

    private:
        friend class UnitTestApprEllipseByArcs;
    };
}
