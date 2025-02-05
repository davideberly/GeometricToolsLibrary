// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Base/GraphicsObject.h>
using namespace gtl;

GraphicsObject::~GraphicsObject()
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

GraphicsObject::GraphicsObject()
    :
    mType(GT_NONE),
    mName("")
{
}

GraphicsObject::GraphicsObject(GraphicsObjectType type)
    :
    mType(type),
    mName("")
{
}

void GraphicsObject::SubscribeForDestruction(std::shared_ptr<ListenerForDestruction> const& listener)
{
    msLFDMutex.lock();
    {
        msLFDSet.insert(listener);
    }
    msLFDMutex.unlock();
}

void GraphicsObject::UnsubscribeForDestruction(std::shared_ptr<ListenerForDestruction> const& listener)
{
    msLFDMutex.lock();
    {
        msLFDSet.erase(listener);
    }
    msLFDMutex.unlock();
}

std::mutex GraphicsObject::msLFDMutex{};
std::set<std::shared_ptr<GraphicsObject::ListenerForDestruction>> GraphicsObject::msLFDSet{};
