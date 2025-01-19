// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Mathematics/Algebra/Matrix.h>
#include <GTL/Graphics/Shaders/VisualProgram.h>
#include <GTL/Graphics/Shaders/ProgramFactory.h>
#include <GTL/Graphics/Resources/Buffers/ConstantBuffer.h>
#include <GTL/Graphics/Resources/Textures/Texture.h>
#include <GTL/Graphics/Resources/Textures/TextureArray.h>

namespace gtl
{
    class VisualEffect
    {
    public:
        // Construction and destruction.
        virtual ~VisualEffect() = default;
        VisualEffect(std::shared_ptr<VisualProgram> const& program);

        // Member access.
        inline std::shared_ptr<VisualProgram> const& GetProgram() const
        {
            return mProgram;
        }

        inline std::shared_ptr<Shader> const& GetVertexShader() const
        {
            return mProgram->GetVertexShader();
        }

        inline std::shared_ptr<Shader> const& GetPixelShader() const
        {
            return mProgram->GetPixelShader();
        }

        inline std::shared_ptr<Shader> const& GetGeometryShader() const
        {
            return mProgram->GetGeometryShader();
        }

        // For convenience, provide a projection-view-world constant buffer
        // that an effect can use if so desired.
        virtual void SetPVWMatrixConstant(std::shared_ptr<ConstantBuffer> const& buffer);

        inline std::shared_ptr<ConstantBuffer> const& GetPVWMatrixConstant() const
        {
            return mPVWMatrixConstant;
        }

        inline void SetPVWMatrix(Matrix4x4<float> const& pvwMatrix)
        {
            *mPVWMatrixConstant->Get<Matrix4x4<float>>() = pvwMatrix;
        }

        inline Matrix4x4<float> const& GetPVWMatrix() const
        {
            return *mPVWMatrixConstant->Get<Matrix4x4<float>>();
        }

    protected:
        // For derived classes to defer construction because they want to
        // create programs via a factory.
        VisualEffect();

        std::shared_ptr<VisualProgram> mProgram;
        BufferUpdater mBufferUpdater;
        TextureUpdater mTextureUpdater;
        TextureArrayUpdater mTextureArrayUpdater;

        // The constant buffer that stores the 4x4 projection-view-world
        // transformation for the Visual object to which this effect is
        // attached.
        std::shared_ptr<ConstantBuffer> mPVWMatrixConstant;
    };
}
