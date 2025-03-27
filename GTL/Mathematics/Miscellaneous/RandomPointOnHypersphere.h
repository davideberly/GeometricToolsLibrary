// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Generate random points on a unit hyperphere. The typical usage is
// listed next.
//
//   RandomPointOnHypersphere<double, 3> rpoh{};
//   std::size_t const numPoints = 4096;
//   std::vector<std::array<double, 3>> points(numPoints);
//   for (auto& point : points)
//   {
//       rpoh.Generate(point);
//   }
//
//   std::vector<std::size_t> histogram (numPoints);
//   double const angle = 0.5;
//   rpoh.Histogram(points, angle, histogram);
//   std::ofstream output("histogram.txt");
//   for (std::size_t i = 0; i < histogram.size; ++i)
//   {
//       output << i << ' ' << histogram[i] << endl;
//   }
//   output.close();

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <array>
#include <cstddef>
#include <random>
#include <type_traits>
#include <vector>

namespace gtl
{
    template <typename T, std::size_t Dimension>
    class RandomPointOnHypersphere
    {
    public:
        RandomPointOnHypersphere()
            :
            mDistribution(-C_<T>(1), C_<T>(1))
        {
            static_assert(
                std::is_floating_point<T>::value,
                "Only float or double are supported.");

            static_assert(
                Dimension >= 2,
                "Invalid dimension");
        }

        // Generate random points on the hypersphere in D-dimensional space,
        //   x[0]^2 + ... + x[D-1]^2 = 1
        // The function selects a random angle in [0,2*pi) and partitions the
        // equation into
        //   x[0]^2 + ... + x[D/2-1]^2 = (cos(A))^2
        // and
        //   x[D/2]^2 + ... + x[D-1]^2 = (sin(A))^2
        // The function initializes all components x[i] to 1.  The partitioned
        // components are updated as x[i] *= cos(A) for 0 <= i < D/2 and
        // x[i] *= sin(A) for D/2 <= i < D. The function is recursively called
        // on the partitioned components.
        void Generate(std::array<T, Dimension>& x)
        {
            x.fill(C_<T>(1));
            GenerateRecursive(x.data(), Dimension);
        }

        // Determine the uniformity of randomly generated points P[] on the
        // hypersphere. Select a positive angle. For each point P[i], count
        // the number H[i] of random points P[j] that lie in the cone with
        // axis P[i] and specified angle. For a suitably large number of
        // points, H[i] should be approximately constant for all i.
        void Histogram(std::vector<std::array<T, Dimension>> const& P,
            T angle, std::vector<std::size_t>& histogram)
        {
            // Count the number of points located in the cone of specified
            // angle about each of the samples.
            T cs = std::cos(angle);
            histogram.resize(P.size());

            for (std::size_t i = 0; i < P.size(); ++i)
            {
                histogram[i] = 0;
                for (std::size_t j = 0; j < P.size(); ++j)
                {
                    // Compute dot product between points P[i] and P[j].
                    T dot = C_<T>(0);
                    for (std::size_t k = 0; k < Dimension; ++k)
                    {
                        dot += P[i][k] * P[j][k];
                    }
                    if (dot >= cs)
                    {
                        // P[j] is inside the cone with axis P[i] and
                        // specified angle.
                        ++histogram[i];
                    }
                }
            }
        }

    private:
        void GenerateRecursive(T* x, std::size_t dimension)
        {
            // Select a random point on a circle.
            T angle = mDistribution(mEngine) * C_PI<T>;
            T cs = std::cos(angle);
            T sn = std::sin(angle);

            if (dimension > 3)
            {
                // Split components into two sets and adjust values.
                std::size_t i, halfDimension = dimension / 2;
                for (i = 0; i < halfDimension; ++i)
                {
                    x[i] *= cs;
                }
                for (i = halfDimension; i < dimension; ++i)
                {
                    x[i] *= sn;
                }

                // Recurse on each half of the components.
                GenerateRecursive(x, halfDimension);
                GenerateRecursive(x + halfDimension, dimension - halfDimension);
            }
            else if (dimension == 3)
            {
                T r = mDistribution(mEngine);
                T sqrtOmRSqr = std::sqrt(std::fabs(C_<T>(1) - r * r));
                x[0] *= r;
                x[1] *= sqrtOmRSqr * cs;
                x[2] *= sqrtOmRSqr * sn;
            }
            else // dimension == 2
            {
                x[0] *= cs;
                x[1] *= sn;
            }
        }

        std::default_random_engine mEngine;
        std::uniform_real_distribution<T> mDistribution;

    private:
        friend class UnitTestRandomPointOnHypersphere;
    };
}
