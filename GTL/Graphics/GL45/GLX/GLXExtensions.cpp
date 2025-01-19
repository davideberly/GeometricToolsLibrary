// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#include <GTL/Graphics/GL45/GL45.h>
#include <EGL/egl.h>
#include <GL/glx.h>
#include <GL/glxext.h>

bool gUseEGLGetProcAddress = false;

void* GetOpenGLFunctionPointer(char const* name)
{
    if (gUseEGLGetProcAddress)
    {
        return (void*)(*eglGetProcAddress)(name);
    }
    else
    {
        return (void*)(*glXGetProcAddress)((GLubyte const*)name);
    }
}

struct SwapIntervalLoader
{
    SwapIntervalLoader()
        :
        sglXSwapInterval(nullptr)
    {
        sglXSwapInterval = (PFNGLXSWAPINTERVALEXTPROC)((void*)(*glXGetProcAddress)((GLubyte const*)name));
    }
    
    ~SwapIntervalLoader()
    {
        sglXSwapInterval = nullptr;
    }

    char const* name = "glXSwapIntervalEXT";
    PFNGLXSWAPINTERVALEXTPROC sglXSwapInterval;
};

static SwapIntervalLoader gLoader{};

void glXSwapInterval(Display* display, unsigned long window, int syncInterval)
{
    if (gLoader.sglXSwapInterval)
    {
        gLoader.sglXSwapInterval(display, window, syncInterval);
        return;
    }
}

