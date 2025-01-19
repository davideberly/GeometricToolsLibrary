// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/Resources/GL45Resource.h>
using namespace gtl;

GL45Resource::GL45Resource(Resource const* gtResource)
    :
    GL45GraphicsObject(gtResource)
{
}

void* GL45Resource::MapForWrite(GLenum target)
{
    glBindBuffer(target, mGLHandle);
    GLvoid* mapped = glMapBuffer(target, GL_WRITE_ONLY);
    glBindBuffer(target, 0);
    return mapped;
}

void GL45Resource::Unmap(GLenum target)
{
    glBindBuffer(target, mGLHandle);
    glUnmapBuffer(target);
    glBindBuffer(target, 0);
}

bool GL45Resource::PreparedForCopy(GLenum access) const
{
    // TODO: DX11 requires a staging resource when copying between CPU and
    // GPU.  Does OpenGL hide the double hop?

    // Verify existence of objects.
    GTL_ARGUMENT_ASSERT(
        mGLHandle != 0,
        "GL object does not exist.");

    // Verify the copy type.
    uint32_t copyType = GetResource()->GetCopy();
    if (copyType == Resource::Copy::CPU_TO_STAGING)  // CPU -> GPU
    {
        if (access == GL_WRITE_ONLY)
        {
            return true;
        }
    }
    else if (copyType == Resource::Copy::STAGING_TO_CPU)  // GPU -> CPU
    {
        if (access == GL_READ_ONLY)
        {
            return true;
        }
    }
    else if (copyType == Resource::Copy::BIDIRECTIONAL)
    {
        if (access == GL_READ_WRITE)
        {
            return true;
        }
        if (access == GL_WRITE_ONLY)
        {
            return true;
        }
        if (access == GL_READ_ONLY)
        {
            return true;
        }
    }

    GTL_RUNTIME_ERROR("Resource has incorrect copy type.");
}
