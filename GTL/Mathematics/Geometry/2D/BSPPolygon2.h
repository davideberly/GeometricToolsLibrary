// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Compute Boolean operations on polygons (union, intersection, difference
// and exclusive-or. The algorithm uses Binary Space Partitioning (BSP)
// trees.

#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/Meshes/EdgeKey.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class BSPPolygon2
    {
    public:
        // vertices
        using Vertex = Vector2<T>;
        using VMap = std::map<Vertex, std::size_t>;
        using VArray = std::vector<Vertex>;

        // edges
        using Edge = EdgeKey<true>;
        using EMap = std::map<Edge, std::size_t>;
        using EArray = std::vector<Edge>;

        BSPPolygon2(T const& epsilon)
            :
            mEpsilon(epsilon >= C_<T>(0) ? epsilon : C_<T>(0)),
            mVMap{},
            mVArray{},
            mEMap{},
            mEArray{},
            mTree{}
        {
        }

        ~BSPPolygon2() = default;

        // Copy semantics.
        BSPPolygon2(BSPPolygon2 const& polygon)
            :
            mEpsilon(C_<T>(0)),
            mVMap{},
            mVArray{},
            mEMap{},
            mEArray{},
            mTree{}
        {
            *this = polygon;
        }

        BSPPolygon2& operator=(BSPPolygon2 const& polygon)
        {
            mEpsilon = polygon.mEpsilon;
            mVMap = polygon.mVMap;
            mVArray = polygon.mVArray;
            mEMap = polygon.mEMap;
            mEArray = polygon.mEArray;
            mTree = (polygon.mTree ? polygon.mTree->GetCopy() : nullptr);
            return *this;
        }

        // Move semantics.
        BSPPolygon2(BSPPolygon2&& polygon) noexcept
            :
            mEpsilon(C_<T>(0)),
            mVMap{},
            mVArray{},
            mEMap{},
            mEArray{},
            mTree{}
        {
            *this = std::move(polygon);
        }

        BSPPolygon2& operator=(BSPPolygon2&& polygon) noexcept
        {
            mEpsilon = polygon.mEpsilon;
            mVMap = std::move(polygon.mVMap);
            mVArray = std::move(polygon.mVArray);
            mEMap = std::move(polygon.mEMap);
            mEArray = std::move(polygon.mEArray);
            mTree = polygon.mTree;
            polygon.mTree = nullptr;
            return *this;
        }

        // Support for deferred construction.
        std::size_t InsertVertex(Vertex const& vertex)
        {
            auto iter = mVMap.find(vertex);
            if (iter != mVMap.end())
            {
                // Vertex already in map, just return its unique index.
                return iter->second;
            }

            // Vertex not in map, insert it and assign it a unique index.
            std::size_t i = mVArray.size();
            mVMap.insert(std::make_pair(vertex, i));
            mVArray.push_back(vertex);
            return i;
        }

        std::size_t InsertEdge(Edge const& edge)
        {
            GTL_ARGUMENT_ASSERT(
                edge[0] != edge[1],
                "Degenerate edges not allowed.");

            auto iter = mEMap.find(edge);
            if (iter != mEMap.end())
            {
                // Edge already in map, just return its unique index.
                return iter->second;
            }

            // Edge not in map, insert it and assign it a unique index.
            std::size_t i = mEArray.size();
            mEMap.insert(std::make_pair(edge, i));
            mEArray.push_back(edge);
            return i;
        }

        void Finalize()
        {
            // The BSPTree2 constructor is badly designed. The '*this'
            // is passed as non-const but the previous code in this function
            // passed 'this->mEArray' as const. The mEArray can be modified
            // via '*this' in the BSPTree2 class, which led to an infinite
            // loop in a test data set for a bug report. For now, make a
            // copy of mEArray and pass it.
            EArray eArray = mEArray;
            mTree = std::make_shared<BSPTree2>(*this, eArray, mEpsilon);
        }

        // Member access.
        inline std::size_t GetNumVertices() const
        {
            return mVMap.size();
        }

        inline Vertex const& GetVertex(std::size_t i) const
        {
            return mVArray[i];
        }

        inline std::size_t GetNumEdges() const
        {
            return mEMap.size();
        }

        inline Edge const& GetEdge(std::size_t i) const
        {
            return mEArray[i];
        }

        // negation
        BSPPolygon2 operator~() const
        {
            GTL_RUNTIME_ASSERT(
                mTree != nullptr,
                "Tree must exist.");

            BSPPolygon2 neg(mEpsilon);
            neg.mVMap = mVMap;
            neg.mVArray = mVArray;

            for (auto const& element : mEMap)
            {
                auto const& edge = element.first;
                neg.InsertEdge(Edge(edge[1], edge[0]));
            }

            neg.mTree = mTree->GetCopy();
            neg.mTree->Negate();
            return neg;
        }

        // intersection
        BSPPolygon2 operator&(BSPPolygon2 const& polygon) const
        {
            GTL_RUNTIME_ASSERT(
                mTree != nullptr,
                "Tree must exist.");

            BSPPolygon2 intersect(mEpsilon);
            GetInsideEdgesFrom(polygon, intersect);
            polygon.GetInsideEdgesFrom(*this, intersect);
            intersect.Finalize();
            return intersect;
        }

        // union
        BSPPolygon2 operator|(BSPPolygon2 const& polygon) const
        {
            return ~(~*this & ~polygon);
        }

        // difference
        BSPPolygon2 operator-(BSPPolygon2 const& polygon) const
        {
            return *this & ~polygon;
        }

        // exclusive or
        BSPPolygon2 operator^(BSPPolygon2 const& polygon) const
        {
            return (*this - polygon) | (polygon - *this);
        }

        // point location (-1 inside, 0 on polygon, 1 outside)
        std::int32_t PointLocation(Vertex const& vertex) const
        {
            GTL_RUNTIME_ASSERT(
                mTree != nullptr,
                "Tree must exist.");

            return mTree->PointLocation(*this, vertex);
        }

        // debugging support
        void Print(std::string const& filename) const
        {
            std::ofstream output(filename);

            std::size_t const numVertices = GetNumVertices();
            output << "vquantity = " << numVertices << std::endl;
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                output << i << "  (" << mVArray[i][0] << ',' << mVArray[i][1] << ')' << std::endl;
            }
            output << std::endl;

            std::size_t const numEdges = GetNumEdges();
            output << "equantity = " << numEdges << std::endl;
            for (std::size_t i = 0; i < numEdges; ++i)
            {
                output << "  <" << mEArray[i][0] << ',' << mEArray[i][1] << '>' << std::endl;
            }
            output << std::endl;

            output << "bsp tree" << std::endl;
            if (mTree)
            {
                mTree->Print(output, 0, 'r');
            }
            output << std::endl;
            output.close();
        }

    private:
        // Binary space partitioning tree support for polygon Boolean
        // operations.
        class BSPTree2
        {
        public:
            // Construction and destruction.
            BSPTree2(T const& epsilon)
                :
                mEpsilon(epsilon >= C_<T>(0) ? epsilon : C_<T>(0)),
                mPosChild{},
                mNegChild{}
            {
            }

            BSPTree2(BSPPolygon2& polygon, EArray const& edges, T const& epsilon)
                :
                mEpsilon(epsilon >= C_<T>(0) ? epsilon : C_<T>(0)),
                mPosChild{},
                mNegChild{}
            {
                GTL_ARGUMENT_ASSERT(
                    edges.size() > 0,
                    "Invalid input.");

                // Construct splitting line from first edge.
                Vertex end0 = polygon.GetVertex(edges[0][0]);
                Vertex end1 = polygon.GetVertex(edges[0][1]);

                // Add edge to coincident list.
                mCoincident.push_back(edges[0]);

                // Split remaining edges.
                EArray posArray{}, negArray{};
                for (std::size_t i = 1; i < edges.size(); ++i)
                {
                    std::size_t v0 = edges[i][0];
                    std::size_t v1 = edges[i][1];
                    Vertex vertex0 = polygon.GetVertex(v0);
                    Vertex vertex1 = polygon.GetVertex(v1);

                    Vertex intr{};
                    std::size_t vmid{};

                    switch (Classify(end0, end1, vertex0, vertex1, intr))
                    {
                    case TRANSVERSE_POSITIVE:
                        // modify edge <V0,V1> to <V0,I>, add new edge <I,V1>
                        vmid = polygon.InsertVertex(intr);
                        if (vmid == v0 || vmid == v1)
                        {
                            // The intersection point is within epsilon of an
                            // endpoint.
                            mCoincident.push_back(edges[i]);
                        }
                        else
                        {
                            polygon.SplitEdge(v0, v1, vmid);
                            posArray.push_back(Edge(vmid, v1));
                            negArray.push_back(Edge(v0, vmid));
                        }
                        break;
                    case TRANSVERSE_NEGATIVE:
                        // modify edge <V0,V1> to <V0,I>, add new edge <I,V1>
                        vmid = polygon.InsertVertex(intr);
                        if (vmid == v0 || vmid == v1)
                        {
                            // The intersection point is within epsilon of an
                            // endpoint.
                            mCoincident.push_back(edges[i]);
                        }
                        else
                        {
                            polygon.SplitEdge(v0, v1, vmid);
                            posArray.push_back(Edge(v0, vmid));
                            negArray.push_back(Edge(vmid, v1));
                        }
                        break;
                    case ALL_POSITIVE:
                        posArray.push_back(edges[i]);
                        break;
                    case ALL_NEGATIVE:
                        negArray.push_back(edges[i]);
                        break;
                    default:  // COINCIDENT
                        mCoincident.push_back(edges[i]);
                        break;
                    }
                }

                if (posArray.size() > 0)
                {
                    mPosChild = std::make_shared<BSPTree2>(polygon, posArray, mEpsilon);
                }

                if (negArray.size() > 0)
                {
                    mNegChild = std::make_shared<BSPTree2>(polygon, negArray, mEpsilon);
                }
            }

            ~BSPTree2() = default;

            // Disallow copying and assignment. Use GetCopy() instead to
            // obtain a deep copy of the BSP tree.
            BSPTree2(const BSPTree2&) = delete;
            BSPTree2& operator= (const BSPTree2&) = delete;

            std::shared_ptr<BSPTree2> GetCopy() const
            {
                auto tree = std::make_shared<BSPTree2>(mEpsilon);

                tree->mCoincident = mCoincident;

                if (mPosChild)
                {
                    tree->mPosChild = mPosChild->GetCopy();
                }

                if (mNegChild)
                {
                    tree->mNegChild = mNegChild->GetCopy();
                }

                return tree;
            }

            // Polygon Boolean operation support.
            void Negate()
            {
                // Reverse coincident edge directions.
                for (auto& edge : mCoincident)
                {
                    std::swap(edge[0], edge[1]);
                }

                // Swap positive and negative subtrees.
                std::swap(mPosChild, mNegChild);

                if (mPosChild)
                {
                    mPosChild->Negate();
                }

                if (mNegChild)
                {
                    mNegChild->Negate();
                }
            }

            void GetPartition(BSPPolygon2 const& polygon, Vertex const& v0,
                Vertex const& v1, BSPPolygon2& pos, BSPPolygon2& neg,
                BSPPolygon2& coSame, BSPPolygon2& coDiff) const
            {
                // Construct splitting line from first coincident edge.
                Vertex end0 = polygon.GetVertex(mCoincident[0][0]);
                Vertex end1 = polygon.GetVertex(mCoincident[0][1]);

                Vertex intr{};

                switch (Classify(end0, end1, v0, v1, intr))
                {
                case TRANSVERSE_POSITIVE:
                    GetPosPartition(polygon, intr, v1, pos, neg, coSame, coDiff);
                    GetNegPartition(polygon, v0, intr, pos, neg, coSame, coDiff);
                    break;
                case TRANSVERSE_NEGATIVE:
                    GetPosPartition(polygon, v0, intr, pos, neg, coSame, coDiff);
                    GetNegPartition(polygon, intr, v1, pos, neg, coSame, coDiff);
                    break;
                case ALL_POSITIVE:
                    GetPosPartition(polygon, v0, v1, pos, neg, coSame, coDiff);
                    break;
                case ALL_NEGATIVE:
                    GetNegPartition(polygon, v0, v1, pos, neg, coSame, coDiff);
                    break;
                default:  // COINCIDENT
                    GetCoPartition(polygon, v0, v1, pos, neg, coSame, coDiff);
                    break;
                }
            }

            // Point-in-polygon support (-1 outside, 0 on polygon, +1 inside).
            std::int32_t PointLocation(BSPPolygon2 const& polygon, Vertex const& vertex) const
            {
                // Construct splitting line from first coincident edge.
                Vertex end0 = polygon.GetVertex(mCoincident[0][0]);
                Vertex end1 = polygon.GetVertex(mCoincident[0][1]);

                switch (Classify(end0, end1, vertex))
                {
                case ALL_POSITIVE:
                    if (mPosChild)
                    {
                        return mPosChild->PointLocation(polygon, vertex);
                    }
                    else
                    {
                        return 1;
                    }
                case ALL_NEGATIVE:
                    if (mNegChild)
                    {
                        return mNegChild->PointLocation(polygon, vertex);
                    }
                    else
                    {
                        return -1;
                    }
                default:  // COINCIDENT
                    return CoPointLocation(polygon, vertex);
                }
            }

            void Print(std::ofstream& outFile, std::size_t level, char type) const
            {
                for (std::size_t i = 0; i < mCoincident.size(); ++i)
                {
                    for (std::size_t j = 0; j < 4 * level; ++j)
                    {
                        outFile << ' ';
                    }

                    outFile << type << " <" << mCoincident[i][0] << ',' <<
                        mCoincident[i][1] << ">" << std::endl;
                }

                if (mPosChild)
                {
                    mPosChild->Print(outFile, level + 1, 'p');
                }

                if (mNegChild)
                {
                    mNegChild->Print(outFile, level + 1, 'n');
                }
            }

        private:
            static std::uint32_t constexpr TRANSVERSE_POSITIVE = 0;
            static std::uint32_t constexpr TRANSVERSE_NEGATIVE = 1;
            static std::uint32_t constexpr ALL_POSITIVE = 2;
            static std::uint32_t constexpr ALL_NEGATIVE = 3;
            static std::uint32_t constexpr COINCIDENT = 4;

            std::uint32_t Classify(Vertex const& end0, Vertex const& end1,
                Vertex const& v0, Vertex const& v1, Vertex& intr) const
            {
                Vertex dir = end1 - end0;
                Vertex nor = Perp(dir);
                Vertex diff0 = v0 - end0;
                Vertex diff1 = v1 - end0;

                T d0 = Dot(nor, diff0);
                T d1 = Dot(nor, diff1);

                if (d0 * d1 < C_<T>(0))
                {
                    // Edge <V0,V1> transversely crosses line. Compute point
                    // of intersection I = V0 + t*(V1 - V0).
                    T t = d0 / (d0 - d1);
                    if (t > mEpsilon)
                    {
                        if (t < C_<T>(1) - mEpsilon)
                        {
                            intr = v0 + t * (v1 - v0);
                            if (d1 > C_<T>(0))
                            {
                                return TRANSVERSE_POSITIVE;
                            }
                            else
                            {
                                return TRANSVERSE_NEGATIVE;
                            }
                        }
                        else
                        {
                            // T is effectively 1 (numerical round-off issue),
                            // so set d1 = 0 and go on to other cases.
                            d1 = C_<T>(0);
                        }
                    }
                    else
                    {
                        // T is effectively 0 (numerical round-off issue), so
                        // set d0 = 0 and go on to other cases.
                        d0 = C_<T>(0);
                    }
                }

                if (d0 > C_<T>(0) || d1 > C_<T>(0))
                {
                    // edge on positive side of line
                    return ALL_POSITIVE;
                }

                if (d0 < C_<T>(0) || d1 < C_<T>(0))
                {
                    // edge on negative side of line
                    return ALL_NEGATIVE;
                }

                return COINCIDENT;
            }

            void GetPosPartition(BSPPolygon2 const& polygon, Vertex const& v0,
                Vertex const& v1, BSPPolygon2& pos, BSPPolygon2& neg,
                BSPPolygon2& coSame, BSPPolygon2& coDiff) const
            {
                if (mPosChild)
                {
                    mPosChild->GetPartition(polygon, v0, v1, pos, neg, coSame, coDiff);
                }
                else
                {
                    std::size_t i0 = pos.InsertVertex(v0);
                    std::size_t i1 = pos.InsertVertex(v1);
                    pos.InsertEdge(Edge(i0, i1));
                }
            }

            void GetNegPartition(BSPPolygon2 const& polygon, Vertex const& v0,
                Vertex const& v1, BSPPolygon2& pos, BSPPolygon2& neg,
                BSPPolygon2& coSame, BSPPolygon2& coDiff) const
            {
                if (mNegChild)
                {
                    mNegChild->GetPartition(polygon, v0, v1, pos, neg, coSame, coDiff);
                }
                else
                {
                    std::size_t i0 = neg.InsertVertex(v0);
                    std::size_t i1 = neg.InsertVertex(v1);
                    neg.InsertEdge(Edge(i0, i1));
                }
            }

            class Interval
            {
            public:
                Interval(T const& inT0, T const& inT1, bool inSameDir, bool inTouching)
                    :
                    t0(inT0),
                    t1(inT1),
                    sameDir(inSameDir),
                    touching(inTouching)
                {
                }

                T t0, t1;
                bool sameDir, touching;
            };

            void GetCoPartition(BSPPolygon2 const& polygon, Vertex const& v0,
                Vertex const& v1, BSPPolygon2& pos, BSPPolygon2& neg,
                BSPPolygon2& coSame, BSPPolygon2& coDiff) const
            {
                // Segment the line containing V0 and V1 by the coincident
                // intervals that intersect <V0,V1>.
                Vertex dir = v1 - v0;
                T tmax = Dot(dir, dir);

                Vertex end0{}, end1{};
                T t0{}, t1{};
                bool sameDir{};

                std::list<Interval> intervals{};
                typename std::list<Interval>::iterator iter{};

                for (auto const& edge : mCoincident)
                {
                    end0 = polygon.GetVertex(edge[0]);
                    end1 = polygon.GetVertex(edge[1]);

                    t0 = Dot(dir, end0 - v0);
                    if (std::fabs(t0) <= mEpsilon)
                    {
                        t0 = C_<T>(0);
                    }
                    else if (std::fabs(t0 - tmax) <= mEpsilon)
                    {
                        t0 = tmax;
                    }

                    t1 = Dot(dir, end1 - v0);
                    if (std::fabs(t1) <= mEpsilon)
                    {
                        t1 = C_<T>(0);
                    }
                    else if (std::fabs(t1 - tmax) <= mEpsilon)
                    {
                        t1 = tmax;
                    }

                    sameDir = (t1 > t0);
                    if (!sameDir)
                    {
                        std::swap(t0, t1);
                    }

                    if (t1 > C_<T>(0) && t0 < tmax)
                    {
                        if (intervals.empty())
                        {
                            intervals.push_front(Interval(t0, t1, sameDir, true));
                        }
                        else
                        {
                            for (iter = intervals.begin(); iter != intervals.end(); ++iter)
                            {
                                if (std::fabs(t1 - iter->t0) <= mEpsilon)
                                {
                                    t1 = iter->t0;
                                }

                                if (t1 <= iter->t0)
                                {
                                    // [t0,t1] is on the left of [I.t0,I.t1]
                                    intervals.insert(iter, Interval(t0, t1, sameDir, true));
                                    break;
                                }

                                // Theoretically, the intervals are disjoint
                                // or intersect only at an endpoint. The
                                // assert makes sure that [t0,t1] is to the
                                // right of [I.t0,I.t1].
                                if (std::fabs(t0 - iter->t1) <= mEpsilon)
                                {
                                    t0 = iter->t1;
                                }

                                GTL_RUNTIME_ASSERT(
                                    t0 >= iter->t1,
                                    "Invalid ordering in BSPTree2::GetCoPartition.");

                                auto last = std::prev(intervals.end());
                                if (iter == last)
                                {
                                    intervals.push_back(Interval(t0, t1, sameDir, true));
                                    break;
                                }
                            }
                        }
                    }
                }

                if (intervals.empty())
                {
                    GetPosPartition(polygon, v0, v1, pos, neg, coSame, coDiff);
                    GetNegPartition(polygon, v0, v1, pos, neg, coSame, coDiff);
                    return;
                }

                // Insert outside intervals between the touching intervals.
                // It is possible that two touching intervals are adjacent,
                // so this is not just a simple alternation of touching and
                // outside intervals.
                Interval& front = intervals.front();
                if (front.t0 > C_<T>(0))
                {
                    intervals.push_front(Interval(C_<T>(0), front.t0, front.sameDir, false));
                }
                else
                {
                    front.t0 = C_<T>(0);
                }

                Interval& back = intervals.back();
                if (back.t1 < tmax)
                {
                    intervals.push_back(Interval(back.t1, tmax, back.sameDir, false));
                }
                else
                {
                    back.t1 = tmax;
                }

                typename std::list<Interval>::iterator iter0 = intervals.begin();
                typename std::list<Interval>::iterator iter1 = intervals.begin();
                for (++iter1; iter1 != intervals.end(); ++iter0, ++iter1)
                {
                    t0 = iter0->t1;
                    t1 = iter1->t0;
                    if (t1 - t0 > mEpsilon)
                    {
                        iter0 = intervals.insert(iter1, Interval(t0, t1, true, false));
                    }
                }

                // Process the segmentation.
                T invTMax = C_<T>(1) / tmax;
                t0 = intervals.front().t0 * invTMax;
                end1 = v0 + (intervals.front().t0 * invTMax) * dir;
                for (iter = intervals.begin(); iter != intervals.end(); ++iter)
                {
                    end0 = end1;
                    t1 = iter->t1 * invTMax;
                    end1 = v0 + (iter->t1 * invTMax) * dir;

                    if (iter->touching)
                    {
                        Edge edge{};
                        if (iter->sameDir)
                        {
                            edge[0] = coSame.InsertVertex(end0);
                            edge[1] = coSame.InsertVertex(end1);
                            if (edge[0] != edge[1])
                            {
                                coSame.InsertEdge(edge);
                            }
                        }
                        else
                        {
                            edge[0] = coDiff.InsertVertex(end1);
                            edge[1] = coDiff.InsertVertex(end0);
                            if (edge[0] != edge[1])
                            {
                                coDiff.InsertEdge(edge);
                            }
                        }
                    }
                    else
                    {
                        GetPosPartition(polygon, end0, end1, pos, neg, coSame, coDiff);
                        GetNegPartition(polygon, end0, end1, pos, neg, coSame, coDiff);
                    }
                }
            }

            // point-in-polygon support
            std::uint32_t Classify(Vertex const& end0, Vertex const& end1, Vertex const& vertex) const
            {
                Vertex dir = end1 - end0;
                Vertex nor = Perp(dir);
                Vertex diff = vertex - end0;
                T c = Dot(nor, diff);

                if (c > mEpsilon)
                {
                    return ALL_POSITIVE;
                }

                if (c < -mEpsilon)
                {
                    return ALL_NEGATIVE;
                }

                return COINCIDENT;
            }

            std::int32_t CoPointLocation(BSPPolygon2 const& polygon, Vertex const& vertex) const
            {
                for (auto const& edge : mCoincident)
                {
                    Vertex end0 = polygon.GetVertex(edge[0]);
                    Vertex end1 = polygon.GetVertex(edge[1]);
                    Vertex dir = end1 - end0;
                    Vertex diff = vertex - end0;
                    T tmax = Dot(dir, dir);
                    T t = Dot(dir, diff);

                    if (-mEpsilon <= t && t <= tmax + mEpsilon)
                    {
                        return 0;
                    }
                }

                // It does not matter which subtree you use.
                if (mPosChild)
                {
                    return mPosChild->PointLocation(polygon, vertex);
                }

                if (mNegChild)
                {
                    return mNegChild->PointLocation(polygon, vertex);
                }

                return 0;
            }

            T mEpsilon;
            EArray mCoincident;
            std::shared_ptr<BSPTree2> mPosChild;
            std::shared_ptr<BSPTree2> mNegChild;
        };

    private:
        void SplitEdge(std::size_t v0, std::size_t v1, std::size_t vmid)
        {
            // Find the edge in the map to get the edge-array index.
            auto iter = mEMap.find(Edge(v0, v1));
            GTL_RUNTIME_ASSERT(
                iter != mEMap.end(),
                "Edge does not exist.");

            std::size_t eIndex = iter->second;

            // Delete edge <V0,V1>.
            mEMap.erase(iter);

            // Insert edge <V0,VM>.
            mEArray[eIndex][1] = vmid;
            mEMap.insert(std::make_pair(mEArray[eIndex], eIndex));

            // Insert edge <VM,V1>.
            InsertEdge(Edge(vmid, v1));
        }

        void GetInsideEdgesFrom(BSPPolygon2 const& polygon, BSPPolygon2& inside) const
        {
            GTL_RUNTIME_ASSERT(
                mTree != nullptr,
                "Tree must exist.");

            BSPPolygon2 ignore(mEpsilon);
            for (std::size_t i = 0; i < polygon.GetNumEdges(); ++i)
            {
                std::size_t v0 = polygon.mEArray[i][0];
                std::size_t v1 = polygon.mEArray[i][1];
                Vertex vertex0 = polygon.mVArray[v0];
                Vertex vertex1 = polygon.mVArray[v1];
                mTree->GetPartition(*this, vertex0, vertex1, ignore, inside, inside, ignore);
            }
        }

        T mEpsilon;
        VMap mVMap;
        VArray mVArray;
        EMap mEMap;
        EArray mEArray;
        std::shared_ptr<BSPTree2> mTree;

    private:
        friend class UnitTestBSPPolygon2;
    };
}
