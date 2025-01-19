// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Effects/SphereMapEffect.h>
using namespace gtl;

SphereMapEffect::SphereMapEffect(std::shared_ptr<ProgramFactory> const& factory,
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

    mVWMatrixConstant = std::make_shared<ConstantBuffer>(sizeof(Matrix4x4<float>), true);
    *mVWMatrixConstant->Get<Matrix4x4<float>>() = Matrix4x4<float>::Identity();

    mSampler = std::make_shared<SamplerState>();
    mSampler->filter = filter;
    mSampler->mode[0] = mode0;
    mSampler->mode[1] = mode1;

    auto const& vshader = mProgram->GetVertexShader();
    vshader->Set("PVWMatrix", mPVWMatrixConstant);
    vshader->Set("VWMatrix", mVWMatrixConstant);
    mProgram->GetPixelShader()->Set("baseTexture", texture, "baseSampler", mSampler);
}

void SphereMapEffect::SetPVWMatrixConstant(std::shared_ptr<ConstantBuffer> const& buffer)
{
    VisualEffect::SetPVWMatrixConstant(buffer);
    mProgram->GetVertexShader()->Set("PVWMatrix", mPVWMatrixConstant);
}


std::string const SphereMapEffect::msGLSLVSSource =
R"(
    uniform PVWMatrix
    {
        mat4 pvwMatrix;
    };

    uniform VWMatrix
    {
        mat4 vwMatrix;
    };

    layout(location = 0) in vec3 inModelPosition;
    layout(location = 1) in vec3 inModelNormal;

    layout(location = 0) out vec2 vertexTCoord;

    void main()
    {
        vec4 modelPosition = vec4(inModelPosition, 1.0f);
        vec4 modelNormal = vec4(inModelNormal, 0.0f);

        vec4 cameraSpacePosition = vwMatrix * modelPosition;
        vec3 cameraSpaceNormal = normalize((vwMatrix * modelNormal).xyz);
        gl_Position = pvwMatrix * modelPosition;

        vec3 eyeDirection = normalize(cameraSpacePosition.xyz);
        vec3 r = reflect(eyeDirection, cameraSpaceNormal);

        float oneMRZ = 1.0f - r.z;
        float invLength = 1.0f / sqrt(r.x * r.x + r.y * r.y + oneMRZ * oneMRZ);
        vertexTCoord = 0.5f * (r.xy * invLength + 1.0f);
    }
)";

std::string const SphereMapEffect::msGLSLPSSource =
R"(
    layout(location = 0) in vec2 vertexTCoord;
    layout(location = 0) out vec4 pixelColor;

    uniform sampler2D baseSampler;

    void main()
    {
        pixelColor = texture(baseSampler, vertexTCoord);
    }
)";

std::string const SphereMapEffect::msHLSLVSSource =
R"(
    cbuffer PVWMatrix
    {
        float4x4 pvwMatrix;
    };

    cbuffer VWMatrix
    {
        float4x4 vwMatrix;
    };

    struct VS_INPUT
    {
        float3 modelPosition : POSITION;
        float3 modelNormal : NORMAL;
    };

    struct VS_OUTPUT
    {
        float2 vertexTCoord : TEXCOORD0;
        float4 clipPosition : SV_POSITION;
    };

    VS_OUTPUT VSMain(VS_INPUT input)
    {
        VS_OUTPUT output;

        float4 modelPosition = float4(input.modelPosition, 1.0f);
        float4 modelNormal = float4(input.modelNormal, 0.0f);

        float4 cameraSpacePosition = mul(vwMatrix, modelPosition);
        float3 cameraSpaceNormal = normalize(mul(vwMatrix, modelNormal).xyz);
        output.clipPosition = mul(pvwMatrix, modelPosition);

        float3 eyeDirection = normalize(cameraSpacePosition.xyz);
        float3 r = reflect(eyeDirection, cameraSpaceNormal);

        float oneMRZ = 1.0f - r.z;
        float invLength = 1.0f / sqrt(r.x * r.x + r.y * r.y + oneMRZ * oneMRZ);
        output.vertexTCoord = 0.5f * (r.xy * invLength + 1.0f);

        return output;
    }
)";

std::string const SphereMapEffect::msHLSLPSSource =
R"(
    struct PS_INPUT
    {
        float2 vertexTCoord : TEXCOORD0;
    };

    struct PS_OUTPUT
    {
        float4 pixelColor : SV_TARGET0;
    };

    Texture2D<float4> baseTexture;
    SamplerState baseSampler;

    PS_OUTPUT PSMain(PS_INPUT input)
    {
        PS_OUTPUT output;
        output.pixelColor = baseTexture.Sample(baseSampler, input.vertexTCoord);
        return output;
    }
)";

ProgramSources const SphereMapEffect::msVSSource =
{
    &msGLSLVSSource,
    &msHLSLVSSource
};

ProgramSources const SphereMapEffect::msPSSource =
{
    &msGLSLPSSource,
    &msHLSLPSSource
};
