// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/Resources/Buffers/Buffer.h>
#include <GTL/Graphics/GL45/Resources/GL45Resource.h>

namespace gtl
{
    class GL45Buffer : public GL45Resource
    {
    protected:
        // Abstract base class.
        virtual ~GL45Buffer();
        GL45Buffer(Buffer const* buffer, GLenum type);

        // Must be called by constructor.
        virtual void Initialize();

    public:
        // Member access.
        inline Buffer* GetBuffer() const
        {
            return static_cast<Buffer*>(mGTObject);
        }

        inline GLenum GetType() const
        {
            return mType;
        }

        inline GLenum GetUsage() const
        {
            return mUsage;
        }

        // TODO: This is a tentative interface; modify as needed.  Make these
        // pure virtual latter (if relevant).  Do we really need these to be
        // virtual?  Revisit the DX11 code and investigate why the choice was
        // made there.
        virtual bool Update();
        virtual bool CopyCpuToGpu();
        virtual bool CopyGpuToCpu();

    protected:
        GLenum mType;
        GLenum mUsage;
    };
}
