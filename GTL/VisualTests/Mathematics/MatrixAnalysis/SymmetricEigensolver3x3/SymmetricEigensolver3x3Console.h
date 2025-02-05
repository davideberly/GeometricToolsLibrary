// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Applications/Console.h>
using namespace gtl;

class SymmetricEigensolver3x3Console : public Console
{
public:
    SymmetricEigensolver3x3Console(Parameters& parameters);

    virtual void Execute() override;

private:
    static double Determinant(std::array<std::array<double, 3>, 3> const& evec);
};
