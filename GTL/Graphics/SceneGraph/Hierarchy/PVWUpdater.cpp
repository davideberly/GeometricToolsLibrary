// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/PVWUpdater.h>
using namespace gtl;

PVWUpdater::PVWUpdater()
    :
    mCamera{},
    mUpdater{}
{
}

PVWUpdater::PVWUpdater(std::shared_ptr<Camera> const& camera, BufferUpdater const& updater)
    :
    mCamera(camera),
    mUpdater(updater)
{
}

void PVWUpdater::Set(std::shared_ptr<Camera> const& camera, BufferUpdater const& updater)
{
    mCamera = camera;
    mUpdater = updater;
}

bool PVWUpdater::Subscribe(Matrix4x4<float> const& worldMatrix,
    std::shared_ptr<ConstantBuffer> const& cbuffer,
    std::string const& pvwMatrixName)
{
    if (cbuffer && cbuffer->HasMember(pvwMatrixName))
    {
        if (mSubscribers.find(&worldMatrix) == mSubscribers.end())
        {
            mSubscribers.insert(std::make_pair(&worldMatrix,
                std::make_pair(cbuffer, pvwMatrixName)));
            return true;
        }
    }
    return false;
}

bool PVWUpdater::Subscribe(std::shared_ptr<Visual> const& visual,
    std::string const& pvwMatrixName)
{
    if (visual)
    {
        auto const& effect = visual->GetEffect();
        if (effect)
        {
            auto const& worldMatrix = visual->worldTransform;
            auto const& cbuffer = effect->GetPVWMatrixConstant();
            return Subscribe(worldMatrix, cbuffer, pvwMatrixName);
        }
    }
    return false;
}

bool PVWUpdater::Unsubscribe(Matrix4x4<float> const& worldMatrix)
{
    return mSubscribers.erase(&worldMatrix) > 0;
}

bool PVWUpdater::Unsubscribe(std::shared_ptr<Visual> const& visual)
{
    if (visual)
    {
        auto const& worldMatrix = visual->worldTransform;
        return Unsubscribe(worldMatrix);
    }
    return false;
}

void PVWUpdater::UnsubscribeAll()
{
    mSubscribers.clear();
}

void PVWUpdater::Update()
{
    if (mCamera)
    {
        Matrix4x4<float> pvMatrix = mCamera->GetProjectionViewMatrix();
        for (auto& element : mSubscribers)
        {
            // Compute the new projection-view-world matrix.  The matrix
            // *element.first is the model-to-world matrix for the associated
            // object.
            auto const& wMatrix = *element.first;
            Matrix4x4<float> pvwMatrix = pvMatrix * wMatrix;

            // Copy the source matrix into the CPU memory of the constant
            // buffer.
            auto const& cbuffer = element.second.first;
            auto const& name = element.second.second;
            cbuffer->SetMember(name, pvwMatrix);

            // Allow the caller to update GPU memory as desired.
            mUpdater(cbuffer);
        }
    }
}

void PVWUpdater::Update(std::vector<Visual*> const& updateSet)
{
    if (mCamera)
    {
        Matrix4x4<float> pvMatrix = mCamera->GetProjectionViewMatrix();
        for (auto& visual : updateSet)
        {
            if (visual)
            {
                auto const& effect = visual->GetEffect();
                if (effect)
                {
                    auto const& wMatrix = visual->worldTransform.GetH();
                    auto const& cbuffer = effect->GetPVWMatrixConstant();

                    // Compute the new projection-view-world matrix.
                    Matrix4x4<float> pvwMatrix = pvMatrix * wMatrix;

                    // Copy the source matrix into the CPU memory of the
                    // constant buffer.
                    effect->SetPVWMatrix(pvwMatrix);

                    // Allow the caller to update GPU memory as desired.
                    mUpdater(cbuffer);
                }
            }
        }
    }
}
