// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.09.21

#pragma once

#include <GTL/Applications/Window3.h>
using namespace gtl;

class ApproximateCylinder3Window3 : public Window3
{
public:
    ApproximateCylinder3Window3(Parameters& parameters);

    virtual void OnIdle() override;

private:
    bool SetEnvironment();
    void CreateScene();

    std::shared_ptr<Visual> mPoints;
    std::shared_ptr<Visual> mCylinder;
    std::shared_ptr<RasterizerState> mNoCullWireState;
};
