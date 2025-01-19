// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Base/GEObject.h>
using namespace gtl;

GEObject::~GEObject()
{
}

GEObject::GEObject(GraphicsObject const* gtObject)
    :
    mGTObject(const_cast<GraphicsObject*>(gtObject)),  // conceptual constness
    mName("")
{
}
