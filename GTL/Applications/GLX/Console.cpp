// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <Applications/GTApplicationsPCH.h>
#include <Applications/GLX/Console.h>
using namespace gte;

Console::Parameters::Parameters()
    :
    display(nullptr),
    window(0),
    deviceCreationFlags(0)
{
}

Console::Parameters::Parameters(std::wstring const& inTitle)
    :
    ConsoleApplication::Parameters(inTitle),
    display(nullptr),
    window(0),
    deviceCreationFlags(0)
{
}

Console::~Console()
{
}

Console::Console(Parameters& parameters)
    :
    ConsoleApplication(parameters),
    mEngine(std::static_pointer_cast<GraphicsEngine>(mBaseEngine))
{
}
