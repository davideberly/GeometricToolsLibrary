// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/Resources/Resource.h>
#include <GTL/Graphics/GL45/Engine/GL45GraphicsObject.h>

namespace gtl
{
    class GL45Resource : public GL45GraphicsObject
    {
    public:
        // Abstract base class.
        virtual ~GL45Resource() = default;
    protected:
        GL45Resource(Resource const* gtResource);

    public:
        // Member access.
        inline Resource* GetResource() const
        {
            return static_cast<Resource*>(mGTObject);
        }

        // TODO: This is a tentative interface; modify as needed.  Make these
        // pure virtual latter (if relevant).
        void* MapForWrite(GLenum target);
        void Unmap(GLenum target);

        virtual bool Update()
        {
            return false;
        }

        virtual bool CopyCpuToGpu()
        {
            return false;
        }

        virtual bool CopyGpuToCpu()
        {
            return false;
        }

        virtual void CopyGpuToGpu(GL45Resource* target)
        {
            (void)target;
            throw std::logic_error(std::string(__FILE__) + "(" + std::string(__FUNCTION__) + "," + std::to_string(__LINE__) + "): Not yet implemented.\n");
        }

    protected:
        // Support for copying between CPU and GPU.
        bool PreparedForCopy(GLenum access) const;
    };
}
