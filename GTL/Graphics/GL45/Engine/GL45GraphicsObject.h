// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#pragma once

#include <GTL/Utility/Exceptions.h>
#include <GTL/Graphics/Base/GEObject.h>
#include <GTL/Graphics/GL45/Engine/GL45.h>

namespace gtl
{
    class GL45GraphicsObject : public GEObject
    {
    public:
        // Abstract base class.
        virtual ~GL45GraphicsObject() = default;
    protected:
        GL45GraphicsObject(GraphicsObject const* gtObject);

    public:
        // Member access.
        inline GLuint GetGLHandle() const
        {
            return mGLHandle;
        }

        // Support for debugging.
        virtual void SetName(std::string const& name) override;

    protected:
        GLuint mGLHandle;
    };
}
