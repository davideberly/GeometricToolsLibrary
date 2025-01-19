// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLApplicationsPCH.h>
#include <GTL/Applications/Application.h>
using namespace gtl;

Application::Application(Parameters const& parameters)
    :
    mBaseEngine(parameters.engine),
    mProgramFactory(parameters.factory)
{
}
