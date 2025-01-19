// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Base/BaseEngine.h>
using namespace gtl;

BaseEngine::BaseEngine()
    :
    mXSize(0),
    mYSize(0),
    mClearDepth(1.0f),
    mClearStencil(0)
{
    mClearColor.fill(1.0f);
}

void BaseEngine::SetFont(std::shared_ptr<Font> const& font)
{
    mActiveFont = font;
}

void BaseEngine::CreateDefaultGlobalState()
{
    mDefaultBlendState = std::make_shared<BlendState>();
    mDefaultDepthStencilState = std::make_shared<DepthStencilState>();
    mDefaultRasterizerState = std::make_shared<RasterizerState>();

#if defined(GTL_GRAPHICS_USE_NAMED_OBJECTS)
    mDefaultBlendState->SetName("BaseEngine::mDefaultBlendState");
    mDefaultDepthStencilState->SetName("BaseEngine::mDefaultDepthStencilState");
    mDefaultRasterizerState->SetName("BaseEngine::mDefaultRasterizerState");
#endif

    SetDefaultBlendState();
    SetDefaultDepthStencilState();
    SetDefaultRasterizerState();
}

void BaseEngine::DestroyDefaultGlobalState()
{
    mDefaultBlendState = nullptr;
    mActiveBlendState = nullptr;
    mDefaultDepthStencilState = nullptr;
    mActiveDepthStencilState = nullptr;
    mDefaultRasterizerState = nullptr;
    mActiveRasterizerState = nullptr;
}
