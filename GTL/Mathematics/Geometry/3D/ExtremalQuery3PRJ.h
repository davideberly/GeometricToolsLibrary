// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// Compute the extreme vertices of a convex polyhedron in a specified
// direction by an exhaustive search over the projection of the
// vertices onto that direction.

#include <GTL/Mathematics/Geometry/3D/ExtremalQuery3.h>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class ExtremalQuery3PRJ : public ExtremalQuery3<T>
    {
    public:
        ExtremalQuery3PRJ(ConvexPolyhedron3<T> const& polytope)
            :
            ExtremalQuery3<T>(polytope),
            mCentroid{}  // zero vector
        {
            for (auto const& vertex : polytope.vertices)
            {
                mCentroid += vertex;
            }
            mCentroid /= static_cast<T>(polytope.vertices.size());
        }

        // Disallow copying and assignment.
        ExtremalQuery3PRJ(ExtremalQuery3PRJ const&) = delete;
        ExtremalQuery3PRJ& operator=(ExtremalQuery3PRJ const&) = delete;

        // Compute the extreme vertices in the specified direction and return
        // the indices of the vertices in the polyhedron vertex array.
        virtual void GetExtremeVertices(Vector3<T> const& direction,
            std::size_t& positiveDirection, std::size_t& negativeDirection) override
        {
            T minValue = std::numeric_limits<T>::max();
            T maxValue = -std::numeric_limits<T>::max();
            negativeDirection = std::numeric_limits<std::size_t>::max();
            positiveDirection = std::numeric_limits<std::size_t>::max();

            for (auto i : this->mPolytope.indices)
            {
                Vector3<T> diff = this->mPolytope.vertices[i] - mCentroid;
                T dot = Dot(direction, diff);
                if (dot < minValue)
                {
                    negativeDirection = i;
                    minValue = dot;
                }
                if (dot > maxValue)
                {
                    positiveDirection = i;
                    maxValue = dot;
                }
            }
        }

    private:
        Vector3<T> mCentroid;

    private:
        friend class UnitTestExtremalQuery3PRJ;
    };
}
