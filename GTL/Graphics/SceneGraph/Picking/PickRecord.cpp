// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/SceneGraph/Picking/PickRecord.h>
using namespace gtl;

PickRecord::PickRecord()
    :
    primitiveType(IP_NONE),
    primitiveIndex(0),
    t(0.0f),
    linePoint{ 0.0f, 0.0f, 0.0f, 1.0f },
    primitivePoint{ 0.0f, 0.0f, 0.0f, 1.0f },
    distanceToLinePoint(0.0f),
    distanceToPrimitivePoint(0.0f),
    distanceBetweenLinePrimitive(0.0f)
{
    for (int32_t i = 0; i < 3; ++i)
    {
        vertexIndex[i] = 0;
        bary[i] = 0.0f;
    }
}
