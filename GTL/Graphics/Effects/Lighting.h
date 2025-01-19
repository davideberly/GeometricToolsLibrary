// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Mathematics/Algebra/Matrix.h>

namespace gtl
{
    struct Lighting
    {
        // Construction.  The defaults are listed for each member.  The '*'
        // channels are not used but are included to match shader-constant
        // packing rules.
        Lighting();

        // (r,g,b,*): default (1,1,1,1)
        Vector4<float> ambient;

        // (r,g,b,*): default (1,1,1,1)
        Vector4<float> diffuse;

        // (r,g,b,*): default (1,1,1,1)
        Vector4<float> specular;

        // (angle,cosAngle,sinAngle,exponent): default (pi/2,0,1,1)
        Vector4<float> spotCutoff;

        // Attenuation is: intensity/(constant + linear * (d + quadratic * d)
        // where d is the distance from the light position to the vertex
        // position.  The distance is in model space.  If the transformation
        // from model space to world space involves uniform scaling, you can
        // include the scaling factor in the 'intensity' component (by
        // multiplication).
        //
        // (constant,linear,quadratic,intensity): default (1,0,0,1)
        Vector4<float> attenuation;
    };
}
