// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.02.05

#pragma once

#include <GTL/Applications/Window2.h>
#include <GTL/Mathematics/Curves/NURBSCircle.h>
using namespace gtl;

class NURBSCircularArcWindow2 : public Window2
{
public:
    NURBSCircularArcWindow2(Parameters& parameters);

    virtual void OnIdle() override;
    virtual void OnDisplay() override;
};
