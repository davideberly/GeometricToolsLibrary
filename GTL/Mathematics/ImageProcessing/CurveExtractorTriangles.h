// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// The level set extraction algorithm implemented here is described
// in Section 2 of the document
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
    class CurveExtractorTriangles : public CurveExtractor<IntType, T>
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
        CurveExtractorTriangles(std::size_t xBound, std::size_t yBound, IntType const* inputPixels)
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
                std::size_t yParity = (y & 1);

                for (std::size_t x = 0, xp = 1; xp < this->mXBound; ++x, ++xp)
                {
                    std::size_t xParity = (x & 1);

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
                    // the square. The x, xp, y and yp values are implicitly
                    // converted from std::int32_t to std::int64_t (which is guaranteed to
                    // be correct).
                    if (xParity == yParity)
                    {
                        ProcessTriangle(vertices, edges, x, y, f00, x, yp, f01, xp, y, f10);
                        ProcessTriangle(vertices, edges, xp, yp, f11, xp, y, f10, x, yp, f01);
                    }
                    else
                    {
                        ProcessTriangle(vertices, edges, x, yp, f01, xp, yp, f11, x, y, f00);
                        ProcessTriangle(vertices, edges, xp, y, f10, x, y, f00, xp, yp, f11);
                    }
                }
            }
        }

    protected:
        void ProcessTriangle(std::vector<Vertex>& vertices, std::vector<Edge>& edges,
            std::int64_t x0, std::int64_t y0, std::int64_t f0,
            std::int64_t x1, std::int64_t y1, std::int64_t f1,
            std::int64_t x2, std::int64_t y2, std::int64_t f2)
        {
            std::int64_t xn0{}, yn0{}, xn1{}, yn1{}, d0{}, d1{};

            if (f0 != 0)
            {
                // convert to case "+**"
                if (f0 < 0)
                {
                    f0 = -f0;
                    f1 = -f1;
                    f2 = -f2;
                }

                if (f1 > 0)
                {
                    if (f2 > 0)
                    {
                        // +++
                        return;
                    }
                    else if (f2 < 0)
                    {
                        // ++-
                        d0 = f0 - f2;
                        xn0 = f0 * x2 - f2 * x0;
                        yn0 = f0 * y2 - f2 * y0;
                        d1 = f1 - f2;
                        xn1 = f1 * x2 - f2 * x1;
                        yn1 = f1 * y2 - f2 * y1;
                        this->AddEdge(vertices, edges, xn0, d0, yn0, d0, xn1, d1, yn1, d1);
                    }
                    else
                    {
                        // ++0
                        this->AddVertex(vertices, x2, 1, y2, 1);
                    }
                }
                else if (f1 < 0)
                {
                    d0 = f0 - f1;
                    xn0 = f0 * x1 - f1 * x0;
                    yn0 = f0 * y1 - f1 * y0;

                    if (f2 > 0)
                    {
                        // +-+
                        d1 = f2 - f1;
                        xn1 = f2 * x1 - f1 * x2;
                        yn1 = f2 * y1 - f1 * y2;
                        this->AddEdge(vertices, edges, xn0, d0, yn0, d0, xn1, d1, yn1, d1);
                    }
                    else if (f2 < 0)
                    {
                        // +--
                        d1 = f2 - f0;
                        xn1 = f2 * x0 - f0 * x2;
                        yn1 = f2 * y0 - f0 * y2;
                        this->AddEdge(vertices, edges, xn0, d0, yn0, d0, xn1, d1, yn1, d1);
                    }
                    else
                    {
                        // +-0
                        this->AddEdge(vertices, edges, x2, 1, y2, 1, xn0, d0, yn0, d0);
                    }
                }
                else
                {
                    if (f2 > 0)
                    {
                        // +0+
                        this->AddVertex(vertices, x1, 1, y1, 1);
                    }
                    else if (f2 < 0)
                    {
                        // +0-
                        d0 = f2 - f0;
                        xn0 = f2 * x0 - f0 * x2;
                        yn0 = f2 * y0 - f0 * y2;
                        this->AddEdge(vertices, edges, x1, 1, y1, 1, xn0, d0, yn0, d0);
                    }
                    else
                    {
                        // +00
                        this->AddEdge(vertices, edges, x1, 1, y1, 1, x2, 1, y2, 1);
                    }
                }
            }
            else if (f1 != 0)
            {
                // convert to case 0+*
                if (f1 < 0)
                {
                    f1 = -f1;
                    f2 = -f2;
                }

                if (f2 > 0)
                {
                    // 0++
                    this->AddVertex(vertices, x0, 1, y0, 1);
                }
                else if (f2 < 0)
                {
                    // 0+-
                    d0 = f1 - f2;
                    xn0 = f1 * x2 - f2 * x1;
                    yn0 = f1 * y2 - f2 * y1;
                    this->AddEdge(vertices, edges, x0, 1, y0, 1, xn0, d0, yn0, d0);
                }
                else
                {
                    // 0+0
                    this->AddEdge(vertices, edges, x0, 1, y0, 1, x2, 1, y2, 1);
                }
            }
            else if (f2 != 0)
            {
                // cases 00+ or 00-
                this->AddEdge(vertices, edges, x0, 1, y0, 1, x1, 1, y1, 1);
            }
            else
            {
                // case 000
                this->AddEdge(vertices, edges, x0, 1, y0, 1, x1, 1, y1, 1);
                this->AddEdge(vertices, edges, x1, 1, y1, 1, x2, 1, y2, 1);
                this->AddEdge(vertices, edges, x2, 1, y2, 1, x0, 1, y0, 1);
            }
        }

    private:
        friend class UnitTestCurveExtractorTriangles;
    };
}
