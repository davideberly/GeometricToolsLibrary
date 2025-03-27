// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The circle containing the arc is in a plane with origin the circle center C
// and unit-length normal N; that is, Dot(N,X-C) = 0. In this plane the circle
// is represented as |X-C| = r where r is the circle radius. The arc is
// defined by two endpoints E0 and E1 on the circle so that E1 is obtained
// from E0 by traversing counterclockwise about the normal line C+s*N. The
// application is responsible for ensuring that the endpoints are on the
// circle, within numerical rounding errors, and that they are ordered
// correctly.

#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>

namespace gtl
{
    template <typename T>
    class Arc3
    {
    public:
        Arc3()
            :
            center{},
            normal{},
            radius(C_<T>(0)),
            end{}
        {
        }

        Arc3(Vector3<T> const& inCenter, Vector3<T> const& inNormal, T const& inRadius,
            std::array<Vector3<T>, 2> const& inEnd)
            :
            center(inCenter),
            normal(inNormal),
            radius(inRadius),
            end(inEnd)
        {
        }

        // Test whether P is on the arc.
        // 
        // Formulated for real arithmetic, |P-C| - r = 0 is necessary for P to
        // be on the circle of the arc. If P is on the circle, then P is on
        // the arc from E0 to E1 when it is on the side of the plane
        // containing E0 with normal Cross(N, E1-E0). This test works for any
        // angle between E0-C and E1-C, even if the angle is larger or equal
        // to pi radians.
        //
        // Formulated for floating-point or rational types, rounding errors
        // cause |P-C| - r rarely to be 0 when P is on (or numerically near)
        // the circle. To allow for this, choose a small and nonnegative
        // tolerance epsilon. The test concludes that P is on the circle when
        // ||P-C| - r| <= epsilon;otherwise, P is not on the circle. If P is
        // on the circle (in the epsilon-tolerance sense), the side-of-line
        // test of the previous paragraph is applied.
        bool Contains(Vector3<T> const& P, T const& epsilon) const
        {
            // If epsilon is negative, function returns false. Please ensure
            // epsilon is nonnegative.

            Vector3<T> PmC = P - center;
            if (Dot(normal, PmC) <= epsilon &&
                std::fabs(Length(PmC) - radius) <= epsilon)
            {
                return Contains(P);
            }
            else
            {
                return false;
            }
        }

        // This function assumes P is on the circle containing the arc with
        // possibly a small amount of floating-point rounding error.
        bool Contains(Vector3<T> const& P) const
        {
            Vector3<T> PmE0 = P - end[0];
            Vector3<T> E1mE0 = end[1] - end[0];
            T dotCross = DotCross(PmE0, E1mE0, normal);
            return dotCross >= C_<T>(0);
        }

        Vector3<T> center;
        Vector3<T> normal;
        T radius;
        std::array<Vector3<T>, 2> end;

    private:
        friend class UnitTestArc3;
    };

    // Comparisons to support sorted containers.
    template <typename T>
    bool operator==(Arc3<T> const& arc0, Arc3<T> const& arc1)
    {
        return arc0.center == arc1.center
            && arc0.normal == arc1.normal
            && arc0.radius == arc1.radius
            && arc0.end == arc1.end;
    }

    template <typename T>
    bool operator!=(Arc3<T> const& arc0, Arc3<T> const& arc1)
    {
        return !operator==(arc0, arc1);
    }

    template <typename T>
    bool operator<(Arc3<T> const& arc0, Arc3<T> const& arc1)
    {
        if (arc0.center < arc1.center)
        {
            return true;
        }

        if (arc0.center > arc1.center)
        {
            return false;
        }

        if (arc0.normal < arc1.normal)
        {
            return true;
        }

        if (arc0.normal > arc1.normal)
        {
            return false;
        }

        if (arc0.radius < arc1.radius)
        {
            return true;
        }

        if (arc0.radius > arc1.radius)
        {
            return false;
        }

        return arc0.end < arc1.end;
    }

    template <typename T>
    bool operator<=(Arc3<T> const& arc0, Arc3<T> const& arc1)
    {
        return !operator<(arc1, arc0);
    }

    template <typename T>
    bool operator>(Arc3<T> const& arc0, Arc3<T> const& arc1)
    {
        return operator<(arc1, arc0);
    }

    template <typename T>
    bool operator>=(Arc3<T> const& arc0, Arc3<T> const& arc1)
    {
        return !operator<(arc0, arc1);
    }
}
