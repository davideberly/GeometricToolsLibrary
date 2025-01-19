// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45TextureRT.h>
using namespace gtl;

GL45TextureRT::GL45TextureRT(TextureRT const* texture)
    :
    GL45Texture2(texture)
{
}

std::shared_ptr<GEObject> GL45TextureRT::Create(void*, GraphicsObject const* object)
{
    if (object->GetType() == GT_TEXTURE_RT)
    {
        return std::make_shared<GL45TextureRT>(
            static_cast<TextureRT const*>(object));
    }

    GTL_RUNTIME_ERROR("Invalid object type.");
}

bool GL45TextureRT::CanAutoGenerateMipmaps() const
{
    auto texture = GetTexture();

    return texture && texture->HasMipmaps() && texture->WantAutogenerateMipmaps();
}
