// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

// DX11/DXGI
#include <GTL/Graphics/DX11/DXGI/DXGIAdapter.h>
#include <GTL/Graphics/DX11/DXGI/DXGIOutput.h>

// DX11/Engine
#include <GTL/Graphics/DX11/Engine/DX11.h>
#include <GTL/Graphics/DX11/Engine/DX11Engine.h>
#include <GTL/Graphics/DX11/Engine/DX11GraphicsObject.h>
#include <GTL/Graphics/DX11/Engine/DX11PerformanceCounter.h>

// DX11/Engine/InputLayout
#include <GTL/Graphics/DX11/InputLayout/DX11InputLayout.h>
#include <GTL/Graphics/DX11/InputLayout/DX11InputLayoutManager.h>

// DX11/Engine/Resources
#include <GTL/Graphics/DX11/Resources/DX11Resource.h>

// DX11/Engine/Resources/Buffers
#include <GTL/Graphics/DX11/Resources/Buffers/DX11Buffer.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11ConstantBuffer.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11IndexBuffer.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11IndirectArgumentsBuffer.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11RawBuffer.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11StructuredBuffer.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11TextureBuffer.h>
#include <GTL/Graphics/DX11/Resources/Buffers/DX11VertexBuffer.h>

// DX11Engine/Resources/Textures
#include <GTL/Graphics/DX11/Resources/Textures/DX11DrawTarget.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11Texture.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11Texture1.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11Texture1Array.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11Texture2.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11Texture2Array.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11Texture3.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11TextureArray.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11TextureCube.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11TextureCubeArray.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11TextureDS.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11TextureRT.h>
#include <GTL/Graphics/DX11/Resources/Textures/DX11TextureSingle.h>

// DX11/Engine/Shaders
#include <GTL/Graphics/DX11/Shaders/DX11ComputeShader.h>
#include <GTL/Graphics/DX11/Shaders/DX11GeometryShader.h>
#include <GTL/Graphics/DX11/Shaders/DX11PixelShader.h>
#include <GTL/Graphics/DX11/Shaders/DX11VertexShader.h>
#include <GTL/Graphics/DX11/Shaders/DX11Shader.h>

// DX11/Engine/State
#include <GTL/Graphics/DX11/State/DX11BlendState.h>
#include <GTL/Graphics/DX11/State/DX11DepthStencilState.h>
#include <GTL/Graphics/DX11/State/DX11DrawingState.h>
#include <GTL/Graphics/DX11/State/DX11RasterizerState.h>
#include <GTL/Graphics/DX11/State/DX11SamplerState.h>

// DX11/HLSL
#include <GTL/Graphics/DX11/HLSL/HLSLBaseBuffer.h>
#include <GTL/Graphics/DX11/HLSL/HLSLByteAddressBuffer.h>
#include <GTL/Graphics/DX11/HLSL/HLSLComputeProgram.h>
#include <GTL/Graphics/DX11/HLSL/HLSLConstantBuffer.h>
#include <GTL/Graphics/DX11/HLSL/HLSLParameter.h>
#include <GTL/Graphics/DX11/HLSL/HLSLFactory.h>
#include <GTL/Graphics/DX11/HLSL/HLSLResource.h>
#include <GTL/Graphics/DX11/HLSL/HLSLResourceBindInfo.h>
#include <GTL/Graphics/DX11/HLSL/HLSLSamplerState.h>
#include <GTL/Graphics/DX11/HLSL/HLSLShader.h>
#include <GTL/Graphics/DX11/HLSL/HLSLShaderFactory.h>
#include <GTL/Graphics/DX11/HLSL/HLSLShaderType.h>
#include <GTL/Graphics/DX11/HLSL/HLSLShaderVariable.h>
#include <GTL/Graphics/DX11/HLSL/HLSLStructuredBuffer.h>
#include <GTL/Graphics/DX11/HLSL/HLSLTexture.h>
#include <GTL/Graphics/DX11/HLSL/HLSLTextureArray.h>
#include <GTL/Graphics/DX11/HLSL/HLSLTextureBuffer.h>
#include <GTL/Graphics/DX11/HLSL/HLSLVisualProgram.h>
