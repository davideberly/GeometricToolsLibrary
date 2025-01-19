// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <limits>
using namespace gtl;

namespace gtl
{
    class UnitTestVector
    {
    public:
        UnitTestVector();

    private:
        // TN   = Vector<T,N>
        // T    = Vector<T>

        void ConstructorsTN();
        void AccessorsTN();
        void ComparisonsTN();
        void SpecialObjectsTN();
        void UnaryOperationsTN();
        void LinearAlgebraicOperationsTN();
        void ComponentwiseAlgebraicOperationsTN();
        void GeometricOperationsTN();
        void NormalizeRobustTN();

        void ConstructorsT();
        void AccessorsT();
        void ComparisonsT();
        void SpecialObjectsT();
        void UnaryOperationsT();
        void LinearAlgebraicOperationsT();
        void ComponentwiseAlgebraicOperationsT();
        void GeometricOperationsT();
        void NormalizeRobustT();

        template <typename T, std::size_t N>
        void DoComparisons(Vector<T, N> const& v0, Vector<T, N> const& v1, bool equal, bool lessThan)
        {
            bool result = (v0 == v1);
            UTAssert(result == equal, "Incorrect operator==");
            result = (v0 < v1);
            UTAssert(result == lessThan, "Incorrect operator<");
            UTAssert((v0 != v1) == !equal, "Incorrect operator!=");
            UTAssert((v0 <= v1) == equal || lessThan, "Incorrect operator<=");
            UTAssert((v0 > v1) == (!equal && !lessThan), "Incorrect operator>");
            UTAssert((v0 >= v1) == !lessThan, "Incorrect operator>=");
        }

        template <typename T>
        void DoComparisons(Vector<T> const& v0, Vector<T> const& v1, bool equal, bool lessThan)
        {
            bool result = (v0 == v1);
            UTAssert(result == equal, "Incorrect operator==");
            result = (v0 < v1);
            UTAssert(result == lessThan, "Incorrect operator<");
            UTAssert((v0 != v1) == !equal, "Incorrect operator!=");
            UTAssert((v0 <= v1) == equal || lessThan, "Incorrect operator<=");
            UTAssert((v0 > v1) == (!equal && !lessThan), "Incorrect operator>");
            UTAssert((v0 >= v1) == !lessThan, "Incorrect operator>=");
        }

        // Specialized tests for Vector2, Vector3 and Vector4.
        void Geometric2();
        void TestIntrinsics2();
        void Geometric3();
        void TestIntrinsics3();
        void Geometric4();
    };
}

UnitTestVector::UnitTestVector()
{
    UTInformation("Mathematics/Algebra/Vector");

    ConstructorsTN();
    AccessorsTN();
    ComparisonsTN();
    SpecialObjectsTN();
    UnaryOperationsTN();
    LinearAlgebraicOperationsTN();
    ComponentwiseAlgebraicOperationsTN();
    GeometricOperationsTN();
    NormalizeRobustTN();

    ConstructorsT();
    AccessorsT();
    ComparisonsT();
    SpecialObjectsT();
    UnaryOperationsT();
    LinearAlgebraicOperationsT();
    ComponentwiseAlgebraicOperationsT();
    GeometricOperationsT();
    NormalizeRobustT();

    Geometric2();
    TestIntrinsics2();

    Geometric3();
    TestIntrinsics3();

    Geometric4();
}

void UnitTestVector::ConstructorsTN()
{
    // Test the default constructor.
    Vector3<float> vDef;
    UTAssert(vDef[0] == 0.0f && vDef[1] == 0.0f && vDef[2] == 0.0f,
        "Default constructor failed");

    // Test the initializer constructor.
    Vector3<float> vInit{ 1.0f, 2.0f, 3.0f };
    UTAssert(vInit[0] == 1.0f && vInit[1] == 2.0f && vInit[2] == 3.0f,
        "Initializer constructor failed.");

    // Test the std::array constructor.
    std::array<float, 3> arrInput{ 1.0f, 2.0f, 3.0f };
    Vector3<float> vArr(arrInput);
    UTAssert(vArr[0] == 1.0f && vArr[1] == 2.0f && vArr[2] == 3.0f,
        "std::array constructor failed.");

    // Test the std::vector constructor.
    std::vector<float> vecInput{ 1.0f, 2.0f, 3.0f };
    Vector3<float> vVec(vecInput);
    UTAssert(vVec[0] == 1.0f && vVec[1] == 2.0f && vVec[2] == 3.0f,
        "std::vector constructor failed.");

    // Test the copy constructors.
    Vector3<float> vCopy = vInit;
    UTAssert(vCopy[0] == 1.0f && vCopy[1] == 2.0f && vCopy[2] == 3.0f,
        "Copy constructor failed.");
}

void UnitTestVector::AccessorsTN()
{
    Vector3<float> v = { 1.0f, 2.0f, 3.0f };
    std::size_t numElements = v.size();
    UTAssert(numElements == 3, "size() failed.");

    float const* datac = v.data();
    UTAssert(datac[0] == v[0] && datac[1] == v[1] && datac[2] == v[2], "data() const failed.");

    float* data = v.data();
    v[0] = 3.0f;
    v[2] = 1.0f;
    UTAssert(data[0] == v[0] && data[1] == v[1] && data[2] == v[2], "data() or operator[] failed.");
}

void UnitTestVector::ComparisonsTN()
{
    Vector2<float> v0 = { 1.0f, 2.0f };
    Vector2<float> v1 = { 3.0f, 4.0f };
    Vector2<float> v2 = { 2.0f, 5.0f };

    DoComparisons(v0, v0, true, false);
    DoComparisons(v0, v1, false, true);
    DoComparisons(v1, v2, false, false);
}

void UnitTestVector::SpecialObjectsTN()
{
    Vector2<float> v = { 1.0f, 2.0f };

    v.fill(3.0f);
    UTAssert(v[0] == 3.0f && v[1] == 3.0f, "Fill failed.");

    MakeZero(v);
    UTAssert(v[0] == 0.0f && v[1] == 0.0f, "MakeZero failed.");
    UTAssert(IsZero(v), "IsZero failed.");

    MakeUnit(0, v);
    UTAssert(v[0] == 1.0f && v[1] == 0.0f, "MakeUnit failed.");
    UTAssert(IsUnit(0, v), "IsUnit failed.");
    MakeUnit(1, v);
    UTAssert(v[0] == 0.0f && v[1] == 1.0f, "MakeUnit failed.");
    UTAssert(IsUnit(1, v), "IsUnit failed.");
}

void UnitTestVector::UnaryOperationsTN()
{
    Vector2<float> v = { 1.0f, 2.0f };

    v = +v;
    UTAssert(v[0] == 1.0f && v[1] == 2.0f, "unary positive failed");

    v = -v;
    UTAssert(v[0] == -1.0f && v[1] == -2.0f, "unary negation failed");
}

void UnitTestVector::LinearAlgebraicOperationsTN()
{
    Vector2<float> v0 = { 1.0f, 2.0f };
    Vector2<float> v1 = { 3.0f, 4.0f };
    Vector2<float> v2;

    v2 = v0 + v1;
    UTAssert(v2[0] == 4.0f && v2[1] == 6.0f, "operator+ failed");

    v2 = v0 - v1;
    UTAssert(v2[0] == -2.0f && v2[1] == -2.0f, "operator+ failed");

    v2 = 2.0f * v0;
    UTAssert(v2[0] == 2.0f && v2[1] == 4.0f, "operator+ failed");

    v2 = v0 / 2.0f;
    UTAssert(v2[0] == 0.5f && v2[1] == 1.0f, "operator+ failed");
}

void UnitTestVector::ComponentwiseAlgebraicOperationsTN()
{
    Vector2<float> v0 = { 1.0f, 2.0f };
    Vector2<float> v1 = { 3.0f, 4.0f };
    Vector2<float> v2;

    v2 = v0 * v1;
    UTAssert(v2[0] == 3.0f && v2[1] == 8.0f, "operator+ failed");

    float const oneThird = 1.0f / 3.0f;
    v2 = v0 / v1;
    UTAssert(v2[0] == oneThird && v2[1] == 0.5f, "operator+ failed");
}

void UnitTestVector::GeometricOperationsTN()
{
    Vector2<float> v0 = { 1.0f, 2.0f };
    Vector2<float> v1 = { 3.0f, 4.0f };
    Vector2<float> v2 = { 5.0f, 6.0f };

    float temp = Dot(v0, v1);
    UTAssert(temp == 11.0f, "Dot failed");

    float const sqrt5 = std::sqrt(5.0f);
    temp = Length(v0);
    UTAssert(temp == sqrt5, "Length failed");

    float const sqrt61 = std::sqrt(61.0f);
    temp = Normalize(v2);
    UTAssert(temp == sqrt61 && v2[0] == 5.0f / sqrt61 && v2[1] == 6.0f / sqrt61, "Normalize failed");
    v2[0] = std::numeric_limits<float>::min();
    v2[1] = std::numeric_limits<float>::min();
    temp = Normalize(v2);
    UTAssert(temp == 0.0f && v2[0] == 0.0f && v2[1] == 0.0f, "Normalize failed");

    std::vector<Vector3<float>> vbasis;
    float const epsilon = 1e-06f;

    vbasis.resize(1);
    vbasis[0] = Vector3<float>{ 1.0f, 2.0f, 0.0f };
    temp = Orthonormalize(vbasis);
    UTAssert(temp == sqrt5
        && vbasis[0][0] == 1.0f / sqrt5
        && vbasis[0][1] == 2.0f / sqrt5
        && vbasis[0][2] == 0.0f, "Orthonormalize failed");

    vbasis.resize(2);
    vbasis[0] = { 1.0f, 2.0f, 0.0f };
    vbasis[1] = { 1.0f, 1.0f, 1.0f };
    temp = Orthonormalize(vbasis);
    Vector3<float> diff0 = vbasis[0] - Vector3<float>{0.447213590f, 0.894427180f, 0.0f};
    Vector3<float> diff1 = vbasis[1] - Vector3<float>{0.365148395f, -0.182574108f, 0.912870884f};
    UTAssert(std::fabs(temp - 1.09544516)
        && diff0[0] <= epsilon && diff0[1] <= epsilon && diff0[2] <= epsilon
        && diff1[0] <= epsilon && diff1[1] <= epsilon && diff1[2] <= epsilon,
        "Orthonormalize failed");
    temp = Dot(vbasis[0], vbasis[1]);
    UTAssert(std::fabs(temp) <= epsilon, "Orthonormalize output wrong");

    vbasis.resize(3);
    vbasis[0] = { 1.0f, 2.0f, 0.0f };
    vbasis[1] = { 1.0f, 1.0f, 1.0f };
    vbasis[2] = { 0.0f, 0.0f, 1.0f };
    temp = Orthonormalize(vbasis);
    diff0 = vbasis[0] - Vector3<float>{0.447213590f, 0.894427180f, 0.0f};
    diff1 = vbasis[1] - Vector3<float>{0.365148395f, -0.182574108f, 0.912870884f};
    Vector3<float> diff2 = vbasis[2] - Vector3<float>{-0.816496551f, 0.408248067f, 0.408248484f};
    UTAssert(std::fabs(temp - 0.408248305)
        && diff0[0] <= epsilon && diff0[1] <= epsilon && diff0[2] <= epsilon
        && diff1[0] <= epsilon && diff1[1] <= epsilon && diff1[2] <= epsilon,
        "Orthonormalize failed");
    temp = Dot(vbasis[0], vbasis[1]);
    UTAssert(std::fabs(temp) <= epsilon, "Orthonormalize output wrong");
    temp = Dot(vbasis[0], vbasis[2]);
    UTAssert(std::fabs(temp) <= epsilon, "Orthonormalize output wrong");
    temp = Dot(vbasis[1], vbasis[2]);
    UTAssert(std::fabs(temp) <= epsilon, "Orthonormalize output wrong");

    vbasis[0] = { 4.0f, 3.0f, -1.0f };
    vbasis[1] = GetOrthogonal(vbasis[0], true);
    UTAssert(std::fabs(vbasis[1][0] - 0.6f) <= epsilon
        && std::fabs(vbasis[1][1] + 0.8f) <= epsilon
        && vbasis[1][2] == 0.0f,
        "GetOrthogonal failed");

    vbasis[0] = { 1.0f, 2.0f, 0.0f };
    vbasis[1] = { 1.0f, 1.0f, 1.0f };
    vbasis[2] = { 0.0f, 3.0f, 1.0f };
    temp = Orthonormalize(vbasis);
    auto vextreme = ComputeExtremes(vbasis);
    diff0 = vextreme.first - Vector3<float>{-0.816496551f, -0.182574108f, 0.0f};
    diff1 = vextreme.second - Vector3<float>{0.447213590f, 0.894427180f, 0.912870884f};
    UTAssert(
        diff0[0] <= epsilon && diff0[1] <= epsilon && diff0[2] <= epsilon &&
        diff1[0] <= epsilon && diff1[1] <= epsilon && diff1[2] <= epsilon,
        "ComputeExtremes failed");

    vbasis[0] = { 1.0f, 2.0f, 3.0f };

    Vector4<float> lift = HLift(vbasis[0], 1.0f);
    UTAssert(lift[0] == 1.0f && lift[1] == 2.0f && lift[2] == 3.0f
        && lift[3] == 1.0f,
        "HLift failed");

    Vector2<float> proj = HProject(vbasis[0]);
    UTAssert(proj[0] == 1.0f && proj[1] == 2.0f, "HProject failed");

    lift = Lift(vbasis[0], 1, 4.0f);
    UTAssert(lift[0] == 1.0f && lift[1] == 4.0f && lift[2] == 2.0f
        && lift[3] == 3.0f,
        "Lift failed");

    proj = Project(vbasis[0], 1);
    UTAssert(proj[0] == 1.0f && proj[1] == 3.0f, "Project failed");
}

void UnitTestVector::NormalizeRobustTN()
{
    Vector2<float> v = { 5.0f, 6.0f };

    float const sqrt61 = std::sqrt(61.0f);
    float length = Normalize(v);
    UTAssert(length == sqrt61 &&
        v[0] == 5.0f / sqrt61 && v[1] == 6.0f / sqrt61,
        "Normalize failed");

    v[0] = std::numeric_limits<float>::min();
    v[1] = std::numeric_limits<float>::min();
    length = Normalize(v);
    UTAssert(length == 0.0f &&
        v[0] == 0.0f && v[1] == 0.0f,
        "Normalize failed");

    v[0] = std::ldexp(1.0f, -149);
    v[1] = 0.0f;
    length = Normalize(v);
    UTAssert(length == 0.0f &&
        v[0] == 0.0f && v[1] == 0.0f,
        "Normalize failed");

    float const minSubnormal = std::ldexp(1.0f, -149);
    v[0] = minSubnormal;
    v[1] = 0.0f;
    length = NormalizeRobust(v);
    UTAssert(length == minSubnormal &&
        v[0] == 1.0f && v[1] == 0.0f,
        "NormalizeRobust failed");

    v[0] = minSubnormal;
    v[1] = -2.0f * minSubnormal;
    length = NormalizeRobust(v);
    UTAssert(length == 2.0f * minSubnormal &&
        v[0] == 0.447213590f && v[1] == -0.894427180f,
        "NormalizeRobust failed");
    // Theoretical length is sqrt(5)/2 * 2^{-148} = 1.118033 * 2^{-148}
    // which rounds to 2^{-128} = 2 * minSubnormal.
}

void UnitTestVector::ConstructorsT()
{
    // Test the default constructor.
    Vector<float> vDef0;
    UTAssert(
        vDef0.size() == 0,
        "Default constructor failed.");

    Vector<float> vDef1(3);
    UTAssert(
        vDef1.size() == 3 &&
        vDef1[0] == 0.0f && vDef1[1] == 0.0f && vDef1[2] == 0.0f,
        "Default constructor failed.");

    // Test the initializer constructor.
    Vector<float> vInit{ 1.0f, 2.0f, 3.0f };
    UTAssert(
        vInit.size() == 3 &&
        vInit[0] == 1.0f && vInit[1] == 2.0f && vInit[2] == 3.0f,
        "Initializer constructor failed.");

    // Test the std::array constructor.
    std::array<float, 3> arrInput{ 1.0f, 2.0f, 3.0f };
    Vector<float> vArr(arrInput);
    UTAssert(
        vArr.size() == 3 &&
        vArr[0] == 1.0f && vArr[1] == 2.0f && vArr[2] == 3.0f,
        "std::array constructor failed.");

    // Test the std::vector constructor.
    std::vector<float> vecInput{ 1.0f, 2.0f, 3.0f };
    Vector<float> vVec(vecInput);
    UTAssert(
        vVec.size() == 3 &&
        vVec[0] == 1.0f && vVec[1] == 2.0f && vVec[2] == 3.0f,
        "std::vector constructor failed.");

    // Test the copy constructors.
    Vector<float> vCopy = vInit;
    UTAssert(
        vCopy.size() == 3 &&
        vCopy[0] == 1.0f && vCopy[1] == 2.0f && vCopy[2] == 3.0f,
        "Copy constructor failed.");
}

void UnitTestVector::AccessorsT()
{
    Vector<float> v = { 1.0f, 2.0f, 3.0f };
    std::size_t numElements = v.size();
    UTAssert(numElements == 3, "GetNumElements failed.");

    float const* datac = v.data();
    UTAssert(datac[0] == v[0] && datac[1] == v[1] && datac[2] == v[2], "size() const failed.");

    float* data = v.data();
    v[0] = 3.0f;
    v[2] = 1.0f;
    UTAssert(data[0] == v[0] && data[1] == v[1] && data[2] == v[2], "size() or operator[] failed.");
}

void UnitTestVector::ComparisonsT()
{
    Vector<float> v0 = { 1.0f, 2.0f };
    Vector<float> v1 = { 3.0f, 4.0f };
    Vector<float> v2 = { 2.0f, 5.0f };

    DoComparisons(v0, v0, true, false);
    DoComparisons(v0, v1, false, true);
    DoComparisons(v1, v2, false, false);
}

void UnitTestVector::SpecialObjectsT()
{
    Vector<float> v = { 1.0f, 2.0f };

    v.fill(3.0f);
    UTAssert(v[0] == 3.0f && v[1] == 3.0f, "Fill failed.");

    MakeZero(v);
    UTAssert(v[0] == 0.0f && v[1] == 0.0f, "MakeZero failed.");
    UTAssert(IsZero(v), "IsZero failed.");

    MakeUnit(0, v);
    UTAssert(v[0] == 1.0f && v[1] == 0.0f, "MakeUnit failed.");
    UTAssert(IsUnit(0, v), "IsUnit failed.");
    MakeUnit(1, v);
    UTAssert(v[0] == 0.0f && v[1] == 1.0f, "MakeUnit failed.");
    UTAssert(IsUnit(1, v), "IsUnit failed.");
}

void UnitTestVector::UnaryOperationsT()
{
    Vector<float> v = { 1.0f, 2.0f };

    v = +v;
    UTAssert(v[0] == 1.0f && v[1] == 2.0f, "unary positive failed");

    v = -v;
    UTAssert(v[0] == -1.0f && v[1] == -2.0f, "unary negation failed");
}

void UnitTestVector::LinearAlgebraicOperationsT()
{
    Vector<float> v0 = { 1.0f, 2.0f };
    Vector<float> v1 = { 3.0f, 4.0f };
    Vector<float> v2;

    v2 = v0 + v1;
    UTAssert(v2[0] == 4.0f && v2[1] == 6.0f, "operator+ failed");

    v2 = v0 - v1;
    UTAssert(v2[0] == -2.0f && v2[1] == -2.0f, "operator+ failed");

    v2 = 2.0f * v0;
    UTAssert(v2[0] == 2.0f && v2[1] == 4.0f, "operator+ failed");

    v2 = v0 / 2.0f;
    UTAssert(v2[0] == 0.5f && v2[1] == 1.0f, "operator+ failed");
}

void UnitTestVector::ComponentwiseAlgebraicOperationsT()
{
    Vector<float> v0 = { 1.0f, 2.0f };
    Vector<float> v1 = { 3.0f, 4.0f };
    Vector<float> v2;

    v2 = v0 * v1;
    UTAssert(v2[0] == 3.0f && v2[1] == 8.0f, "operator+ failed");

    float const oneThird = 1.0f / 3.0f;
    v2 = v0 / v1;
    UTAssert(v2[0] == oneThird && v2[1] == 0.5f, "operator+ failed");
}

void UnitTestVector::GeometricOperationsT()
{
    Vector<float> v0 = { 1.0f, 2.0f };
    Vector<float> v1 = { 3.0f, 4.0f };
    Vector<float> v2 = { 5.0f, 6.0f };

    float temp = Dot(v0, v1);
    UTAssert(temp == 11.0f, "Dot failed");

    float const sqrt5 = std::sqrt(5.0f);
    temp = Length(v0);
    UTAssert(temp == sqrt5, "Length failed");

    float const sqrt61 = std::sqrt(61.0f);
    temp = Normalize(v2);
    UTAssert(temp == sqrt61 && v2[0] == 5.0f / sqrt61 && v2[1] == 6.0f / sqrt61, "Normalize failed");
    v2[0] = std::numeric_limits<float>::min();
    v2[1] = std::numeric_limits<float>::min();
    temp = Normalize(v2);
    UTAssert(temp == 0.0f && v2[0] == 0.0f && v2[1] == 0.0f, "Normalize failed");

    std::vector<Vector<float>> vbasis;
    float const epsilon = 1e-06f;

    vbasis.resize(1);
    vbasis[0] = { 1.0f, 2.0f, 0.0f };
    temp = Orthonormalize(vbasis);
    UTAssert(temp == sqrt5
        && vbasis[0][0] == 1.0f / sqrt5
        && vbasis[0][1] == 2.0f / sqrt5
        && vbasis[0][2] == 0.0f, "Orthonormalize failed");

    vbasis.resize(2);
    vbasis[0] = { 1.0f, 2.0f, 0.0f };
    vbasis[1] = { 1.0f, 1.0f, 1.0f };
    temp = Orthonormalize(vbasis);
    Vector<float> diff0 = vbasis[0] - Vector<float>{0.447213590f, 0.894427180f, 0.0f};
    Vector<float> diff1 = vbasis[1] - Vector<float>{0.365148395f, -0.182574108f, 0.912870884f};
    UTAssert(std::fabs(temp - 1.09544516)
        && diff0[0] <= epsilon && diff0[1] <= epsilon && diff0[2] <= epsilon
        && diff1[0] <= epsilon && diff1[1] <= epsilon && diff1[2] <= epsilon,
        "Orthonormalize failed");
    temp = Dot(vbasis[0], vbasis[1]);
    UTAssert(std::fabs(temp) <= epsilon, "Orthonormalize output wrong");

    vbasis.resize(3);
    vbasis[0] = { 1.0f, 2.0f, 0.0f };
    vbasis[1] = { 1.0f, 1.0f, 1.0f };
    vbasis[2] = { 0.0f, 0.0f, 1.0f };
    temp = Orthonormalize(vbasis);
    diff0 = vbasis[0] - Vector<float>{0.447213590f, 0.894427180f, 0.0f};
    diff1 = vbasis[1] - Vector<float>{0.365148395f, -0.182574108f, 0.912870884f};
    Vector<float> diff2 = vbasis[2] - Vector<float>{-0.816496551f, 0.408248067f, 0.408248484f};
    UTAssert(std::fabs(temp - 0.408248305)
        && diff0[0] <= epsilon && diff0[1] <= epsilon && diff0[2] <= epsilon
        && diff1[0] <= epsilon && diff1[1] <= epsilon && diff1[2] <= epsilon,
        "Orthonormalize failed");
    temp = Dot(vbasis[0], vbasis[1]);
    UTAssert(std::fabs(temp) <= epsilon, "Orthonormalize output wrong");
    temp = Dot(vbasis[0], vbasis[2]);
    UTAssert(std::fabs(temp) <= epsilon, "Orthonormalize output wrong");
    temp = Dot(vbasis[1], vbasis[2]);
    UTAssert(std::fabs(temp) <= epsilon, "Orthonormalize output wrong");

    vbasis[0] = { 4.0f, 3.0f, -1.0f };
    vbasis[1] = GetOrthogonal(vbasis[0], true);
    UTAssert(std::fabs(vbasis[1][0] - 0.6f) <= epsilon
        && std::fabs(vbasis[1][1] + 0.8f) <= epsilon
        && vbasis[1][2] == 0.0f,
        "GetOrthogonal failed");

    vbasis[0] = { 1.0f, 2.0f, 0.0f };
    vbasis[1] = { 1.0f, 1.0f, 1.0f };
    vbasis[2] = { 0.0f, 3.0f, 1.0f };
    temp = Orthonormalize(vbasis);
    auto vextreme = ComputeExtremes(vbasis);
    diff0 = vextreme.first - Vector<float>{-0.816496551f, -0.182574108f, 0.0f};
    diff1 = vextreme.second - Vector<float>{0.447213590f, 0.894427180f, 0.912870884f};
    UTAssert(
        diff0[0] <= epsilon && diff0[1] <= epsilon && diff0[2] <= epsilon &&
        diff1[0] <= epsilon && diff1[1] <= epsilon && diff1[2] <= epsilon,
        "ComputeExtremes failed");

    vbasis[0] = { 1.0f, 2.0f, 3.0f };

    Vector<float> lift = HLift(vbasis[0], 1.0f);
    UTAssert(lift[0] == 1.0f && lift[1] == 2.0f && lift[2] == 3.0f
        && lift[3] == 1.0f,
        "HLift failed");

    Vector<float> proj = HProject(vbasis[0]);
    UTAssert(proj[0] == 1.0f && proj[1] == 2.0f, "HProject failed");

    lift = Lift(vbasis[0], 1, 4.0f);
    UTAssert(lift[0] == 1.0f && lift[1] == 4.0f && lift[2] == 2.0f
        && lift[3] == 3.0f,
        "Lift failed");

    proj = Project(vbasis[0], 1);
    UTAssert(proj[0] == 1.0f && proj[1] == 3.0f, "Project failed");
}

void UnitTestVector::NormalizeRobustT()
{
    Vector<float> v = { 5.0f, 6.0f };

    float const sqrt61 = std::sqrt(61.0f);
    float length = Normalize(v);
    UTAssert(length == sqrt61 &&
        v[0] == 5.0f / sqrt61 && v[1] == 6.0f / sqrt61,
        "Normalize failed");

    v[0] = std::numeric_limits<float>::min();
    v[1] = std::numeric_limits<float>::min();
    length = Normalize(v);
    UTAssert(length == 0.0f &&
        v[0] == 0.0f && v[1] == 0.0f,
        "Normalize failed");

    v[0] = std::ldexp(1.0f, -149);
    v[1] = 0.0f;
    length = Normalize(v);
    UTAssert(length == 0.0f &&
        v[0] == 0.0f && v[1] == 0.0f,
        "Normalize failed");

    float const minSubnormal = std::ldexp(1.0f, -149);
    v[0] = minSubnormal;
    v[1] = 0.0f;
    length = NormalizeRobust(v);
    UTAssert(length == minSubnormal &&
        v[0] == 1.0f && v[1] == 0.0f,
        "NormalizeRobust failed");

    v[0] = minSubnormal;
    v[1] = -2.0f * minSubnormal;
    length = NormalizeRobust(v);
    UTAssert(length == 2.0f * minSubnormal &&
        v[0] == 0.447213590f && v[1] == -0.894427180f,
        "NormalizeRobust failed");
    // Theoretical length is sqrt(5)/2 * 2^{-148} = 1.118033 * 2^{-148}
    // which rounds to 2^{-128} = 2 * minSubnormal.
}

void UnitTestVector::Geometric2()
{
    float const epsilon = 1e-06f;
    Vector2<float> v0{ 1.0f, 2.0f };
    Vector2<float> v1{ 3.0f, 4.0f };
    Vector2<float> v2{}, expV0{}, expV1{}, expV2{};
    float temp{}, length{};

    v2 = Perp(v0);
    expV2 = { 2.0f, -1.0f };
    UTAssert(v2 == expV2, "Perp failed");

    float const sqrt5 = std::sqrt(5.0f);
    v2 = UnitPerp(v0);
    expV2 = { 2.0f / sqrt5, -1.0f / sqrt5 };
    length = Length(v2 - expV2);
    UTAssert(length <= epsilon, "UnitPerp failed");

    temp = std::fabs(DotPerp(v0, v1) + 2.0f);
    UTAssert(temp <= epsilon, "DotPerp failed");

    bool success = ComputeOrthonormalBasis(v0, v1);
    expV0 = { 1.0f / sqrt5, 2.0f / sqrt5 };
    expV1 = { -2.0f / sqrt5, 1.0f / sqrt5 };
    UTAssert(
        success &&
        Length(v0 - expV0) <= epsilon &&
        Length(v1 - expV1) <= epsilon, "ComputeOrthonormalBasis failed");
    temp = std::fabs(DotPerp(v0, v1) - 1.0f);
    UTAssert(temp <= epsilon, "ComputeOrthogonalComplement failed");

    std::array<Vector2<float>, 3> v{};
    Vector2<float> p{};
    Vector3<float> pBary{}, expBary{};
    std::array<float, 3> bary{};
    v[0] = { 1.0f, 2.0f };
    v[1] = { 3.0f, 5.0f };
    v[2] = { 0.0f, 10.0f };
    p = { 1.0f, 2.5f };
    success = ComputeBarycentrics(p, v[0], v[1], v[2], bary);
    pBary = bary;
    expBary = { 0.921052635f, 0.0263157897f, 0.0526315756f };
    UTAssert(success && Length(pBary - expBary) <= epsilon, "ComputeBarycentrics failed");
    Vector2<float> q = bary[0] * v[0] + bary[1] * v[1] + bary[2] * v[2];
    float diffQ0P0 = std::fabs(q[0] - p[0]);
    float diffQ1P1 = std::fabs(q[1] - p[1]);
    UTAssert(diffQ0P0 <= epsilon && diffQ1P1 <= epsilon, "ComputeBarycentrics failed");
}

void UnitTestVector::TestIntrinsics2()
{
    float const epsilon = 1e-06f;
    std::vector<Vector2<float>> v(4);

    v[0] = { 0.0f, 0.0f };
    v[1] = { 1.0f, 0.0f };
    v[2] = { 2.0f, 0.0f };
    v[3] = { 1.5f, 1e-07f };
    Intrinsics2<float> intrinsics1;
    intrinsics1(v, epsilon);
    UTAssert(
        intrinsics1.dimension == 1 &&
        intrinsics1.min[0] == 0.0f &&
        intrinsics1.min[1] == 0.0f &&
        intrinsics1.max[0] == 2.0f &&
        intrinsics1.max[1] == 1e-07f &&
        intrinsics1.maxRange == 2.0f &&
        intrinsics1.origin[0] == 0.0f &&
        intrinsics1.origin[1] == 0.0f &&
        intrinsics1.direction[0][0] == 1.0f &&
        intrinsics1.direction[0][1] == 0.0f &&
        intrinsics1.direction[1][0] == 0.0f &&
        intrinsics1.direction[1][1] == 1.0f &&
        intrinsics1.extreme[0] == 0 &&
        intrinsics1.extreme[1] == 2 &&
        intrinsics1.extreme[2] == 2 &&
        intrinsics1.extremeCCW == false,
        "GetIntrinsics failed");

    v[0] = { 0.0f, 0.0f };
    v[1] = { 1.0f, 0.0f };
    v[2] = { 2.0f, 0.0f };
    v[3] = { 1.5f, 1e-05f };
    Intrinsics2<float> intrinsics2;
    intrinsics2(v, epsilon);
    UTAssert(
        intrinsics2.dimension == 2 &&
        intrinsics2.min[0] == 0.0f &&
        intrinsics2.min[1] == 0.0f &&
        intrinsics2.max[0] == 2.0f &&
        intrinsics2.max[1] == 1e-05f &&
        intrinsics2.maxRange == 2.0f &&
        intrinsics2.origin[0] == 0.0f &&
        intrinsics2.origin[1] == 0.0f &&
        intrinsics2.direction[0][0] == 1.0f &&
        intrinsics2.direction[0][1] == 0.0f &&
        intrinsics2.direction[1][0] == 0.0f &&
        intrinsics2.direction[1][1] == 1.0f &&
        intrinsics2.extreme[0] == 0 &&
        intrinsics2.extreme[1] == 2 &&
        intrinsics2.extreme[2] == 3 &&
        intrinsics2.extremeCCW == true,
        "GetIntrinsics failed");
}

void UnitTestVector::Geometric3()
{
    float const epsilon = 1e-06f;
    Vector3<float> v0{ 1.0f, 2.0f, 3.0f };
    Vector3<float> v1{ 4.0f, 5.0f, 6.0f };
    Vector3<float> v2{}, expV0{}, expV1{}, expV2{}, diff{};
    float temp{}, length{};
    bool success{};

    v2 = Cross(v0, v1);
    expV2 = { -3.0f, 6.0f, -3.0f };
    UTAssert(v2 == expV2, "Cross failed");
    temp = std::fabs(Dot(v0, v2));
    UTAssert(temp <= epsilon, "UnitCross failed");
    temp = std::fabs(Dot(v1, v2));
    UTAssert(temp <= epsilon, "UnitCross failed");

    v2 = UnitCross(v0, v1);
    expV2 = { -0.408248276f, 0.816496551f, -0.408248276f };
    UTAssert(Length(v2 - expV2) <= epsilon, "UnitCross failed");
    temp = std::fabs(Dot(v0, v2));
    UTAssert(temp <= epsilon, "UnitCross failed");
    temp = std::fabs(Dot(v1, v2));
    UTAssert(temp <= epsilon, "UnitCross failed");

    temp = DotCross(v0, v1, v2);
    length = std::fabs(temp - 7.34846878f);
    UTAssert(length <= epsilon, "DotCross failed");

    // ComputeOrthonormalBasis with 1 input
    v0 = { 1.0f, 2.0f, 3.0f };
    success = ComputeOrthonormalBasis(1, v0, v1, v2);
    UTAssert(success, "ComputeOrthonormalBasis failed");
    expV0 = { 0.267261237f, 0.534522474f, 0.801783681f };
    expV1 = { 0.0f, 0.832050264f, -0.554700196f };
    expV2 = { -0.963624120f, 0.148249879f, 0.222374797f };
    UTAssert(
        Length(v0 - expV0) <= epsilon &&
        Length(v1 - expV1) <= epsilon &&
        Length(v2 - expV2) <= epsilon,
        "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v0) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v1) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v2, v2) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v1)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(DotCross(v0, v1, v2) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");

    // ComputeOrthonormalBasis with 2 inputs
    v0 = { 1.0f, 2.0f, 3.0f };
    v1 = { 4.0f, 5.0f, 6.0f };
    success = ComputeOrthonormalBasis(2, v0, v1, v2);
    UTAssert(success, "ComputeOrthonormalBasis failed");
    expV0 = { 0.267261237f, 0.534522474f, 0.801783681f };
    expV1 = { 0.872871637f, 0.218218029f, -0.436435580f };
    expV2 = { -0.408248305f, 0.816496611f, -0.408248335f };
    UTAssert(
        Length(v0 - expV0) <= epsilon &&
        Length(v1 - expV1) <= epsilon &&
        Length(v2 - expV2) <= epsilon,
        "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v0) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v1) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v2, v2) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v1)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(DotCross(v0, v1, v2) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");

    // ComputeOrthonormalBasis with 2 inputs
    v0 = { 1.0f, 2.0f, 3.0f };
    v1 = { 4.0f, 5.0f, 6.0f };
    v2 = { 7.0f, 8.0f, 9.0f };
    success = ComputeOrthonormalBasis(2, v0, v1, v2);
    UTAssert(success, "ComputeOrthonormalBasis failed");
    expV0 = { 0.267261237f, 0.534522474f, 0.801783681f };
    expV1 = { 0.872871637f, 0.218218029f, -0.436435580f };
    expV2 = { -0.408248305f, 0.816496611f, -0.408248335f };
    UTAssert(
        Length(v0 - expV0) <= epsilon &&
        Length(v1 - expV1) <= epsilon &&
        Length(v2 - expV2) <= epsilon,
        "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v0) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v1) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v2, v2) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v1)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(DotCross(v0, v1, v2) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");

    Vector3<float> v[4], p;
    std::array<float, 4> bary;
    v[0] = { 1.0f, 2.0f, 0.0f };
    v[1] = { 3.0f, 5.0f, 0.0f };
    v[2] = { 0.0f, 10.0f, 0.0f };
    v[3] = { 0.0f, 0.0f, 1.0f };
    p = { 1.0f, 2.5f, 0.1f };
    success = ComputeBarycentrics(p, v[0], v[1], v[2], v[3], bary);
    UTAssert(success, "ComputeBarycentrics failed");
    std::array<float, 4> bdiff =
    {
        std::fabs(bary[0] - 0.763157904f),
        std::fabs(bary[1] - 0.0789473653f),
        std::fabs(bary[2] - 0.0578947403f),
        std::fabs(bary[3] - 0.0999999940f)
    };
    UTAssert(bdiff[0] <= epsilon && bdiff[1] <= epsilon
        && bdiff[2] <= epsilon && bdiff[3] <= epsilon,
        "ComputeBarycentrics failed");
    Vector3<float> q = bary[0] * v[0] + bary[1] * v[1] + bary[2] * v[2] + bary[3] * v[3];
    bdiff[0] = std::fabs(q[0] - p[0]);
    bdiff[1] = std::fabs(q[1] - p[1]);
    bdiff[2] = std::fabs(q[2] - p[2]);
    UTAssert(bdiff[0] <= epsilon && bdiff[1] <= epsilon && bdiff[2] <= epsilon,
        "ComputeBarycentrics failed");
}

void UnitTestVector::TestIntrinsics3()
{
    float const epsilon = 1e-06f;
    std::vector<Vector3<float>> v(5);

    v[0] = { 0.0f, 0.0f, 0.0f };
    v[1] = { 1.0f, 0.0f, 0.0f };
    v[2] = { 2.0f, 0.0f, 0.0f };
    v[3] = { 1.5f, 1e-07f, 0.0f };
    v[4] = { 0.5f, -1e-07f, 0.0f };
    Intrinsics3<float> intrinsics1;
    intrinsics1(v, epsilon);
    UTAssert(
        intrinsics1.dimension == 1 &&
        intrinsics1.min[0] == 0.0f &&
        intrinsics1.min[1] == -1e-07f &&
        intrinsics1.max[0] == 2.0f &&
        intrinsics1.max[1] == 1e-07f &&
        intrinsics1.maxRange == 2.0f &&
        intrinsics1.origin[0] == 0.0f &&
        intrinsics1.origin[1] == 0.0f &&
        intrinsics1.origin[2] == 0.0f &&
        intrinsics1.direction[0][0] == 1.0f &&
        intrinsics1.direction[0][1] == 0.0f &&
        intrinsics1.direction[0][2] == 0.0f &&
        intrinsics1.direction[1][0] == 0.0f &&
        intrinsics1.direction[1][1] == 0.0f &&
        intrinsics1.direction[1][2] == 1.0f &&
        intrinsics1.direction[2][0] == 0.0f &&
        intrinsics1.direction[2][1] == -1.0f &&
        intrinsics1.direction[2][2] == 0.0f &&
        intrinsics1.extreme[0] == 0 &&
        intrinsics1.extreme[1] == 2 &&
        intrinsics1.extreme[2] == 2 &&
        intrinsics1.extreme[3] == 2 &&
        intrinsics1.extremeCCW == false,
        "GetIntrinsics failed");

    v[0] = { 0.0f, 0.0f, 0.0f };
    v[1] = { 1.0f, 0.0f, 0.0f };
    v[2] = { 1.0f, 1.0f, 0.0f };
    v[3] = { 1e-07f, 2e-07f, 1e-07f };
    v[4] = { 2e-07f, 1e-07f, -1e-07f };
    Intrinsics3<float> intrinsics2;
    intrinsics2(v, epsilon);
    UTAssert(
        intrinsics2.dimension == 2 &&
        intrinsics2.min[0] == 0.0f &&
        intrinsics2.min[1] == 0.0f &&
        intrinsics2.min[2] == -1e-07f &&
        intrinsics2.max[0] == 1.0f &&
        intrinsics2.max[1] == 1.0f &&
        intrinsics2.max[2] == 1e-07f &&
        intrinsics2.maxRange == 1.0f &&
        intrinsics2.origin[0] == 0.0f &&
        intrinsics2.origin[1] == 0.0f &&
        intrinsics2.origin[2] == 0.0f &&
        intrinsics2.direction[0][0] == 1.0f &&
        intrinsics2.direction[0][1] == 0.0f &&
        intrinsics2.direction[0][2] == 0.0f &&
        intrinsics2.direction[1][0] == 0.0f &&
        intrinsics2.direction[1][1] == 1.0f &&
        intrinsics2.direction[1][2] == 0.0f &&
        intrinsics2.direction[2][0] == 0.0f &&
        intrinsics2.direction[2][1] == 0.0f &&
        intrinsics2.direction[2][2] == 1.0f &&
        intrinsics2.extreme[0] == 0 &&
        intrinsics2.extreme[1] == 1 &&
        intrinsics2.extreme[2] == 2 &&
        intrinsics2.extreme[3] == 2 &&
        intrinsics2.extremeCCW == false,
        "GetIntrinsics failed");

    v[0] = { 0.0f, 0.0f, 0.0f };
    v[1] = { 1.0f, 0.0f, 0.0f };
    v[2] = { 1.0f, 1.0f, 0.0f };
    v[3] = { 1e-05f, 2e-05f, 1e-05f };
    v[4] = { 2e-05f, 1e-05f, -1e-05f };
    Intrinsics3<float> intrinsics3;
    intrinsics3(v, epsilon);
    UTAssert(
        intrinsics3.dimension == 3 &&
        intrinsics3.min[0] == 0.0f &&
        intrinsics3.min[1] == 0.0f &&
        intrinsics3.min[2] == -1e-05f &&
        intrinsics3.max[0] == 1.0f &&
        intrinsics3.max[1] == 1.0f &&
        intrinsics3.max[2] == 1e-05f &&
        intrinsics3.maxRange == 1.0f &&
        intrinsics3.origin[0] == 0.0f &&
        intrinsics3.origin[1] == 0.0f &&
        intrinsics3.origin[2] == 0.0f &&
        intrinsics3.direction[0][0] == 1.0f &&
        intrinsics3.direction[0][1] == 0.0f &&
        intrinsics3.direction[0][2] == 0.0f &&
        intrinsics3.direction[1][0] == 0.0f &&
        intrinsics3.direction[1][1] == 1.0f &&
        intrinsics3.direction[1][2] == 0.0f &&
        intrinsics3.direction[2][0] == 0.0f &&
        intrinsics3.direction[2][1] == 0.0f &&
        intrinsics3.direction[2][2] == 1.0f &&
        intrinsics3.extreme[0] == 0 &&
        intrinsics3.extreme[1] == 1 &&
        intrinsics3.extreme[2] == 2 &&
        intrinsics3.extreme[3] == 3 &&
        intrinsics3.extremeCCW == true,
        "GetIntrinsics failed");
}

void UnitTestVector::Geometric4()
{
    float const epsilon = 1e-06f;
    Vector4<float> v0{ 1.0f, 2.0f, 3.0f, 4.0f };
    Vector4<float> v1{ 5.0f, 6.0f, 7.0f, 8.0f };
    Vector4<float> v2{ 1.0f, 0.0f, 1.0f, 0.0f };
    Vector4<float> v3{}, expV0{}, expV1{}, expV2{}, expV3{}, diff{};
    float temp{};
    float length{};
    bool success{};

    v3 = HyperCross(v0, v1, v2);
    expV3 = { 8.0f, -8.0f, -8.0f, 8.0f };
    UTAssert(v3 == expV3, "HyperCross failed");
    temp = std::fabs(Dot(v0, v3));
    UTAssert(temp <= epsilon, "HyperCross failed");
    temp = std::fabs(Dot(v1, v3));
    UTAssert(temp <= epsilon, "HyperCross failed");
    temp = std::fabs(Dot(v2, v3));
    UTAssert(temp <= epsilon, "HyperCross failed");

    v3 = UnitHyperCross(v0, v1, v2);
    expV3 = { 0.5f, -0.5f, -0.5f, 0.5f };
    UTAssert(v3 == expV3, "UnitHyperCross failed");
    temp = std::fabs(Dot(v0, v3));
    UTAssert(temp <= epsilon, "HyperCross failed");
    temp = std::fabs(Dot(v1, v3));
    UTAssert(temp <= epsilon, "HyperCross failed");
    temp = std::fabs(Dot(v2, v3));
    UTAssert(temp <= epsilon, "HyperCross failed");

    temp = DotHyperCross(v0, v1, v2, v3);
    length = std::fabs(temp - 16.0f);
    UTAssert(length <= epsilon, "DotHyperCross failed");

    // ComputeOrthonormalBasis
    //   numInputs == 1, maxIndex >= 2
    //   numInputs < 3,  maxIndex >= 3
    v0 = { 1.0f, 2.0f, 3.0f, 4.0f };
    success = ComputeOrthonormalBasis(1, v0, v1, v2, v3);
    UTAssert(success, "ComputeOrthonormalBasis failed");
    expV0 = { 0.182574183f, 0.365148365f, 0.547722518f, 0.730296731f };
    expV1 = { 0.0f, 0.0f, -0.8f, 0.6f };
    expV2 = { 0.0f, -0.928476691f, 0.222834393f, 0.297112554f };
    expV3 = { -0.983192086f, 0.0678063557f, 0.101709522f, 0.135612711f };
    UTAssert(
        Length(v0 - expV0) <= epsilon &&
        Length(v1 - expV1) <= epsilon &&
        Length(v2 - expV2) <= epsilon &&
        Length(v3 - expV3) <= epsilon,
        "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v0) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v1) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v2, v2) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v3, v3) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v1)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v3)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v3)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v2, v3)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(DotHyperCross(v0, v1, v2, v3) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");

    // ComputeOrthonormalBasis
    //   numInputs == 1, maxIndex < 2
    //   numInputs < 3,  maxIndex == 0
    v0 = { 4.0f, 3.0f, 2.0f, 1.0f };
    success = ComputeOrthonormalBasis(1, v0, v1, v2, v3);
    UTAssert(success, "ComputeOrthonormalBasis failed");
    expV0 = { 0.730296731f, 0.547722518f, 0.365148365f, 0.182574183f };
    expV1 = { -0.6f, 0.8f, 0.0f, 0.0f };
    expV2 = { 0.156892926f, 0.117669679f, 0.0f, -0.980580688f };
    expV3 = { 0.286445946f, 0.214834437f, -0.930949330f, 0.0716114864f };
    UTAssert(
        Length(v0 - expV0) <= epsilon &&
        Length(v1 - expV1) <= epsilon &&
        Length(v2 - expV2) <= epsilon &&
        Length(v3 - expV3) <= epsilon,
        "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v0) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v1) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v2, v2) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v3, v3) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v1)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v3)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v3)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v2, v3)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(DotHyperCross(v0, v1, v2, v3) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");

    // ComputeOrthonormalBasis
    //   numInputs == 2
    //   numInputs <  3, maxIndex <= 2
    v0 = { 1.0f, 2.0f, 3.0f, 4.0f };
    v1 = { 5.0f, 6.0f, 7.0f, 8.0f };
    success = ComputeOrthonormalBasis(2, v0, v1, v2, v3);
    UTAssert(success, "ComputeOrthonormalBasis failed");
    expV0 = { 0.182574183f, 0.365148365f, 0.547722518f, 0.730296731f };
    expV1 = { 0.816496611f, 0.408248365f, 0.0f, -0.408248216f };
    expV2 = { -0.267261416f, 0.0f, 0.801783741f, -0.534522414f };
    expV3 = { 0.478091419f, -0.836660087f, 0.239045799f, 0.119522847f };
    UTAssert(
        Length(v0 - expV0) <= epsilon &&
        Length(v1 - expV1) <= epsilon &&
        Length(v2 - expV2) <= epsilon &&
        Length(v3 - expV3) <= epsilon,
        "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v0) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v1) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v2, v2) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v3, v3) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v1)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v3)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v3)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v2, v3)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(DotHyperCross(v0, v1, v2, v3) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");

    // ComputeOrthonormalBasis
    //   numInputs == 3
    //   numInputs <  4
    v0 = { 1.0f, 2.0f, 3.0f, 4.0f };
    v1 = { 5.0f, 6.0f, 7.0f, 8.0f };
    v2 = { 1.0f, 0.0f, 1.0f, 0.0f };
    success = ComputeOrthonormalBasis(3, v0, v1, v2, v3);
    UTAssert(success, "ComputeOrthonormalBasis failed");
    expV0 = { 0.182574183f, 0.365148365f, 0.547722518f, 0.730296731f };
    expV1 = { 0.816496611f, 0.408248365f, 0.0f, -0.408248216f };
    expV2 = { 0.223606497f, -0.670820713f, 0.670820296f, -0.223606735f };
    expV3 = { 0.5f, -0.5f, -0.5f, 0.5f };
    UTAssert(
        Length(v0 - expV0) <= epsilon &&
        Length(v1 - expV1) <= epsilon &&
        Length(v2 - expV2) <= epsilon &&
        Length(v3 - expV3) <= epsilon,
        "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v0) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v1) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v2, v2) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v3, v3) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v1)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v0, v3)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v2)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v1, v3)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(Dot(v2, v3)) <= epsilon, "ComputeOrthonormalBasis failed");
    UTAssert(std::fabs(DotHyperCross(v0, v1, v2, v3) - 1.0f) <= epsilon, "ComputeOrthonormalBasis failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Algebra/Vector.h>

namespace gtl
{
    template class Vector<float, 2>;
    using VF2 = Vector<float, 2>;
    using VF3 = Vector<float, 3>;
    template bool operator==(VF2 const&, VF2 const&);
    template bool operator!=(VF2 const&, VF2 const&);
    template bool operator<(VF2 const&, VF2 const&);
    template bool operator<=(VF2 const&, VF2 const&);
    template bool operator>(VF2 const&, VF2 const&);
    template bool operator>=(VF2 const&, VF2 const&);
    template void MakeZero(VF2&);
    template bool IsZero(VF2 const&);
    template void MakeOne(VF2&);
    template bool IsOne(VF2 const&);
    template void MakeUnit(std::size_t, VF2&);
    template bool IsUnit(std::size_t, VF2 const&);
    template VF2 operator+(VF2 const&);
    template VF2 operator-(VF2 const&);
    template VF2 operator+(VF2 const&, VF2 const&);
    template VF2& operator+=(VF2&, VF2 const&);
    template VF2 operator-(VF2 const&, VF2 const&);
    template VF2& operator-=(VF2&, VF2 const&);
    template VF2 operator*(VF2 const&, float const&);
    template VF2 operator*(float const&, VF2 const&);
    template VF2& operator*=(VF2&, float const&);
    template VF2 operator/(VF2 const&, float const&);
    template VF2& operator/=(VF2&, float const&);
    template VF2 operator*(VF2 const&, VF2 const&);
    template VF2& operator*=(VF2&, VF2 const&);
    template VF2 operator/(VF2 const&, VF2 const&);
    template VF2& operator/=(VF2&, VF2 const&);
    template float Dot(VF2 const&, VF2 const&);
    template float Length(VF2 const&);
    template float Normalize(VF2&);
    template float NormalizeRobust(VF2&);
    template float Orthonormalize(std::vector<VF2>&);
    template VF2 GetOrthogonal(VF2 const& v, bool);
    template std::pair<VF2, VF2> ComputeExtremes(std::array<VF2, 3> const&);
    template std::pair<VF2, VF2> ComputeExtremes(std::vector<VF2> const&);
    template VF3 HLift(VF2 const&, float const&);
    template VF2 HProject(VF3 const&);
    template VF3 Lift(VF2 const&, std::size_t, float const&);
    template VF2 Project(VF3 const&, std::size_t);

    template class Vector<float>;
    using VFD = Vector<float>;
    template bool operator==(VFD const&, VFD const&);
    template bool operator!=(VFD const&, VFD const&);
    template bool operator<(VFD const&, VFD const&);
    template bool operator<=(VFD const&, VFD const&);
    template bool operator>(VFD const&, VFD const&);
    template bool operator>=(VFD const&, VFD const&);
    template void MakeZero(VFD&);
    template bool IsZero(VFD const&);
    template void MakeOne(VFD&);
    template bool IsOne(VFD const&);
    template void MakeUnit(std::size_t, VFD&);
    template bool IsUnit(std::size_t, VFD const&);
    template VFD operator+(VFD const&);
    template VFD operator-(VFD const&);
    template VFD operator+(VFD const&, VFD const&);
    template VFD& operator+=(VFD&, VFD const&);
    template VFD operator-(VFD const&, VFD const&);
    template VFD& operator-=(VFD&, VFD const&);
    template VFD operator*(VFD const&, float const&);
    template VFD operator*(float const&, VFD const&);
    template VFD& operator*=(VFD&, float const&);
    template VFD operator/(VFD const&, float const&);
    template VFD& operator/=(VFD&, float const&);
    template VFD operator*(VFD const&, VFD const&);
    template VFD& operator*=(VFD&, VFD const&);
    template VFD operator/(VFD const&, VFD const&);
    template VFD& operator/=(VFD&, VFD const&);
    template float Dot(VFD const&, VFD const&);
    template float Length(VFD const&);
    template float Normalize(VFD&);
    template float NormalizeRobust(VFD&);
    template float Orthonormalize(std::vector<VFD>&);
    template VFD GetOrthogonal(VFD const& v, bool);
    template std::pair<VFD, VFD> ComputeExtremes(std::array<VFD, 3> const&);
    template std::pair<VFD, VFD> ComputeExtremes(std::vector<VFD> const&);
    template VFD HLift(VFD const&, float const&);
    template VFD HProject(VFD const&);
    template VFD Lift(VFD const&, std::size_t, float const&);
    template VFD Project(VFD const&, std::size_t);

    template class Vector<double, 2>;
    using VD2 = Vector<double, 2>;
    using VD3 = Vector<double, 3>;
    template bool operator==(VD2 const&, VD2 const&);
    template bool operator!=(VD2 const&, VD2 const&);
    template bool operator<(VD2 const&, VD2 const&);
    template bool operator<=(VD2 const&, VD2 const&);
    template bool operator>(VD2 const&, VD2 const&);
    template bool operator>=(VD2 const&, VD2 const&);
    template void MakeZero(VD2&);
    template bool IsZero(VD2 const&);
    template void MakeOne(VD2&);
    template bool IsOne(VD2 const&);
    template void MakeUnit(std::size_t, VD2&);
    template bool IsUnit(std::size_t, VD2 const&);
    template VD2 operator+(VD2 const&);
    template VD2 operator-(VD2 const&);
    template VD2 operator+(VD2 const&, VD2 const&);
    template VD2& operator+=(VD2&, VD2 const&);
    template VD2 operator-(VD2 const&, VD2 const&);
    template VD2& operator-=(VD2&, VD2 const&);
    template VD2 operator*(VD2 const&, double const&);
    template VD2 operator*(double const&, VD2 const&);
    template VD2& operator*=(VD2&, double const&);
    template VD2 operator/(VD2 const&, double const&);
    template VD2& operator/=(VD2&, double const&);
    template VD2 operator*(VD2 const&, VD2 const&);
    template VD2& operator*=(VD2&, VD2 const&);
    template VD2 operator/(VD2 const&, VD2 const&);
    template VD2& operator/=(VD2&, VD2 const&);
    template double Dot(VD2 const&, VD2 const&);
    template double Length(VD2 const&);
    template double Normalize(VD2&);
    template double NormalizeRobust(VD2&);
    template double Orthonormalize(std::vector<VD2>&);
    template VD2 GetOrthogonal(VD2 const& v, bool);
    template std::pair<VD2, VD2> ComputeExtremes(std::array<VD2, 3> const&);
    template std::pair<VD2, VD2> ComputeExtremes(std::vector<VD2> const&);
    template VD3 HLift(VD2 const&, double const&);
    template VD2 HProject(VD3 const&);
    template VD3 Lift(VD2 const&, std::size_t, double const&);
    template VD2 Project(VD3 const&, std::size_t);

    template class Vector<double>;
    using VDD = Vector<double>;
    template bool operator==(VDD const&, VDD const&);
    template bool operator!=(VDD const&, VDD const&);
    template bool operator<(VDD const&, VDD const&);
    template bool operator<=(VDD const&, VDD const&);
    template bool operator>(VDD const&, VDD const&);
    template bool operator>=(VDD const&, VDD const&);
    template void MakeZero(VDD&);
    template bool IsZero(VDD const&);
    template void MakeOne(VDD&);
    template bool IsOne(VDD const&);
    template void MakeUnit(std::size_t, VDD&);
    template bool IsUnit(std::size_t, VDD const&);
    template VDD operator+(VDD const&);
    template VDD operator-(VDD const&);
    template VDD operator+(VDD const&, VDD const&);
    template VDD& operator+=(VDD&, VDD const&);
    template VDD operator-(VDD const&, VDD const&);
    template VDD& operator-=(VDD&, VDD const&);
    template VDD operator*(VDD const&, double const&);
    template VDD operator*(double const&, VDD const&);
    template VDD& operator*=(VDD&, double const&);
    template VDD operator/(VDD const&, double const&);
    template VDD& operator/=(VDD&, double const&);
    template VDD operator*(VDD const&, VDD const&);
    template VDD& operator*=(VDD&, VDD const&);
    template VDD operator/(VDD const&, VDD const&);
    template VDD& operator/=(VDD&, VDD const&);
    template double Dot(VDD const&, VDD const&);
    template double Length(VDD const&);
    template double Normalize(VDD&);
    template double NormalizeRobust(VDD&);
    template double Orthonormalize(std::vector<VDD>&);
    template VDD GetOrthogonal(VDD const& v, bool);
    template std::pair<VDD, VDD> ComputeExtremes(std::array<VDD, 3> const&);
    template std::pair<VDD, VDD> ComputeExtremes(std::vector<VDD> const&);
    template VDD HLift(VDD const&, double const&);
    template VDD HProject(VDD const&);
    template VDD Lift(VDD const&, std::size_t, double const&);
    template VDD Project(VDD const&, std::size_t);

    template VF2 Perp(VF2 const&);
    template VF2 UnitPerp(VF2 const&);
    template float DotPerp(VF2 const&, VF2 const&);
    template bool ComputeOrthonormalBasis(VF2&, VF2&);
    template bool ComputeBarycentrics(VF2 const&, VF2 const&, VF2 const&, VF2 const&, std::array<float, 3>&);
    template class Intrinsics2<float>;

    template VD2 Perp(VD2 const&);
    template VD2 UnitPerp(VD2 const&);
    template double DotPerp(VD2 const&, VD2 const&);
    template bool ComputeOrthonormalBasis(VD2&, VD2&);
    template bool ComputeBarycentrics(VD2 const&, VD2 const&, VD2 const&, VD2 const&, std::array<double, 3>&);
    template class Intrinsics2<double>;

    template VF3 UnitCross(VF3 const&, VF3 const&);
    template float DotCross(VF3 const&, VF3 const&, VF3 const&);
    template bool ComputeOrthonormalBasis(std::size_t, VF3&, VF3&, VF3&);
    template void ComputeOrthogonalComplement(VF3 const&, VF3&, VF3&);
    template bool ComputeBarycentrics(VF3 const&, VF3 const&, VF3 const&, VF3 const&, VF3 const&, std::array<float, 4>&);
    template class Intrinsics3<float>;

    template VD3 UnitCross(VD3 const&, VD3 const&);
    template double DotCross(VD3 const&, VD3 const&, VD3 const&);
    template bool ComputeOrthonormalBasis(std::size_t, VD3&, VD3&, VD3&);
    template void ComputeOrthogonalComplement(VD3 const&, VD3&, VD3&);
    template bool ComputeBarycentrics(VD3 const&, VD3 const&, VD3 const&, VD3 const&, VD3 const&, std::array<double, 4>&);
    template class Intrinsics3<double>;

    using VF4 = Vector4<float>;
    template VF4 HyperCross(VF4 const&, VF4 const&, VF4 const&);
    template VF4 UnitHyperCross(VF4 const&, VF4 const&, VF4 const&);
    template float DotHyperCross(VF4 const&, VF4 const&, VF4 const&, VF4 const&);
    template bool ComputeOrthonormalBasis(std::size_t, VF4&, VF4&, VF4&, VF4&);

    using VD4 = Vector<double, 4>;
    template VD4 HyperCross(VD4 const&, VD4 const&, VD4 const&);
    template VD4 UnitHyperCross(VD4 const&, VD4 const&, VD4 const&);
    template double DotHyperCross(VD4 const&, VD4 const&, VD4 const&, VD4 const&);
    template bool ComputeOrthonormalBasis(std::size_t, VD4&, VD4&, VD4&, VD4&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Vector<Rational, 2>;
    template class Vector<Rational>;
    using VR2 = Vector<Rational, 2>;
    using VR3 = Vector<Rational, 3>;
    template bool operator==(VR2 const&, VR2 const&);
    template bool operator!=(VR2 const&, VR2 const&);
    template bool operator<(VR2 const&, VR2 const&);
    template bool operator<=(VR2 const&, VR2 const&);
    template bool operator>(VR2 const&, VR2 const&);
    template bool operator>=(VR2 const&, VR2 const&);
    template void MakeZero(VR2&);
    template bool IsZero(VR2 const&);
    template void MakeOne(VR2&);
    template bool IsOne(VR2 const&);
    template void MakeUnit(std::size_t, VR2&);
    template bool IsUnit(std::size_t, VR2 const&);
    template VR2 operator+(VR2 const&);
    template VR2 operator-(VR2 const&);
    template VR2 operator+(VR2 const&, VR2 const&);
    template VR2& operator+=(VR2&, VR2 const&);
    template VR2 operator-(VR2 const&, VR2 const&);
    template VR2& operator-=(VR2&, VR2 const&);
    template VR2 operator*(VR2 const&, Rational const&);
    template VR2 operator*(Rational const&, VR2 const&);
    template VR2& operator*=(VR2&, Rational const&);
    template VR2 operator/(VR2 const&, Rational const&);
    template VR2& operator/=(VR2&, Rational const&);
    template VR2 operator*(VR2 const&, VR2 const&);
    template VR2& operator*=(VR2&, VR2 const&);
    template VR2 operator/(VR2 const&, VR2 const&);
    template VR2& operator/=(VR2&, VR2 const&);
    template Rational Dot(VR2 const&, VR2 const&);
    template Rational Length(VR2 const&);
    template Rational Normalize(VR2&);
    template Rational NormalizeRobust(VR2&);
    template Rational Orthonormalize(std::vector<VR2>&);
    template VR2 GetOrthogonal(VR2 const& v, bool);
    template std::pair<VR2, VR2> ComputeExtremes(std::array<VR2, 3> const&);
    template std::pair<VR2, VR2> ComputeExtremes(std::vector<VR2> const&);
    template VR3 HLift(VR2 const&, Rational const&);
    template VR2 HProject(VR3 const&);
    template VR3 Lift(VR2 const&, std::size_t, Rational const&);
    template VR2 Project(VR3 const&, std::size_t);

    using VRD = Vector<Rational>;
    template bool operator==(VRD const&, VRD const&);
    template bool operator!=(VRD const&, VRD const&);
    template bool operator<(VRD const&, VRD const&);
    template bool operator<=(VRD const&, VRD const&);
    template bool operator>(VRD const&, VRD const&);
    template bool operator>=(VRD const&, VRD const&);
    template void MakeZero(VRD&);
    template bool IsZero(VRD const&);
    template void MakeOne(VRD&);
    template bool IsOne(VRD const&);
    template void MakeUnit(std::size_t, VRD&);
    template bool IsUnit(std::size_t, VRD const&);
    template VRD operator+(VRD const&);
    template VRD operator-(VRD const&);
    template VRD operator+(VRD const&, VRD const&);
    template VRD& operator+=(VRD&, VRD const&);
    template VRD operator-(VRD const&, VRD const&);
    template VRD& operator-=(VRD&, VRD const&);
    template VRD operator*(VRD const&, Rational const&);
    template VRD operator*(Rational const&, VRD const&);
    template VRD& operator*=(VRD&, Rational const&);
    template VRD operator/(VRD const&, Rational const&);
    template VRD& operator/=(VRD&, Rational const&);
    template VRD operator*(VRD const&, VRD const&);
    template VRD& operator*=(VRD&, VRD const&);
    template VRD operator/(VRD const&, VRD const&);
    template VRD& operator/=(VRD&, VRD const&);
    template Rational Dot(VRD const&, VRD const&);
    template Rational Length(VRD const&);
    template Rational Normalize(VRD&);
    template Rational NormalizeRobust(VRD&);
    template Rational Orthonormalize(std::vector<VRD>&);
    template VRD GetOrthogonal(VRD const& v, bool);
    template std::pair<VRD, VRD> ComputeExtremes(std::array<VRD, 3> const&);
    template std::pair<VRD, VRD> ComputeExtremes(std::vector<VRD> const&);
    template VRD HLift(VRD const&, Rational const&);
    template VRD HProject(VRD const&);
    template VRD Lift(VRD const&, std::size_t, Rational const&);
    template VRD Project(VRD const&, std::size_t);

    template VR2 Perp(VR2 const&);
    template VR2 UnitPerp(VR2 const&);
    template Rational DotPerp(VR2 const&, VR2 const&);
    template bool ComputeOrthonormalBasis(VR2&, VR2&);
    template bool ComputeBarycentrics(VR2 const&, VR2 const&, VR2 const&, VR2 const&, std::array<Rational, 3>&);
    template class Intrinsics2<Rational>;

    template VR3 Cross(VR3 const&, VR3 const&);
    template VR3 UnitCross(VR3 const&, VR3 const&);
    template Rational DotCross(VR3 const&, VR3 const&, VR3 const&);
    template bool ComputeOrthonormalBasis(std::size_t, VR3&, VR3&, VR3&);
    template bool ComputeBarycentrics(VR3 const&, VR3 const&, VR3 const&, VR3 const&, VR3 const&, std::array<Rational, 4>&);
    template class Intrinsics3<Rational>;

    using VR4 = Vector<Rational, 4>;
    template VR4 HyperCross(VR4 const&, VR4 const&, VR4 const&);
    template VR4 UnitHyperCross(VR4 const&, VR4 const&, VR4 const&);
    template Rational DotHyperCross(VR4 const&, VR4 const&, VR4 const&, VR4 const&);
    template bool ComputeOrthonormalBasis(std::size_t, VR4&, VR4&, VR4&, VR4&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Vector)
