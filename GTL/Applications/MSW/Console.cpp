// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLApplicationsPCH.h>
#include <GTL/Applications/MSW/Console.h>
using namespace gtl;

Console::Parameters::Parameters()
    :
    deviceCreationFlags(0)
{
}

Console::Parameters::Parameters(std::wstring const& inTitle)
    :
    ConsoleApplication::Parameters(inTitle),
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
