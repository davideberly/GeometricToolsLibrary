// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

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

        // Compute the 6-connected components of a binary image with
        // background 0 and foreground 1. The input image is modified to
        // avoid the cost of making a copy. On output, the image values
        // are the labels for the components. The array components[k],
        // k >= 1, contains the indices for the k-th component.
        static void GetComponents6(Image3<SInteger>& image,
            std::vector<std::vector<std::size_t>>& components)
        {
            std::array<OffsetType, 6> neighbors{};
            image.GetNeighborhood(neighbors);
            Morphology<SInteger>::GetComponents(neighbors.size(), neighbors.data(),
                image.size(), image.data(), components);
        }

        // Compute the 18-connected components of a binary image with
        // background 0 and foreground 1. The input image is modified to
        // avoid the cost of making a copy. On output, the image values
        // are the labels for the components. The array components[k],
        // k >= 1, contains the indices for the k-th component.
        static void GetComponents18(Image3<SInteger>& image,
            std::vector<std::vector<std::size_t>>& components)
        {
            std::array<OffsetType, 18> neighbors{};
            image.GetNeighborhood(neighbors);
            Morphology<SInteger>::GetComponents(neighbors.size(), neighbors.data(),
                image.size(), image.data(), components);
        }

        // Compute the 26-connected components of a binary image with
        // background 0 and foreground 1. The input image is modified to
        // avoid the cost of making a copy. On output, the image values
        // are the labels for the components. The array components[k],
        // k >= 1, contains the indices for the k-th component.
        static void GetComponents26(Image3<SInteger>& image,
            std::vector<std::vector<std::size_t>>& components)
        {
            std::array<OffsetType, 26> neighbors{};
            image.GetNeighborhood(neighbors);
            Morphology<SInteger>::GetComponents(neighbors.size(), neighbors.data(),
                image.size(), image.data(), components);
        }

        // Compute a dilation with a structuring element consisting of the
        // 6-connected neighbors of each voxel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image.
        static void Dilate6(Image3<SInteger> const& input,
            Image3<SInteger>& output)
        {
            std::array<std::array<OffsetType, 3>, 6> neighbors;
            input.GetNeighborhood(neighbors);
            Dilate(input, neighbors.size(), neighbors.data(), output);
        }

        // Compute a dilation with a structuring element consisting of the
        // 18-connected neighbors of each voxel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image.
        static void Dilate18(Image3<SInteger> const& input,
            Image3<SInteger>& output)
        {
            std::array<std::array<OffsetType, 3>, 18> neighbors;
            input.GetNeighborhood(neighbors);
            Dilate(input, neighbors.size(), neighbors.data(), output);
        }

        // Compute a dilation with a structuring element consisting of the
        // 26-connected neighbors of each voxel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image.
        static void Dilate26(Image3<SInteger> const& input,
            Image3<SInteger>& output)
        {
            std::array<std::array<OffsetType, 3>, 26> neighbors;
            input.GetNeighborhood(neighbors);
            Dilate(input, neighbors.size(), neighbors.data(), output);
        }

        // Compute a dilation with a structing element consisting of neighbors
        // specified by offsets relative to the voxel. The input image is
        // binary with background 0 and foreground 1. The output image must
        // be an object different from the input image.
        static void Dilate(Image3<SInteger> const& input, std::size_t numNeighbors,
            std::array<OffsetType, 3> const* neighbors, Image3<SInteger>& output)
        {
            GTL_ARGUMENT_ASSERT(
                &output != &input && input.size() > 0 &&
                numNeighbors > 0 && neighbors != nullptr,
                "Invalid argument.");

            output = input;

            // If the voxel at (x,y,z) is foreground, then the voxels at
            // (x+dx,y+dy,z+dz) are set to foreground where (dx,dy,dz) is in
            // the neighbors array. Boundary testing is used to avoid
            // accessing out-of-range voxels.
            OffsetType const xSize = static_cast<OffsetType>(input.size(0));
            OffsetType const ySize = static_cast<OffsetType>(input.size(1));
            OffsetType const zSize = static_cast<OffsetType>(input.size(2));
            for (OffsetType z = 0; z < zSize; ++z)
            {
                for (OffsetType y = 0; y < ySize; ++y)
                {
                    for (OffsetType x = 0; x < xSize; ++x)
                    {
                        if (input(x, y, z) == 1)
                        {
                            for (std::size_t j = 0; j < numNeighbors; ++j)
                            {
                                OffsetType xNbr = x + neighbors[j][0];
                                OffsetType yNbr = y + neighbors[j][1];
                                OffsetType zNbr = z + neighbors[j][2];
                                if (0 <= xNbr && xNbr < xSize &&
                                    0 <= yNbr && yNbr < ySize &&
                                    0 <= zNbr && zNbr < zSize)
                                {
                                    output(x, y, z) = 1;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Compute an erosion with a structuring element consisting of the
        // 6-connected neighbors of each voxel. The input image is binary with
        // background 0 and foreground 1. The output image must be an object
        // different from the input image. If zeroExterior is true, the image
        // exterior is assumed to be 0, so 1-valued boundary voxels are set
        // to 0; otherwise, boundary voxels are set to 0 only when they have
        // neighboring image voxels that are 0.
        static void Erode6(Image3<SInteger> const& input, bool zeroExterior,
            Image3<SInteger>& output)
        {
            std::array<std::array<OffsetType, 3>, 6> neighbors{};
            input.GetNeighborhood(neighbors);
            Erode(input, zeroExterior, neighbors.size(), neighbors.data(), output);
        }

        // Compute an erosion with a structuring element consisting of the
        // 18-connected neighbors of each voxel. The input image is binary with
        // background 0 and foreground 1. The output image must be an object
        // different from the input image. If zeroExterior is true, the image
        // exterior is assumed to be 0, so 1-valued boundary voxels are set
        // to 0; otherwise, boundary voxels are set to 0 only when they have
        // neighboring image voxels that are 0.
        static void Erode18(Image3<SInteger> const& input, bool zeroExterior,
            Image3<SInteger>& output)
        {
            std::array<std::array<OffsetType, 3>, 18> neighbors{};
            input.GetNeighborhood(neighbors);
            Erode(input, zeroExterior, neighbors.size(), neighbors.data(), output);
        }

        // Compute an erosion with a structuring element consisting of the
        // 26-connected neighbors of each voxel. The input image is binary with
        // background 0 and foreground 1. The output image must be an object
        // different from the input image. If zeroExterior is true, the image
        // exterior is assumed to be 0, so 1-valued boundary voxels are set
        // to 0; otherwise, boundary voxels are set to 0 only when they have
        // neighboring image voxels that are 0.
        static void Erode26(Image3<SInteger> const& input, bool zeroExterior,
            Image3<SInteger>& output)
        {
            std::array<std::array<OffsetType, 3>, 26> neighbors{};
            input.GetNeighborhood(neighbors);
            Erode(input, zeroExterior, neighbors.size(), neighbors.data(), output);
        }

        // Compute an erosion with a structuring element consisting of
        // neighbors specified by offsets relative to the pixel. The input
        // image is binary with background 0 and foreground 1. The output
        // image must be an object different from the input image. If
        // zeroExterior is true, the image exterior is assumed to be 0, so
        // 1-valued boundary pixels are set to 0; otherwise, boundary pixels
        // are set to 0 only when they have neighboring image pixels that
        // are 0.
        static void Erode(Image3<SInteger> const& input, bool zeroExterior,
            std::size_t numNeighbors, std::array<OffsetType, 3> const* neighbors,
            Image3<SInteger>& output)
        {
            GTL_ARGUMENT_ASSERT(
                &output != &input && input.size() > 0 &&
                numNeighbors > 0 && neighbors != nullptr,
                "Invalid argument.");

            output = input;

            // If the voxel at (x,y,z) is foreground, it is changed to
            // background when at least one neighbor (x+dx,y+dy,z+dz) is
            // background, where (dx,dy,dz) is in the neighbors array.
            OffsetType const xSize = static_cast<OffsetType>(input.size(0));
            OffsetType const ySize = static_cast<OffsetType>(input.size(1));
            OffsetType const zSize = static_cast<OffsetType>(input.size(2));
            for (OffsetType z = 0; z < zSize; ++z)
            {
                for (OffsetType y = 0; y < ySize; ++y)
                {
                    for (OffsetType x = 0; x < xSize; ++x)
                    {
                        if (input(x, y, z) == 1)
                        {
                            for (std::size_t j = 0; j < numNeighbors; ++j)
                            {
                                OffsetType xNbr = x + neighbors[j][0];
                                OffsetType yNbr = y + neighbors[j][1];
                                OffsetType zNbr = y + neighbors[j][2];
                                if (0 <= xNbr && xNbr < xSize &&
                                    0 <= yNbr && yNbr < ySize &&
                                    0 <= zNbr && zNbr < zSize)
                                {
                                    if (input(xNbr, yNbr, zNbr) == 0)
                                    {
                                        output(x, y, z) = 0;
                                        break;
                                    }
                                }
                                else if (zeroExterior)
                                {
                                    output(x, y, z) = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Compute an opening with a structuring element consisting of the
        // 6-connected neighbors of each voxel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image. If zeroExterior is true, the
        // image exterior is assumed to consist of 0-valued voxels; otherwise,
        // the image exterior is assumed to consist of 1-valued voxels.
        static void Open6(Image3<SInteger> const& input, bool zeroExterior,
            Image3<SInteger>& output)
        {
            Image3<SInteger> temp(input.size(0), input.size(1), input.size(2));
            Erode6(input, zeroExterior, temp);
            Dilate6(temp, output);
        }

        // Compute an opening with a structuring element consisting of the
        // 18-connected neighbors of each voxel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image. If zeroExterior is true, the
        // image exterior is assumed to consist of 0-valued voxels; otherwise,
        // the image exterior is assumed to consist of 1-valued voxels.
        static void Open18(Image3<SInteger> const& input, bool zeroExterior,
            Image3<SInteger>& output)
        {
            Image3<SInteger> temp(input.size(0), input.size(1), input.size(2));
            Erode18(input, zeroExterior, temp);
            Dilate18(temp, output);
        }

        // Compute an opening with a structuring element consisting of the
        // 26-connected neighbors of each voxel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image. If zeroExterior is true, the
        // image exterior is assumed to consist of 0-valued voxels; otherwise,
        // the image exterior is assumed to consist of 1-valued voxels.
        static void Open26(Image3<SInteger> const& input, bool zeroExterior,
            Image3<SInteger>& output)
        {
            Image3<SInteger> temp(input.size(0), input.size(1), input.size(2));
            Erode26(input, zeroExterior, temp);
            Dilate26(temp, output);
        }

        // Compute an opening with a structuring element consisting of
        // neighbors specified by offsets relative to the voxel. The input
        // image is binary with background 0 and foreground 1. The output
        // image must be an object different from the input image. If
        // zeroExterior is true, the image exterior is assumed to consist of
        // 0-valued voxels; otherwise, the image exterior is assumed to
        // consist of 1-valued voxels.
        static void Open(Image3<SInteger> const& input, bool zeroExterior,
            std::size_t numNeighbors, std::array<OffsetType, 3> const* neighbors,
            Image3<SInteger>& output)
        {
            Image3<SInteger> temp(input.size(0), input.size(1), input.size(2));
            Erode(input, zeroExterior, numNeighbors, neighbors, temp);
            Dilate(temp, numNeighbors, neighbors, output);
        }

        // Compute a closing with a structuring element consisting of the
        // 6-connected neighbors of each voxel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image. If zeroExterior is true, the
        // image exterior is assumed to consist of 0-valued voxels; otherwise,
        // the image exterior is assumed to consist of 1-valued voxels.
        static void Close6(Image3<SInteger> const& input, bool zeroExterior,
            Image3<SInteger>& output)
        {
            Image3<SInteger> temp(input.size(0), input.size(1), input.size(2));
            Dilate6(input, temp);
            Erode6(temp, zeroExterior, output);
        }

        // Compute a closing with a structuring element consisting of the
        // 18-connected neighbors of each voxel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image. If zeroExterior is true, the
        // image exterior is assumed to consist of 0-valued voxels; otherwise,
        // the image exterior is assumed to consist of 1-valued voxels.
        static void Close18(Image3<SInteger> const& input, bool zeroExterior,
            Image3<SInteger>& output)
        {
            Image3<SInteger> temp(input.size(0), input.size(1), input.size(2));
            Dilate18(input, temp);
            Erode18(temp, zeroExterior, output);
        }

        // Compute a closing with a structuring element consisting of the
        // 26-connected neighbors of each voxel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image. If zeroExterior is true, the
        // image exterior is assumed to consist of 0-valued voxels; otherwise,
        // the image exterior is assumed to consist of 1-valued voxels.
        static void Close26(Image3<SInteger> const& input, bool zeroExterior,
            Image3<SInteger>& output)
        {
            Image3<SInteger> temp(input.size(0), input.size(1), input.size(2));
            Dilate26(input, temp);
            Erode26(temp, zeroExterior, output);
        }

        // Compute a closing with a structuring element consisting of
        // neighbors specified by offsets relative to the voxel. The input
        // image is binary with background 0 and foreground 1. The output
        // image must be an object different from the input image. If
        // zeroExterior is true, the image exterior is assumed to consist of
        // 0-valued voxels; otherwise, the image exterior is assumed to
        // consist of 1-valued voxels.
        static void Close(Image3<SInteger> const& input, bool zeroExterior,
            std::size_t numNeighbors, std::array<OffsetType, 3> const* neighbors,
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
        static void FloodFill6(Image3<SInteger>& image, std::size_t sx, std::size_t sy, std::size_t sz)
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
        static void GetL1Distance(Image3<SInteger>& image, std::size_t& maxDistance,
            std::size_t& xMax, std::size_t& yMax, std::size_t& zMax)
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
