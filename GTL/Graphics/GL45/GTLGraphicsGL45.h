// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

// GL45/Engine
#include <GTL/Graphics/GL45/Engine/GL45.h>
#include <GTL/Graphics/GL45/Engine/GL45Engine.h>
#include <GTL/Graphics/GL45/Engine/GL45GraphicsObject.h>

// GL45/InputLayout
#include <GTL/Graphics/GL45/InputLayout/GL45InputLayout.h>
#include <GTL/Graphics/GL45/InputLayout/GL45InputLayoutManager.h>

// GL45/Resources
#include <GTL/Graphics/GL45/Resources/GL45Resource.h>

// GL45/Resources/Buffers
#include <GTL/Graphics/GL45/Resources/Buffers/GL45AtomicCounterBuffer.h>
#include <GTL/Graphics/GL45/Resources/Buffers/GL45Buffer.h>
#include <GTL/Graphics/GL45/Resources/Buffers/GL45ConstantBuffer.h>
#include <GTL/Graphics/GL45/Resources/Buffers/GL45IndexBuffer.h>
#include <GTL/Graphics/GL45/Resources/Buffers/GL45StructuredBuffer.h>
#include <GTL/Graphics/GL45/Resources/Buffers/GL45VertexBuffer.h>

// GL45/Resources/Textures
#include <GTL/Graphics/GL45/Resources/Textures/GL45DrawTarget.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45Texture.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45Texture1.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45Texture1Array.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45Texture2.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45Texture2Array.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45Texture3.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45TextureArray.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45TextureCube.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45TextureCubeArray.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45TextureDS.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45TextureRT.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45TextureSingle.h>

// GL45/State
#include <GTL/Graphics/GL45/State/GL45BlendState.h>
#include <GTL/Graphics/GL45/State/GL45DrawingState.h>
#include <GTL/Graphics/GL45/State/GL45DepthStencilState.h>
#include <GTL/Graphics/GL45/State/GL45RasterizerState.h>
#include <GTL/Graphics/GL45/State/GL45SamplerState.h>

// GL45/GLSL
#include <GTL/Graphics/GL45/GLSL/GLSLComputeProgram.h>
#include <GTL/Graphics/GL45/GLSL/GLSLProgramFactory.h>
#include <GTL/Graphics/GL45/GLSL/GLSLReflection.h>
#include <GTL/Graphics/GL45/GLSL/GLSLShader.h>
#include <GTL/Graphics/GL45/GLSL/GLSLVisualProgram.h>

#if defined(GTL_USE_MSWINDOWS)
#include <GTL/Graphics/GL45/WGL/WGLEngine.h>
#endif

#if defined(GTL_USE_LINUX)
#include <GTL/Graphics/GL45/GLX/GLXEngine.h>
#endif
