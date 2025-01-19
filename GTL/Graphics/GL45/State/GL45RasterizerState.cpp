// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/State/GL45RasterizerState.h>
using namespace gtl;

GL45RasterizerState::GL45RasterizerState(RasterizerState const* rasterizerState)
    :
    GL45DrawingState(rasterizerState)
{
    mFillMode = msFillMode[rasterizerState->fill];
    mCullFace = msCullFace[rasterizerState->cull];
    mFrontFace = (rasterizerState->frontCCW ? GL_CCW : GL_CW);
    mDepthScale = rasterizerState->slopeScaledDepthBias;
    mDepthBias = static_cast<float>(rasterizerState->depthBias);
    mEnableScissor = (rasterizerState->enableScissor ? GL_TRUE : GL_FALSE);
}

std::shared_ptr<GEObject> GL45RasterizerState::Create(void*, GraphicsObject const* object)
{
    if (object->GetType() == GT_RASTERIZER_STATE)
    {
        return std::make_shared<GL45RasterizerState>(
            static_cast<RasterizerState const*>(object));
    }

    GTL_RUNTIME_ERROR("Invalid object type.");
}

void GL45RasterizerState::Enable()
{
    glPolygonMode(GL_FRONT_AND_BACK, mFillMode);

    if (mCullFace != 0)
    {
        glEnable(GL_CULL_FACE);
        glFrontFace(mFrontFace);
        glCullFace(mCullFace);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

    if (mDepthScale != 0.0f && mDepthBias != 0.0f)
    {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glEnable(GL_POLYGON_OFFSET_POINT);
        glPolygonOffset(mDepthScale, mDepthBias);
    }
    else
    {
        glDisable(GL_POLYGON_OFFSET_FILL);
        glDisable(GL_POLYGON_OFFSET_LINE);
        glDisable(GL_POLYGON_OFFSET_POINT);
    }
}


GLenum const GL45RasterizerState::msFillMode[] =
{
    GL_FILL,
    GL_LINE
};

GLenum const GL45RasterizerState::msCullFace[] =
{
    0,
    GL_FRONT,
    GL_BACK
};
