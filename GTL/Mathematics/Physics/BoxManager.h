// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.12

#pragma once

#include <GTL/Mathematics/Intersection/ND/IntrAlignedBoxAlignedBox.h>
#include <GTL/Mathematics/Meshes/EdgeKey.h>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <set>
#include <vector>

namespace gtl
{
    template <typename T>
    class BoxManager
    {
    public:
        BoxManager(std::vector<AlignedBox3<T>>& boxes)
            :
            mBoxes(boxes),
            mXEndpoints{},
            mYEndpoints{},
            mZEndpoints{},
            mOverlap{},
            mXLookup{},
            mYLookup{},
            mZLookup{}
        {
            Initialize();
        }

        // No default construction, copy construction, or assignment are
        // allowed.
        BoxManager() = delete;
        BoxManager(BoxManager const&) = delete;
        BoxManager& operator=(BoxManager const&) = delete;

        // This function is called by the constructor and does the
        // sort-and-sweep to initialize the update system. However, if you
        // add or remove items from the array of boxes after the constructor
        // call, you will need to call this function once before you start the
        // multiple calls of the update function.
        void Initialize()
        {
            // Get the box endpoints.
            std::size_t intrSize = mBoxes.size();
            std::size_t endpSize = 2 * intrSize;
            mXEndpoints.resize(endpSize);
            mYEndpoints.resize(endpSize);
            mZEndpoints.resize(endpSize);
            for (std::size_t i = 0, j = 0; i < intrSize; ++i)
            {
                mXEndpoints[j].type = 0;
                mXEndpoints[j].value = mBoxes[i].min[0];
                mXEndpoints[j].index = i;
                mYEndpoints[j].type = 0;
                mYEndpoints[j].value = mBoxes[i].min[1];
                mYEndpoints[j].index = i;
                mZEndpoints[j].type = 0;
                mZEndpoints[j].value = mBoxes[i].min[2];
                mZEndpoints[j].index = i;
                ++j;

                mXEndpoints[j].type = 1;
                mXEndpoints[j].value = mBoxes[i].max[0];
                mXEndpoints[j].index = i;
                mYEndpoints[j].type = 1;
                mYEndpoints[j].value = mBoxes[i].max[1];
                mYEndpoints[j].index = i;
                mZEndpoints[j].type = 1;
                mZEndpoints[j].value = mBoxes[i].max[2];
                mZEndpoints[j].index = i;
                ++j;
            }

            // Sort the box endpoints.
            std::sort(mXEndpoints.begin(), mXEndpoints.end());
            std::sort(mYEndpoints.begin(), mYEndpoints.end());
            std::sort(mZEndpoints.begin(), mZEndpoints.end());

            // Create the interval-to-endpoint lookup tables.
            mXLookup.resize(endpSize);
            mYLookup.resize(endpSize);
            mZLookup.resize(endpSize);
            for (std::size_t j = 0; j < endpSize; ++j)
            {
                mXLookup[2 * mXEndpoints[j].index + mXEndpoints[j].type] = j;
                mYLookup[2 * mYEndpoints[j].index + mYEndpoints[j].type] = j;
                mZLookup[2 * mZEndpoints[j].index + mZEndpoints[j].type] = j;
            }

            // Active set of boxes (stored by index in array).
            std::set<std::size_t> active{};

            // Set of overlapping boxes (stored by pairs of indices in array).
            mOverlap.clear();

            // Sweep through the endpoints to determine overlapping
            // x-intervals.
            for (std::size_t i = 0; i < endpSize; ++i)
            {
                Endpoint const& endpoint = mXEndpoints[i];
                std::size_t index = endpoint.index;
                if (endpoint.type == 0)  // an interval 'begin' value
                {
                    // In the 1D problem, the current interval overlaps with
                    // all the active intervals. In 3D we also need to check
                    // for y-overlap and z-overlap.
                    for (auto activeIndex : active)
                    {
                        // Rectangles activeIndex and index overlap in the
                        // x-dimension. Test for overlap in the y-dimension
                        // and z-dimension.
                        AlignedBox3<T> const& b0 = mBoxes[activeIndex];
                        AlignedBox3<T> const& b1 = mBoxes[index];
                        if (b0.max[1] >= b1.min[1] && b0.min[1] <= b1.max[1] &&
                            b0.max[2] >= b1.min[2] && b0.min[2] <= b1.max[2])
                        {
                            if (activeIndex < index)
                            {
                                mOverlap.insert(EdgeKey<false>(activeIndex, index));
                            }
                            else
                            {
                                mOverlap.insert(EdgeKey<false>(index, activeIndex));
                            }
                        }
                    }
                    active.insert(index);
                }
                else  // an interval 'end' value
                {
                    active.erase(index);
                }
            }
        }

        // After the system is initialized, you can move the boxes using this
        // function. It is not enough to modify the input array of boxes
        // because the endpoint values stored internally by this class must
        // also change. You can also retrieve the current boxes
        // information.
        void SetBox(std::size_t i, AlignedBox3<T> const& box)
        {
            mBoxes[i] = box;
            std::size_t twoI = 2 * i;
            mXEndpoints[mXLookup[twoI]].value = box.min[0];
            mXEndpoints[mXLookup[twoI + 1]].value = box.max[0];
            mYEndpoints[mYLookup[twoI]].value = box.min[1];
            mYEndpoints[mYLookup[twoI + 1]].value = box.max[1];
            mZEndpoints[mZLookup[twoI]].value = box.min[2];
            mZEndpoints[mZLookup[twoI + 1]].value = box.max[2];
        }

        inline void GetBox(std::size_t i, AlignedBox3<T>& box) const
        {
            box = mBoxes[i];
        }

        // When you are finished moving boxes, call this function to determine
        // the overlapping boxes. An incremental update is applied to
        // determine the new set of overlapping boxes.
        void Update()
        {
            InsertionSort(mXEndpoints, mXLookup);
            InsertionSort(mYEndpoints, mYLookup);
            InsertionSort(mZEndpoints, mZLookup);
        }

        // If (i,j) is in the overlap set, then box i and box j are
        // overlapping. The indices are those for the the input array. The
        // set elements (i,j) are stored so that i < j.
        inline std::set<EdgeKey<false>> const& GetOverlap() const
        {
            return mOverlap;
        }

    private:
        class Endpoint
        {
        public:
            Endpoint()
                :
                value(C_<T>(0)),
                type(0),
                index(0)
            {
            }

            // Support for sorting of endpoints.
            bool operator<(Endpoint const& endpoint) const
            {
                if (value < endpoint.value)
                {
                    return true;
                }
                if (value > endpoint.value)
                {
                    return false;
                }
                return type < endpoint.type;
            }

            T value;        // endpoint value
            std::uint32_t type;  // '0' if interval min, '1' if interval max.
            std::size_t index;   // index of interval containing this endpoint
        };

        void InsertionSort(std::vector<Endpoint>& endpoint, std::vector<std::size_t>& lookup)
        {
            // Apply an insertion sort. Under the assumption that the boxes
            // have not changed much since the last call, the endpoints are
            // nearly sorted. The insertion sort should be very fast in this
            // case.

            TIQuery<T, AlignedBox3<T>, AlignedBox3<T>> query{};
            for (std::size_t j = 1; j < endpoint.size(); ++j)
            {
                Endpoint key = endpoint[j];
                std::int64_t i = static_cast<std::int64_t>(j - 1);
                while (i >= 0 && key < endpoint[i])
                {
                    Endpoint e0 = endpoint[i];
                    Endpoint e1 = endpoint[i + 1];

                    // Update the overlap status.
                    if (e0.type == 0)
                    {
                        if (e1.type == 1)
                        {
                            // The 'b' of interval E0.mIndex was smaller than
                            // the 'e' of interval E1.mIndex, and the
                            // intervals *might have been* overlapping. Now
                            // 'b' and 'e' are swapped, and the intervals
                            // cannot overlap. Remove the pair from the
                            // overlap set. The removal operation needs to
                            // find the pair and erase it if it exists.
                            // Finding the pair is the expensive part of the
                            // operation, so there is no real time savings in
                            // testing for existence first, then deleting if
                            // it does.
                            mOverlap.erase(EdgeKey<false>(e0.index, e1.index));
                        }
                    }
                    else
                    {
                        if (e1.type == 0)
                        {
                            // The 'b' of interval E1.index was larger than
                            // the 'e' of interval E0.index, and the intervals
                            // were not overlapping. Now 'b' and 'e' are
                            // swapped, and the intervals *might be*
                            // overlapping. Determine if they are overlapping
                            // and then insert.
                            if (query(mBoxes[e0.index], mBoxes[e1.index]).intersect)
                            {
                                mOverlap.insert(EdgeKey<false>(e0.index, e1.index));
                            }
                        }
                    }

                    // Reorder the items to maintain the sorted list.
                    endpoint[i] = e1;
                    endpoint[i + 1] = e0;
                    lookup[2 * e1.index + e1.type] = i;
                    lookup[2 * e0.index + e0.type] = i + 1;
                    --i;
                }
                endpoint[i + 1] = key;
                lookup[2 * key.index + key.type] = i + 1;
            }
        }

        std::vector<AlignedBox3<T>>& mBoxes;
        std::vector<Endpoint> mXEndpoints, mYEndpoints, mZEndpoints;
        std::set<EdgeKey<false>> mOverlap;

        // The intervals are indexed 0 <= i < n. The endpoint array has 2*n
        // entries. The original 2*n interval values are ordered as
        //   b[0], e[0], b[1], e[1], ..., b[n-1], e[n-1]
        // When the endpoint array is sorted, the mapping between interval
        // values and endpoints is lost. In order to modify interval values
        // that are stored in the endpoint array, we need to maintain the
        // mapping. This is done by the following lookup table of 2*n
        // entries. The value mLookup[2*i] is the index of b[i] in the
        // endpoint array. The value mLookup[2*i+1] is the index of e[i]
        // in the endpoint array.
        std::vector<std::size_t> mXLookup, mYLookup, mZLookup;

    private:
        friend class UnitTestBoxManager;
    };
}
