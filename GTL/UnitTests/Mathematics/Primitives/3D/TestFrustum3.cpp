#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/3D/Frustum3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestFrustum3
    {
    public:
        UnitTestFrustum3();

    private:
        void Test();
    };
}

UnitTestFrustum3::UnitTestFrustum3()
{
    UTInformation("Mathematics/Primitives/3D/Frustum3");

    Test();
}

void UnitTestFrustum3::Test()
{
    Frustum3<double> frustum0{};
    UTAssert(IsZero(frustum0.origin), "Expecting zero origin.");
    UTAssert(IsZero(frustum0.dVector), "Expecting zero dVector.");
    UTAssert(IsZero(frustum0.uVector), "Expecting zero uVector.");
    UTAssert(IsZero(frustum0.rVector), "Expecting zero rVector.");
    UTAssert(frustum0.dMin == 0.0, "Expecting zero dMin.");
    UTAssert(frustum0.dMax == 0.0, "Expecting zero dMax.");
    UTAssert(frustum0.uBound == 0.0, "Expecting zero uBound.");
    UTAssert(frustum0.rBound == 0.0, "Expecting zero rBound.");
    UTAssert(frustum0.GetDRatio() == 0.0, "Expecting zero mDRatio.");
    UTAssert(frustum0.GetMTwoUF() == 0.0, "Expecting zero mMTwoUF.");
    UTAssert(frustum0.GetMTwoRF() == 0.0, "Expecting zero mMTwoRF.");

    Vector3<double> origin{ 1.0, 1.0, 1.0 };
    Vector3<double> dVector{ 0.0, 0.0, 1.0 };
    Vector3<double> uVector{ 0.0, 1.0, 0.0 };
    Vector3<double> rVector = Cross(dVector, uVector);
    double dMin = 0.5;
    double dMax = 2.0;
    double uBound = 1.5;
    double rBound = 2.0;
    Frustum3<double> frustum1(origin, dVector, uVector, rVector, dMin, dMax, uBound, rBound);
    UTAssert(frustum1.origin == origin, "Incorrect origin.");
    UTAssert(frustum1.dVector == dVector, "Incorrect dVector.");
    UTAssert(frustum1.uVector == uVector, "Incorrect uVector.");
    UTAssert(frustum1.rVector == rVector, "Incorrect rVector.");
    UTAssert(frustum1.dMin == dMin, "Incorrect dMin.");
    UTAssert(frustum1.dMax == dMax, "Incorrect dMax.");
    UTAssert(frustum1.uBound == uBound, "Incorrect uBound.");
    UTAssert(frustum1.rBound == rBound, "Incorrect rBound.");
    UTAssert(frustum1.GetDRatio() == 4.0, "Incorrect mDRatio.");
    UTAssert(frustum1.GetMTwoUF() == -6.0, "Incorrect mMTwoUF.");
    UTAssert(frustum1.GetMTwoRF() == -8.0, "Incorrect mMTwoRF.");

    std::array<Vector3<double>, 8> vertices{};
    frustum1.ComputeVertices(vertices);
    UTAssert((vertices[0] == Vector3<double>{ 3.0, -0.5, 1.5 }), "Invalid vertex 0.");
    UTAssert((vertices[1] == Vector3<double>{ -1.0, -0.5, 1.5 }), "Invalid vertex 1.");
    UTAssert((vertices[2] == Vector3<double>{ -1.0, 2.5, 1.5 }), "Invalid vertex 2.");
    UTAssert((vertices[3] == Vector3<double>{ 3.0, 2.5, 1.5 }), "Invalid vertex 3.");
    UTAssert((vertices[4] == Vector3<double>{ 9.0, -5.0, 3.0 }), "Invalid vertex 4.");
    UTAssert((vertices[5] == Vector3<double>{ -7.0, -5.0, 3.0 }), "Invalid vertex 5.");
    UTAssert((vertices[6] == Vector3<double>{ -7.0, 7.0, 3.0 }), "Invalid vertex 6.");
    UTAssert((vertices[7] == Vector3<double>{ 9.0, 7.0, 3.0 }), "Invalid vertex 7.");

    bool result{};
    result = (frustum0 == frustum1);
    UTAssert(result == false, "operator== failed");
    result = (frustum0 != frustum1);
    UTAssert(result == true, "operator!= failed");
    result = (frustum0 < frustum1);
    UTAssert(result == true, "operator< failed");
    result = (frustum0 <= frustum1);
    UTAssert(result == true, "operator<= failed");
    result = (frustum0 > frustum1);
    UTAssert(result == false, "operator> failed");
    result = (frustum0 >= frustum1);
    UTAssert(result == false, "operator>= failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/3D/Frustum3.h>

namespace gtl
{
    template class Frustum3<float>;
    template bool operator==(Frustum3<float> const&, Frustum3<float> const&);
    template bool operator!=(Frustum3<float> const&, Frustum3<float> const&);
    template bool operator< (Frustum3<float> const&, Frustum3<float> const&);
    template bool operator<=(Frustum3<float> const&, Frustum3<float> const&);
    template bool operator> (Frustum3<float> const&, Frustum3<float> const&);
    template bool operator>=(Frustum3<float> const&, Frustum3<float> const&);

    template class Frustum3<double>;
    template bool operator==(Frustum3<double> const&, Frustum3<double> const&);
    template bool operator!=(Frustum3<double> const&, Frustum3<double> const&);
    template bool operator< (Frustum3<double> const&, Frustum3<double> const&);
    template bool operator<=(Frustum3<double> const&, Frustum3<double> const&);
    template bool operator> (Frustum3<double> const&, Frustum3<double> const&);
    template bool operator>=(Frustum3<double> const&, Frustum3<double> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Frustum3<Rational>;
    template bool operator==(Frustum3<Rational> const&, Frustum3<Rational> const&);
    template bool operator!=(Frustum3<Rational> const&, Frustum3<Rational> const&);
    template bool operator< (Frustum3<Rational> const&, Frustum3<Rational> const&);
    template bool operator<=(Frustum3<Rational> const&, Frustum3<Rational> const&);
    template bool operator> (Frustum3<Rational> const&, Frustum3<Rational> const&);
    template bool operator>=(Frustum3<Rational> const&, Frustum3<Rational> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Frustum3)
