// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Mathematics/Algebra/Vector.h>

namespace gtl
{
    class Material
    {
    public:
        // Construction.
        Material();

        // (r,g,b,*): default (0,0,0,1)
        Vector4<float> emissive;

        // (r,g,b,*): default (0,0,0,1)
        Vector4<float> ambient;

        // (r,g,b,a): default (0,0,0,1)
        Vector4<float> diffuse;

        // (r,g,b,specularPower): default (0,0,0,1)
        Vector4<float> specular;
    };
}
