// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.19

#pragma once

#if defined(GTL_USE_MSWINDOWS)

#if defined(GTL_USE_DIRECTX)
#include <GTL/Graphics/DX11/Engine/DX11Engine.h>
#include <GTL/Graphics/DX11/HLSL/HLSLProgramFactory.h>
#endif

#if defined(GTL_USE_OPENGL)
#include <GTL/Graphics/GL45/WGL/WGLEngine.h>
#include <GTL/Graphics/GL45/GLSL/GLSLProgramFactory.h>
#endif

#endif

#if defined(GTL_USE_LINUX)
#include <GTL/Graphics/GL45/GLX/GLXEngine.h>
#include <GTL/Graphics/GL45/GLSL/GLSLProgramFactory.h>
#endif
