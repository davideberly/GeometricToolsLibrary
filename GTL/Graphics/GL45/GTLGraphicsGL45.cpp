// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsGL45PCH.h>

// On Microsoft Windows 10/11, the preprocessor symbol GTL_USE_MSWINDOWS is
// added to the Visual Studio project configurations. If a project uses
// DirectX 11 (or later), the symbol GTL_USE_DIRECTX is added to the Visual
// Studio projects. If a project uses OpenGL, the symbol GTL_USE_OPENGL is
// added to the Visual Studio projects.
//
// On Linux, the make system has the defines GTL_USE_LINUX and GTL_USE_OPENGL.

#if defined(GTL_USE_MSWINDOWS)

// The _MSC_VER macro has integer values as described at the webpage
// https://learn.microsoft.com/en-us/cpp/overview/compiler-versions?view=msvc-170
// Currently, projects are provided only for Visual Studio 2022 or later.
#if !defined(_MSC_VER)
#error Microsoft Visual Studio 2015 or later is required.
#endif
#if _MSC_VER < 1930
#error Microsoft Visual Studio 2015 or later is required.
#endif

#if !defined(GTL_USE_OPENGL)
#error GTL_USE_OPENGL must be defined in the project settings.
#endif

#endif
