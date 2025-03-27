// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The intersection queries are based on the document
// https://www.geometrictools.com/Documentation/ClipConvexPolygonByHyperplane.pdf

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/Hyperplane.h>
#include <algorithm>
#include <cstddef>
#include <limits>
#include <list>
#include <vector>

namespace gtl
{
    template <typename T, std::size_t N>
    class TIQuery<T, std::vector<Vector<T, N>>, Hyperplane<T, N>>
    {
    public:
        enum class Configuration
        {
            SPLIT,
            POSITIVE_SIDE_VERTEX,
            POSITIVE_SIDE_EDGE,
            POSITIVE_SIDE_STRICT,
            NEGATIVE_SIDE_VERTEX,
            NEGATIVE_SIDE_EDGE,
            NEGATIVE_SIDE_STRICT,
            CONTAINED,
            INVALID_POLYGON
        };

        struct Output
        {
            Output()
                :
                intersect(false),
                configuration(Configuration::INVALID_POLYGON)
            {
            }

            bool intersect;
            Configuration configuration;
        };

        Output operator()(std::vector<Vector<T, N>> const& polygon, Hyperplane<T, N> const& hyperplane)
        {
            Output output{};

            std::size_t const numVertices = polygon.size();
            if (numVertices < 3)
            {
                // The convex polygon must have at least 3 vertices.
                output.intersect = false;
                output.configuration = Configuration::INVALID_POLYGON;
                return output;
            }

            // Determine on which side of the hyperplane each vertex lies.
            std::size_t numPositive = 0, numNegative = 0, numZero = 0;
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                T h = Dot(hyperplane.normal, polygon[i]) - hyperplane.constant;
                if (h > C_<T>(0))
                {
                    ++numPositive;
                }
                else if (h < C_<T>(0))
                {
                    ++numNegative;
                }
                else
                {
                    ++numZero;
                }
            }

            if (numPositive > 0)
            {
                if (numNegative > 0)
                {
                    output.intersect = true;
                    output.configuration = Configuration::SPLIT;
                }
                else if (numZero == 0)
                {
                    output.intersect = false;
                    output.configuration = Configuration::POSITIVE_SIDE_STRICT;
                }
                else if (numZero == 1)
                {
                    output.intersect = true;
                    output.configuration = Configuration::POSITIVE_SIDE_VERTEX;
                }
                else // numZero > 1
                {
                    output.intersect = true;
                    output.configuration = Configuration::POSITIVE_SIDE_EDGE;
                }
            }
            else if (numNegative > 0)
            {
                if (numZero == 0)
                {
                    output.intersect = false;
                    output.configuration = Configuration::NEGATIVE_SIDE_STRICT;
                }
                else if (numZero == 1)
                {
                    // The polygon touches the plane in a vertex or an edge.
                    output.intersect = true;
                    output.configuration = Configuration::NEGATIVE_SIDE_VERTEX;
                }
                else // numZero > 1
                {
                    output.intersect = true;
                    output.configuration = Configuration::NEGATIVE_SIDE_EDGE;
                }
            }
            else  // numZero == numVertices
            {
                output.intersect = true;
                output.configuration = Configuration::CONTAINED;
            }

            return output;
        }

    private:
        friend class UnitTestIntrConvexPolygonHyperplane;
    };

    template <typename T, std::size_t N>
    class FIQuery<T, std::vector<Vector<T, N>>, Hyperplane<T, N>>
    {
    public:
        enum class Configuration
        {
            SPLIT,
            POSITIVE_SIDE_VERTEX,
            POSITIVE_SIDE_EDGE,
            POSITIVE_SIDE_STRICT,
            NEGATIVE_SIDE_VERTEX,
            NEGATIVE_SIDE_EDGE,
            NEGATIVE_SIDE_STRICT,
            CONTAINED,
            INVALID_POLYGON
        };

        struct Output
        {
            Output()
                :
                intersect(false),
                configuration(Configuration::INVALID_POLYGON),
                intersection{},
                positivePolygon{},
                negativePolygon{}
            {
            }

            // The intersection is either empty, a single vertex, a single
            // edge or the polygon is contained by the hyperplane.
            bool intersect;
            Configuration configuration;
            std::vector<Vector<T, N>> intersection;

            // If 'configuration' is POSITIVE_* or SPLIT, this polygon is the
            // portion of the query input 'polygon' on the positive side of
            // the hyperplane with possibly a vertex or edge on the hyperplane.
            std::vector<Vector<T, N>> positivePolygon;

            // If 'configuration' is NEGATIVE_* or SPLIT, this polygon is the
            // portion of the query input 'polygon' on the negative side of
            // the hyperplane with possibly a vertex or edge on the hyperplane.
            std::vector<Vector<T, N>> negativePolygon;
        };

        Output operator()(std::vector<Vector<T, N>> const& polygon, Hyperplane<T, N> const& hyperplane)
        {
            Output output{};

            std::size_t const numVertices = polygon.size();
            if (numVertices < 3)
            {
                // The convex polygon must have at least 3 vertices.
                output.intersect = false;
                output.configuration = Configuration::INVALID_POLYGON;
                return output;
            }

            // Determine on which side of the hyperplane the vertices live.
            // The index maxPosIndex stores the index of the vertex on the
            // positive side of the hyperplane that is farthest from the
            // hyperplane. The index maxNegIndex stores the index of the
            // vertex on the negative side of the hyperplane that is farthest
            // from the hyperplane. If one or the other such vertex does not
            // exist, the corresponding index will remain its initial value of
            // max(std::size_t).
            std::vector<T> height(numVertices);
            std::vector<std::size_t> zeroHeightIndices{};
            zeroHeightIndices.reserve(numVertices);
            std::size_t numPositive = 0, numNegative = 0;
            T maxPosHeight = C_<T>(0);
            T maxNegHeight = C_<T>(0);
            std::size_t maxPosIndex = std::numeric_limits<std::size_t>::max();
            std::size_t maxNegIndex = std::numeric_limits<std::size_t>::max();
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                height[i] = Dot(hyperplane.normal, polygon[i]) - hyperplane.constant;
                if (height[i] > C_<T>(0))
                {
                    ++numPositive;
                    if (height[i] > maxPosHeight)
                    {
                        maxPosHeight = height[i];
                        maxPosIndex = i;
                    }
                }
                else if (height[i] < C_<T>(0))
                {
                    ++numNegative;
                    if (height[i] < maxNegHeight)
                    {
                        maxNegHeight = height[i];
                        maxNegIndex = i;
                    }
                }
                else
                {
                    zeroHeightIndices.push_back(i);
                }
            }

            if (numPositive > 0)
            {
                if (numNegative > 0)
                {
                    output.intersect = true;
                    output.configuration = Configuration::SPLIT;

                    bool doSwap = (maxPosHeight < -maxNegHeight);
                    if (doSwap)
                    {
                        for (auto& h : height)
                        {
                            h = -h;
                        }
                        std::swap(maxPosIndex, maxNegIndex);
                    }

                    SplitPolygon(polygon, height, maxPosIndex, output);

                    if (doSwap)
                    {
                        std::swap(output.positivePolygon, output.negativePolygon);
                    }
                }
                else
                {
                    std::size_t numZero = zeroHeightIndices.size();
                    if (numZero == 0)
                    {
                        output.intersect = false;
                        output.configuration = Configuration::POSITIVE_SIDE_STRICT;
                    }
                    else if (numZero == 1)
                    {
                        output.intersect = true;
                        output.configuration = Configuration::POSITIVE_SIDE_VERTEX;
                        output.intersection =
                        {
                            polygon[zeroHeightIndices[0]]
                        };
                    }
                    else // numZero > 1
                    {
                        output.intersect = true;
                        output.configuration = Configuration::POSITIVE_SIDE_EDGE;
                        output.intersection =
                        {
                            polygon[zeroHeightIndices[0]],
                            polygon[zeroHeightIndices[1]]
                        };
                    }
                    output.positivePolygon = polygon;
                }
            }
            else if (numNegative > 0)
            {
                std::size_t numZero = zeroHeightIndices.size();
                if (numZero == 0)
                {
                    output.intersect = false;
                    output.configuration = Configuration::NEGATIVE_SIDE_STRICT;
                }
                else if (numZero == 1)
                {
                    output.intersect = true;
                    output.configuration = Configuration::NEGATIVE_SIDE_VERTEX;
                    output.intersection =
                    {
                        polygon[zeroHeightIndices[0]]
                    };
                }
                else  // numZero > 1
                {
                    output.intersect = true;
                    output.configuration = Configuration::NEGATIVE_SIDE_EDGE;
                    output.intersection =
                    {
                        polygon[zeroHeightIndices[0]],
                        polygon[zeroHeightIndices[1]]
                    };
                }
                output.negativePolygon = polygon;
            }
            else  // numZero == numVertices
            {
                output.intersect = true;
                output.configuration = Configuration::CONTAINED;
                output.intersection = polygon;
            }

            return output;
        }

    protected:
        void SplitPolygon(std::vector<Vector<T, N>> const& polygon,
            std::vector<T> const& height, std::size_t maxPosIndex, Output& output)
        {
            // Find the largest contiguous subset of indices for which
            // height[i] >= 0.
            std::size_t const numVertices = polygon.size();
            std::list<Vector<T, N>> positiveList{};
            positiveList.push_back(polygon[maxPosIndex]);
            std::size_t end0 = maxPosIndex;
            std::size_t end0prev = std::numeric_limits<std::size_t>::max();
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                end0prev = (end0 + numVertices - 1) % numVertices;
                if (height[end0prev] >= C_<T>(0))
                {
                    positiveList.push_front(polygon[end0prev]);
                    end0 = end0prev;
                }
                else
                {
                    break;
                }
            }

            std::size_t end1 = maxPosIndex;
            std::size_t end1next = std::numeric_limits<std::size_t>::max();
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                end1next = (end1 + 1) % numVertices;
                if (height[end1next] >= C_<T>(0))
                {
                    positiveList.push_back(polygon[end1next]);
                    end1 = end1next;
                }
                else
                {
                    break;
                }
            }

            std::size_t index = end1next;
            std::list<Vector<T, N>> negativeList{};
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                negativeList.push_back(polygon[index]);
                index = (index + 1) % numVertices;
                if (index == end0)
                {
                    break;
                }
            }

            // Clip the polygon.
            if (height[end0] > C_<T>(0))
            {
                T t = -height[end0prev] / (height[end0] - height[end0prev]);
                T omt = C_<T>(1) - t;
                Vector<T, N> V = omt * polygon[end0prev] + t * polygon[end0];
                positiveList.push_front(V);
                negativeList.push_back(V);
                output.intersection.push_back(V);
            }
            else
            {
                negativeList.push_back(polygon[end0]);
                output.intersection.push_back(polygon[end0]);
            }

            if (height[end1] > C_<T>(0))
            {
                T t = -height[end1next] / (height[end1] - height[end1next]);
                T omt = C_<T>(1) - t;
                Vector<T, N> V = omt * polygon[end1next] + t * polygon[end1];
                positiveList.push_back(V);
                negativeList.push_front(V);
                output.intersection.push_back(V);
            }
            else
            {
                negativeList.push_front(polygon[end1]);
                output.intersection.push_back(polygon[end1]);
            }

            output.positivePolygon.reserve(positiveList.size());
            for (auto const& p : positiveList)
            {
                output.positivePolygon.push_back(p);
            }

            output.negativePolygon.reserve(negativeList.size());
            for (auto const& p : negativeList)
            {
                output.negativePolygon.push_back(p);
            }
        }

    private:
        friend class UnitTestIntrConvexPolygonHyperplane;
    };
}
