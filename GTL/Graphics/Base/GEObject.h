// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/Base/GraphicsObject.h>

namespace gtl
{
    class GEObject
    {
    public:
        // Abstract base class.
        virtual ~GEObject();
    protected:
        GEObject(GraphicsObject const* gtObject);

    public:
        // Member access.
        inline GraphicsObject* GetGraphicsObject() const
        {
            return mGTObject;
        }

        // Support for debugging.
        virtual void SetName(std::string const& name) = 0;

        inline std::string const& GetName() const
        {
            return mName;
        }

    protected:
        GraphicsObject* mGTObject;
        std::string mName;
    };
}
