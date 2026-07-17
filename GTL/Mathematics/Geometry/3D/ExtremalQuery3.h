// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// ExtremalQuery3 is the abstract base class for ExtremalQuery3BSP and
// ExtremalQuery3PRJ.

#include <GTL/Mathematics/Primitives/3D/ConvexPolyhedron3.h>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class ExtremalQuery3
    {
    public:
        // Abstract base class.
        virtual ~ExtremalQuery3() = default;

        // Disallow copying and assignment.
        ExtremalQuery3(ExtremalQuery3 const&) = delete;
        ExtremalQuery3& operator=(ExtremalQuery3 const&) = delete;

        // Member access.
        inline ConvexPolyhedron3<T> const& GetPolytope() const
        {
            return mPolytope;
        }

        inline std::vector<Vector3<T>> const& GetFaceNormals() const
        {
            return mFaceNormals;
        }

        // Compute the extreme vertices in the specified direction and return
        // the indices of the vertices in the polyhedron vertex array.
        virtual void GetExtremeVertices(Vector3<T> const& direction,
            std::size_t& positiveDirection, std::size_t& negativeDirection) = 0;

    protected:
        // The caller must ensure that the input polyhedron is convex.
        ExtremalQuery3(ConvexPolyhedron3<T> const& polytope)
            :
            mPolytope(polytope),
            mFaceNormals{}
        {
            // Create the face normals.
            std::size_t const numTriangles = mPolytope.indices.size() / 3;
            mFaceNormals.resize(numTriangles);
            for (std::size_t t = 0; t < numTriangles; ++t)
            {
                Vector3<T> v0 = mPolytope.vertices[mPolytope.indices[3 * t + 0]];
                Vector3<T> v1 = mPolytope.vertices[mPolytope.indices[3 * t + 1]];
                Vector3<T> v2 = mPolytope.vertices[mPolytope.indices[3 * t + 2]];
                Vector3<T> edge1 = v1 - v0;
                Vector3<T> edge2 = v2 - v0;
                mFaceNormals[t] = UnitCross(edge1, edge2);
            }
        }

        ConvexPolyhedron3<T> const& mPolytope;
        std::vector<Vector3<T>> mFaceNormals;

    private:
        friend class UnitTestExtremalQuery3;
    };
}
