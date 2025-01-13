// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Algebra/AxisAngle.h>
using namespace gtl;

namespace gtl
{
    class UnitTestAxisAngle
    {
    public:
        UnitTestAxisAngle();

    private:
        void Constructors();
    };
}

UnitTestAxisAngle::UnitTestAxisAngle()
{
    UTInformation("Mathematics/Algebra/AxisAngle");

    Constructors();
}

void UnitTestAxisAngle::Constructors()
{
    AxisAngle<float> aadef;
    Vector<float, 3> vzero;
    UTAssert(aadef.axis == vzero && aadef.angle == 0.0f,
        "Default constructor failed.");

    Vector<float, 3> axis{ 1.0f, 2.0f, 3.0f };
    Normalize(axis);
    float angle = 0.123456f;
    AxisAngle<float> aa(axis, angle);
    UTAssert(
        aa.axis[0] == axis[0] &&
        aa.axis[1] == axis[1] &&
        aa.axis[2] == axis[2] &&
        aa.angle == angle,
        "Element constructor failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Algebra/AxisAngle.h>

namespace gtl
{
    template class AxisAngle<float>;
    template class AxisAngle<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class AxisAngle<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(AxisAngle)
