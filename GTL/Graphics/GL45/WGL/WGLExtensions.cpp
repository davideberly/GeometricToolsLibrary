// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#include <GTL/Graphics/GL45/Engine/GL45.h>
#include <GTL/Graphics/GL45/GL/wglext.h>
#include <cassert>
#include <cstdint>

void* GetOpenGLFunctionPointer(char const* name)
{
    return reinterpret_cast<void*>(wglGetProcAddress(name));
}

static PFNWGLSWAPINTERVALEXTPROC swglSwapIntervalEXT = nullptr;
static PFNWGLGETSWAPINTERVALEXTPROC swglGetSwapIntervalEXT = nullptr;

BOOL __stdcall wglSwapIntervalEXT(int interval)
{
    if (swglSwapIntervalEXT)
    {
        return swglSwapIntervalEXT(interval);
    }
    else
    {
        // The swap intervals extension is required.
        assert(false);
        return FALSE;
    }
}

int __stdcall wglGetSwapIntervalEXT(void)
{
    if (swglGetSwapIntervalEXT)
    {
        return swglGetSwapIntervalEXT();
    }
    else
    {
        // The swap intervals extension is required.
        assert(false);
        return 0;
    }
}

void InitializeWGL()
{
    swglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(
        GetOpenGLFunctionPointer("wglSwapIntervalEXT"));

    swglGetSwapIntervalEXT = reinterpret_cast<PFNWGLGETSWAPINTERVALEXTPROC>(
        GetOpenGLFunctionPointer("wglGetSwapIntervalEXT"));
}
