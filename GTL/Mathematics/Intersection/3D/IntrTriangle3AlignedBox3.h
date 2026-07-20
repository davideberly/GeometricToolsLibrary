// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2025
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
// https://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 0.0.2026.07.19

#pragma once

// The test-intersection query is based on the document
// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf
// The find-intersection query clips the triangle against the faces of
// the oriented box.

#include <GTL/Mathematics/Primitives/ND/AlignedBox.h>
#include <GTL/Mathematics/Intersection/3D/IntrTriangle3CanonicalBox3.h>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Triangle3<T>, AlignedBox3<T>>
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

        Output operator()(Triangle3<T> const& triangle, AlignedBox3<T> const& box)
        {
            Output output{};

            // Transform the aligned box to a canonical box. Transform the
            // vertices accordingly.
            T const half = static_cast<T>(0.5);
            CanonicalBox3<T> canonicalBox = half * (box.max - box.min);
            Vector3<T> alignedBoxCenter = half * (box.max + box.min);

            Triangle3<T> transformedTriangle{};
            for (std::size_t i = 0; i < 3; ++i)
            {
                transformedTriangle.v[i] = triangle.v[i] - alignedBoxCenter;
            }

            TIQuery<T, Triangle3<T>, CanonicalBox3<T>> query{};
            output.intersect = query(transformedTriangle, canonicalBox).intersect;
            return output;
        }

    private:
        friend class UnitTestTriangle3AlignedBox3;
    };

    template <typename T>
    class FIQuery<T, Triangle3<T>, AlignedBox3<T>>
    {
    public:
        using PPQuery = FIQuery<T, std::vector<Vector3<T>>, Plane3<T>>;
        using PPOutput = typename PPQuery::Output;

        struct Output
        {
            Output()
                :
                insidePolygon{},
                outsidePolygons{}
            {
            }

            std::vector<Vector3<T>> insidePolygon;
            std::vector<std::vector<Vector3<T>>> outsidePolygons;
        };

        Output operator()(Triangle3<T> const& triangle, AlignedBox3<T> const& box)
        {
            Output output{};

            // Start with the triangle and clip it against each face of the
            // box. The largest number of vertices for the polygon of
            // intersection is 7.
            output.insidePolygon.resize(3);
            for (std::size_t i = 0; i < 3; ++i)
            {
                output.insidePolygon[i] = triangle.v[i];
            }

            // Create planes for the box faces that with normals that point
            // inside the box.
            T const half = static_cast<T>(0.5);
            Vector3<T> center = half * (box.max + box.min);
            Vector3<T> extent = half * (box.max - box.min);
            std::array<Plane3<T>, 6> planes{};
            planes[0].normal = -Vector3<T>::Unit(0);
            planes[0].constant = Dot(planes[0].normal, center) - extent[0];
            planes[1].normal = -Vector3<T>::Unit(1);
            planes[1].constant = Dot(planes[1].normal, center) - extent[1];
            planes[2].normal = -Vector3<T>::Unit(2);
            planes[2].constant = Dot(planes[2].normal, center) - extent[2];
            planes[3].normal = +Vector3<T>::Unit(0);
            planes[3].constant = Dot(planes[3].normal, center) - extent[0];
            planes[4].normal = +Vector3<T>::Unit(1);
            planes[4].constant = Dot(planes[4].normal, center) - extent[1];
            planes[5].normal = +Vector3<T>::Unit(2);
            planes[5].constant = Dot(planes[5].normal, center) - extent[2];

            for (auto const& plane : planes)
            {
                PPOutput ppOutput = PPQuery{}(output.insidePolygon, plane);
                switch (ppOutput.configuration)
                {
                case PPQuery::Configuration::SPLIT:
                    output.insidePolygon = ppOutput.positivePolygon;
                    output.outsidePolygons.push_back(ppOutput.negativePolygon);
                    break;
                case PPQuery::Configuration::POSITIVE_SIDE_VERTEX:
                case PPQuery::Configuration::POSITIVE_SIDE_EDGE:
                case PPQuery::Configuration::POSITIVE_SIDE_STRICT:
                    // The output.insidePolygon is already
                    // ppOutput.positivePolygon, but to make it clear,
                    // assign it here.
                    output.insidePolygon = ppOutput.positivePolygon;
                    break;
                case PPQuery::Configuration::NEGATIVE_SIDE_VERTEX:
                case PPQuery::Configuration::NEGATIVE_SIDE_EDGE:
                case PPQuery::Configuration::NEGATIVE_SIDE_STRICT:
                    output.insidePolygon.clear();
                    output.outsidePolygons.push_back(ppOutput.negativePolygon);
                    return output;
                case PPQuery::Configuration::CONTAINED:
                    // A triangle coplanar with a box face will be processed
                    // as if it were inside the box.
                    output.insidePolygon = ppOutput.intersection;
                    break;
                default:
                    output.insidePolygon.clear();
                    output.outsidePolygons.clear();
                    break;
                }
            }

            return output;
        }

    private:
        friend class UnitTestTriangle3AlignedBox3;
    };
}
