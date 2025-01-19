// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Graphics/Resources/Textures/DrawTarget.h>
using namespace gtl;

DrawTarget::~DrawTarget()
{
    msLFDMutex.lock();
    {
        for (auto const& listener : msLFDSet)
        {
            listener->OnDestroy(this);
        }
    }
    msLFDMutex.unlock();
}

DrawTarget::DrawTarget(uint32_t numRenderTargets, uint32_t rtFormat,
    uint32_t width, uint32_t height, bool hasRTMipmaps,
    bool createRTStorage, uint32_t dsFormat, bool createDSStorage)
{
    GTL_ARGUMENT_ASSERT(
        numRenderTargets > 0,
        "Number of targets must be at least one.");

    mRTTextures.resize(numRenderTargets);
    for (auto& texture : mRTTextures)
    {
        texture = std::make_shared<TextureRT>(rtFormat, width, height,
            hasRTMipmaps, createRTStorage);
    }

    if (dsFormat != DF_UNKNOWN)
    {
        if (DataFormat::IsDepth(dsFormat))
        {
            mDSTexture = std::make_shared<TextureDS>(dsFormat, width,
                height, createDSStorage);
            return;
        }

        GTL_RUNTIME_ERROR(
            "Invalid depth-stencil format.");
    }
}

uint32_t DrawTarget::GetRTFormat() const
{
    GTL_ARGUMENT_ASSERT(
        mRTTextures.size() > 0,
        "Unexpected condition.");

    return mRTTextures[0]->GetFormat();
}

uint32_t DrawTarget::GetWidth() const
{
    GTL_ARGUMENT_ASSERT(
        mRTTextures.size() > 0,
        "Unexpected condition.");

    return mRTTextures[0]->GetWidth();
}

uint32_t DrawTarget::GetHeight() const
{
    GTL_ARGUMENT_ASSERT(
        mRTTextures.size() > 0,
        "Unexpected condition.");

    return mRTTextures[0]->GetHeight();
}

bool DrawTarget::HasRTMipmaps() const
{
    GTL_ARGUMENT_ASSERT(
        mRTTextures.size() > 0,
        "Unexpected condition.");

    return mRTTextures[0]->HasMipmaps();
}

uint32_t DrawTarget::GetDSFormat() const
{
    GTL_ARGUMENT_ASSERT(
        mDSTexture != nullptr,
        "Unexpected condition.");

    return mDSTexture->GetFormat();
}

std::shared_ptr<TextureRT> const DrawTarget::GetRTTexture(uint32_t i) const
{
    GTL_ARGUMENT_ASSERT(
        i < static_cast<uint32_t>(mRTTextures.size()),
        "Unexpected condition.");

    return mRTTextures[i];
}

void DrawTarget::AutogenerateRTMipmaps()
{
    if (HasRTMipmaps())
    {
        for (auto& texture : mRTTextures)
        {
            texture->AutogenerateMipmaps();
        }
    }
}

bool DrawTarget::WantAutogenerateRTMipmaps() const
{
    GTL_ARGUMENT_ASSERT(
        mRTTextures.size() > 0,
        "Unexpected condition.");

    return mRTTextures[0]->WantAutogenerateMipmaps();
}

void DrawTarget::SubscribeForDestruction(std::shared_ptr<ListenerForDestruction> const& listener)
{
    msLFDMutex.lock();
    {
        msLFDSet.insert(listener);
    }
    msLFDMutex.unlock();
}

void DrawTarget::UnsubscribeForDestruction(std::shared_ptr<ListenerForDestruction> const& listener)
{
    msLFDMutex.lock();
    {
        msLFDSet.erase(listener);
    }
    msLFDMutex.unlock();
}


std::mutex DrawTarget::msLFDMutex;
std::set<std::shared_ptr<DrawTarget::ListenerForDestruction>> DrawTarget::msLFDSet;
