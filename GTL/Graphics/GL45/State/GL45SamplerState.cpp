// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/State/GL45SamplerState.h>
using namespace gtl;

GL45SamplerState::~GL45SamplerState()
{
    glDeleteSamplers(1, &mGLHandle);
}

GL45SamplerState::GL45SamplerState(SamplerState const* samplerState)
    :
    GL45DrawingState(samplerState)
{
    glGenSamplers(1, &mGLHandle);

    glSamplerParameteri(mGLHandle, GL_TEXTURE_WRAP_S, msMode[samplerState->mode[0]]);
    glSamplerParameteri(mGLHandle, GL_TEXTURE_WRAP_T, msMode[samplerState->mode[1]]);
    glSamplerParameteri(mGLHandle, GL_TEXTURE_WRAP_R, msMode[samplerState->mode[2]]);

    // TODO: GL_TEXTURE_MAX_ANISOTROPY_EXT is not defined?
    // glSamplerParameterf(samplerState, GL_TEXTURE_MAX_ANISOTROPY_EXT,
    //   samplerState->maxAnisotropy);

    glSamplerParameterf(mGLHandle, GL_TEXTURE_MIN_LOD, samplerState->minLOD);
    glSamplerParameterf(mGLHandle, GL_TEXTURE_MAX_LOD, samplerState->maxLOD);
    glSamplerParameterf(mGLHandle, GL_TEXTURE_LOD_BIAS, samplerState->mipLODBias);

    glSamplerParameterfv(mGLHandle, GL_TEXTURE_BORDER_COLOR, &samplerState->borderColor[0]);

    switch(samplerState->filter)
    {
    case SamplerState::Filter::MIN_P_MAG_P_MIP_P:
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case SamplerState::Filter::MIN_P_MAG_P_MIP_L:
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case SamplerState::Filter::MIN_P_MAG_L_MIP_P:
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case SamplerState::Filter::MIN_P_MAG_L_MIP_L:
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case SamplerState::Filter::MIN_L_MAG_P_MIP_P:
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case SamplerState::Filter::MIN_L_MAG_P_MIP_L:
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case SamplerState::Filter::MIN_L_MAG_L_MIP_P:
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case SamplerState::Filter::MIN_L_MAG_L_MIP_L:
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glSamplerParameteri(mGLHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    default:
        GTL_RUNTIME_ERROR("Unknown sampler state filter.");
    }
}

std::shared_ptr<GEObject> GL45SamplerState::Create(void*, GraphicsObject const* object)
{
    if (object->GetType() == GT_SAMPLER_STATE)
    {
        return std::make_shared<GL45SamplerState>(
            static_cast<SamplerState const*>(object));
    }

    GTL_RUNTIME_ERROR("Invalid object type.");
}


GLint const GL45SamplerState::msMode[] =
{
    GL_REPEAT,          // WRAP
    GL_MIRRORED_REPEAT, // MIRROR
    GL_CLAMP_TO_EDGE,   // CLAMP
    GL_CLAMP_TO_BORDER, // BORDER
    GL_MIRRORED_REPEAT  // MIRROR_ONCE
};
