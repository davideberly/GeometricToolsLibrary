// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Effects/Texture2Effect.h>
using namespace gtl;

Texture2Effect::Texture2Effect(std::shared_ptr<ProgramFactory> const& factory,
    std::shared_ptr<Texture2> const& texture,
    SamplerState::Filter filter, SamplerState::Mode mode0, SamplerState::Mode mode1)
    :
    mTexture(texture)
{
    int32_t api = factory->GetAPI();
    mProgram = factory->CreateFromSources(*msVSSource[api], *msPSSource[api], "");
    GTL_RUNTIME_ASSERT(
        mProgram != nullptr,
        "Failed to compile shader programs.");

    mSampler = std::make_shared<SamplerState>();
    mSampler->filter = filter;
    mSampler->mode[0] = mode0;
    mSampler->mode[1] = mode1;

    mProgram->GetVertexShader()->Set("PVWMatrix", mPVWMatrixConstant);
    mProgram->GetPixelShader()->Set("baseTexture", texture, "baseSampler", mSampler);
}

void Texture2Effect::SetPVWMatrixConstant(std::shared_ptr<ConstantBuffer> const& buffer)
{
    VisualEffect::SetPVWMatrixConstant(buffer);
    mProgram->GetVertexShader()->Set("PVWMatrix", mPVWMatrixConstant);
}


std::string const Texture2Effect::msGLSLVSSource =
R"(
    uniform PVWMatrix
    {
        mat4 pvwMatrix;
    };

    layout(location = 0) in vec3 modelPosition;
    layout(location = 1) in vec2 modelTCoord;
    layout(location = 0) out vec2 vertexTCoord;

    void main()
    {
        vertexTCoord = modelTCoord;
        gl_Position = pvwMatrix * vec4(modelPosition, 1.0f);
    }
)";

std::string const Texture2Effect::msGLSLPSSource =
R"(
    uniform sampler2D baseSampler;

    layout(location = 0) in vec2 vertexTCoord;
    layout(location = 0) out vec4 pixelColor;

    void main()
    {
        pixelColor = texture(baseSampler, vertexTCoord);
    }
)";

std::string const Texture2Effect::msHLSLVSSource =
R"(
    cbuffer PVWMatrix
    {
        float4x4 pvwMatrix;
    };

    struct VS_INPUT
    {
        float3 modelPosition : POSITION;
        float2 modelTCoord : TEXCOORD0;
    };

    struct VS_OUTPUT
    {
        float2 vertexTCoord : TEXCOORD0;
        float4 clipPosition : SV_POSITION;
    };

    VS_OUTPUT VSMain(VS_INPUT input)
    {
        VS_OUTPUT output;
        output.clipPosition = mul(pvwMatrix, float4(input.modelPosition, 1.0f));
        output.vertexTCoord = input.modelTCoord;
        return output;
    }
)";

std::string const Texture2Effect::msHLSLPSSource =
R"(
    Texture2D baseTexture;
    SamplerState baseSampler;

    struct PS_INPUT
    {
        float2 vertexTCoord : TEXCOORD0;
    };

    struct PS_OUTPUT
    {
        float4 pixelColor : SV_TARGET0;
    };

    PS_OUTPUT PSMain(PS_INPUT input)
    {
        PS_OUTPUT output;
        output.pixelColor = baseTexture.Sample(baseSampler, input.vertexTCoord);
        return output;
    }
)";

ProgramSources const Texture2Effect::msVSSource =
{
    &msGLSLVSSource,
    &msHLSLVSSource
};

ProgramSources const Texture2Effect::msPSSource =
{
    &msGLSLPSSource,
    &msHLSLPSSource
};
