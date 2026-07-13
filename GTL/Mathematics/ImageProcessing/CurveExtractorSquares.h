// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// The level set extraction algorithm implemented here is described
// in Section 3 of the document
// https://www.geometrictools.com/Documentation/ExtractLevelCurves.pdf

#include <GTL/Mathematics/ImageProcessing/CurveExtractor.h>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace gtl
{
    // The image type IntType must be one of the integer types: std::int8_t,
    // std::int16_t, std::int32_t, std::uint8_t, std::uint16_t or
    // std::uint32_t. Internal integer computations are performed using
    // std::int64_t. The type T is for extraction to floating-point vertices.
    template <typename IntType, typename T>
    class CurveExtractorSquares : public CurveExtractor<IntType, T>
    {
    public:
        // Convenient type definitions.
        using Vertex = typename CurveExtractor<IntType, T>::Vertex;
        using Edge = typename CurveExtractor<IntType, T>::Edge;

        // The input is a 2D image with lexicographically ordered pixels (x,y)
        // stored in a linear array. Pixel (x,y) is stored in the array at
        // location index = x + xBound * y. The inputs xBound and yBound must
        // each be 2 or larger so that there is at least one image square to
        // process. The inputPixels must be nonnull and point to contiguous
        // storage that contains at least xBound * yBound elements.
        CurveExtractorSquares(std::size_t xBound, std::size_t yBound, IntType const* inputPixels)
            :
            CurveExtractor<IntType, T>(xBound, yBound, inputPixels)
        {
        }

        // Extract level curves and return rational vertices. Use the
        // base-class Extract if you want real-valued vertices.
        virtual void Extract(IntType level, std::vector<Vertex>& vertices,
            std::vector<Edge>& edges) override
        {
            // Adjust the image so that the level set is F(x,y) = 0.
            std::int64_t levelI64 = static_cast<std::int64_t>(level);
            for (std::size_t i = 0; i < this->mPixels.size(); ++i)
            {
                std::int64_t inputI64 = static_cast<std::int64_t>(this->mInputPixels[i]);
                this->mPixels[i] = inputI64 - levelI64;
            }

            vertices.clear();
            edges.clear();
            for (std::size_t y = 0, yp = 1; yp < this->mYBound; ++y, ++yp)
            {
                for (std::size_t x = 0, xp = 1; xp < this->mXBound; ++x, ++xp)
                {
                    // Get the image values at the corners of the square.
                    std::size_t i00 = x + this->mXBound * y;
                    std::size_t i10 = i00 + 1;
                    std::size_t i01 = i00 + this->mXBound;
                    std::size_t i11 = i10 + this->mXBound;
                    std::int64_t f00 = this->mPixels[i00];
                    std::int64_t f10 = this->mPixels[i10];
                    std::int64_t f01 = this->mPixels[i01];
                    std::int64_t f11 = this->mPixels[i11];

                    // Construct the vertices and edges of the level curve in
                    // the square.
                    std::int64_t ix = static_cast<std::int64_t>(x);
                    std::int64_t ixp = static_cast<std::int64_t>(xp);
                    std::int64_t iy = static_cast<std::int64_t>(x);
                    std::int64_t iyp = static_cast<std::int64_t>(xp);
                    ProcessSquare(vertices, edges, ix, ixp, iy, iyp, f00, f10, f11, f01);
                }
            }
        }

    protected:
        void ProcessSquare(std::vector<Vertex>& vertices, std::vector<Edge>& edges,
            std::int64_t x, std::int64_t xp, std::int64_t y, std::int64_t yp,
            std::int64_t f00, std::int64_t f10, std::int64_t f11, std::int64_t f01)
        {
            std::int64_t xn0{}, yn0{}, xn1{}, yn1{}, d0{}, d1{}, d2{}, d3{}, det{};

            if (f00 != 0)
            {
                // convert to case "+***"
                if (f00 < 0)
                {
                    f00 = -f00;
                    f10 = -f10;
                    f11 = -f11;
                    f01 = -f01;
                }

                if (f10 > 0)
                {
                    if (f11 > 0)
                    {
                        if (f01 > 0)
                        {
                            // ++++
                            return;
                        }
                        else if (f01 < 0)
                        {
                            // +++-
                            d0 = f11 - f01;
                            xn0 = f11 * x - f01 * xp;
                            d1 = f00 - f01;
                            yn1 = f00 * yp - f01 * y;
                            this->AddEdge(vertices, edges, xn0, d0, yp, 1, x, 1, yn1, d1);
                        }
                        else
                        {
                            // +++0
                            this->AddVertex(vertices, x, 1, yp, 1);
                        }
                    }
                    else if (f11 < 0)
                    {
                        d0 = f10 - f11;
                        yn0 = f10 * yp - f11 * y;

                        if (f01 > 0)
                        {
                            // ++-+
                            d1 = f01 - f11;
                            xn1 = f01 * xp - f11 * x;
                            this->AddEdge(vertices, edges, xp, 1, yn0, d0, xn1, d1, yp, 1);
                        }
                        else if (f01 < 0)
                        {
                            // ++--
                            d1 = f01 - f00;
                            yn1 = f01 * y - f00 * yp;
                            this->AddEdge(vertices, edges, x, 1, yn1, d1, xp, 1, yn0, d0);
                        }
                        else
                        {
                            // ++-0
                            this->AddEdge(vertices, edges, x, 1, yp, 1, xp, 1, yn0, d0);
                        }
                    }
                    else
                    {
                        if (f01 > 0)
                        {
                            // ++0+
                            this->AddVertex(vertices, xp, 1, yp, 1);
                        }
                        else if (f01 < 0)
                        {
                            // ++0-
                            d0 = f01 - f00;
                            yn0 = f01 * y - f00 * yp;
                            this->AddEdge(vertices, edges, xp, 1, yp, 1, x, 1, yn0, d0);
                        }
                        else
                        {
                            // ++00
                            this->AddEdge(vertices, edges, xp, 1, yp, 1, x, 1, yp, 1);
                        }
                    }
                }
                else if (f10 < 0)
                {
                    d0 = f00 - f10;
                    xn0 = f00 * xp - f10 * x;

                    if (f11 > 0)
                    {
                        d1 = f11 - f10;
                        yn1 = f11 * y - f10 * yp;

                        if (f01 > 0)
                        {
                            // +-++
                            this->AddEdge(vertices, edges, xn0, d0, y, 1, xp, 1, yn1, d1);
                        }
                        else if (f01 < 0)
                        {
                            // +-+-
                            d3 = f11 - f01;
                            xn1 = f11 * x - f01 * xp;
                            d2 = f01 - f00;
                            yn0 = f01 * y - f00 * yp;

                            if (d0*d3 > 0)
                            {
                                det = xn1 * d0 - xn0 * d3;
                            }
                            else
                            {
                                det = xn0 * d3 - xn1 * d0;
                            }

                            if (det > 0)
                            {
                                this->AddEdge(vertices, edges, xn1, d3, yp, 1, xp, 1, yn1, d1);
                                this->AddEdge(vertices, edges, xn0, d0, y, 1, x, 1, yn0, d2);
                            }
                            else if (det < 0)
                            {
                                this->AddEdge(vertices, edges, xn1, d3, yp, 1, x, 1, yn0, d2);
                                this->AddEdge(vertices, edges, xn0, d0, y, 1, xp, 1, yn1, d1);
                            }
                            else
                            {
                                this->AddEdge(vertices, edges, xn0, d0, yn0, d2, xn0, d0, y, 1);
                                this->AddEdge(vertices, edges, xn0, d0, yn0, d2, xn0, d0, yp, 1);
                                this->AddEdge(vertices, edges, xn0, d0, yn0, d2, x, 1, yn0, d2);
                                this->AddEdge(vertices, edges, xn0, d0, yn0, d2, xp, 1, yn0, d2);
                            }
                        }
                        else
                        {
                            // +-+0
                            this->AddEdge(vertices, edges, xn0, d0, y, 1, xp, 1, yn1, d1);
                            this->AddVertex(vertices, x, 1, yp, 1);
                        }
                    }
                    else if (f11 < 0)
                    {
                        if (f01 > 0)
                        {
                            // +--+
                            d1 = f11 - f01;
                            xn1 = f11 * x - f01 * xp;
                            this->AddEdge(vertices, edges, xn0, d0, y, 1, xn1, d1, yp, 1);
                        }
                        else if (f01 < 0)
                        {
                            // +---
                            d1 = f01 - f00;
                            yn1 = f01 * y - f00 * yp;
                            this->AddEdge(vertices, edges, x, 1, yn1, d1, xn0, d0, y, 1);
                        }
                        else
                        {
                            // +--0
                            this->AddEdge(vertices, edges, x, 1, yp, 1, xn0, d0, y, 1);
                        }
                    }
                    else
                    {
                        if (f01 > 0)
                        {
                            // +-0+
                            this->AddEdge(vertices, edges, xp, 1, yp, 1, xn0, d0, y, 1);
                        }
                        else if (f01 < 0)
                        {
                            // +-0-
                            d1 = f01 - f00;
                            yn1 = f01 * y - f00 * yp;
                            this->AddEdge(vertices, edges, x, 1, yn1, d1, xn0, d0, y, 1);
                            this->AddVertex(vertices, xp, 1, yp, 1);
                        }
                        else
                        {
                            // +-00
                            this->AddEdge(vertices, edges, xp, 1, yp, 1, xn0, d0, yp, 1);
                            this->AddEdge(vertices, edges, xn0, d0, yp, 1, x, 1, yp, 1);
                            this->AddEdge(vertices, edges, xn0, d0, yp, 1, xn0, d0, y, 1);
                        }
                    }
                }
                else
                {
                    if (f11 > 0)
                    {
                        if (f01 > 0)
                        {
                            // +0++
                            this->AddVertex(vertices, xp, 1, y, 1);
                        }
                        else if (f01 < 0)
                        {
                            // +0+-
                            d0 = f11 - f01;
                            xn0 = f11 * x - f01 * xp;
                            d1 = f00 - f01;
                            yn1 = f00 * yp - f01 * y;
                            this->AddEdge(vertices, edges, xn0, d0, yp, 1, x, 1, yn1, d1);
                            this->AddVertex(vertices, xp, 1, y, 1);
                        }
                        else
                        {
                            // +0+0
                            this->AddVertex(vertices, xp, 1, y, 1);
                            this->AddVertex(vertices, x, 1, yp, 1);
                        }
                    }
                    else if (f11 < 0)
                    {
                        if (f01 > 0)
                        {
                            // +0-+
                            d0 = f11 - f01;
                            xn0 = f11 * x - f01 * xp;
                            this->AddEdge(vertices, edges, xp, 1, y, 1, xn0, d0, yp, 1);
                        }
                        else if (f01 < 0)
                        {
                            // +0--
                            d0 = f01 - f00;
                            yn0 = f01 * y - f00 * yp;
                            this->AddEdge(vertices, edges, xp, 1, y, 1, x, 1, yn0, d0);
                        }
                        else
                        {
                            // +0-0
                            this->AddEdge(vertices, edges, xp, 1, y, 1, x, 1, yp, 1);
                        }
                    }
                    else
                    {
                        if (f01 > 0)
                        {
                            // +00+
                            this->AddEdge(vertices, edges, xp, 1, y, 1, xp, 1, yp, 1);
                        }
                        else if (f01 < 0)
                        {
                            // +00-
                            d0 = f00 - f01;
                            yn0 = f00 * yp - f01 * y;
                            this->AddEdge(vertices, edges, xp, 1, y, 1, xp, 1, yn0, d0);
                            this->AddEdge(vertices, edges, xp, 1, yn0, d0, xp, 1, yp, 1);
                            this->AddEdge(vertices, edges, xp, 1, yn0, d0, x, 1, yn0, d0);
                        }
                        else
                        {
                            // +000
                            this->AddEdge(vertices, edges, x, 1, yp, 1, x, 1, y, 1);
                            this->AddEdge(vertices, edges, x, 1, y, 1, xp, 1, y, 1);
                        }
                    }
                }
            }
            else if (f10 != 0)
            {
                // convert to case 0+**
                if (f10 < 0)
                {
                    f10 = -f10;
                    f11 = -f11;
                    f01 = -f01;
                }

                if (f11 > 0)
                {
                    if (f01 > 0)
                    {
                        // 0+++
                        this->AddVertex(vertices, x, 1, y, 1);
                    }
                    else if (f01 < 0)
                    {
                        // 0++-
                        d0 = f11 - f01;
                        xn0 = f11 * x - f01 * xp;
                        this->AddEdge(vertices, edges, x, 1, y, 1, xn0, d0, yp, 1);
                    }
                    else
                    {
                        // 0++0
                        this->AddEdge(vertices, edges, x, 1, yp, 1, x, 1, y, 1);
                    }
                }
                else if (f11 < 0)
                {
                    if (f01 > 0)
                    {
                        // 0+-+
                        d0 = f10 - f11;
                        yn0 = f10 * yp - f11 * y;
                        d1 = f01 - f11;
                        xn1 = f01 * xp - f11 * x;
                        this->AddEdge(vertices, edges, xp, 1, yn0, d0, xn1, d1, yp, 1);
                        this->AddVertex(vertices, x, 1, y, 1);
                    }
                    else if (f01 < 0)
                    {
                        // 0+--
                        d0 = f10 - f11;
                        yn0 = f10 * yp - f11 * y;
                        this->AddEdge(vertices, edges, x, 1, y, 1, xp, 1, yn0, d0);
                    }
                    else
                    {
                        // 0+-0
                        d0 = f10 - f11;
                        yn0 = f10 * yp - f11 * y;
                        this->AddEdge(vertices, edges, x, 1, y, 1, x, 1, yn0, d0);
                        this->AddEdge(vertices, edges, x, 1, yn0, d0, x, 1, yp, 1);
                        this->AddEdge(vertices, edges, x, 1, yn0, d0, xp, 1, yn0, d0);
                    }
                }
                else
                {
                    if (f01 > 0)
                    {
                        // 0+0+
                        this->AddVertex(vertices, x, 1, y, 1);
                        this->AddVertex(vertices, xp, 1, yp, 1);
                    }
                    else if (f01 < 0)
                    {
                        // 0+0-
                        this->AddEdge(vertices, edges, x, 1, y, 1, xp, 1, yp, 1);
                    }
                    else
                    {
                        // 0+00
                        this->AddEdge(vertices, edges, xp, 1, yp, 1, x, 1, yp, 1);
                        this->AddEdge(vertices, edges, x, 1, yp, 1, x, 1, y, 1);
                    }
                }
            }
            else if (f11 != 0)
            {
                // convert to case 00+*
                if (f11 < 0)
                {
                    f11 = -f11;
                    f01 = -f01;
                }

                if (f01 > 0)
                {
                    // 00++
                    this->AddEdge(vertices, edges, x, 1, y, 1, xp, 1, y, 1);
                }
                else if (f01 < 0)
                {
                    // 00+-
                    d0 = f01 - f11;
                    xn0 = f01 * xp - f11 * x;
                    this->AddEdge(vertices, edges, x, 1, y, 1, xn0, d0, y, 1);
                    this->AddEdge(vertices, edges, xn0, d0, y, 1, xp, 1, y, 1);
                    this->AddEdge(vertices, edges, xn0, d0, y, 1, xn0, d0, yp, 1);
                }
                else
                {
                    // 00+0
                    this->AddEdge(vertices, edges, xp, 1, y, 1, xp, 1, yp, 1);
                    this->AddEdge(vertices, edges, xp, 1, yp, 1, x, 1, yp, 1);
                }
            }
            else if (f01 != 0)
            {
                // cases 000+ or 000-
                this->AddEdge(vertices, edges, x, 1, y, 1, xp, 1, y, 1);
                this->AddEdge(vertices, edges, xp, 1, y, 1, xp, 1, yp, 1);
            }
            else
            {
                // case 0000
                this->AddEdge(vertices, edges, x, 1, y, 1, xp, 1, y, 1);
                this->AddEdge(vertices, edges, xp, 1, y, 1, xp, 1, yp, 1);
                this->AddEdge(vertices, edges, xp, 1, yp, 1, x, 1, yp, 1);
                this->AddEdge(vertices, edges, x, 1, yp, 1, x, 1, y, 1);
            }
        }

    private:
        friend class UnitTestCurveExtractorSquares;
    };
}
