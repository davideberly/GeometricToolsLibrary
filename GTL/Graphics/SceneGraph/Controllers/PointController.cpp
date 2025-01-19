// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Mathematics/Algebra/Rotation.h>
#include <GTL/Graphics/Resources/Buffers/IndexBuffer.h>
#include <GTL/Graphics/SceneGraph/Controllers/PointController.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/Visual.h>
#include <cstdint>
using namespace gtl;

PointController::PointController(BufferUpdater const& postUpdate)
    :
    systemLinearSpeed(0.0f),
    systemAngularSpeed(0.0f),
    systemLinearAxis(Vector3<float>::Unit(2)),
    systemAngularAxis(Vector3<float>::Unit(2)),
    mPostUpdate(postUpdate)
{
}

bool PointController::Update(double applicationTime)
{
    if (!Controller::Update(applicationTime))
    {
        return false;
    }

    float ctrlTime = static_cast<float>(GetControlTime(applicationTime));

    UpdateSystemMotion(ctrlTime);
    UpdatePointMotion(ctrlTime);
    return true;
}

void PointController::SetObject(ControlledObject* object)
{
    mPointLinearSpeed.clear();
    mPointAngularSpeed.clear();
    mPointLinearAxis.clear();
    mPointAngularAxis.clear();

    auto visual = dynamic_cast<Visual*>(object);
    GTL_ARGUMENT_ASSERT(
        visual != nullptr,
        "Object is not of type Visual.");

    auto const& ibuffer = visual->GetIndexBuffer();
    uint32_t primitiveType = ibuffer->GetPrimitiveType();
    GTL_ARGUMENT_ASSERT(
        primitiveType == IP_POLYPOINT,
        "Geometric primitive must be points.");

    // The vertex buffer for a Visual controlled by a PointController must
    // have 3-tuple or 4-tuple float-valued position that occurs at the
    // beginning (offset 0) of the vertex structure.
    auto const& vbuffer = visual->GetVertexBuffer();
    VertexFormat vformat = vbuffer->GetFormat();
    int32_t index = vformat.GetIndex(VASemantic::POSITION, 0);
    GTL_ARGUMENT_ASSERT(
        index >= 0,
        "Vertex format does not have VASemantic::POSITION.");

    uint32_t type = vformat.GetType(index);
    GTL_ARGUMENT_ASSERT(
        type == DF_R32G32B32_FLOAT || type == DF_R32G32B32A32_FLOAT,
        "Invalid position type.");

    uint32_t offset = vformat.GetOffset(index);
    GTL_ARGUMENT_ASSERT(
        offset == 0,
        "Position offset must be 0.");

    // If the vertex buffer for a Visual controlled by a PointController has
    // normal vectors, they must be 3-tuple or 4-tuple float-valued that
    // occurs at an offset >= sizeof(Vector3<float>>).
    index = vformat.GetIndex(VASemantic::NORMAL, 0);
    if (index >= 0)
    {
        type = vformat.GetType(index);
        GTL_ARGUMENT_ASSERT(
            type == DF_R32G32B32_FLOAT || type == DF_R32G32B32A32_FLOAT,
            "Invalid normal type.");
    }

    size_t numPoints = static_cast<size_t>(vbuffer->GetNumElements());
    mPointLinearSpeed.resize(numPoints);
    mPointAngularSpeed.resize(numPoints);
    mPointLinearAxis.resize(numPoints);
    mPointAngularAxis.resize(numPoints);
    for (size_t i = 0; i < numPoints; ++i)
    {
        mPointLinearSpeed[i] = 0.0f;
        mPointAngularSpeed[i] = 0.0f;
        mPointLinearAxis[i] = Vector3<float>::Unit(2);
        mPointAngularAxis[i] = Vector3<float>::Unit(2);
    }

    Controller::SetObject(object);
}

void PointController::UpdateSystemMotion(float ctrlTime)
{
    auto visual = static_cast<Visual*>(mObject);

    float distance = ctrlTime * systemLinearSpeed;
    Vector3<float> currentTrn = visual->localTransform.GetTranslation();
    Vector3<float> deltaTrn = distance * systemLinearAxis;
    visual->localTransform.SetTranslation(currentTrn + deltaTrn);

    float angle = ctrlTime * systemAngularSpeed;
    Matrix3x3<float> currentRot = visual->localTransform.GetRotation();
    Matrix3x3<float> deltaRot = Rotation<float>(AxisAngle<float>(systemAngularAxis, angle));
    visual->localTransform.SetRotation(deltaRot * currentRot);
}

void PointController::UpdatePointMotion(float ctrlTime)
{
    auto visual = static_cast<Visual*>(mObject);
    auto const& vbuffer = visual->GetVertexBuffer();
    VertexFormat vformat = vbuffer->GetFormat();
    uint32_t numVertices = vbuffer->GetNumElements();
    char* vertices = vbuffer->GetData();
    size_t vertexSize = static_cast<size_t>(vformat.GetVertexSize());

    for (uint32_t i = 0; i < numVertices; ++i)
    {
        Vector3<float>& position = *reinterpret_cast<Vector3<float>*>(vertices);
        float distance = ctrlTime * mPointLinearSpeed[i];
        Vector3<float> deltaTrn = distance * mPointLinearAxis[i];
        position += deltaTrn;
        vertices += vertexSize;
    }

    int32_t index = vformat.GetIndex(VASemantic::NORMAL, 0);
    if (index >= 0)
    {
        uint32_t offset = vformat.GetOffset(index);
        vertices = vbuffer->GetData() + offset;
        for (uint32_t i = 0; i < numVertices; ++i)
        {
            Vector3<float>& normal = *reinterpret_cast<Vector3<float>*>(vertices);
            Normalize(normal);
            float angle = ctrlTime * mPointAngularSpeed[i];
            Matrix3x3<float> deltaRot = Rotation<float>(AxisAngle<float>(mPointAngularAxis[i], angle));
            normal = deltaRot * normal;
            vertices += vertexSize;
        }
    }

    visual->UpdateModelBound();
    visual->UpdateModelNormals();
    mPostUpdate(vbuffer);
}
