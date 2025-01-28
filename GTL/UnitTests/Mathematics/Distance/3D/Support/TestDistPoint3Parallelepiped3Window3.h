#pragma once

#include <Applications/Window3.h>
#include <Mathematics/DistPoint3Parallelepiped3.h>
#include <Graphics/MeshFactory.h>
#include <Graphics/ConstantColorEffect.h>
using namespace gte;

class TestDistPoint3Parallelepiped3Window3 : public Window3
{
public:
    TestDistPoint3Parallelepiped3Window3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    void CreateScene();

    void CreateMeshFace(std::size_t i, MeshFactory& mf,
        std::array<Vector3<float>, 8> const& vertices,
        std::array<std::size_t, 4> const& cornerIndices,
        Vector4<float> const& color);

    void DoQuery();

    using PPQuery = DCPQuery<float, Vector3<float>, Parallelepiped3<float>>;
    using PPResult = PPQuery::Result;
    PPQuery mQuery;
    PPResult mResult;
    Vector3<float> mPoint, mClosest;
    Parallelepiped3<float> mParallelepiped;

    std::shared_ptr<RasterizerState> mNoCullState;
    std::shared_ptr<RasterizerState> mNoCullWireState;
    std::shared_ptr<Visual> mMeshPoint, mMeshClosest, mMeshSegment;

    // Faces of the parallelepiped. Indices
    // 0: -x face
    // 1: +x face
    // 2: -y face
    // 3: +y face
    // 4: -z face
    // 5: +z face
    std::array<std::shared_ptr<Visual>, 6> mMeshFace;

    static float constexpr radius = 3.0f;
    static float constexpr twoPi = static_cast<float>(GTE_C_TWO_PI);
    static float constexpr delta = static_cast<float>(GTE_C_DEG_TO_RAD);
    float mTheta, mPhi;
};
