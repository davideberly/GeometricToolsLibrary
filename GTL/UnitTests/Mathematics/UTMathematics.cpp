// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#include "UTMathematics.h"
#include <UnitTestsExceptions.h>
#include <UnitTestsConfiguration.h>
using namespace gtl;

void UTMathematics::Execute()
{
#if defined(UT_MATHEMATICS)
    UTInformation("Mathematics");
    Algebra::Execute();
    Approximation2D::Execute();
    Approximation3D::Execute();
    ApproximationND::Execute();
    ApproximationPolynomial::Execute();
    Arithmetic::Execute();
    Containment2D::Execute();
    Containment3D::Execute();
    ContainmentND::Execute();
    Curves::Execute();
    DifferentialEquations::Execute();
    Distance2D::Execute();
    Distance3D::Execute();
    DistanceND::Execute();
    Functions::Execute();
    Geometry1D::Execute();
    Geometry2D::Execute();
    Geometry3D::Execute();
    GeometryND::Execute();
    ImageProcessing::Execute();
    Integration::Execute();
    Interpolation1D::Execute();
    Interpolation2D::Execute();
    Interpolation3D::Execute();
    InterpolationND::Execute();
    Intersection1D::Execute();
    Intersection2D::Execute();
    Intersection3D::Execute();
    IntersectionND::Execute();
    MatrixAnalysis::Execute();
    Meshes::Execute();
    Minimizers::Execute();
    Miscellaneous::Execute();
    Physics::Execute();
    Primitives2D::Execute();
    Primitives3D::Execute();
    PrimitivesND::Execute();
    Projection::Execute();
    RootFinders::Execute();
    Surfaces::Execute();
    Volumes::Execute();
#endif
}

void UTMathematics::Algebra::Execute()
{
#if defined(UT_MATHEMATICS_ALGEBRA)
    UTInformation("Mathematics/Algebra");

#if defined(UT_MATHEMATICS_AFFINETRANSFORM)
    Precall();
    extern void TestAffineTransform();
    TestAffineTransform();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_AXISANGLE)
    Precall();
    extern void TestAxisAngle();
    TestAxisAngle();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONVERTCOORDINATES)
    Precall();
    extern void TestConvertCoordinates();
    TestConvertCoordinates();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DUALQUATERNION)
    Precall();
    extern void TestDualQuaternion();
    TestDualQuaternion();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EULERANGLES)
    Precall();
    extern void TestEulerAngles();
    TestEulerAngles();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_LIEGROUPSALGEBRAS)
    Precall();
    extern void TestLieGroupsAlgebras();
    TestLieGroupsAlgebras();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MATRIX)
    Precall();
    extern void TestMatrix();
    TestMatrix();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MATRIXACCESSOR)
    Precall();
    extern void TestMatrixAccessor();
    TestMatrixAccessor();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_POLYNOMIAL)
    Precall();
    extern void TestPolynomial();
    TestPolynomial();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_QUATERNION)
    Precall();
    extern void TestQuaternion();
    TestQuaternion();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_RIGIDMOTION)
    Precall();
    extern void TestRigidMotion();
    TestRigidMotion();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_VECTOR)
    Precall();
    extern void TestVector();
    TestVector();
    Postcall();
#endif
#endif
}


void UTMathematics::Approximation2D::Execute()
{
#if defined(UT_MATHEMATICS_APPROXIMATION_2D)
    UTInformation("Mathematics/Approximation/2D");

#if defined(UT_MATHEMATICS_APPRCIRCLE2)
    Precall();
    extern void TestApprCircle2();
    TestApprCircle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRCURVEBYARCS)
    Precall();
    extern void TestApprCurveByArcs();
    TestApprCurveByArcs();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRELLIPSE2)
    Precall();
    extern void TestApprEllipse2();
    TestApprEllipse2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRELLIPSEBYARCS)
    Precall();
    extern void TestApprEllipseByArcs();
    TestApprEllipseByArcs();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRHEIGHTLINE2)
    Precall();
    extern void TestApprHeightLine2();
    TestApprHeightLine2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRORTHOGONALLINE2)
    Precall();
    extern void TestApprOrthogonalLine2();
    TestApprOrthogonalLine2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRPARABOLA2)
    Precall();
    extern void TestApprParabola2();
    TestApprParabola2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRPARALLELLINES2)
    Precall();
    extern void TestApprParallelLines2();
    TestApprParallelLines2();
    Postcall();
#endif
#endif
}

void UTMathematics::Approximation3D::Execute()
{
#if defined(UT_MATHEMATICS_APPROXIMATION_3D)
    UTInformation("Mathematics/Approximation/3D");

#if defined(UT_MATHEMATICS_APPRCONE3)
    Precall();
    extern void TestApprCone3();
    TestApprCone3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRCYLINDER3)
    Precall();
    extern void TestApprCylinder3();
    TestApprCylinder3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRELLIPSOID3)
    Precall();
    extern void TestApprEllipsoid3();
    TestApprEllipsoid3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRGREATARC3)
    Precall();
    extern void TestApprGreatArc3();
    TestApprGreatArc3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRGREATCIRCLE3)
    Precall();
    extern void TestApprGreatCircle3();
    TestApprGreatCircle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRHEIGHTPLANE3)
    Precall();
    extern void TestApprHeightPlane3();
    TestApprHeightPlane3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRHELMERTTRANSFORMATION3)
    Precall();
    extern void TestApprHelmertTransformation3();
    TestApprHelmertTransformation3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRORTHOGONALLINE3)
    Precall();
    extern void TestApprOrthogonalLine3();
    TestApprOrthogonalLine3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRORTHOGONALPLANE3)
    Precall();
    extern void TestApprOrthogonalPlane3();
    TestApprOrthogonalPlane3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRPARABOLOID3)
    Precall();
    extern void TestApprParaboloid3();
    TestApprParaboloid3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRSPHERE3)
    Precall();
    extern void TestApprSphere3();
    TestApprSphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRTORUS3)
    Precall();
    extern void TestApprTorus3();
    TestApprTorus3();
    Postcall();
#endif
#endif
}

void UTMathematics::ApproximationND::Execute()
{
#if defined(UT_MATHEMATICS_APPROXIMATION_ND)
    UTInformation("Mathematics/Approximation/ND");

#if defined(UT_MATHEMATICS_APPRGAUSSIANDISTRIBUTION)
    Precall();
    extern void TestApprGaussianDistribution();
    TestApprGaussianDistribution();
    Postcall();
#endif
#endif
}

void UTMathematics::ApproximationPolynomial::Execute()
{
#if defined(UT_MATHEMATICS_APPROXIMATION)
    UTInformation("Mathematics/Approximation");

#if defined(UT_MATHEMATICS_APPRPOLYNOMIAL2)
    Precall();
    extern void TestApprPolynomial2();
    TestApprPolynomial2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRPOLYNOMIAL3)
    Precall();
    extern void TestApprPolynomial3();
    TestApprPolynomial3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRPOLYNOMIAL4)
    Precall();
    extern void TestApprPolynomial4();
    TestApprPolynomial4();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APPRPOLYNOMIALSPECIAL)
    Precall();
    extern void TestApprPolynomialSpecial();
    TestApprPolynomialSpecial();
    Postcall();
#endif
#endif
}

void UTMathematics::Arithmetic::Execute()
{
#if defined(UT_MATHEMATICS_ARITHMETIC)
    UTInformation("Mathematics/Arithmetic");

#if defined(UT_MATHEMATICS_APCONVERSION)
    Precall();
    extern void TestAPConversion();
    TestAPConversion();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_APINTERVAL)
    Precall();
    extern void TestAPInterval();
    TestAPInterval();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_BITHACKS)
    Precall();
    extern void TestBitHacks();
    TestBitHacks();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_BSNUMBER)
    Precall();
    extern void TestBSNumber();
    TestBSNumber();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_BSPRECISION)
    Precall();
    extern void TestBSPrecision();
    TestBSPrecision();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_BSRATIONAL)
    Precall();
    extern void TestBSRational();
    TestBSRational();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONSTANTS)
    Precall();
    extern void TestConstants();
    TestConstants();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_IEEEBINARY)
    Precall();
    extern void TestIEEEBinary();
    TestIEEEBinary();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_IEEEBINARY16)
    Precall();
    extern void TestIEEEBinary16();
    TestIEEEBinary16();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_IEEEFUNCTIONS)
    Precall();
    extern void TestIEEEFunctions();
    TestIEEEFunctions();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_QFNUMBER)
    Precall();
    extern void TestQFNumber();
    TestQFNumber();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SWINTERVAL)
    Precall();
    extern void TestSWInterval();
    TestSWInterval();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_UINTEGERALU32)
    Precall();
    extern void TestUIntegerALU32();
    TestUIntegerALU32();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_UINTEGERAP32)
    Precall();
    extern void TestUIntegerAP32();
    TestUIntegerAP32();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_UINTEGERFP32)
    Precall();
    extern void TestUIntegerFP32();
    TestUIntegerFP32();
    Postcall();
#endif
#endif
}

void UTMathematics::Containment2D::Execute()
{
#if defined(UT_MATHEMATICS_CONTAINMENT_2D)
    UTInformation("Mathematics/Containment/2D");

#if defined(UT_MATHEMATICS_CONTCIRCLE2)
    Precall();
    extern void TestContCircle2();
    TestContCircle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTELLIPSE2)
    Precall();
    extern void TestContEllipse2();
    TestContEllipse2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTELLIPSE2MINCR)
    Precall();
    extern void TestContEllipse2MinCR();
    TestContEllipse2MinCR();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTORIENTEDBOX2)
    Precall();
    extern void TestContOrientedBox2();
    TestContOrientedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTPOINTINPOLYGON2)
    Precall();
    extern void TestContPointInPolygon2();
    TestContPointInPolygon2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTSCRIBECIRCLE2)
    Precall();
    extern void TestContScribeCircle2();
    TestContScribeCircle2();
    Postcall();
#endif
#endif
}

void UTMathematics::Containment3D::Execute()
{
#if defined(UT_MATHEMATICS_CONTAINMENT_3D)
    UTInformation("Mathematics/Containment/3D");

#if defined(UT_MATHEMATICS_CONTCAPSULE3)
    Precall();
    extern void TestContCapsule3();
    TestContCapsule3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTCYLINDER3)
    Precall();
    extern void TestContCylinder3();
    TestContCylinder3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTELLIPSOID3)
    Precall();
    extern void TestContEllipsoid3();
    TestContEllipsoid3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTELLIPSOID3MINCR)
    Precall();
    extern void TestContEllipsoid3MinCR();
    TestContEllipsoid3MinCR();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTLOZENGE3)
    Precall();
    extern void TestContLozenge3();
    TestContLozenge3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTORIENTEDBOX3)
    Precall();
    extern void TestContOrientedBox3();
    TestContOrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTPOINTINPOLYHEDRON3)
    Precall();
    extern void TestContPointInPolyhedron3();
    TestContPointInPolyhedron3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTSCRIBECIRCLE3)
    Precall();
    extern void TestContScribeCircle3();
    TestContScribeCircle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTSCRIBESPHERE3)
    Precall();
    extern void TestContScribeSphere3();
    TestContScribeSphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTSPHERE3)
    Precall();
    extern void TestContSphere3();
    TestContSphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTTETRAHEDRON3)
    Precall();
    extern void TestContTetrahedron3();
    TestContTetrahedron3();
    Postcall();
#endif
#endif
}

void UTMathematics::ContainmentND::Execute()
{
#if defined(UT_MATHEMATICS_CONTAINMENT_ND)
    UTInformation("Mathematics/Containment/ND");

#if defined(UT_MATHEMATICS_CONTALIGNEDBOX)
    Precall();
    extern void TestContAlignedBox();
    TestContAlignedBox();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONTCONE)
    Precall();
    extern void TestContCone();
    TestContCone();
    Postcall();
#endif
#endif
}

void UTMathematics::Curves::Execute()
{
#if defined(UT_MATHEMATICS_CURVES)
    UTInformation("Mathematics/Curves");

#if defined(UT_MATHEMATICS_BASISFUNCTION)
    Precall();
    extern void TestBasisFunction();
    TestBasisFunction();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_BEZIERCURVE)
    Precall();
    extern void TestBezierCurve();
    TestBezierCurve();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_BSPLINECURVE)
    Precall();
    extern void TestBSplineCurve();
    TestBSplineCurve();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_BSPLINECURVEFIT)
    Precall();
    extern void TestBSplineCurveFit();
    TestBSplineCurveFit();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_BSPLINEGEODESIC)
    Precall();
    extern void TestBSplineGeodesic();
    TestBSplineGeodesic();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ELLIPSOIDGEODESIC)
    Precall();
    extern void TestEllipsoidGeodesic();
    TestEllipsoidGeodesic();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_FRENETFRAME)
    Precall();
    extern void TestFrenetFrame();
    TestFrenetFrame();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_IMPLICITCURVE2)
    Precall();
    extern void TestImplicitCurve2();
    TestImplicitCurve2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_NATURALCUBICSPLINE)
    Precall();
    extern void TestNaturalCubicSpline();
    TestNaturalCubicSpline();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_NATURALQUINTICSPLINE)
    Precall();
    extern void TestNaturalQuinticSpline();
    TestNaturalQuinticSpline();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_NURBSCIRCLE)
    Precall();
    extern void TestNURBSCircle();
    TestNURBSCircle();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_NURBSCURVE)
    Precall();
    extern void TestNURBSCurve();
    TestNURBSCurve();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_PARAMETRICCURVE)
    Precall();
    extern void TestParametricCurve();
    TestParametricCurve();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_POLYNOMIALCURVE)
    Precall();
    extern void TestPolynomialCurve();
    TestPolynomialCurve();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_REPARAMETERIZEBYARCLENGTH)
    Precall();
    extern void TestReparameterizeByArclength();
    TestReparameterizeByArclength();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_RIEMANNIANGEODESIC)
    Precall();
    extern void TestRiemannianGeodesic();
    TestRiemannianGeodesic();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_TCBSPLINECURVE)
    Precall();
    extern void TestTCBSplineCurve();
    TestTCBSplineCurve();
    Postcall();
#endif
#endif
}

void UTMathematics::DifferentialEquations::Execute()
{
#if defined(UT_MATHEMATICS_DIFFERENTIALEQUATIONS)
    UTInformation("Mathematics/DifferentialEquations");

#if defined(UT_MATHEMATICS_ODEEULER)
    Precall();
    extern void TestOdeEuler();
    TestOdeEuler();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ODEIMPLICITEULER)
    Precall();
    extern void TestOdeImplicitEuler();
    TestOdeImplicitEuler();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ODEMIDPOINT)
    Precall();
    extern void TestOdeMidpoint();
    TestOdeMidpoint();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ODERUNGEKUTTA4)
    Precall();
    extern void TestOdeRungeKutta4();
    TestOdeRungeKutta4();
    Postcall();
#endif
#endif
}

void UTMathematics::Distance2D::Execute()
{
#if defined(UT_MATHEMATICS_DISTANCE_2D)
    UTInformation("Mathematics/Distance/2D");

#if defined(UT_MATHEMATICS_DISTCIRCLE2CIRCLE2)
    Precall();
    extern void TestDistCircle2Circle2();
    TestDistCircle2Circle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINE2ALIGNEDBOX2)
    Precall();
    extern void TestDistLine2AlignedBox2();
    TestDistLine2AlignedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINE2ARC2)
    Precall();
    extern void TestDistLine2Arc2();
    TestDistLine2Arc2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINE2CIRCLE2)
    Precall();
    extern void TestDistLine2Circle2();
    TestDistLine2Circle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINE2ORIENTEDBOX2)
    Precall();
    extern void TestDistLine2OrientedBox2();
    TestDistLine2OrientedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINE2TRIANGLE2)
    Precall();
    extern void TestDistLine2Triangle2();
    TestDistLine2Triangle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINT2ARC2)
    Precall();
    extern void TestDistPoint2Arc2();
    TestDistPoint2Arc2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINT2CIRCLE2)
    Precall();
    extern void TestDistPoint2Circle2();
    TestDistPoint2Circle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINT2PARALLELOGRAM2)
    Precall();
    extern void TestDistPoint2Parallelogram2();
    TestDistPoint2Parallelogram2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAY2ALIGNEDBOX2)
    Precall();
    extern void TestDistRay2AlignedBox2();
    TestDistRay2AlignedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAY2ARC2)
    Precall();
    extern void TestDistRay2Arc2();
    TestDistRay2Arc2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAY2CIRCLE2)
    Precall();
    extern void TestDistRay2Circle2();
    TestDistRay2Circle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAY2ORIENTEDBOX2)
    Precall();
    extern void TestDistRay2OrientedBox2();
    TestDistRay2OrientedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAY2TRIANGLE2)
    Precall();
    extern void TestDistRay2Triangle2();
    TestDistRay2Triangle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTSEGMENT2ALIGNEDBOX2)
    Precall();
    extern void TestDistSegment2AlignedBox2();
    TestDistSegment2AlignedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTSEGMENT2ARC2)
    Precall();
    extern void TestDistSegment2Arc2();
    TestDistSegment2Arc2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTSEGMENT2CIRCLE2)
    Precall();
    extern void TestDistSegment2Circle2();
    TestDistSegment2Circle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTSEGMENT2ORIENTEDBOX2)
    Precall();
    extern void TestDistSegment2OrientedBox2();
    TestDistSegment2OrientedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTSEGMENT2TRIANGLE2)
    Precall();
    extern void TestDistSegment2Triangle2();
    TestDistSegment2Triangle2();
    Postcall();
#endif
#endif
}

void UTMathematics::Distance3D::Execute()
{
#if defined(UT_MATHEMATICS_DISTANCE_3D)
    UTInformation("Mathematics/Distance/3D");

#if defined(UT_MATHEMATICS_DISTALIGNEDBOX3ORIENTEDBOX3)
    Precall();
    extern void TestDistAlignedBox3OrientedBox3();
    TestDistAlignedBox3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTCIRCLE3CIRCLE3)
    Precall();
    extern void TestDistCircle3Circle3();
    TestDistCircle3Circle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINE3ALIGNEDBOX3)
    Precall();
    extern void TestDistLine3AlignedBox3();
    TestDistLine3AlignedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINE3ARC3)
    Precall();
    extern void TestDistLine3Arc3();
    TestDistLine3Arc3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINE3CANONICALBOX3)
    Precall();
    extern void TestDistLine3CanonicalBox3();
    TestDistLine3CanonicalBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINE3CIRCLE3)
    Precall();
    extern void TestDistLine3Circle3();
    TestDistLine3Circle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINE3ORIENTEDBOX3)
    Precall();
    extern void TestDistLine3OrientedBox3();
    TestDistLine3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINE3RECTANGLE3)
    Precall();
    extern void TestDistLine3Rectangle3();
    TestDistLine3Rectangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINE3TRIANGLE3)
    Precall();
    extern void TestDistLine3Triangle3();
    TestDistLine3Triangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINEARCIRCULARSUPPORT3)
    Precall();
    extern void TestDistLinearCircularSupport3();
    TestDistLinearCircularSupport3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTORIENTEDBOX3CONE3)
    Precall();
    extern void TestDistOrientedBox3Cone3();
    TestDistOrientedBox3Cone3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTORIENTEDBOX3ORIENTEDBOX3)
    Precall();
    extern void TestDistOrientedBox3OrientedBox3();
    TestDistOrientedBox3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPLANE3ALIGNEDBOX3)
    Precall();
    extern void TestDistPlane3AlignedBox3();
    TestDistPlane3AlignedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPLANE3CANONICALBOX3)
    Precall();
    extern void TestDistPlane3CanonicalBox3();
    TestDistPlane3CanonicalBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPLANE3ORIENTEDBOX3)
    Precall();
    extern void TestDistPlane3OrientedBox3();
    TestDistPlane3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINT3ARC3)
    Precall();
    extern void TestDistPoint3Arc3();
    TestDistPoint3Arc3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINT3CIRCLE3)
    Precall();
    extern void TestDistPoint3Circle3();
    TestDistPoint3Circle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINT3CONVEXPOLYHEDRON3)
    Precall();
    extern void TestDistPoint3ConvexPolyhedron3();
    TestDistPoint3ConvexPolyhedron3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINT3CYLINDER3)
    Precall();
    extern void TestDistPoint3Cylinder3();
    TestDistPoint3Cylinder3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINT3FRUSTUM3)
    Precall();
    extern void TestDistPoint3Frustum3();
    TestDistPoint3Frustum3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINT3PARALLELEPIPED3)
    Precall();
    extern void TestDistPoint3Parallelepiped3();
    TestDistPoint3Parallelepiped3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINT3TETRAHEDRON3)
    Precall();
    extern void TestDistPoint3Tetrahedron3();
    TestDistPoint3Tetrahedron3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAY3ALIGNEDBOX3)
    Precall();
    extern void TestDistRay3AlignedBox3();
    TestDistRay3AlignedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAY3ARC3)
    Precall();
    extern void TestDistRay3Arc3();
    TestDistRay3Arc3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAY3CANONICALBOX3)
    Precall();
    extern void TestDistRay3CanonicalBox3();
    TestDistRay3CanonicalBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAY3CIRCLE3)
    Precall();
    extern void TestDistRay3Circle3();
    TestDistRay3Circle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAY3ORIENTEDBOX3)
    Precall();
    extern void TestDistRay3OrientedBox3();
    TestDistRay3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAY3RECTANGLE3)
    Precall();
    extern void TestDistRay3Rectangle3();
    TestDistRay3Rectangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAY3TRIANGLE3)
    Precall();
    extern void TestDistRay3Triangle3();
    TestDistRay3Triangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRECTANGLE3ALIGNEDBOX3)
    Precall();
    extern void TestDistRectangle3AlignedBox3();
    TestDistRectangle3AlignedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRECTANGLE3CANONICALBOX3)
    Precall();
    extern void TestDistRectangle3CanonicalBox3();
    TestDistRectangle3CanonicalBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRECTANGLE3ORIENTEDBOX3)
    Precall();
    extern void TestDistRectangle3OrientedBox3();
    TestDistRectangle3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRECTANGLE3RECTANGLE3)
    Precall();
    extern void TestDistRectangle3Rectangle3();
    TestDistRectangle3Rectangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTSEGMENT3ALIGNEDBOX3)
    Precall();
    extern void TestDistSegment3AlignedBox3();
    TestDistSegment3AlignedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTSEGMENT3ARC3)
    Precall();
    extern void TestDistSegment3Arc3();
    TestDistSegment3Arc3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTSEGMENT3CANONICALBOX3)
    Precall();
    extern void TestDistSegment3CanonicalBox3();
    TestDistSegment3CanonicalBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTSEGMENT3CIRCLE3)
    Precall();
    extern void TestDistSegment3Circle3();
    TestDistSegment3Circle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTSEGMENT3ORIENTEDBOX3)
    Precall();
    extern void TestDistSegment3OrientedBox3();
    TestDistSegment3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTSEGMENT3RECTANGLE3)
    Precall();
    extern void TestDistSegment3Rectangle3();
    TestDistSegment3Rectangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTSEGMENT3TRIANGLE3)
    Precall();
    extern void TestDistSegment3Triangle3();
    TestDistSegment3Triangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTTETRAHEDRON3TETRAHEDRON3)
    Precall();
    extern void TestDistTetrahedron3Tetrahedron3();
    TestDistTetrahedron3Tetrahedron3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTTRIANGLE3ALIGNEDBOX3)
    Precall();
    extern void TestDistTriangle3AlignedBox3();
    TestDistTriangle3AlignedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTTRIANGLE3CANONICALBOX3)
    Precall();
    extern void TestDistTriangle3CanonicalBox3();
    TestDistTriangle3CanonicalBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTTRIANGLE3ORIENTEDBOX3)
    Precall();
    extern void TestDistTriangle3OrientedBox3();
    TestDistTriangle3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTTRIANGLE3TRIANGLE3)
    Precall();
    extern void TestDistTriangle3Triangle3();
    TestDistTriangle3Triangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTTRIANGLE3RECTANGLE3)
    Precall();
    extern void TestDistTriangle3Rectangle3();
    TestDistTriangle3Rectangle3();
    Postcall();
#endif
#endif
}

void UTMathematics::DistanceND::Execute()
{
#if defined(UT_MATHEMATICS_DISTANCE_ND)
    UTInformation("Mathematics/Distance/ND");

#if defined(UT_MATHEMATICS_DISTALIGNEDBOXALIGNEDBOX)
    Precall();
    extern void TestDistAlignedBoxAlignedBox();
    TestDistAlignedBoxAlignedBox();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINELINE)
    Precall();
    extern void TestDistLineLine();
    TestDistLineLine();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINERAY)
    Precall();
    extern void TestDistLineRay();
    TestDistLineRay();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTLINESEGMENT)
    Precall();
    extern void TestDistLineSegment();
    TestDistLineSegment();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINTALIGNEDBOX)
    Precall();
    extern void TestDistPointAlignedBox();
    TestDistPointAlignedBox();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINTCANONICALBOX)
    Precall();
    extern void TestDistPointCanonicalBox();
    TestDistPointCanonicalBox();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINTHYPERELLIPSOID)
    Precall();
    extern void TestDistPointHyperellipsoid();
    TestDistPointHyperellipsoid();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINTHYPERPLANE)
    Precall();
    extern void TestDistPointHyperplane();
    TestDistPointHyperplane();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINTLINE)
    Precall();
    extern void TestDistPointLine();
    TestDistPointLine();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINTORIENTEDBOX)
    Precall();
    extern void TestDistPointOrientedBox();
    TestDistPointOrientedBox();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINTRAY)
    Precall();
    extern void TestDistPointRay();
    TestDistPointRay();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINTRECTANGLE)
    Precall();
    extern void TestDistPointRectangle();
    TestDistPointRectangle();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINTSEGMENT)
    Precall();
    extern void TestDistPointSegment();
    TestDistPointSegment();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTPOINTTRIANGLE)
    Precall();
    extern void TestDistPointTriangle();
    TestDistPointTriangle();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAYRAY)
    Precall();
    extern void TestDistRayRay();
    TestDistRayRay();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTRAYSEGMENT)
    Precall();
    extern void TestDistRaySegment();
    TestDistRaySegment();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISTSEGMENTSEGMENT)
    Precall();
    extern void TestDistSegmentSegment();
    TestDistSegmentSegment();
    Postcall();
#endif
#endif
}

void UTMathematics::Functions::Execute()
{
#if defined(UT_MATHEMATICS_FUNCTIONS)
    UTInformation("Mathematics/Functions");

#if defined(UT_MATHEMATICS_ACOSESTIMATE)
    Precall();
    extern void TestACosEstimate();
    TestACosEstimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ASINESTIMATE)
    Precall();
    extern void TestASinEstimate();
    TestASinEstimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ATANESTIMATE)
    Precall();
    extern void TestATanEstimate();
    TestATanEstimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CHEBYSHEVRATIO)
    Precall();
    extern void TestChebyshevRatio();
    TestChebyshevRatio();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CHEBYSHEVRATIOESTIMATE)
    Precall();
    extern void TestChebyshevRatioEstimate();
    TestChebyshevRatioEstimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_COSESTIMATE)
    Precall();
    extern void TestCosEstimate();
    TestCosEstimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXP2ESTIMATE)
    Precall();
    extern void TestExp2Estimate();
    TestExp2Estimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXPESTIMATE)
    Precall();
    extern void TestExpEstimate();
    TestExpEstimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INVSQRTESTIMATE)
    Precall();
    extern void TestInvSqrtEstimate();
    TestInvSqrtEstimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_LOG2ESTIMATE)
    Precall();
    extern void TestLog2Estimate();
    TestLog2Estimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_LOGESTIMATE)
    Precall();
    extern void TestLogEstimate();
    TestLogEstimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_REMEZALGORITHM)
    Precall();
    extern void TestRemezAlgorithm();
    TestRemezAlgorithm();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ROTATIONESTIMATE)
    Precall();
    extern void TestRotationEstimate();
    TestRotationEstimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SINESTIMATE)
    Precall();
    extern void TestSinEstimate();
    TestSinEstimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SLERP)
    Precall();
    extern void TestSlerp();
    TestSlerp();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SLERPESTIMATE)
    Precall();
    extern void TestSlerpEstimate();
    TestSlerpEstimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SQRTESTIMATE)
    Precall();
    extern void TestSqrtEstimate();
    TestSqrtEstimate();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_TANESTIMATE)
    Precall();
    extern void TestTanEstimate();
    TestTanEstimate();
    Postcall();
#endif
#endif
}

void UTMathematics::Geometry1D::Execute()
{
#if defined(UT_MATHEMATICS_GEOMETRY_1D)
    UTInformation("Mathematics/Geometry/1D");

#if defined(UT_MATHEMATICS_DISJOINTINTERVALS)
    Precall();
    extern void TestDisjointIntervals();
    TestDisjointIntervals();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXACTSIGNDETERMINANT2)
    Precall();
    extern void TestExactSignDeterminant2();
    TestExactSignDeterminant2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXACTSIGNDETERMINANT3)
    Precall();
    extern void TestExactSignDeterminant3();
    TestExactSignDeterminant3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXACTSIGNDETERMINANT4)
    Precall();
    extern void TestExactSignDeterminant4();
    TestExactSignDeterminant4();
    Postcall();
#endif
#endif
}

void UTMathematics::Geometry2D::Execute()
{
#if defined(UT_MATHEMATICS_GEOMETRY_2D)
    UTInformation("Mathematics/Geometry/2D");

#if defined(UT_MATHEMATICS_BSPPOLYGON2)
    Precall();
    extern void TestBSPPolygon2();
    TestBSPPolygon2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONSTRAINEDDELAUNAY2)
    Precall();
    extern void TestConstrainedDelaunay2();
    TestConstrainedDelaunay2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONVEXHULL2)
    Precall();
    extern void TestConvexHull2();
    TestConvexHull2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONVEXHULLSIMPLEPOLYGON)
    Precall();
    extern void TestConvexHullSimplePolygon();
    TestConvexHullSimplePolygon();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DELAUNAY2)
    Precall();
    extern void TestDelaunay2();
    TestDelaunay2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DISJOINTRECTANGLES)
    Precall();
    extern void TestDisjointRectangles();
    TestDisjointRectangles();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXACTCOLINEAR2)
    Precall();
    extern void TestExactColinear2();
    TestExactColinear2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXACTTOCIRCUMCIRCLE2)
    Precall();
    extern void TestExactToCircumcircle2();
    TestExactToCircumcircle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXACTTOLINE2)
    Precall();
    extern void TestExactToLine2();
    TestExactToLine2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXACTTOLINEEXTENDED2)
    Precall();
    extern void TestExactToLineExtended2();
    TestExactToLineExtended2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXACTTOTRIANGLE2)
    Precall();
    extern void TestExactToTriangle2();
    TestExactToTriangle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INCREMENTALDELAUNAY2)
    Precall();
    extern void TestIncrementalDelaunay2();
    TestIncrementalDelaunay2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ISPLANARGRAPH)
    Precall();
    extern void TestIsPlanarGraph();
    TestIsPlanarGraph();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MINIMALCYCLEBASIS)
    Precall();
    extern void TestMinimalCycleBasis();
    TestMinimalCycleBasis();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MINIMUMAREABOX2)
    Precall();
    extern void TestMinimumAreaBox2();
    TestMinimumAreaBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MINIMUMAREACIRCLE2)
    Precall();
    extern void TestMinimumAreaCircle2();
    TestMinimumAreaCircle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MINIMUMWIDTHPOINTS2)
    Precall();
    extern void TestMinimumWidthPoints2();
    TestMinimumAreaBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_POLYGONTREE)
    Precall();
    extern void TestPolygonTree();
    TestPolygonTree();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_POLYGONWINDINGORDER)
    Precall();
    extern void TestPolygonWindingOrder();
    TestPolygonWindingOrder();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ROTATINGCALIPERS)
    Precall();
    extern void TestRotatingCalipers();
    TestRotatingCalipers();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SEPARATEPOINTS2)
    Precall();
    extern void TestSeparatePoints2();
    TestSeparatePoints2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_TRIANGULATECDT)
    Precall();
    extern void TestTriangulateCDT();
    TestTriangulateCDT();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_TRIANGULATEEC)
    Precall();
    extern void TestTriangulateEC();
    TestTriangulateEC();
    Postcall();
#endif
#endif
}

void UTMathematics::Geometry3D::Execute()
{
#if defined(UT_MATHEMATICS_GEOMETRY_3D)
    UTInformation("Mathematics/Geometry/3D");

#if defined(UT_MATHEMATICS_ALIGNEDBOXBV)
    Precall();
    extern void TestAlignedBoxBV();
    TestAlignedBoxBV();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ALIGNEDBOXTREEOFPOINTS)
    Precall();
    extern void TestAlignedBoxTreeOfPoints();
    TestAlignedBoxTreeOfPoints();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ALIGNEDBOXTREEOFSEGMENTS)
    Precall();
    extern void TestAlignedBoxTreeOfSegments();
    TestAlignedBoxTreeOfSegments();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ALIGNEDBOXTREEOFTRIANGLES)
    Precall();
    extern void TestAlignedBoxTreeOfTriangles();
    TestAlignedBoxTreeOfTriangles();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONFORMALMAPGENUS0)
    Precall();
    extern void TestConformalMapGenus0();
    TestConformalMapGenus0();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONVEXHULL3)
    Precall();
    extern void TestConvexHull3();
    TestConvexHull3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DELAUNAY3)
    Precall();
    extern void TestDelaunay3();
    TestDelaunay3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXACTCOLINEAR3)
    Precall();
    extern void TestExactColinear3();
    TestExactColinear3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXACTCOPLANAR3)
    Precall();
    extern void TestExactCoplanar3();
    TestExactCoplanar3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXACTTOCIRCUMSPHERE3)
    Precall();
    extern void TestExactToCircumsphere3();
    TestExactToCircumsphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXACTTOPLANE3)
    Precall();
    extern void TestExactToPlane3();
    TestExactToPlane3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXACTTOTETRAHEDRON3)
    Precall();
    extern void TestExactToTetrahedron3();
    TestExactToTetrahedron3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXTREMALQUERY3BSP)
    Precall();
    extern void TestExtremalQuery3BSP();
    TestExtremalQuery3BSP();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EXTREMALQUERY3PRJ)
    Precall();
    extern void TestExtremalQuery3PRJ();
    TestExtremalQuery3PRJ();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_GENERATEMESHUV)
    Precall();
    extern void TestGenerateMeshUV();
    TestGenerateMeshUV();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MINIMUMVOLUMEBOX3)
    Precall();
    extern void TestMinimumVolumeBox3();
    TestMinimumVolumeBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MINIMUMVOLUMESPHERE3)
    Precall();
    extern void TestMinimumVolumeSphere3();
    TestMinimumVolumeSphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ORIENTEDBOXBV)
    Precall();
    extern void TestOrientedBoxBV();
    TestOrientedBoxBV();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ORIENTEDBOXTREEOFPOINTS)
    Precall();
    extern void TestOrientedBoxTreeOfPoints();
    TestOrientedBoxTreeOfPoints();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ORIENTEDBOXTREEOFSEGMENTS)
    Precall();
    extern void TestOrientedBoxTreeOfSegments();
    TestOrientedBoxTreeOfSegments();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ORIENTEDBOXTREEOFTRIANGLES)
    Precall();
    extern void TestOrientedBoxTreeOfTriangles();
    TestOrientedBoxTreeOfTriangles();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SEPARATEPOINTS3)
    Precall();
    extern void TestSeparatePoints3();
    TestSeparatePoints3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SPLITMESHBYPLANE)
    Precall();
    extern void TestSplitMeshByPlane();
    TestSplitMeshByPlane();
    Postcall();
#endif
#endif
}

void UTMathematics::GeometryND::Execute()
{
#if defined(UT_MATHEMATICS_GEOMETRY_ND)
    UTInformation("Mathematics/Geometry/ND");

#if defined(UT_MATHEMATICS_CLODPOLYLINE)
    Precall();
    extern void TestCLODPolyline();
    TestCLODPolyline();
    Postcall();
#endif
#endif
}

void UTMathematics::ImageProcessing::Execute()
{
#if defined(UT_MATHEMATICS_IMAGEPROCESSING)
    UTInformation("Mathematics/ImageProcessing");

#if defined(UT_MATHEMATICS_CURVATUREFLOW2)
    Precall();
    extern void TestCurvatureFlow2();
    TestCurvatureFlow2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CURVATUREFLOW3)
    Precall();
    extern void TestCurvatureFlow3();
    TestCurvatureFlow3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CURVEEXTRACTOR)
    Precall();
    extern void TestCurveExtractor();
    TestCurveExtractor();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CURVEEXTRACTORSQUARES)
    Precall();
    extern void TestCurveExtractorSquares();
    TestCurveExtractorSquares();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CURVEEXTRACTORTRIANGLES)
    Precall();
    extern void TestCurveExtractorTriangles();
    TestCurveExtractorTriangles();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_FASTGAUSSIANBLUR1)
    Precall();
    extern void TestFastGaussianBlur1();
    TestFastGaussianBlur1();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_FASTGAUSSIANBLUR2)
    Precall();
    extern void TestFastGaussianBlur2();
    TestFastGaussianBlur2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_FASTGAUSSIANBLUR3)
    Precall();
    extern void TestFastGaussianBlur3();
    TestFastGaussianBlur3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_FASTMARCH)
    Precall();
    extern void TestFastMarch();
    TestFastMarch();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_GAUSSIANBLUR2)
    Precall();
    extern void TestGaussianBlur2();
    TestGaussianBlur2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_GAUSSIANBLUR3)
    Precall();
    extern void TestGaussianBlur3();
    TestGaussianBlur3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_GRADIENTANISOTROPIC2)
    Precall();
    extern void TestGradientAnisotropic2();
    TestGradientAnisotropic2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_GRADIENTANISOTROPIC3)
    Precall();
    extern void TestGradientAnisotropic3();
    TestGradientAnisotropic3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_IMAGE2)
    Precall();
    extern void TestImage2();
    TestImage2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_IMAGE3)
    Precall();
    extern void TestImage3();
    TestImage3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MARCHINGCUBES)
    Precall();
    extern void TestMarchingCubes();
    TestMarchingCubes();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MORPHOLOGY)
    Precall();
    extern void TestMorphology();
    TestMorphology();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MORPHOLOGY2)
    Precall();
    extern void TestMorphology2();
    TestMorphology2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MORPHOLOGY3)
    Precall();
    extern void TestMorphology3();
    TestMorphology3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_PDEFILTER)
    Precall();
    extern void TestPdeFilter();
    TestPdeFilter();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_PDEFILTER1)
    Precall();
    extern void TestPdeFilter1();
    TestPdeFilter1();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_PDEFILTER2)
    Precall();
    extern void TestPdeFilter2();
    TestPdeFilter2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_PDEFILTER3)
    Precall();
    extern void TestPdeFilter3();
    TestPdeFilter3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_RASTERIZE2)
    Precall();
    extern void TestRasterize2();
    TestRasterize2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_RASTERIZE3)
    Precall();
    extern void TestRasterize3();
    TestRasterize3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SURFACEEXTRACTOR)
    Precall();
    extern void TestSurfaceExtractor();
    TestSurfaceExtractor();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SURFACEEXTRACTORCUBES)
    Precall();
    extern void TestSurfaceExtractorCubes();
    TestSurfaceExtractorCubes();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SURFACEEXTRACTORMC)
    Precall();
    extern void TestSurfaceExtractorMC();
    TestSurfaceExtractorMC();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SURFACEEXTRACTORTETRAHEDRA)
    Precall();
    extern void TestSurfaceExtractorTetrahedra();
    TestSurfaceExtractorTetrahedra();
    Postcall();
#endif
#endif
}

void UTMathematics::Integration::Execute()
{
#if defined(UT_MATHEMATICS_INTEGRATION)
    UTInformation("Mathematics/Integration");

#if defined(UT_MATHEMATICS_INTGGAUSSIANQUADRATURE)
    Precall();
    extern void TestIntgGaussianQuadrature();
    TestIntgGaussianQuadrature();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTGROMBERG)
    Precall();
    extern void TestIntgRomberg();
    TestIntgRomberg();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTGTRAPEZOIDRULE)
    Precall();
    extern void TestIntgTrapezoidRule();
    TestIntgTrapezoidRule();
    Postcall();
#endif
#endif
}

void UTMathematics::Interpolation1D::Execute()
{
#if defined(UT_MATHEMATICS_INTERPOLATION_1D)
    UTInformation("Mathematics/Interpolation/1D");

#if defined(UT_MATHEMATICS_INTPAKIMANONUNIFORM1)
    Precall();
    extern void TestIntpAkimaNonuniform1();
    TestIntpAkimaNonuniform1();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPAKIMAUNIFORM1)
    Precall();
    extern void TestIntpAkimaUniform1();
    TestIntpAkimaUniform1();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPBSPLINEUNIFORM1)
    Precall();
    extern void TestIntpBSplineUniform1();
    TestIntpBSplineUniform1();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPHERMITECUBIC1)
    Precall();
    extern void TestIntpHermiteCubic1();
    TestIntpHermiteCubic1();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPHERMITEQUINTIC1)
    Precall();
    extern void TestIntpHermiteQuintic1();
    TestIntpHermiteQuintic1();
    Postcall();
#endif
#endif
}

void UTMathematics::Interpolation2D::Execute()
{
#if defined(UT_MATHEMATICS_INTERPOLATION_2D)
    UTInformation("Mathematics/Interpolation/2D");

#if defined(UT_MATHEMATICS_INTPAKIMAUNIFORM2)
    Precall();
    extern void TestIntpAkimaUniform2();
    TestIntpAkimaUniform2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPBSPLINEUNIFORM2)
    Precall();
    extern void TestIntpBSplineUniform2();
    TestIntpBSplineUniform2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPCUBIC2)
    Precall();
    extern void TestIntpCubic2();
    TestIntpCubic2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPHERMITECUBIC2)
    Precall();
    extern void TestIntpHermiteCubic2();
    TestIntpHermiteCubic2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPHERMITEQUINTIC2)
    Precall();
    extern void TestIntpHermiteQuintic2();
    TestIntpHermiteQuintic2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPLINEAR2)
    Precall();
    extern void TestIntpLinear2();
    TestIntpLinear2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPLINEARNONUNIFORM2)
    Precall();
    extern void TestIntpLinearNonuniform2();
    TestIntpLinearNonuniform2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPQUADRATICNONUNIFORM2)
    Precall();
    extern void TestIntpQuadraticNonuniform2();
    TestIntpQuadraticNonuniform2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPTHINPLATESPLINE2)
    Precall();
    extern void TestIntpThinPlateSpline2();
    TestIntpThinPlateSpline2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPVECTORFIELD2)
    Precall();
    extern void TestIntpVectorField2();
    TestIntpVectorField2();
    Postcall();
#endif
#endif
}

void UTMathematics::Interpolation3D::Execute()
{
#if defined(UT_MATHEMATICS_INTERPOLATION_3D)
    UTInformation("Mathematics/Interpolation");

#if defined(UT_MATHEMATICS_INTPAKIMAUNIFORM3)
    Precall();
    extern void TestIntpAkimaUniform3();
    TestIntpAkimaUniform3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPBSPLINEUNIFORM3)
    Precall();
    extern void TestIntpBSplineUniform3();
    TestIntpBSplineUniform3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPCUBIC3)
    Precall();
    extern void TestIntpCubic3();
    TestIntpCubic3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPHERMITECUBIC3)
    Precall();
    extern void TestIntpHermiteCubic3();
    TestIntpHermiteCubic3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPHERMITEQUINTIC3)
    Precall();
    extern void TestIntpHermiteQuintic3();
    TestIntpHermiteQuintic3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPLINEAR3)
    Precall();
    extern void TestIntpLinear3();
    TestIntpLinear3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPLINEARNONUNIFORM3)
    Precall();
    extern void TestIntpLinearNonuniform3();
    TestIntpLinearNonuniform3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPSPHERE3)
    Precall();
    extern void TestIntpSphere3();
    TestIntpSphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTPTHINPLATESPLINE3)
    Precall();
    extern void TestIntpThinPlateSpline3();
    TestIntpThinPlateSpline3();
    Postcall();
#endif
#endif
}

void UTMathematics::InterpolationND::Execute()
{
#if defined(UT_MATHEMATICS_INTERPOLATION_ND)
    UTInformation("Mathematics/Interpolation");

#if defined(UT_MATHEMATICS_INTPBSPLINEUNIFORM)
    Precall();
    extern void TestIntpBSplineUniform();
    TestIntpBSplineUniform();
    Postcall();
#endif
#endif
}

void UTMathematics::Intersection1D::Execute()
{
#if defined(UT_MATHEMATICS_INTERSECTION_1D)
    UTInformation("Mathematics/Intersection/1D");

#if defined(UT_MATHEMATICS_INTRINTERVALS)
    Precall();
    extern void TestIntrIntervals();
    TestIntrIntervals();
    Postcall();
#endif
#endif
}

void UTMathematics::Intersection2D::Execute()
{
#if defined(UT_MATHEMATICS_INTERSECTION_2D)
    UTInformation("Mathematics/Intersection/2D");

#if defined(UT_MATHEMATICS_INTRALIGNEDBOX2CIRCLE2)
    Precall();
    extern void TestIntrAlignedBox2Circle2();
    TestIntrAlignedBox2Circle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRALIGNEDBOX2ORIENTEDBOX2)
    Precall();
    extern void TestIntrAlignedBox2OrientedBox2();
    TestIntrAlignedBox2OrientedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRALIGNEDARC2ARC2)
    Precall();
    extern void TestIntrArc2Arc2();
    TestIntrArc2Arc2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRALIGNEDCIRCLE2ARC2)
    Precall();
    extern void TestIntrCircle2Arc2();
    TestIntrCircle2Arc2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRALIGNEDCIRCLE2CIRCLE2)
    Precall();
    extern void TestIntrCircle2Circle2();
    TestIntrCircle2Circle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRAREAELLIPSE2ELLIPSE2)
    Precall();
    extern void TestIntrAreaEllipse2Ellipse2();
    TestIntrAreaEllipse2Ellipse2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRELLIPSE2ELLIPSE2)
    Precall();
    extern void TestIntrEllipse2Ellipse2();
    TestIntrEllipse2Ellipse2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRHALFSPACE2POLYGON2)
    Precall();
    extern void TestIntrHalfspace2Polygon2();
    TestIntrHalfspace2Polygon2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE2ALIGNEDBOX2)
    Precall();
    extern void TestIntrLine2AlignedBox2();
    TestIntrLine2AlignedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE2ARC2)
    Precall();
    extern void TestIntrLine2Arc2();
    TestIntrLine2Arc2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE2CIRCLE2)
    Precall();
    extern void TestIntrLine2Circle2();
    TestIntrLine2Circle2();
    Postcall();
#endif


#if defined(UT_MATHEMATICS_INTRLINE2LINE2)
    Precall();
    extern void TestIntrLine2Line2();
    TestIntrLine2Line2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE2ORIENTEDBOX2)
    Precall();
    extern void TestIntrLine2OrientedBox2();
    TestIntrLine2OrientedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE2RAY2)
    Precall();
    extern void TestIntrLine2Ray2();
    TestIntrLine2Ray2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE2SEGMENT2)
    Precall();
    extern void TestIntrLine2Segment2();
    TestIntrLine2Segment2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE2TRIANGLE2)
    Precall();
    extern void TestIntrLine2Triangle2();
    TestIntrLine2Triangle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRORIENTEDBOX2CIRCLE2)
    Precall();
    extern void TestIntrOrientedBox2Circle2();
    TestIntrOrientedBox2Circle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRORIENTEDBOX2CONE2)
    Precall();
    extern void TestIntrOrientedBox2Cone2();
    TestIntrOrientedBox2Cone2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY2ALIGNEDBOX2)
    Precall();
    extern void TestIntrRay2AlignedBox2();
    TestIntrRay2AlignedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY2ARC2)
    Precall();
    extern void TestIntrRay2Arc2();
    TestIntrRay2Arc2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY2CIRCLE2)
    Precall();
    extern void TestIntrRay2Circle2();
    TestIntrRay2Circle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY2ORIENTEDBOX2)
    Precall();
    extern void TestIntrRay2OrientedBox2();
    TestIntrRay2OrientedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY2RAY2)
    Precall();
    extern void TestIntrRay2Ray2();
    TestIntrRay2Ray2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY2SEGMENT2)
    Precall();
    extern void TestIntrRay2Segment2();
    TestIntrRay2Segment2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY2TRIANGLE2)
    Precall();
    extern void TestIntrRay2Triangle2();
    TestIntrRay2Triangle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT2ALIGNEDBOX2)
    Precall();
    extern void TestIntrSegment2AlignedBox2();
    TestIntrSegment2AlignedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT2ARC2)
    Precall();
    extern void TestIntrSegment2Arc2();
    TestIntrSegment2Arc2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT2CIRCLE2)
    Precall();
    extern void TestIntrSegment2Circle2();
    TestIntrSegment2Circle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT2ORIENTEDBOX2)
    Precall();
    extern void TestIntrSegment2OrientedBox2();
    TestIntrSegment2OrientedBox2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT2SEGMENT2)
    Precall();
    extern void TestIntrSegment2Segment2();
    TestIntrSegment2Segment2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT2TRIANGLE2)
    Precall();
    extern void TestIntrSegment2Triangle2();
    TestIntrSegment2Triangle2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRTRIANGLE2TRIANGLE2)
    Precall();
    extern void TestIntrTriangle2Triangle2();
    TestIntrTriangle2Triangle2();
    Postcall();
#endif
#endif
}

void UTMathematics::Intersection3D::Execute()
{
#if defined(UT_MATHEMATICS_INTERSECTION_3D)
    UTInformation("Mathematics/Intersection/3D");

#if defined(UT_MATHEMATICS_INTRALIGNEDBOX3CONE3)
    Precall();
    extern void TestIntrAlignedBox3Cone3();
    TestIntrAlignedBox3Cone3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRALIGNEDBOX3CYLINDER3)
    Precall();
    extern void TestIntrAlignedBox3Cylinder3();
    TestIntrAlignedBox3Cylinder3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRALIGNEDBOX3ORIENTEDBOX3)
    Precall();
    extern void TestIntrAlignedBox3OrientedBox3();
    TestIntrAlignedBox3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRALIGNEDBOX3SPHERE3)
    Precall();
    extern void TestIntrAlignedBox3Sphere3();
    TestIntrAlignedBox3Sphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRCANONICALBOX3CYLINDER3)
    Precall();
    extern void TestIntrCanonicalBox3Cylinder3();
    TestIntrCanonicalBox3Cylinder3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRCAPSULE3CAPSULE3)
    Precall();
    extern void TestIntrCapsule3Capsule3();
    TestIntrCapsule3Capsule3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRCONVEXMESH3PLANE3)
    Precall();
    extern void TestIntrConvexMesh3Plane3();
    TestIntrConvexMesh3Plane3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRCYLINDER3CYLINDER3)
    Precall();
    extern void TestIntrCylinder3Cylinder3();
    TestIntrCylinder3Cylinder3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRELLIPSOID3ELLIPSOID3)
    Precall();
    extern void TestIntrEllipsoid3Ellipsoid3();
    TestIntrEllipsoid3Ellipsoid3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRHALFSPACE3CAPSULE3)
    Precall();
    extern void TestIntrHalfspace3Capsule3();
    TestIntrHalfspace3Capsule3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRHALFSPACE3CYLINDER3)
    Precall();
    extern void TestIntrHalfspace3Cylinder3();
    TestIntrHalfspace3Cylinder3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRHALFSPACE3ELLIPSOID3)
    Precall();
    extern void TestIntrHalfspace3Ellipsoid3();
    TestIntrHalfspace3Ellipsoid3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRHALFSPACE3ORIENTEDBOX3)
    Precall();
    extern void TestIntrHalfspace3OrientedBox3();
    TestIntrHalfspace3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRHALFSPACE3SEGMENT3)
    Precall();
    extern void TestIntrHalfspace3Segment3();
    TestIntrHalfspace3Segment3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRHALFSPACE3SPHERE3)
    Precall();
    extern void TestIntrHalfspace3Sphere3();
    TestIntrHalfspace3Sphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRHALFSPACE3TRIANGLE3)
    Precall();
    extern void TestIntrHalfspace3Triangle3();
    TestIntrHalfspace3Triangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE3ALIGNEDBOX3)
    Precall();
    extern void TestIntrLine3AlignedBox3();
    TestIntrLine3AlignedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE3CAPSULE3)
    Precall();
    extern void TestIntrLine3Capsule3();
    TestIntrLine3Capsule3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE3CONE3)
    Precall();
    extern void TestIntrLine3Cone3();
    TestIntrLine3Cone3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE3CYLINDER3)
    Precall();
    extern void TestIntrLine3Cylinder3();
    TestIntrLine3Cylinder3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE3ELLIPSOID3)
    Precall();
    extern void TestIntrLine3Ellipsoid3();
    TestIntrLine3Ellipsoid3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE3ORIENTEDBOX3)
    Precall();
    extern void TestIntrLine3OrientedBox3();
    TestIntrLine3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE3SPHERE3)
    Precall();
    extern void TestIntrLine3Sphere3();
    TestIntrLine3Sphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINE3TRIANGLE3)
    Precall();
    extern void TestIntrLine3Triangle3();
    TestIntrLine3Triangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRORIENTEDBOX3CONE3)
    Precall();
    extern void TestIntrOrientedBox3Cone3();
    TestIntrOrientedBox3Cone3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRORIENTEDBOX3CYLINDER3)
    Precall();
    extern void TestIntrOrientedBox3Cylinder3();
    TestIntrOrientedBox3Cylinder3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRORIENTEDBOX3FRUSTUM3)
    Precall();
    extern void TestIntrOrientedBox3Frustum3();
    TestIntrOrientedBox3Frustum3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRORIENTEDBOX3ORIENTEDBOX3)
    Precall();
    extern void TestIntrOrientedBox3OrientedBox3();
    TestIntrOrientedBox3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRORIENTEDBOX3SPHERE3)
    Precall();
    extern void TestIntrOrientedBox3Sphere3();
    TestIntrOrientedBox3Sphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRPLANE3CAPSULE3)
    Precall();
    extern void TestIntrPlane3Capsule3();
    TestIntrPlane3Capsule3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRPLANE3CIRCLE3)
    Precall();
    extern void TestIntrPlane3Circle3();
    TestIntrPlane3Circle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRPLANE3CYLINDER3)
    Precall();
    extern void TestIntrPlane3Cylinder3();
    TestIntrPlane3Cylinder3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRPLANE3ELLIPSOID3)
    Precall();
    extern void TestIntrPlane3Ellipsoid3();
    TestIntrPlane3Ellipsoid3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRPLANE3ORIENTEDBOX3)
    Precall();
    extern void TestIntrPlane3OrientedBox3();
    TestIntrPlane3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRPLANE3PLANE3)
    Precall();
    extern void TestIntrPlane3Plane3();
    TestIntrPlane3Plane3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRPLANE3SPHERE3)
    Precall();
    extern void TestIntrPlane3Sphere3();
    TestIntrPlane3Sphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRPLANE3TRIANGLE3)
    Precall();
    extern void TestIntrPlane3Triangle3();
    TestIntrPlane3Triangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY3ALIGNEDBOX3)
    Precall();
    extern void TestIntrRay3AlignedBox3();
    TestIntrRay3AlignedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY3CAPSULE3)
    Precall();
    extern void TestIntrRay3Capsule3();
    TestIntrRay3Capsule3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY3CONE3)
    Precall();
    extern void TestIntrRay3Cone3();
    TestIntrRay3Cone3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY3CYLINDER3)
    Precall();
    extern void TestIntrRay3Cylinder3();
    TestIntrRay3Cylinder3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY3ELLIPSOID3)
    Precall();
    extern void TestIntrRay3Ellipsoid3();
    TestIntrRay3Ellipsoid3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY3ORIENTEDBOX3)
    Precall();
    extern void TestIntrRay3OrientedBox3();
    TestIntrRay3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY3SPHERE3)
    Precall();
    extern void TestIntrRay3Sphere3();
    TestIntrRay3Sphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAY3TRIANGLE3)
    Precall();
    extern void TestIntrRay3Triangle3();
    TestIntrRay3Triangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT3ALIGNEDBOX3)
    Precall();
    extern void TestIntrSegment3AlignedBox3();
    TestIntrSegment3AlignedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT3CAPSULE3)
    Precall();
    extern void TestIntrSegment3Capsule3();
    TestIntrSegment3Capsule3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT3CONE3)
    Precall();
    extern void TestIntrSegment3Cone3();
    TestIntrSegment3Cone3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT3CYLINDER3)
    Precall();
    extern void TestIntrSegment3Cylinder3();
    TestIntrSegment3Cylinder3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT3ELLIPSOID3)
    Precall();
    extern void TestIntrSegment3Ellipsoid3();
    TestIntrSegment3Ellipsoid3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT3ORIENTEDBOX3)
    Precall();
    extern void TestIntrSegment3OrientedBox3();
    TestIntrSegment3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT3SPHERE3)
    Precall();
    extern void TestIntrSegment3Sphere3();
    TestIntrSegment3Sphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENT3TRIANGLE3)
    Precall();
    extern void TestIntrSegment3Triangle3();
    TestIntrSegment3Triangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSPHERE3CONE3)
    Precall();
    extern void TestIntrSphere3Cone3();
    TestIntrSphere3Cone3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSPHERE3FRUSTUM3)
    Precall();
    extern void TestIntrSphere3Frustum3();
    TestIntrSphere3Frustum3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSPHERE3SPHERE3)
    Precall();
    extern void TestIntrSphere3Sphere3();
    TestIntrSphere3Sphere3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSPHERE3TRIANGLE3)
    Precall();
    extern void TestIntrSphere3Triangle3();
    TestIntrSphere3Triangle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRTETRAHEDRON3TETRAHEDRON3)
    Precall();
    extern void TestIntrTetrahedron3Tetrahedron3();
    TestIntrTetrahedron3Tetrahedron3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRTRIANGLE3CYLINDER3)
    Precall();
    extern void TestIntrTriangle3Cylinder3();
    TestIntrTriangle3Cylinder3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRTRIANGLE3ORIENTEDBOX3)
    Precall();
    extern void TestIntrTriangle3OrientedBox3();
    TestIntrTriangle3OrientedBox3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRTRIANGLE3TRIANGLE3)
    Precall();
    extern void TestIntrTriangle3Triangle3();
    TestIntrTriangle3Triangle3();
    Postcall();
#endif
#endif
}

void UTMathematics::IntersectionND::Execute()
{
#if defined(UT_MATHEMATICS_INTERSECTION_ND)
    UTInformation("Mathematics/Intersection/ND");

#if defined(UT_MATHEMATICS_INTRALIGNEDBOXALIGNEDBOX)
    Precall();
    extern void TestIntrAlignedBoxAlignedBox();
    TestIntrAlignedBoxAlignedBox();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRCONVEXPOLYGONHYPERPLANE)
    Precall();
    extern void TestIntrConvexPolygonHyperplane();
    TestIntrConvexPolygonHyperplane();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRLINEHYPERPLANE)
    Precall();
    extern void TestIntrLineHyperplane();
    TestIntrLineHyperplane();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRRAYHYPERPLANE)
    Precall();
    extern void TestIntrRayHyperplane();
    TestIntrRayHyperplane();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INTRSEGMENTHYPERPLANE)
    Precall();
    extern void TestIntrSegmentHyperplane();
    TestIntrSegmentHyperplane();
    Postcall();
#endif
#endif
}

void UTMathematics::MatrixAnalysis::Execute()
{
#if defined(UT_MATHEMATICS_MATRIXANALYSIS)
    UTInformation("Mathematics/MatrixAnalysis");

#if defined(UT_MATHEMATICS_BANDEDMATRIX)
    Precall();
    extern void TestBandedMatrix();
    TestBandedMatrix();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_BLOCKCHOLESKYDECOMPOSITION)
    Precall();
    extern void TestBlockCholeskyDecomposition();
    TestBlockCholeskyDecomposition();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_BLOCKLDLTDECOMPOSITION)
    Precall();
    extern void TestBlockLDLTDecomposition();
    TestBlockLDLTDecomposition();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CHOLESKYDECOMPOSITION)
    Precall();
    extern void TestCholeskyDecomposition();
    TestCholeskyDecomposition();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_GAUSSIANELIMINATION)
    Precall();
    extern void TestGaussianElimination();
    TestGaussianElimination();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_LDLTDECOMPOSITION)
    Precall();
    extern void TestLDLTDecomposition();
    TestLDLTDecomposition();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_LINEARSYSTEM)
    Precall();
    extern void TestLinearSystem();
    TestLinearSystem();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SINGULARVALUEDECOMPOSITION)
    Precall();
    extern void TestSingularValueDecomposition();
    TestSingularValueDecomposition();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SYMMETRICEIGENSOLVER)
    Precall();
    extern void TestSymmetricEigensolver();
    TestSymmetricEigensolver();
    Postcall();
#endif
#endif
}

void UTMathematics::Meshes::Execute()
{
#if defined(UT_MATHEMATICS_MESHES)
    UTInformation("Mathematics/Meshes");

#if defined(UT_MATHEMATICS_DYNAMICETMANIFOLDMESH)
    Precall();
    extern void TestDynamicETManifoldMesh();
    TestDynamicETManifoldMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DYNAMICETNONMANIFOLDMESH)
    Precall();
    extern void TestDynamicETNonmanifoldMesh();
    TestDynamicETNonmanifoldMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DYNAMICTSMANIFOLDMESH)
    Precall();
    extern void TestDynamicTSManifoldMesh();
    TestDynamicTSManifoldMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DYNAMICVETMANIFOLDMESH)
    Precall();
    extern void TestDynamicVETManifoldMesh();
    TestDynamicVETManifoldMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DYNAMICVETNONMANIFOLDMESH)
    Precall();
    extern void TestDynamicVETNonmanifoldMesh();
    TestDynamicVETNonmanifoldMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DYNAMICVTSMANIFOLDMESH)
    Precall();
    extern void TestDynamicVTSManifoldMesh();
    TestDynamicVTSManifoldMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_EDGEKEY)
    Precall();
    extern void TestEdgeKey();
    TestEdgeKey();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_FEATUREKEY)
    Precall();
    extern void TestFeatureKey();
    TestFeatureKey();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INDEXATTRIBUTE)
    Precall();
    extern void TestIndexAttribute();
    TestIndexAttribute();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MESH)
    Precall();
    extern void TestMesh();
    TestMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MESHCURVATURE)
    Precall();
    extern void TestMeshCurvature();
    TestMeshCurvature();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MESHSMOOTHER)
    Precall();
    extern void TestMeshSmoother();
    TestMeshSmoother();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_PLANARMESH)
    Precall();
    extern void TestPlanarMesh();
    TestPlanarMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_RECTANGLEMESH)
    Precall();
    extern void TestRectangleMesh();
    TestRectangleMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_RECTANGLEPATCHMESH)
    Precall();
    extern void TestRectanglePatchMesh();
    TestRectanglePatchMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_STATICVETMANIFOLDMESH)
    Precall();
    extern void TestStaticVETManifoldMesh();
    TestStaticVETManifoldMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_STATICVTSMANIFOLDMESH)
    Precall();
    extern void TestStaticVTSManifoldMesh();
    TestStaticVTSManifoldMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_TETRAHEDRONKEY)
    Precall();
    extern void TestTetrahedronKey();
    TestTetrahedronKey();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_TRIANGLEKEY)
    Precall();
    extern void TestTriangleKey();
    TestTriangleKey();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_TUBEMESH)
    Precall();
    extern void TestTubeMesh();
    TestTubeMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_UNIQUEVERTICESSIMPLICES)
    Precall();
    extern void TestUniqueVerticesSimplices();
    TestUniqueVerticesSimplices();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_VERTEXATTRIBUTE)
    Precall();
    extern void TestVertexAttribute();
    TestVertexAttribute();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_VERTEXCOLLAPSEMESH)
    Precall();
    extern void TestVertexCollapseMesh();
    TestVertexCollapseMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_VETFEATURESKS)
    Precall();
    extern void TestVETFeaturesKS();
    TestVETFeaturesKS();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_VETMANIFOLDMESHKS)
    Precall();
    extern void TestVETManifoldMeshKS();
    TestVETManifoldMeshKS();
    Postcall();
#endif
#endif
}

void UTMathematics::Minimizers::Execute()
{
#if defined(UT_MATHEMATICS_MINIMIZERS)
    UTInformation("Mathematics/Minimizers");

#if defined(UT_MATHEMATICS_BRENTSMINIMIZER)
    Precall();
    extern void TestBrentsMinimizer();
    TestBrentsMinimizer();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_GAUSSNEWTONMINIMIZER)
    Precall();
    extern void TestGaussNewtonMinimizer();
    TestGaussNewtonMinimizer();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_LCPSOLVER)
    Precall();
    extern void TestLCPSolver();
    TestLCPSolver();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_LEVENBERGMARQUARDTMINIMIZER)
    Precall();
    extern void TestLevenbergMarquardtMinimizer();
    TestLevenbergMarquardtMinimizer();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_POWELLSMINIMIZER)
    Precall();
    extern void TestPowellsMinimizer();
    TestPowellsMinimizer();
    Postcall();
#endif
#endif
}

void UTMathematics::Miscellaneous::Execute()
{
#if defined(UT_MATHEMATICS_MISCELLANEOUS)
    UTInformation("Mathematics/Miscellaneous");

#if defined(UT_MATHEMATICS_CIRCLETHROUGHPOINTSPECIFIEDTANGENTANDRADIUS)
    Precall();
    extern void TestCircleThroughPointSpecifiedTangentAndRadius();
    TestCircleThroughPointSpecifiedTangentAndRadius();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CIRCLETHROUGHTWOPOINTSSPECIFIEDRADIUS)
    Precall();
    extern void TestCircleThroughTwoPointsSpecifiedRadius();
    TestCircleThroughTwoPointsSpecifiedRadius();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_RANDOMPOINTONHYPERSPHERE)
    Precall();
    extern void TestRandomPointOnHypersphere();
    TestRandomPointOnHypersphere();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_INSCRIBEDFIXEDASPECTRECTINQUAD)
    Precall();
    extern void TestInscribedFixedAspectRectInQuad();
    TestInscribedFixedAspectRectInQuad();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SORTPOINTSONCIRCLE)
    Precall();
    extern void TestSortPointsOnCircle();
    TestSortPointsOnCircle();
    Postcall();
#endif
#endif
}

void UTMathematics::Physics::Execute()
{
#if defined(UT_MATHEMATICS_PHYSICS)
    UTInformation("Mathematics/Physics");

#if defined(UT_MATHEMATICS_BOXMANAGER)
    Precall();
    extern void TestBoxManager();
    TestBoxManager();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MASSSPRINGARBITRARY)
    Precall();
    extern void TestMassSpringArbitrary();
    TestMassSpringArbitrary();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MASSSPRINGCURVE)
    Precall();
    extern void TestMassSpringCurve();
    TestMassSpringCurve();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MASSSPRINGSURFACE)
    Precall();
    extern void TestMassSpringSurface();
    TestMassSpringSurface();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_MASSSPRINGVOLUME)
    Precall();
    extern void TestMassSpringVolume();
    TestMassSpringVolume();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_PARTICLESYSTEM)
    Precall();
    extern void TestParticleSystem();
    TestParticleSystem();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_POLYHEDRALMASSPROPERTIES)
    Precall();
    extern void TestPolyhedralMassProperties();
    TestPolyhedralMassProperties();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_RECTANGLEMANAGER)
    Precall();
    extern void TestRectangleManager();
    TestRectangleManager();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_RIGIDBODY)
    Precall();
    extern void TestRigidBody();
    TestRigidBody();
    Postcall();
#endif

#endif
}

void UTMathematics::Primitives2D::Execute()
{
#if defined(UT_MATHEMATICS_PRIMITIVES_2D)
    UTInformation("Mathematics/Primitives/2D");

#if defined(UT_MATHEMATICS_ARC2)
    Precall();
    extern void TestArc2();
    TestArc2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_PARALLELOGRAM2)
    Precall();
    extern void TestParallelogram2();
    TestParallelogram2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_POLYGON2)
    Precall();
    extern void TestPolygon2();
    TestPolygon2();
    Postcall();
#endif
#endif
}

void UTMathematics::Primitives3D::Execute()
{
#if defined(UT_MATHEMATICS_PRIMITIVES_3D)
    UTInformation("Mathematics/Primitives/3D");

#if defined(UT_MATHEMATICS_ARC3)
    Precall();
    extern void TestArc3();
    TestArc3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CIRCLE3)
    Precall();
    extern void TestCircle3();
    TestCircle3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONVEXMESH3)
    Precall();
    extern void TestConvexMesh3();
    TestConvexMesh3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONVEXPOLYHEDRON3)
    Precall();
    extern void TestConvexPolyhedron3();
    TestConvexPolyhedron3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ELLIPSE3)
    Precall();
    extern void TestEllipse3();
    TestEllipse3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_FRUSTUM3)
    Precall();
    extern void TestFrustum3();
    TestFrustum3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_PARALLELEPIPED3)
    Precall();
    extern void TestParallelepiped3();
    TestParallelepiped3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_POLYHEDRON3)
    Precall();
    extern void TestPolyhedron3();
    TestPolyhedron3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_TETRAHEDRON3)
    Precall();
    extern void TestTetrahedron3();
    TestTetrahedron3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_TORUS3)
    Precall();
    extern void TestTorus3();
    TestTorus3();
    Postcall();
#endif
#endif
}

void UTMathematics::PrimitivesND::Execute()
{
#if defined(UT_MATHEMATICS_PRIMITIVES_ND)
    UTInformation("Mathematics/Primitives/ND");

#if defined(UT_MATHEMATICS_ALIGNEDBOX)
    Precall();
    extern void TestAlignedBox();
    TestAlignedBox();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CANONICALBOX)
    Precall();
    extern void TestCanonicalBox();
    TestCanonicalBox();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CAPSULE)
    Precall();
    extern void TestCapsule();
    TestCapsule();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CONE)
    Precall();
    extern void TestCone();
    TestCone();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_CYLINDER)
    Precall();
    extern void TestCylinder();
    TestCylinder();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_HALFSPACE)
    Precall();
    extern void TestHalfspace();
    TestHalfspace();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_HYPERELLIPSOID)
    Precall();
    extern void TestHyperellipsoid();
    TestHyperellipsoid();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_HYPERPLANE)
    Precall();
    extern void TestHyperplane();
    TestHyperplane();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_HYPERSPHERE)
    Precall();
    extern void TestHypersphere();
    TestHypersphere();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_LINE)
    Precall();
    extern void TestLine();
    TestLine();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_LOZENGE)
    Precall();
    extern void TestLozenge();
    TestLozenge();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ORIENTEDBOX)
    Precall();
    extern void TestOrientedBox();
    TestOrientedBox();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_RAY)
    Precall();
    extern void TestRay();
    TestRay();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_RECTANGLE)
    Precall();
    extern void TestRectangle();
    TestRectangle();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SEGMENT)
    Precall();
    extern void TestSegment();
    TestSegment();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_SEGMENTMESH)
    Precall();
    extern void TestSegmentMesh();
    TestSegmentMesh();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_TRIANGLE)
    Precall();
    extern void TestTriangle();
    TestTriangle();
    Postcall();
#endif
#endif
}

void UTMathematics::Projection::Execute()
{
#if defined(UT_MATHEMATICS_PROJECTION)
    UTInformation("Mathematics/Projection");

#if defined(UT_MATHEMATICS_PROJECTELLIPSOIDTOPLANE)
    Precall();
    extern void TestProjectEllipsoidToPlane();
    TestProjectEllipsoidToPlane();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_PROJECTHYPERELLIPSOIDTOLINE)
    Precall();
    extern void TestProjectHyperellipsoidToLine();
    TestProjectHyperellipsoidToLine();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_PROJECTPOINTSTOCOORDINATEPLANE)
    Precall();
    extern void TestProjectPointsToCoordinatePlane();
    TestProjectPointsToCoordinatePlane();
    Postcall();
#endif
#endif
}

void UTMathematics::RootFinders::Execute()
{
#if defined(UT_MATHEMATICS_ROOTFINDERS)
    UTInformation("Mathematics/RootFinders");

#if defined(UT_MATHEMATICS_POLYNOMIALROOT)
    Precall();
    extern void TestPolynomialRoot();
    TestPolynomialRoot();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ROOTSBISECTION1)
    Precall();
    extern void TestRootsBisection1();
    TestRootsBisection1();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ROOTSBISECTION2)
    Precall();
    extern void TestRootsBisection2();
    TestRootsBisection2();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ROOTSBRENTSMETHOD)
    Precall();
    extern void TestRootsBrentsMethod();
    TestRootsBrentsMethod();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ROOTSCUBIC)
    Precall();
    extern void TestRootsCubic();
    TestRootsCubic();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ROOTSGENERALPOLYNOMIAL)
    Precall();
    extern void TestRootsGeneralPolynomial();
    TestRootsGeneralPolynomial();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ROOTSLINEAR)
    Precall();
    extern void TestRootsLinear();
    TestRootsLinear();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ROOTSPOLYNOMIAL)
    Precall();
    extern void TestRootsPolynomial();
    TestRootsPolynomial();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ROOTSQUADRATIC)
    Precall();
    extern void TestRootsQuadratic();
    TestRootsQuadratic();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_ROOTSQUARTIC)
    Precall();
    extern void TestRootsQuartic();
    TestRootsQuartic();
    Postcall();
#endif
#endif
}

void UTMathematics::Surfaces::Execute()
{
#if defined(UT_MATHEMATICS_SURFACES)
    UTInformation("Mathematics/Surfaces");

#if defined(UT_MATHEMATICS_BSPLINESURFACE)
    Precall();
    extern void TestBSplineSurface();
    TestBSplineSurface();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_BSPLINESURFACEFIT)
    Precall();
    extern void TestBSplineSurfaceFit();
    TestBSplineSurfaceFit();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_DARBOUXFRAME)
    Precall();
    extern void TestDarbouxFrame();
    TestDarbouxFrame();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_IMPLICITSURFACE3)
    Precall();
    extern void TestImplicitSurface3();
    TestImplicitSurface3();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_NURBSSPHERE)
    Precall();
    extern void TestNURBSSphere();
    TestNURBSSphere();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_NURBSSURFACE)
    Precall();
    extern void TestNURBSSurface();
    TestNURBSSurface();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_PARAMETRICSURFACE)
    Precall();
    extern void TestParametricSurface();
    TestParametricSurface();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_QUADRICSURFACE)
    Precall();
    extern void TestQuadricSurface();
    TestQuadricSurface();
    Postcall();
#endif
#endif
}

void UTMathematics::Volumes::Execute()
{
#if defined(UT_MATHEMATICS_VOLUMES)
    UTInformation("Mathematics/Volumes");

#if defined(UT_MATHEMATICS_BSPLINEVOLUME)
    Precall();
    extern void TestBSplineVolume();
    TestBSplineVolume();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_NURBSVOLUME)
    Precall();
    extern void TestNURBSVolume();
    TestNURBSVolume();
    Postcall();
#endif

#if defined(UT_MATHEMATICS_PARAMETRICVOLUME)
    Precall();
    extern void TestParametricVolume();
    TestParametricVolume();
    Postcall();
#endif
#endif
}
