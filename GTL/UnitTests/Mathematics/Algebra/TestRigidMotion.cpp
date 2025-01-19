// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRigidMotion
    {
    public:
        UnitTestRigidMotion();

    private:
        void TestMatrix2();
        void TestMatrixQuaternion();
        void TestMatrixAxisAngle();
        void TestMatrixEulerAngles();
        void TestQuaternionAxisAngle();
        void TestQuaternionEulerAngles();
        void TestAxisAngleEulerAngles();
        void TestDualQuaternionRigid();
    };
}

UnitTestRigidMotion::UnitTestRigidMotion()
{
    UTInformation("Mathematics/Algebra/RigidMotion");

    TestMatrix2();
    TestMatrixQuaternion();
    TestMatrixAxisAngle();
    TestMatrixEulerAngles();
    TestQuaternionAxisAngle();
    TestQuaternionEulerAngles();
    TestAxisAngleEulerAngles();
    TestDualQuaternionRigid();
}

void UnitTestRigidMotion::TestMatrix2()
{
    double angle0 = 0.12345, angle1 = 0.0;
    Matrix<double, 2, 2> R0, R1;

    RigidMotion<double>::Convert(angle0, R0);
    UTAssert(
        R0(0, 0) == 0.99238972111148815 &&
        R0(0, 1) == -0.12313667785133202 &&
        R0(1, 0) == 0.12313667785133202 &&
        R0(1, 1) == 0.99238972111148815,
        "Convert angle to matrix failed.");

    RigidMotion<double>::Convert(R0, angle1);
    double error = std::fabs(angle1 - angle0);
    UTAssert(
        error <= 1e-16,
        "Convert matrix to angle failed.");

    RigidMotion<double>::Convert(angle1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-16,
        "Convert angle to matrix failed.");
}

void UnitTestRigidMotion::TestMatrixQuaternion()
{
    Quaternion<double> q0, q1;
    Matrix<double, 3, 3> R0, R1;

    // z^2 + w^2 >= x^2 + y^2 and w^2 >= z^2
    q0 = Quaternion<double>(1.0, 2.0, 3.0, 4.0);
    Normalize(q0);

    RigidMotion<double>::Convert(q0, R0);
    UTAssert(
        R0(0, 0) == 0.13333333333333353 &&
        R0(0, 1) == -0.66666666666666663 &&
        R0(0, 2) == 0.73333333333333317 &&
        R0(1, 0) == 0.93333333333333324 &&
        R0(1, 1) == 0.33333333333333348 &&
        R0(1, 2) == 0.13333333333333336 &&
        R0(2, 0) == -0.33333333333333326 &&
        R0(2, 1) == 0.66666666666666652 &&
        R0(2, 2) == 0.66666666666666674,
        "Convert quaternion to matrix failed.");

    RigidMotion<double>::Convert(R0, q1);
    double error = Length(q1 - q0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to quaternion failed.");

    RigidMotion<double>::Convert(q1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert quaternion to matrix failed.");

    // z^2 + w^2 >= x^2 + y^2 and w^2 >= z^2
    q0 = Quaternion<double>(1.0, 2.0, 4.0, 3.0);
    Normalize(q0);

    RigidMotion<double>::Convert(q0, R0);
    RigidMotion<double>::Convert(R0, q1);
    error = Length(q1 - q0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to quaternion failed.");

    RigidMotion<double>::Convert(q1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert quaternion to matrix failed.");

    // x^2 + y^2 >= z^2 + w^2 and y^2 >= x^2
    q0 = Quaternion<double>(3.0, 4.0, 1.0, 2.0);
    Normalize(q0);

    RigidMotion<double>::Convert(q0, R0);
    RigidMotion<double>::Convert(R0, q1);
    error = Length(q1 - q0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to quaternion failed.");

    RigidMotion<double>::Convert(q1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert quaternion to matrix failed.");

    // x^2 + y^2 >= z^2 + w^2 and x^2 >= y^2
    q0 = Quaternion<double>(4.0, 3.0, 1.0, 2.0);
    Normalize(q0);

    RigidMotion<double>::Convert(q0, R0);
    RigidMotion<double>::Convert(R0, q1);
    error = Length(q1 - q0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to quaternion failed.");

    RigidMotion<double>::Convert(q1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert quaternion to matrix failed.");
}

void UnitTestRigidMotion::TestMatrixAxisAngle()
{
    AxisAngle<double> aa0(Vector<double, 3>{ 1.0, 2.0, 3.0 }, 0.123456), aa1;
    Normalize(aa0.axis);
    Matrix<double, 3, 3> R0, R1;

    // 0 < angle <= pi/2
    RigidMotion<double>::Convert(aa0, R0);
    UTAssert(
        R0(0, 0) == 0.99293262639687541 &&
        R0(0, 1) == -0.097646470185707490 &&
        R0(0, 2) == 0.067453437991513196 &&
        R0(1, 0) == 0.099821046678976594 &&
        R0(1, 1) == 0.99456355876682723 &&
        R0(1, 2) == -0.029649388070877008 &&
        R0(2, 0) == -0.064191573251609527 &&
        R0(2, 1) == 0.036173117550684357 &&
        R0(2, 2) == 0.99728177938341356,
        "Convert axis-angle to matrix failed.");

    RigidMotion<double>::Convert(R0, aa1);
    RigidMotion<double>::Convert(aa1, R1);
    double error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert axis-angle to matrix failed.");

    // pi/2 < angle <= pi
    aa0.angle = 0.75 * C_PI<double>;
    RigidMotion<double>::Convert(aa0, R0);
    UTAssert(
        R0(0, 0) == -0.58517058253036547 &&
        R0(0, 1) == -0.32307431220147687 &&
        R0(0, 2) == 0.74377306897777318 &&
        R0(1, 0) == 0.81081910682620484 &&
        R0(1, 1) == -0.21936198656181954 &&
        R0(1, 2) == 0.54263495543247819 &&
        R0(2, 0) == -0.012155877040681329 &&
        R0(2, 1) == 0.92059942844170550 &&
        R0(2, 2) == 0.39031900671909026,
        "Convert axis-angle to matrix failed.");

    RigidMotion<double>::Convert(R0, aa1);
    RigidMotion<double>::Convert(aa1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert axis-angle to matrix failed.");

    aa0.axis = -aa0.axis;
    RigidMotion<double>::Convert(aa0, R0);
    RigidMotion<double>::Convert(R0, aa1);
    RigidMotion<double>::Convert(aa1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert axis-angle to matrix failed.");

    // angle 0
    aa0.axis = -aa0.axis;
    aa0.angle = 0.0;
    RigidMotion<double>::Convert(aa0, R0);
    RigidMotion<double>::Convert(R0, aa1);
    RigidMotion<double>::Convert(aa1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert axis-angle to matrix failed.");

    // Choose angle pi. Step into Convert(R0, aa1). Floating-point rounding
    // errors lead to a.angle < C_PI<double>. Set the next statement
    // to be in each of the 4 blocks where the angle is pi. In all cases,
    // the error is about 2e-08.
    aa0.angle = std::acos(-1.0);
    RigidMotion<double>::Convert(aa0, R0);
    RigidMotion<double>::Convert(R0, aa1);
    RigidMotion<double>::Convert(aa1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-7,
        "Convert axis-angle to matrix failed.");

    // angle epsilon
    aa0.angle = 1e-7;
    RigidMotion<double>::Convert(aa0, R0);
    RigidMotion<double>::Convert(R0, aa1);
    RigidMotion<double>::Convert(aa1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-8,
        "Convert axis-angle to matrix failed.");
}

void UnitTestRigidMotion::TestMatrixEulerAngles()
{
    Matrix<double, 3, 3> R0, R1;
    EulerAngles<double> euler0(1, 2, 0, 0.123456, -0.987654, 0.2040305);
    EulerAngles<double> euler1(0, 1, 2);

    RigidMotion<double>::Convert(euler0, R0);
    UTAssert(
        R0(0, 0) == 0.54645866144484900 &&
        R0(0, 1) == 0.83473645274462827 &&
        R0(0, 2) == 0.067808449261275119 &&
        R0(1, 0) == -0.78624993890892114 &&
        R0(1, 1) == 0.53922802586703489 &&
        R0(1, 2) == -0.30173526423879926 &&
        R0(2, 0) == -0.28843364037092079 &&
        R0(2, 1) == 0.11157145951745578 &&
        R0(2, 2) == 0.95097731020436194,
        "Convert Euler angles to matrix failed.");

    RigidMotion<double>::Convert(R0, euler1);
    UTAssert(
        euler1.axis[0] == 0 && euler1.axis[1] == 1 && euler1.axis[2] == 2 &&
        euler1.angle[0] == 0.11678904438022251 &&
        euler1.angle[1] == 0.29259054882590135 &&
        euler1.angle[2] == -0.96342113097200044,
        "Convert matrix to Euler angles failed.");
    RigidMotion<double>::Convert(euler1, R1);
    double error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to Euler angles failed.");

    euler1.axis = { 0, 2, 1 };
    RigidMotion<double>::Convert(R0, euler1);
    UTAssert(
        euler1.axis[0] == 0 && euler1.axis[1] == 2 && euler1.axis[2] == 1 &&
        euler1.angle[0] == 0.51016012322626447 &&
        euler1.angle[1] == -0.90471638577467017 &&
        euler1.angle[2] == 0.48565773496153092,
        "Convert matrix to Euler angles failed.");
    RigidMotion<double>::Convert(euler1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to Euler angles failed.");

    euler1.axis = { 1, 0, 2 };
    RigidMotion<double>::Convert(R0, euler1);
    UTAssert(
        euler1.axis[0] == 1 && euler1.axis[1] == 0 && euler1.axis[2] == 2 &&
        euler1.angle[0] == 0.29448370080306685 &&
        euler1.angle[1] == 0.11180424300142544 &&
        euler1.angle[2] == -0.99724810711494227,
        "Convert matrix to Euler angles failed.");
    RigidMotion<double>::Convert(euler1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to Euler angles failed.");

    euler1.axis = { 1, 2, 0 };
    RigidMotion<double>::Convert(R0, euler1);
    UTAssert(
        euler1.axis[0] == 1 && euler1.axis[1] == 2 && euler1.axis[2] == 0 &&
        euler1.angle[0] == 0.12345599999999997 &&
        euler1.angle[1] == -0.98765399999999992 &&
        euler1.angle[2] == 0.20403050000000000,
        "Convert matrix to Euler angles failed.");
    RigidMotion<double>::Convert(euler1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to Euler angles failed.");

    euler1.axis = { 2, 0, 1 };
    RigidMotion<double>::Convert(R0, euler1);
    UTAssert(
        euler1.axis[0] == 2 && euler1.axis[1] == 0 && euler1.axis[2] == 1 &&
        euler1.angle[0] == -0.96964886109411541 &&
        euler1.angle[1] == 0.30651222681486540 &&
        euler1.angle[2] == 0.071183486032565191,
        "Convert matrix to Euler angles failed.");
    RigidMotion<double>::Convert(euler1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to Euler angles failed.");

    euler1.axis = { 2, 1, 0 };
    RigidMotion<double>::Convert(R0, euler1);
    UTAssert(
        euler1.axis[0] == 2 && euler1.axis[1] == 1 && euler1.axis[2] == 0 &&
        euler1.angle[0] == -0.99116046190700424 &&
        euler1.angle[1] == 0.067860520788624576 &&
        euler1.angle[2] == 0.30724242938710428,
        "Convert matrix to Euler angles failed.");
    RigidMotion<double>::Convert(euler1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to Euler angles failed.");

    euler1.axis = { 0, 1, 0 };
    RigidMotion<double>::Convert(R0, euler1);
    UTAssert(
        euler1.axis[0] == 0 && euler1.axis[1] == 1 && euler1.axis[2] == 0 &&
        euler1.angle[0] == 1.4897409421785239 &&
        euler1.angle[1] == 0.99266648549741532 &&
        euler1.angle[2] == -1.2191923459304410,
        "Convert matrix to Euler angles failed.");
    RigidMotion<double>::Convert(euler1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to Euler angles failed.");

    euler1.axis = { 0, 2, 0 };
    RigidMotion<double>::Convert(R0, euler1);
    UTAssert(
        euler1.axis[0] == 0 && euler1.axis[1] == 2 && euler1.axis[2] == 0 &&
        euler1.angle[0] == 3.0605372689734205 &&
        euler1.angle[1] == 0.99266648549741532 &&
        euler1.angle[2] == -2.7899886727253378,
        "Convert matrix to Euler angles failed.");
    RigidMotion<double>::Convert(euler1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to Euler angles failed.");

    euler1.axis = { 1, 0, 1 };
    RigidMotion<double>::Convert(R0, euler1);
    UTAssert(
        euler1.axis[0] == 1 && euler1.axis[1] == 0 && euler1.axis[2] == 1 &&
        euler1.angle[0] == -1.2043634403790879 &&
        euler1.angle[1] == 1.0012761466019844 &&
        euler1.angle[2] == 1.4379231587391479,
        "Convert matrix to Euler angles failed.");
    RigidMotion<double>::Convert(euler1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to Euler angles failed.");

    euler1.axis = { 1, 2, 1 };
    RigidMotion<double>::Convert(R0, euler1);
    UTAssert(
        euler1.axis[0] == 1 && euler1.axis[1] == 2 && euler1.axis[2] == 1 &&
        euler1.angle[0] == -2.7751597671739843 &&
        euler1.angle[1] == 1.0012761466019844 &&
        euler1.angle[2] == 3.0087194855340447,
        "Convert matrix to Euler angles failed.");
    RigidMotion<double>::Convert(euler1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to Euler angles failed.");

    euler1.axis = { 2, 0, 2 };
    RigidMotion<double>::Convert(R0, euler1);
    UTAssert(
        euler1.axis[0] == 2 && euler1.axis[1] == 0 && euler1.axis[2] == 2 &&
        euler1.angle[0] == -1.2017047281344122 &&
        euler1.angle[1] == 0.31441548063659702 &&
        euler1.angle[2] == 0.22105580759401675,
        "Convert matrix to Euler angles failed.");
    RigidMotion<double>::Convert(euler1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to Euler angles failed.");

    euler1.axis = { 2, 1, 2 };
    RigidMotion<double>::Convert(R0, euler1);
    UTAssert(
        euler1.axis[0] == 2 && euler1.axis[1] == 1 && euler1.axis[2] == 2 &&
        euler1.angle[0] == 0.36909159866048435 &&
        euler1.angle[1] == 0.31441548063659702 &&
        euler1.angle[2] == -1.3497405192008798,
        "Convert matrix to Euler angles failed.");
    RigidMotion<double>::Convert(euler1, R1);
    error = L2Norm(R1 - R0);
    UTAssert(
        error <= 1e-15,
        "Convert matrix to Euler angles failed.");
}

void UnitTestRigidMotion::TestQuaternionAxisAngle()
{
    Quaternion<double> q0, q1;
    AxisAngle<double> aa0, aa1;

    // z^2 + w^2 >= x^2 + y^2 and w^2 >= z^2
    q0 = Quaternion<double>(1.0, 2.0, 3.0, 4.0);
    Normalize(q0);
    RigidMotion<double>::Convert(q0, aa0);
    UTAssert(
        aa0.axis[0] == 0.26726124191242440 &&
        aa0.axis[1] == 0.53452248382484879 && 
        aa0.axis[2] == 0.80178372573727319 &&
        aa0.angle == 1.5040801783846716,
        "Convert quaternion to axis-angle failed.");

    RigidMotion<double>::Convert(aa0, q1);
    double error = Length(q1 - q0);
    UTAssert(
        error <= 1e-15,
        "Convert axis-angle to quaternion failed.");

    RigidMotion<double>::Convert(q1, aa1);
    error = Length(aa1.axis - aa0.axis) + std::fabs(aa1.angle - aa0.angle);
    UTAssert(
        error <= 1e-15,
        "Conversion quaternion to axis-angle failed.");
}

void UnitTestRigidMotion::TestQuaternionEulerAngles()
{
    Quaternion<double> q0, q1;
    EulerAngles<double> euler0(1, 2, 0), euler1(0, 1, 2);

    q0 = Quaternion<double>(1.0, 2.0, 3.0, 4.0);
    Normalize(q0);
    RigidMotion<double>::Convert(q0, euler0);
    UTAssert(
        euler0.angle[0] == 1.3909428270024180 &&
        euler0.angle[1] == 0.72972765622696634 &&
        euler0.angle[2] == 1.1071487177940902,
        "Conversion quaternion to Euler angles failed.");

    RigidMotion<double>::Convert(euler0, q1);
    double error = Length(q1 - q0);
    UTAssert(
        error <= 1e-15,
        "Conversion Euler angles to quaternion failed.");

    RigidMotion<double>::Convert(q1, euler1);
    UTAssert(
        euler1.angle[0] == 0.78539816339744806 &&
        euler1.angle[1] == 0.33983690945412193 &&
        euler1.angle[2] == 1.4288992721907323,
        "Conversion quaternion to Euler angles failed.");

    RigidMotion<double>::Convert(euler1, q1);
    error = Length(q1 - q0);
    UTAssert(
        error <= 1e-15,
        "Conversion Euler angles to quaternion failed.");
}

void UnitTestRigidMotion::TestAxisAngleEulerAngles()
{
    AxisAngle<double> aa0, aa1;
    EulerAngles<double> euler0(1, 2, 0), euler1(0, 1, 2);

    aa0.axis = { 1.0, 2.0, 3.0 };
    Normalize(aa0.axis);
    aa0.angle = 0.123456;

    RigidMotion<double>::Convert(aa0, euler0);
    UTAssert(
        euler0.angle[0] == 0.067829334485175627 &&
        euler0.angle[1] == 0.097802313589462980 &&
        euler0.angle[2] == 0.036354820648252982,
        "Convert axis-angle to Euler angles failed.");

    RigidMotion<double>::Convert(euler0, aa1);
    double error = Length(aa1.axis - aa0.axis) + std::fabs(aa1.angle - aa0.angle);
    UTAssert(
        error <= 1e-14,
        "Convert Euler angles to axis-angle failed.");

    RigidMotion<double>::Convert(aa1, euler1);
    UTAssert(
        euler1.angle[0] == 0.036255817807670239 &&
        euler1.angle[1] == 0.064235739380083340 &&
        euler1.angle[2] == 0.10019490260417907,
        "Convert axis-angle to Euler angles failed.");

    RigidMotion<double>::Convert(euler1, aa1);
    error = Length(aa1.axis - aa0.axis) + std::fabs(aa1.angle - aa0.angle);
    UTAssert(
        error <= 1e-14,
        "Convert Euler angles to axis-angle failed.");
}

void UnitTestRigidMotion::TestDualQuaternionRigid()
{
    DualQuaternion<double> d0, d1;
    Quaternion<double> q0, q1;
    Matrix<double, 3, 3> R0, R1;
    Vector<double, 3> T0, T1;

    d0[0] = Quaternion<double>(1.0, 2.0, 3.0, 4.0);
    Normalize(d0[0]);
    d0[1] = Quaternion<double>(2.0, 0.0, 1.0, -1.0);
    Normalize(d0[1]);

    RigidMotion<double>::Convert(d0, q0, T0);
    UTAssert(
        q0 == d0[0] &&
        T0[0] == 1.6397831834998458 &&
        T0[1] == 1.0434983894999019 &&
        T0[2] == 0.44721359549995798,
        "Conversion dual quaternion to quaterion-translation failed.");

    RigidMotion<double>::Convert(d0, R0, T0);
    RigidMotion<double>::Convert(R0, q0);
    double error = Length(q0 - d0[0]);
    UTAssert(
        error <= 1e-15 &&
        T0[0] == 1.6397831834998458 &&
        T0[1] == 1.0434983894999019 &&
        T0[2] == 0.44721359549995798,
        "Conversion dual quaternion to rotation-translation failed.");

    RigidMotion<double>::Convert(R0, T0, d1);

    RigidMotion<double>::Convert(q0, T0, d1);
    UTAssert(
        d1[0] == q0 &&
        d1[1][0] == 0.80288830457893057 &&
        d1[1][1] == -0.027216552697590712 &&
        d1[1][2] == 0.36742346141747673 &&
        d1[1][3] == -0.46268139585904461,
        "Conversion quaterion-translation to dual quaternion failed.");

    RigidMotion<double>::Convert(d1, q1, T1);
    error = Length(T1 - T0);
    UTAssert(
        q1 == d1[0] &&
        error <= 1e-15,
        "Conversion dual quaternion to quaterion-translation failed.");

    RigidMotion<double>::Convert(d1, R1, T1);
    RigidMotion<double>::Convert(R1, q1);
    error = Length(q1 - d1[0]) + Length(T1 - T0);
    UTAssert(
        error <= 1e-15,
        "Conversion dual quaternion to rotation-translation failed.");

    Matrix<double, 4, 4> H;
    RigidMotion<double>::Convert(R0, T0, H);
    RigidMotion<double>::Convert(H, R1, T1);
    error = L1Norm(R1 - R0) + Length(T1 - T0);
    UTAssert(
        error <= 1e-15,
        "Conversion between rigid 3x3 and rigid 4x4 failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Algebra/RigidMotion.h>

namespace gtl
{
    template class RigidMotion<float>;
    template class RigidMotion<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class RigidMotion<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RigidMotion)
