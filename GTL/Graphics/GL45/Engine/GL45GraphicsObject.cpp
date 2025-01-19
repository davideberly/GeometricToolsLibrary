// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/Engine/GL45GraphicsObject.h>
using namespace gtl;

GL45GraphicsObject::GL45GraphicsObject(GraphicsObject const* gtObject)
    :
    GEObject(gtObject),
    mGLHandle(0)
{
}

void GL45GraphicsObject::SetName(std::string const& name)
{
    // TODO:  Determine how to tag OpenGL objects with names?
    mName = name;
}
