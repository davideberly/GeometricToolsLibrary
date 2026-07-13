// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// Morphological operations on 2D images.

#include <GTL/Mathematics/ImageProcessing/Morphology.h>
#include <GTL/Mathematics/ImageProcessing/Image2.h>
#include <GTL/Utility/Exceptions.h>
#include <array>
#include <cstddef>
#include <limits>
#include <vector>

namespace gtl
{
    // The parameter SInteger must be either std::int32_t or std::int64_t.
    template <typename SInteger>
    class Morphology2 : public Morphology<SInteger>
    {
    public:
        using OffsetType = typename Morphology<SInteger>::OffsetType;

        // Compute the 4-connected components of a binary image with
        // background 0 and foreground 1. The input image is modified to
        // avoid the cost of making a copy. On output, the image values
        // are the labels for the components. The array components[k],
        // k >= 1, contains the indices for the k-th component.
        static void GetComponents4(Image2<SInteger>& image,
            std::vector<std::vector<std::size_t>>& components)
        {
            std::array<OffsetType, 4> neighbors{};
            image.GetNeighborhood(neighbors);
            Morphology<SInteger>::GetComponents(neighbors.size(), neighbors.data(),
                image.size(), image.data(), components);
        }

        // Compute the 8-connected components of a binary image with
        // background 0 and foreground 1. The input image is modified to
        // avoid the cost of making a copy. On output, the image values
        // are the labels for the components. The array components[k],
        // k >= 1, contains the indices for the k-th component.
        static void GetComponents8(Image2<SInteger>& image,
            std::vector<std::vector<std::size_t>>& components)
        {
            std::array<OffsetType, 8> neighbors{};
            image.GetNeighborhood(neighbors);
            Morphology<SInteger>::GetComponents(neighbors.size(), neighbors.data(),
                image.size(), image.data(), components);
        }

        // Compute a dilation with a structuring element consisting of the
        // 4-connected neighbors of each pixel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image.
        static void Dilate4(Image2<SInteger> const& input,
            Image2<SInteger>& output)
        {
            std::array<std::array<OffsetType, 2>, 4> neighbors{};
            input.GetNeighborhood(neighbors);
            Dilate(input, neighbors.size(), neighbors.data(), output);
        }

        // Compute a dilation with a structuring element consisting of the
        // 8-connected neighbors of each pixel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image.
        static void Dilate8(Image2<SInteger> const& input,
            Image2<SInteger>& output)
        {
            std::array<std::array<OffsetType, 2>, 8> neighbors{};
            input.GetNeighborhood(neighbors);
            Dilate(input, neighbors.size(), neighbors.data(), output);
        }

        // Compute a dilation with a structing element consisting of neighbors
        // specified by offsets relative to the pixel. The input image is
        // binary with background 0 and foreground 1. The output image must
        // be an object different from the input image.
        static void Dilate(Image2<SInteger> const& input, std::size_t numNeighbors,
            std::array<OffsetType, 2> const* neighbors, Image2<SInteger>& output)
        {
            GTL_ARGUMENT_ASSERT(
                &output != &input && input.size() > 0 &&
                numNeighbors > 0 && neighbors != nullptr,
                "Invalid argument.");

            output = input;

            // If the pixel at (x,y) is foreground, then the pixels at
            // (x+dx,y+dy) are set to foreground where (dx,dy) is in the
            // neighbors array. Boundary testing is used to avoid accessing
            // out-of-range pixels.
            OffsetType const xSize = static_cast<OffsetType>(input.size(0));
            OffsetType const ySize = static_cast<OffsetType>(input.size(1));
            for (OffsetType y = 0; y < ySize; ++y)
            {
                for (OffsetType x = 0; x < xSize; ++x)
                {
                    if (input(x, y) == 1)
                    {
                        for (std::size_t j = 0; j < numNeighbors; ++j)
                        {
                            OffsetType xNbr = x + neighbors[j][0];
                            OffsetType yNbr = y + neighbors[j][1];
                            if (0 <= xNbr && xNbr < xSize &&
                                0 <= yNbr && yNbr < ySize)
                            {
                                output(xNbr, yNbr) = 1;
                            }
                        }
                    }
                }
            }
        }

        // Compute an erosion with a structuring element consisting of the
        // 4-connected neighbors of each pixel. The input image is binary with
        // background 0 and foreground 1. The output image must be an object
        // different from the input image. If zeroExterior is true, the image
        // exterior is assumed to be 0, so 1-valued boundary pixels are set
        // to 0; otherwise, boundary pixels are set to 0 only when they have
        // neighboring image pixels that are 0.
        static void Erode4(Image2<SInteger> const& input, bool zeroExterior,
            Image2<SInteger>& output)
        {
            std::array<std::array<OffsetType, 2>, 4> neighbors{};
            input.GetNeighborhood(neighbors);
            Erode(input, zeroExterior, neighbors.size(), neighbors.data(), output);
        }

        // Compute an erosion with a structuring element consisting of the
        // 8-connected neighbors of each pixel. The input image is binary with
        // background 0 and foreground 1. The output image must be an object
        // different from the input image. If zeroExterior is true, the image
        // exterior is assumed to be 0, so 1-valued boundary pixels are set
        // to 0; otherwise, boundary pixels are set to 0 only when they have
        // neighboring image pixels that are 0.
        static void Erode8(Image2<SInteger> const& input, bool zeroExterior,
            Image2<SInteger>& output)
        {
            std::array<std::array<OffsetType, 2>, 8> neighbors{};
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
        static void Erode(Image2<SInteger> const& input, bool zeroExterior,
            std::size_t numNeighbors, std::array<OffsetType, 2> const* neighbors,
            Image2<SInteger>& output)
        {
            GTL_ARGUMENT_ASSERT(
                &output != &input && input.size() > 0 &&
                numNeighbors > 0 && neighbors != nullptr,
                "Invalid argument.");

            output = input;

            // If the pixel at (x,y) is foreground, it is changed to
            // background when at least one neighbor (x+dx,y+dy) is
            // background, where (dx,dy) is in the neighbors array.
            OffsetType const xSize = static_cast<OffsetType>(input.size(0));
            OffsetType const ySize = static_cast<OffsetType>(input.size(1));
            for (OffsetType y = 0; y < ySize; ++y)
            {
                for (OffsetType x = 0; x < xSize; ++x)
                {
                    if (input(x, y) == 1)
                    {
                        for (std::size_t j = 0; j < numNeighbors; ++j)
                        {
                            OffsetType xNbr = x + neighbors[j][0];
                            OffsetType yNbr = y + neighbors[j][1];
                            if (0 <= xNbr && xNbr < xSize &&
                                0 <= yNbr && yNbr < ySize)
                            {
                                if (input(xNbr, yNbr) == 0)
                                {
                                    output(x, y) = 0;
                                    break;
                                }
                            }
                            else if (zeroExterior)
                            {
                                output(x, y) = 0;
                                break;
                            }
                        }
                    }
                }
            }
        }

        // Compute an opening with a structuring element consisting of the
        // 4-connected neighbors of each pixel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image. If zeroExterior is true, the
        // image exterior is assumed to consist of 0-valued pixels; otherwise,
        // the image exterior is assumed to consist of 1-valued pixels.
        static void Open4(Image2<SInteger> const& input, bool zeroExterior,
            Image2<SInteger>& output)
        {
            Image2<SInteger> temp(input.size(0), input.size(1));
            Erode4(input, zeroExterior, temp);
            Dilate4(temp, output);
        }

        // Compute an opening with a structuring element consisting of the
        // 8-connected neighbors of each pixel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image. If zeroExterior is true, the
        // image exterior is assumed to consist of 0-valued pixels; otherwise,
        // the image exterior is assumed to consist of 1-valued pixels.
        static void Open8(Image2<SInteger> const& input, bool zeroExterior,
            Image2<SInteger>& output)
        {
            Image2<SInteger> temp(input.size(0), input.size(1));
            Erode8(input, zeroExterior, temp);
            Dilate8(temp, output);
        }

        // Compute an opening with a structuring element consisting of
        // neighbors specified by offsets relative to the pixel. The input
        // image is binary with background 0 and foreground 1. The output
        // image must be an object different from the input image. If
        // zeroExterior is true, the image exterior is assumed to consist of
        // 0-valued pixels; otherwise, the image exterior is assumed to
        // consist of 1-valued pixels.
        static void Open(Image2<SInteger> const& input, bool zeroExterior,
            std::size_t numNeighbors, std::array<OffsetType, 2> const* neighbors,
            Image2<SInteger>& output)
        {
            Image2<SInteger> temp(input.size(0), input.size(1));
            Erode(input, zeroExterior, numNeighbors, neighbors, temp);
            Dilate(temp, numNeighbors, neighbors, output);
        }

        // Compute a closing with a structuring element consisting of the
        // 4-connected neighbors of each pixel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image. If zeroExterior is true, the
        // image exterior is assumed to consist of 0-valued pixels; otherwise,
        // the image exterior is assumed to consist of 1-valued pixels.
        static void Close4(Image2<SInteger> const& input, bool zeroExterior,
            Image2<SInteger>& output)
        {
            Image2<SInteger> temp(input.size(0), input.size(1));
            Dilate4(input, temp);
            Erode4(temp, zeroExterior, output);
        }

        // Compute a closing with a structuring element consisting of the
        // 8-connected neighbors of each pixel. The input image is binary
        // with background 0 and foreground 1. The output image must be an
        // object different from the input image. If zeroExterior is true, the
        // image exterior is assumed to consist of 0-valued pixels; otherwise,
        // the image exterior is assumed to consist of 1-valued pixels.
        static void Close8(Image2<SInteger> const& input, bool zeroExterior,
            Image2<SInteger>& output)
        {
            Image2<SInteger> temp(input.size(0), input.size(1));
            Dilate8(input, temp);
            Erode8(temp, zeroExterior, output);
        }

        // Compute a closing with a structuring element consisting of
        // neighbors specified by offsets relative to the pixel. The input
        // image is binary with background 0 and foreground 1. The output
        // image must be an object different from the input image. If
        // zeroExterior is true, the image exterior is assumed to consist of
        // 0-valued pixels; otherwise, the image exterior is assumed to
        // consist of 1-valued pixels.
        static void Close(Image2<SInteger> const& input, bool zeroExterior,
            std::size_t numNeighbors, std::array<OffsetType, 2> const* neighbors,
            Image2<SInteger>& output)
        {
            Image2<SInteger> temp(input.size(0), input.size(1));
            Dilate(input, numNeighbors, neighbors, temp);
            Erode(temp, zeroExterior, numNeighbors, neighbors, output);
        }

        // Locate a pixel and walk around the edge of a component in a binary
        // image with background 0 and foreground 1. The input (x,y) is where
        // the search starts for a 1-pixel. The search visits pixels using the
        // 1-dimensional index. If the search encounters a component from the
        // outside, the walk is around the outside boundary of the component.
        // If the component has a hole and (x,y) is inside the hole, once the
        // search encounters the component from the inside, the walk is around
        // the inside boundary of the component. The function returns 'true'
        // on a successful walk. The return value is 'false' when no boundary
        // was found from the starting pixel (x,y). On output, the image has
        // background 0, foreground 1 and boundary 2.
        static bool ExtractBoundary(std::size_t x, std::size_t y, Image2<SInteger>& image,
            std::vector<std::size_t>& boundary)
        {
            // Find a first boundary pixel.
            std::size_t const numPixels = image.size();
            std::size_t i{};
            for (i = image.index(x, y); i < numPixels; ++i)
            {
                if (image[i] == 1)
                {
                    break;
                }
            }
            if (i == numPixels)
            {
                // No boundary pixel found.
                return false;
            }

            std::array<OffsetType, 8> const dx = { -1,  0, +1, +1, +1,  0, -1, -1 };
            std::array<OffsetType, 8> const dy = { -1, -1, -1,  0, +1, +1, +1,  0 };

            // Create a new point list that contains the first boundary point.
            boundary.push_back(i);

            // The direction from background 0 to boundary pixel 1 is
            // (dx[7],dy[7]).
            std::vector<std::size_t> coord = image.coordinate(i);
            std::size_t x0 = coord[0], y0 = coord[1];
            std::size_t cx = x0, cy = y0;
            std::size_t nx = x0 - 1, ny = y0, dir = 7;

            // Traverse the boundary in clockwise order. Mark visited pixels
            // with 2.
            image(cx, cy) = 2;
            for (;;)
            {
                std::size_t j{}, nbr{};
                for (j = 0, nbr = dir; j < 8; ++j, nbr = (nbr + 1) % 8)
                {
                    nx = cx + dx[nbr];
                    ny = cy + dy[nbr];
                    if (image(nx, ny) != 0)  // next boundary pixel found
                    {
                        break;
                    }
                }

                if (j == 8)
                {
                    // (cx,cy) is isolated
                    break;
                }

                if (nx == x0 && ny == y0)
                {
                    // boundary traversal completed
                    break;
                }

                // (nx,ny) is the next boundary point.
                boundary.push_back(image.index(nx, ny));

                // Mark visited boundary pixels with 2.
                image(nx, ny) = 2;

                // Start the search for the next point.
                cx = nx;
                cy = ny;
                dir = (j + 5 + dir) % 8;
            }

            return true;
        }

        // Use a depth-first search for filling a 4-connected background
        // region of a binary image with background 0 and foreground 1. This
        // is nonrecursive, simulated by using a heap-allocated stack. The
        // input (x,y) is the seed point that starts the fill. On output the
        // background is 0, foreground is 1 and the filled region is 2.
        static void FloodFill4(Image2<SInteger>& image, std::size_t sx, std::size_t sy)
        {
            // Test for a valid seed.
            SInteger const xSize = static_cast<SInteger>(image.size(0));
            SInteger const ySize = static_cast<SInteger>(image.size(1));
            SInteger const x = static_cast<SInteger>(sx);
            SInteger const y = static_cast<SInteger>(sy);
            if (x >= xSize || y >= ySize)
            {
                // The seed point is outside the image domain, so there is
                // nothing to fill.
                return;
            }

            // Allocate the maximum amount of space needed for the stack.
            // An empty stack has top = std::numeric_limits<std::size_t>::max().
            std::vector<std::array<SInteger, 2>> stack(image.size());

            // Push seed point onto stack if it is background. All points
            // pushed onto stack are background.
            std::size_t top = 0;
            std::array<SInteger, 2> point{ x, y }, neighbor{ 0, 0 };
            stack[top] = point;

            while (top != std::numeric_limits<std::size_t>::max())
            {
                // Read top of stack. Do not pop, because we need to return
                // to this top value later to restart the fill in a different
                // direction.
                point = stack[top];

                // Fill the pixel.
                image(x, y) = 2;

                neighbor = { point[0] + 1, point[1] };
                if (neighbor[0] < xSize &&
                    image(neighbor[0], neighbor[1]) == 0)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0] - 1, point[1] };
                if (0 <= neighbor[0] &&
                    image(neighbor[0], neighbor[1]) == 0)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0], point[1] + 1 };
                if (neighbor[1] < ySize &&
                    image(neighbor[0], neighbor[1]) == 0)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0], point[1] - 1 };
                if (0 <= neighbor[1] &&
                    image(neighbor[0], neighbor[1]) == 0)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                // Done in all directions, pop and return to search other
                // directions of predecessor.
                --top;
            }
        }

        // Compute the L1-distance transform of the binary image, where the
        // foreground is 1 and the background is 0. The function returns the
        // maximum distance and a point at which the maximum distance is
        // attained. Pixels outside the domain are assumed to be background.
        static void GetL1Distance(Image2<SInteger>& image, std::size_t& maxDistance,
            std::size_t& xMax, std::size_t& yMax)
        {
            std::size_t const xSize = image.size(0);
            std::size_t const ySize = image.size(1);

            // Use a grass-fire approach, computing distance from boundary to
            // interior one pass at a time.
            bool changeMade = true;
            SInteger distance;
            for (distance = 1, xMax = 0, yMax = 0; changeMade; ++distance)
            {
                changeMade = false;
                SInteger distanceP1 = distance + 1;
                for (std::size_t ym1 = 0, y = 1, yp1 = 2; yp1 < ySize; ym1 = y, y = yp1++)
                {
                    for (std::size_t xm1 = 0, x = 1, xp1 = 2; xp1 < xSize; xm1 = x, x = xp1++)
                    {
                        if (image(x, y) == distance)
                        {
                            if (image(xm1, y) >= distance &&
                                image(xp1, y) >= distance &&
                                image(x, ym1) >= distance &&
                                image(x, yp1) >= distance)
                            {
                                image(x, y) = distanceP1;
                                xMax = x;
                                yMax = y;
                                changeMade = true;
                            }
                        }
                    }
                }
            }

            --distance;
            maxDistance = static_cast<std::size_t>(distance);
        }

    private:
        friend class UnitTestMorphology2;
    };
}
