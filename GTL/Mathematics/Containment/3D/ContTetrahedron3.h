// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

#include <GTL/Mathematics/Primitives/3D/Tetrahedron3.h>

namespace gtl
{
    // Test for containment of a point by a tetrahedron.
    template <typename T>
    bool InContainer(Vector3<T> const& point, Tetrahedron3<T> const& tetra)
    {
        // A loop over the faces is not used in order to avoid redundant
        // computations of edge directions. The difference vector is the same
        // for the first 3 faces but differs for the last face.
        Vector3<T> edge10{}, edge20{}, edge30{}, edge21{}, edge31{};
        Vector3<T> diffP0{}, diffP1{};
        T tsp{};

        // face <0,2,1>
        edge20 = tetra.v[2] - tetra.v[0];
        edge10 = tetra.v[1] - tetra.v[0];
        diffP0 = point - tetra.v[0];
        tsp = DotCross(edge20, edge10, diffP0);
        if (tsp > C_<T>(0))
        {
            return false;
        }

        // face <0,1,3>
        edge30 = tetra.v[3] - tetra.v[0];
        tsp = DotCross(edge10, edge30, diffP0);
        if (tsp > C_<T>(0))
        {
            return false;
        }

        // face <0,3,2>
        tsp = DotCross(edge30, edge20, diffP0);
        if (tsp > C_<T>(0))
        {
            return false;
        }

        // face<1,2,3>
        edge21 = tetra.v[2] - tetra.v[1];
        edge31 = tetra.v[3] - tetra.v[1];
        diffP1 = point - tetra.v[1];
        tsp = DotCross(edge21, edge31, diffP1);
        if (tsp > C_<T>(0))
        {
            return false;
        }

        return true;
    }
}
