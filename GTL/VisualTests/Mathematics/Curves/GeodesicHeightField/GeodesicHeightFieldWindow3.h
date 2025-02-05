// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Graphics/SceneGraph/Picking/Picker.h>
#include <GTL/Mathematics/Curves/BSplineGeodesic.h>
using namespace gtl;

class GeodesicHeightFieldWindow3 : public Window3
{
public:
    GeodesicHeightFieldWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;
    virtual bool OnMouseClick(MouseButton button, MouseState state,
        int32_t x, int32_t y, uint32_t modifiers) override;

private:
    bool SetEnvironment();
    void CreateScene();

    struct Vertex
    {
        Vector3<float> position, normal;
        Vector2<float> tcoord;
    };

    std::function<void(int32_t, int32_t)> mDrawCallback;

    std::shared_ptr<RasterizerState> mNoCullState, mNoCullWireState;
    std::shared_ptr<Visual> mMesh;
    std::shared_ptr<Texture2> mTexture;
    Vector4<float> mLightWorldDirection;
    Picker mPicker;

    BSplineSurface<double, 3> mSurface;
    std::unique_ptr<BSplineGeodesic<double>> mGeodesic;

    int32_t mSelected;
    std::array<int32_t, 2> mXIntr, mYIntr;
    std::array<Vector<double>, 2> mPoint;
    std::vector<Vector<double>> mPath;
    size_t mPathQuantity;
    double mDistance, mCurvature;

    std::array<float, 4> mTextColor;
};
