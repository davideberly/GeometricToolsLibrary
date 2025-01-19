// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45TextureDS.h>
using namespace gtl;

GL45TextureDS::GL45TextureDS(TextureDS const* texture)
    :
    GL45Texture2(texture)
{
}

std::shared_ptr<GEObject> GL45TextureDS::Create(void*, GraphicsObject const* object)
{
    if (object->GetType() == GT_TEXTURE_DS)
    {
        return std::make_shared<GL45TextureDS>(
            static_cast<TextureDS const*>(object));
    }

    GTL_RUNTIME_ERROR("Invalid object type.");
}

bool GL45TextureDS::CanAutoGenerateMipmaps() const
{
    return false;
}
