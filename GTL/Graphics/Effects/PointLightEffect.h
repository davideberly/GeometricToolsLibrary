// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/Effects/LightEffect.h>

namespace gtl
{
    class PointLightEffect : public LightEffect
    {
    public:
        // Construction.  Set 'select' to 0 for per-vertex effects or to 1 for
        // per-pixel effects.
        PointLightEffect(std::shared_ptr<ProgramFactory> const& factory,
            BufferUpdater const& updater, int32_t select, std::shared_ptr<Material> const& material,
            std::shared_ptr<Lighting> const& lighting, std::shared_ptr<LightCameraGeometry> const& geometry);

        // After you set or modify 'material', 'light', or 'geometry', call
        // the update to inform any listener that the corresponding constant
        // buffer has changed.
        virtual void UpdateMaterialConstant();
        virtual void UpdateLightingConstant();
        virtual void UpdateGeometryConstant();

    private:
        struct InternalMaterial
        {
            Vector4<float> emissive;
            Vector4<float> ambient;
            Vector4<float> diffuse;
            Vector4<float> specular;
        };

        struct InternalLighting
        {
            Vector4<float> ambient;
            Vector4<float> diffuse;
            Vector4<float> specular;
            Vector4<float> attenuation;
        };

        struct InternalGeometry
        {
            Vector4<float> lightModelPosition;
            Vector4<float> cameraModelPosition;
        };

        // Shader source code as strings.
        static std::string const msGLSLVSSource[2];
        static std::string const msGLSLPSSource[2];
        static std::string const msHLSLVSSource[2];
        static std::string const msHLSLPSSource[2];
        static ProgramSources const msVSSource[2];
        static ProgramSources const msPSSource[2];
    };
}
