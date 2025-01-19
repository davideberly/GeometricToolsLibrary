// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLApplicationsPCH.h>
#include <GTL/Mathematics/ImageProcessing/Rasterize2.h>
#include <GTL/Applications/Window2.h>
#include <GTL/Graphics/State/SamplerState.h>
using namespace gtl;

Window2::Window2(Parameters& parameters)
    :
    Window(parameters),
    mPixelColor(0),
    mThick(0),
    mClampToWindow(true),
    mDoFlip(false),
    mScreenTextureNeedsUpdate(false)
{
    mOverlay = std::make_shared<OverlayEffect>(mProgramFactory,
        mXSize, mYSize, mXSize, mYSize,
        SamplerState::Filter::MIN_P_MAG_P_MIP_P,
        SamplerState::Mode::CLAMP,
        SamplerState::Mode::CLAMP,
        true);

    mScreenTexture = std::make_shared<Texture2>(DF_R8G8B8A8_UNORM, mXSize, mYSize);
    mScreenTexture->SetUsage(Resource::Usage::DYNAMIC_UPDATE);
    mOverlay->SetTexture(mScreenTexture);

    // The default is to disable depth and stenciling.  For layered drawing in
    // the z-direction, an application can choose to restore the default mode
    // of depth and stenciling turned on.
    mNoDepthStencilState = std::make_shared<DepthStencilState>();
    mNoDepthStencilState->depthEnable = false;
    mNoDepthStencilState->stencilEnable = false;
    mEngine->SetDepthStencilState(mNoDepthStencilState);

    // Callback functions for ImageUtility2 functions.
    mDrawPixel = [this](int32_t x, int32_t y) { SetPixel(x, y, mPixelColor); };
    mDrawThickPixel = [this](int32_t x, int32_t y)
    {
        for (int32_t dy = -mThick; dy <= mThick; ++dy)
        {
            for (int32_t dx = -mThick; dx <= mThick; ++dx)
            {
                SetPixel(x + dx, y + dy, mPixelColor);
            }
        }
    };
}

bool Window2::OnResize(int32_t xSize, int32_t ySize)
{
    if (xSize != mXSize || ySize != mYSize)
    {
        mXSize = xSize;
        mYSize = ySize;

        mOverlay = std::make_shared<OverlayEffect>(mProgramFactory,
            mXSize, mYSize, mXSize, mYSize,
            SamplerState::Filter::MIN_P_MAG_P_MIP_P,
            SamplerState::Mode::CLAMP,
            SamplerState::Mode::CLAMP,
            true);

        mScreenTexture = std::make_shared<Texture2>(DF_R8G8B8A8_UNORM, mXSize, mYSize);
        mScreenTexture->SetUsage(Resource::Usage::DYNAMIC_UPDATE);

        mOverlay->SetTexture(mScreenTexture);

        mEngine->Resize(static_cast<uint32_t>(mXSize), static_cast<uint32_t>(mYSize));
    }
    return true;
}

void Window2::OnDisplay()
{
    if (mScreenTextureNeedsUpdate)
    {
        mEngine->Update(mScreenTexture);
        mScreenTextureNeedsUpdate = false;
    }

    mEngine->Draw(mOverlay);
    DrawScreenOverlay();
    mEngine->DisplayColorBuffer(0);
}

void Window2::DrawScreenOverlay()
{
    // Stub for derived classes.
}

void Window2::ClearScreen(uint32_t color)
{
    uint32_t const numTexels = mScreenTexture->GetNumElements();
    uint32_t* texels = mScreenTexture->Get<uint32_t>();
    for (uint32_t i = 0; i < numTexels; ++i)
    {
        *texels++ = color;
    }
}

void Window2::SetPixel(int32_t x, int32_t y, uint32_t color)
{
    if (mClampToWindow)
    {
        if (x < 0 || x >= mXSize || y < 0 || y >= mYSize)
        {
            return;
        }
    }

    if (mDoFlip)
    {
        y = mYSize - 1 - y;
    }

    mScreenTexture->Get<uint32_t>()[x + mXSize * y] = color;
}

uint32_t Window2::GetPixel(int32_t x, int32_t y)
{
    if (mClampToWindow)
    {
        if (x < 0 || x >= mXSize || y < 0 || y >= mYSize)
        {
            return 0;
        }
    }

    if (mDoFlip)
    {
        y = mYSize - 1 - y;
    }

    return mScreenTexture->Get<uint32_t>()[x + mXSize * y];
}

void Window2::DrawThickPixel(int32_t x, int32_t y, int32_t thick, uint32_t color)
{
    mPixelColor = color;
    Rasterize2<int32_t>::DrawThickPixel(x, y, thick, mDrawPixel);
}

void Window2::DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
{
    mPixelColor = color;
    Rasterize2<int32_t>::DrawLine(x0, y0, x1, y1, mDrawPixel);
}

void Window2::DrawThickLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t thick, uint32_t color)
{
    mPixelColor = color;
    mThick = thick;
    Rasterize2<int32_t>::DrawLine(x0, y0, x1, y1, mDrawThickPixel);
}

void Window2::DrawRectangle(int32_t xMin, int32_t yMin, int32_t xMax, int32_t yMax, uint32_t color, bool solid)
{
    mPixelColor = color;
    Rasterize2<int32_t>::DrawRectangle(xMin, yMin, xMax, yMax, solid, mDrawPixel);
}

void Window2::DrawThickRectangle(int32_t xMin, int32_t yMin, int32_t xMax, int32_t yMax, int32_t thick, uint32_t color, bool solid)
{
    mPixelColor = color;
    mThick = thick;
    Rasterize2<int32_t>::DrawRectangle(xMin, yMin, xMax, yMax, solid, mDrawThickPixel);
}

void Window2::DrawCircle(int32_t xCenter, int32_t yCenter, int32_t radius, uint32_t color, bool solid)
{
    mPixelColor = color;
    Rasterize2<int32_t>::DrawCircle(xCenter, yCenter, radius, solid, mDrawPixel);
}

void Window2::DrawThickCircle(int32_t xCenter, int32_t yCenter, int32_t radius, int32_t thick, uint32_t color, bool solid)
{
    mPixelColor = color;
    mThick = thick;
    Rasterize2<int32_t>::DrawCircle(xCenter, yCenter, radius, solid, mDrawThickPixel);
}

void Window2::DrawEllipse(int32_t xCenter, int32_t yCenter, int32_t xExtent, int32_t yExtent, uint32_t color)
{
    mPixelColor = color;
    Rasterize2<int32_t>::DrawEllipse(xCenter, yCenter, xExtent, yExtent, mDrawPixel);
}

void Window2::DrawThickEllipse(int32_t xCenter, int32_t yCenter, int32_t xExtent, int32_t yExtent, int32_t thick, uint32_t color)
{
    mPixelColor = color;
    mThick = thick;
    Rasterize2<int32_t>::DrawEllipse(xCenter, yCenter, xExtent, yExtent, mDrawThickPixel);
}

void Window2::DrawFloodFill4(int32_t x, int32_t y, uint32_t foreColor, uint32_t backColor)
{
    Rasterize2<int32_t>::DrawFloodFill4<uint32_t>(x, y, mXSize, mYSize, foreColor, backColor,
        [this](int32_t x, int32_t y, uint32_t color) { SetPixel(x, y, color); },
        [this](int32_t x, int32_t y) { return GetPixel(x, y); });
}
