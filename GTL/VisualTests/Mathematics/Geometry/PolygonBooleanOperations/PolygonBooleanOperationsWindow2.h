// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Applications/Window2.h>
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Geometry/2D/BSPPolygon2.h>
using namespace gtl;

// If Numeric is 'double' or 'float', a very small positive mEpsilon might
// cause BSPPolygon2 function calls to throw exceptions. This is a result
// of floating-point rounding errors. If you use exact arithmetic with
// Numeric set to BSRational<UIntegerAP32>, the results are correct and
// no exceptions are thrown.
//
// WARNING. As the depth of the BSP tree increases, the number of bits
// required for rational arithmetic increases. Eventually, the number of
// bits is so large that the Boolean operations will not complete within
// a reasonable amount of time. TODO: I need to replace BSPPolygon2 by
// a sort-and-sweep approach in order to have better performance when the
// numeric type is rational.

#define USE_RATIONAL_ARITHMETIC
#if defined(USE_RATIONAL_ARITHMETIC)
using Numeric = BSRational<UIntegerAP32>;
#else
using Numeric = double;
#endif

class PolygonBooleanOperationsWindow2 : public Window2
{
public:
    PolygonBooleanOperationsWindow2(Parameters& parameters);

    virtual void OnDisplay() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    std::unique_ptr<BSPPolygon2<Numeric>> ConstructInvertedEll();
    std::unique_ptr<BSPPolygon2<Numeric>> ConstructPentagon();
    std::unique_ptr<BSPPolygon2<Numeric>> ConstructSquare();
    std::unique_ptr<BSPPolygon2<Numeric>> ConstructSShape();
    std::unique_ptr<BSPPolygon2<Numeric>> ConstructPolyWithHoles();

    void DoBoolean();
    void DrawPolySolid(BSPPolygon2<Numeric>& polygon, uint32_t color);

    Numeric mEpsilon;
    BSPPolygon2<Numeric> mIntersection, mUnion, mDiff01, mDiff10, mXor;
    std::unique_ptr<BSPPolygon2<Numeric>> mPoly0;
    std::unique_ptr<BSPPolygon2<Numeric>> mPoly1;
    BSPPolygon2<Numeric>* mActive;
    int32_t mChoice;
    Numeric mSize;
};
