// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLApplicationsPCH.h>
#include <GTL/Applications/ConsoleApplication.h>
using namespace gtl;

ConsoleApplication::Parameters::Parameters()
    :
    title(L""),
    created(false)
{
}

ConsoleApplication::Parameters::Parameters(std::wstring const& inTitle)
    :
    title(inTitle),
    created(false)
{
}

ConsoleApplication::ConsoleApplication(Parameters const& parameters)
    :
    Application(parameters),
    mTitle(parameters.title)
{
}
