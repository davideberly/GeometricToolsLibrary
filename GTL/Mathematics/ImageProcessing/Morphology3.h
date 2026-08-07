// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.08.07

#pragma once

#include <GTL/Mathematics/ImageProcessing/Morphology.h>
#include <GTL/Mathematics/ImageProcessing/Image3.h>
#include <GTL/Utility/Exceptions.h>
#include <array>
#include <cstddef>
#include <limits>
#include <vector>

namespace gtl
{
    // The parameter SInteger must be either std::int32_t or std::int64_t.
    template <typename SInteger>
    class Morphology3 : public Morphology<SInteger>
    {
    public:
        using OffsetType = typename Morphology<SInteger>::OffsetType;

        // Compute the N-connected components of a binary image
        // (N is 6, 18, or 26). The input image is modified to avoid the cost
        // of making a copy. On output, the image values are the labels for
        // the components. The array components[k], k >= 1, contains the
        // indices for the k-th component.
        template <std::size_t N>
        static void GetComponents(
            Image3<SInteger>& image,
            std::vector<std::vector<std::size_t>>& components)
        {
            std::array<OffsetType, N> neighbors{};
            image.GetNeighborhood(neighbors);
            Morphology<SInteger>::GetComponents(neighbors.size(), neighbors.data(),
                image.size(), image.data(), components);
        }

        // Compute a dilation with a structuring element consisting of the
        // N-connected neighbors of each voxel (N is 6, 18, or 26). The input
        // image is binary with 0 for background and 1 for foreground. The
        // output image must be an object different from the input image.
        template <std::size_t N>
        static void Dilate(
            Image3<SInteger> const& input,
            Image3<SInteger>& output)
        {
            std::array<std::array<OffsetType, 3>, N> neighbors;
            input.GetNeighborhood(neighbors);
            Dilate(input, neighbors.size(), neighbors.data(), output);
        }

        // Compute a dilation with a structing element consisting of neighbors
        // specified by offsets relative to the voxel. The input image is
        // binary with 0 for background and 1 for foreground. The output
        // image must be an object different from the input image.
        static void Dilate(
            Image3<SInteger> const& input,
            std::size_t numNeighbors,
            std::array<OffsetType, 3> const* neighbors,
            Image3<SInteger>& output)
        {
            GTL_ARGUMENT_ASSERT(
                &output != &input && input.size() > 0 &&
                numNeighbors > 0 && neighbors != nullptr,
                "Invalid argument.");

            output = input;

            // If the voxel at (i0,i1,i2) is 1, then the voxels at
            // (k0,k1,k2) = (i0+nbr0,i1+nbr1,i2+nbr2) are set to 1 where
            // (nbr0,nbr1,nbr2) is in the neighbors array. Boundary
            // testing is used to avoid accessing out-of-range pixels.
            OffsetType const dim0 = static_cast<OffsetType>(input.size(0));
            OffsetType const dim1 = static_cast<OffsetType>(input.size(1));
            OffsetType const dim2 = static_cast<OffsetType>(input.size(2));
            for (OffsetType i2 = 0; i2 < dim2; ++i2)
            {
                for (OffsetType i1 = 0; i1 < dim1; ++i1)
                {
                    for (OffsetType i0 = 0; i0 < dim0; ++i0)
                    {
                        if (input(i0, i1, i2) == 1)
                        {
                            for (std::size_t j = 0; j < numNeighbors; ++j)
                            {
                                OffsetType k0 = i0 + neighbors[j][0];
                                OffsetType k1 = i1 + neighbors[j][1];
                                OffsetType k2 = i2 + neighbors[j][2];
                                if (0 <= k0 && k0 < dim0 &&
                                    0 <= k1 && k1 < dim1 &&
                                    0 <= k2 && k2 < dim2)
                                {
                                    output(k0, k1, k2) = 1;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Compute an erosion with a structuring element consisting of the
        // N-connected neighbors of each voxel (N is 6, 18, or 26). The input
        // image is binary with 0 for background and 1 for foreground. The
        // output/ image must be an object different from the input image. If
        // zeroExterior is true, the image exterior is assumed to be 0, so
        // 1-valued boundary voxels are set to 0; otherwise, boundary voxels
        // are set to 0 only when they have neighboring image voxels that
        // are 0.
        template <std::size_t N>
        static void Erode(
            Image3<SInteger> const& input,
            bool zeroExterior,
            Image3<SInteger>& output)
        {
            std::array<std::array<OffsetType, 3>, N> neighbors{};
            input.GetNeighborhood(neighbors);
            Erode(input, zeroExterior, neighbors.size(), neighbors.data(), output);
        }

        // Compute an erosion with a structuring element consisting of
        // neighbors specified by offsets relative to the voxel. The input
        // image is binary with 0 for background and 1 for foreground. The
        // output image must be an object different from the input image. If
        // zeroExterior is true, the image exterior is assumed to be 0, so
        // 1-valued boundary voxels are set to 0; otherwise, boundary voxels
        // are set to 0 only when they have neighboring image voxels that
        // are 0.
        static void Erode(
            Image3<SInteger> const& input,
            bool zeroExterior,
            std::size_t numNeighbors,
            std::array<OffsetType, 3> const* neighbors,
            Image3<SInteger>& output)
        {
            GTL_ARGUMENT_ASSERT(
                &output != &input && input.size() > 0 &&
                numNeighbors > 0 && neighbors != nullptr,
                "Invalid argument.");

            output = input;

            // If the pixel at (i0,i1,i2) is 1, it is changed to 0 when at
            // least one neighbor (k0,k1,k2) = (i0+nbr0,i1+nbr1,i2+nbr2) is 0,
            // where (nbr0,nbr1,nbr2) is in the neighbors array.
            OffsetType const dim0 = static_cast<OffsetType>(input.size(0));
            OffsetType const dim1 = static_cast<OffsetType>(input.size(1));
            OffsetType const dim2 = static_cast<OffsetType>(input.size(2));
            for (OffsetType i2 = 0; i2 < dim2; ++i2)
            {
                for (OffsetType i1 = 0; i1 < dim1; ++i1)
                {
                    for (OffsetType i0 = 0; i0 < dim0; ++i0)
                    {
                        if (input(i0, i1, i2) == 1)
                        {
                            for (std::size_t j = 0; j < numNeighbors; ++j)
                            {
                                OffsetType k0 = i0 + neighbors[j][0];
                                OffsetType k1 = i1 + neighbors[j][1];
                                OffsetType k2 = i1 + neighbors[j][2];
                                if (0 <= k0 && k0 < dim0 &&
                                    0 <= k1 && k1 < dim1 &&
                                    0 <= k2 && k2 < dim2)
                                {
                                    if (input(k0, k1, k2) == 0)
                                    {
                                        output(i0, i1, i2) = 0;
                                        break;
                                    }
                                }
                                else if (zeroExterior)
                                {
                                    output(i0, i1, i2) = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Compute an opening with a structuring element consisting of the
        // N-connected neighbors of each pixel (N is 6, 18, or 26). The input image
        // is binary with 0 for background and 1 for foreground. The output
        // image must be an object different from the input image. If
        // zeroExterior is true, the image exterior is assumed to consist of
        // 0-valued pixels; otherwise, the image exterior is assumed to
        // consist of 1-valued pixels.
        template <std::size_t N>
        static void Open(
            Image3<SInteger> const& input,
            bool zeroExterior,
            Image3<SInteger>& output)
        {
            Image3<SInteger> temp(input.size(0), input.size(1), input.size(2));
            Erode<N>(input, zeroExterior, temp);
            Dilate<N>(temp, output);
        }

        // Compute an opening with a structuring element consisting of
        // neighbors specified by offsets relative to the voxel. The input
        // image is binary with 0 for background and 1 for foreground. The
        // output image must be an object different from the input image. If
        // zeroExterior is true, the image exterior is assumed to consist of
        // 0-valued voxels; otherwise, the image exterior is assumed to
        // consist of 1-valued voxels.
        static void Open(
            Image3<SInteger> const& input,
            bool zeroExterior,
            std::size_t numNeighbors,
            std::array<OffsetType, 3> const* neighbors,
            Image3<SInteger>& output)
        {
            Image3<SInteger> temp(input.size(0), input.size(1), input.size(2));
            Erode(input, zeroExterior, numNeighbors, neighbors, temp);
            Dilate(temp, numNeighbors, neighbors, output);
        }

        // Compute a closing with a structuring element consisting of the
        // N-connected neighbors of each voxel. The input image is binary
        // with 0 for background and 1 for foreground. The output image must
        // be an object different from the input image. If zeroExterior is
        // true, the image exterior is assumed to consist of 0-valued voxels;
        // otherwise, the image exterior is assumed to consist of 1-valued
        // voxels.
        template <std::size_t N>
        static void Close(Image3<SInteger> const& input, bool zeroExterior,
            Image3<SInteger>& output)
        {
            Image3<SInteger> temp(input.size(0), input.size(1), input.size(2));
            Dilate<N>(input, temp);
            Erode<N>(temp, zeroExterior, output);
        }

        // Compute a closing with a structuring element consisting of
        // neighbors specified by offsets relative to the voxel. The input
        // image is binary with 0 for background and 1 for foreground. The
        // output image must be an object different from the input image. If
        // zeroExterior is true, the image exterior is assumed to consist of
        // 0-valued voxels; otherwise, the image exterior is assumed to
        // consist of 1-valued voxels.
        static void Close(
            Image3<SInteger> const& input,
            bool zeroExterior,
            std::size_t numNeighbors, 
            std::array<OffsetType, 3> const* neighbors,
            Image3<SInteger>& output)
        {
            Image3<SInteger> temp(input.size(0), input.size(1), input.size(2));
            Dilate(input, numNeighbors, neighbors, temp);
            Erode(temp, zeroExterior, numNeighbors, neighbors, output);
        }

        // Use a depth-first search for filling a 6-connected background
        // region of a binary image with background 0 and foreground 1. This
        // is nonrecursive, simulated by using a heap-allocated stack. The
        // input (x,y,z) is the seed point that starts the fill. On output the
        // background is 0, foreground is 1 and the filled region is 2.
        static void FloodFill6(
            Image3<SInteger>& image,
            std::size_t sx,
            std::size_t sy,
            std::size_t sz)
        {
            // Test for a valid seed.
            SInteger const xSize = static_cast<SInteger>(image.size(0));
            SInteger const ySize = static_cast<SInteger>(image.size(1));
            SInteger const zSize = static_cast<SInteger>(image.size(2));
            SInteger const x = static_cast<SInteger>(sx);
            SInteger const y = static_cast<SInteger>(sy);
            SInteger const z = static_cast<SInteger>(sz);
            if (x >= xSize || y >= ySize || z >= zSize)
            {
                // The seed point is outside the image domain, so there is
                // nothing to fill.
                return;
            }

            // Allocate the maximum amount of space needed for the stack.
            // An empty stack has top = std::numeric_limits<std::size_t>::max().
            std::vector<std::array<SInteger, 3>> stack(image.size());

            // Push seed point onto stack if it is background. All points
            // pushed onto stack are background.
            std::size_t top = 0;
            std::array<SInteger, 3> point{ x, y, z }, neighbor{ 0, 0, 0 };
            stack[top] = point;

            while (top != std::numeric_limits<std::size_t>::max())
            {
                // Read top of stack. Do not pop, because we need to return
                // to this top value later to restart the fill in a different
                // direction.
                point = stack[top];

                // Fill the pixel.
                image(x, y, z) = 2;

                neighbor = { point[0] + 1, point[1], point[2] };
                if (neighbor[0] < xSize &&
                    image(neighbor[0], neighbor[1], neighbor[2]) == 0)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0] - 1, point[1], point[2] };
                if (0 <= neighbor[0] &&
                    image(neighbor[0], neighbor[1], neighbor[2]) == 0)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0], point[1] + 1, point[2] };
                if (neighbor[1] < ySize &&
                    image(neighbor[0], neighbor[1], neighbor[2]) == 0)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0], point[1] - 1, point[2] };
                if (0 <= neighbor[1] &&
                    image(neighbor[0], neighbor[1], neighbor[2]) == 0)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0], point[1], point[2] + 1 };
                if (neighbor[2] < zSize &&
                    image(neighbor[0], neighbor[1], neighbor[2]) == 0)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0], point[1], point[2] - 1 };
                if (0 <= neighbor[2] &&
                    image(neighbor[0], neighbor[1], neighbor[2]) == 0)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                // Done in all directions, pop and return to search other
                // directions for the predecessor.
                --top;
            }
        }

        // Compute the L1-distance transform of the binary image, where the
        // foreground is 1 and the background is 0. The function returns the
        // maximum distance and a point at which the maximum distance is
        // attained.
        static void GetL1Distance(
            Image3<SInteger>& image,
            std::size_t& maxDistance,
            std::size_t& xMax,
            std::size_t& yMax,
            std::size_t& zMax)
        {
            std::size_t const xSize = image.size(0);
            std::size_t const ySize = image.size(1);
            std::size_t const zSize = image.size(2);

            // Use a grass-fire approach, computing distance from boundary to
            // interior one pass at a time.
            bool changeMade = true;
            SInteger distance{};
            for (distance = 1, xMax = 0, yMax = 0, zMax = 0; changeMade; ++distance)
            {
                changeMade = false;
                SInteger distanceP1 = distance + 1;
                for (std::size_t zm1 = 0, z = 1, zp1 = 2; zp1 < zSize; zm1 = z, z = zp1++)
                {
                    for (std::size_t ym1 = 0, y = 1, yp1 = 2; yp1 < ySize; ym1 = y, y = yp1++)
                    {
                        for (std::size_t xm1 = 0, x = 1, xp1 = 2; xp1 < xSize; xm1 = x, x = xp1++)
                        {
                            if (image(x, y, z) == distance)
                            {
                                if (image(xm1, y, z) >= distance &&
                                    image(xp1, y, z) >= distance &&
                                    image(x, ym1, z) >= distance &&
                                    image(x, yp1, z) >= distance &&
                                    image(x, y, zm1) >= distance &&
                                    image(x, y, zp1) >= distance)
                                {
                                    image(x, y, z) = distanceP1;
                                    xMax = x;
                                    yMax = y;
                                    zMax = z;
                                    changeMade = true;
                                }
                            }
                        }
                    }
                }
            }

            --distance;
            maxDistance = static_cast<std::size_t>(distance);
        }

    private:
        friend class UnitTestMorphology3;
    };
}
