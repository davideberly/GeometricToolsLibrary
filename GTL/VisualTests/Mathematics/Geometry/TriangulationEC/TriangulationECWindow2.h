// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 8.0.2025.10.06

#pragma once

#include <GTL/Applications/Window2.h>
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Geometry/2D/TriangulateEC.h>
using namespace gtl;

class TriangulationECWindow2 : public Window2
{
public:
    TriangulationECWindow2(Parameters& parameters);

    virtual void OnDisplay() override;
    virtual bool OnCharPress(std::uint8_t key, std::int32_t x, std::int32_t y) override;

private:
    void ClearAll();
    void UnindexedSimplePolygon();  // mExample = 0
    void IndexedSimplePolygon();    // mExample = 1
    void OneNestedPolygon();        // mExample = 2
    void TwoNestedPolygons();       // mExample = 3
    void TreeOfNestedPolygons();    // mExample = 4

    // The inputs (i0,i1,i2) are a permutation of (0,1,2).  The goal is to
    // trap algorithm errors due to order of inner polygons.
    // mExample = 5, 6, 7, 8, 9, 10
    void FourBoxesThreeNested(std::size_t i0, std::size_t i1, std::size_t i2);

    typedef BSRational<UIntegerAP32> Rational;
    typedef TriangulateEC<float, Rational> Triangulator;

    std::vector<Vector2<float>> mPositions;
    std::vector<std::size_t> mOuter, mInner0, mInner1, mInner2;
    std::shared_ptr<PolygonTree> mTree;
    std::vector<Vector2<float>> mFillSeeds;
    std::vector<std::array<std::size_t, 3>> mTriangles;
    std::size_t mExample;
};
