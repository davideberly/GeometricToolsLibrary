// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Algebra/EulerAngles.h>
using namespace gtl;

namespace gtl
{
    class UnitTestEulerAngles
    {
    public:
        UnitTestEulerAngles();

    private:
        void Constructors();
    };
}

UnitTestEulerAngles::UnitTestEulerAngles()
{
    UTInformation("Mathematics/Algebra/EulerAngles");

    Constructors();
}

void UnitTestEulerAngles::Constructors()
{
    EulerAngles<float> eaDefault;
    UTAssert(
        eaDefault.axis[0] == 0 &&
        eaDefault.axis[1] == 0 &&
        eaDefault.axis[2] == 0 &&
        eaDefault.angle[0] == 0.0f &&
        eaDefault.angle[1] == 0.0f &&
        eaDefault.angle[2] == 0.0f &&
        eaDefault.result == EulerAngles<float>::invalid,
        "EulerAngle default constructor failed.");

    EulerAngles<float> ea(1, 2, 0, 0.1f, 0.2f, 0.3f);
    UTAssert(
        ea.axis[0] == 1 &&
        ea.axis[1] == 2 &&
        ea.axis[2] == 0 &&
        ea.angle[0] == 0.1f &&
        ea.angle[1] == 0.2f &&
        ea.angle[2] == 0.3f &&
        ea.result == EulerAngles<float>::unique,
        "EulerAngle constructor failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Algebra/EulerAngles.h>

namespace gtl
{
    template class EulerAngles<float>;
    template class EulerAngles<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class EulerAngles<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(EulerAngles)
