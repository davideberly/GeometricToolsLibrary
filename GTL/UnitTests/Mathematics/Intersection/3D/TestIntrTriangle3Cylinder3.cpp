#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrTriangle3Cylinder3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrTriangle3Cylinder3
    {
    public:
        UnitTestIntrTriangle3Cylinder3();

    private:
        void Test0A();
        void Test0B();
        void Test1A();
        void Test1B();
        void Test2A();
        void Test2B();
        void Test3A();
        void Test3B();
        void Test3C();
        void Test3D();
        void Test3E();
        void Test4A();
        void Test4B();
        void Test4C();
        void Test4D();
        void Test4E();
        void Test4F();
        void Test5();

        Cylinder3<double> mCylinder;
        Triangle3<double> mTriangle;
        TIQuery<double, Triangle3<double>, Cylinder3<double>> mQuery;
        bool mIntersects;
    };
}

UnitTestIntrTriangle3Cylinder3::UnitTestIntrTriangle3Cylinder3()
    :
    mCylinder{},
    mTriangle{},
    mQuery{},
    mIntersects(false)
{
    UTInformation("Mathematics/Intersection/3D/IntrTriangle3Cylinder3");

    mCylinder.center = { 0.0f, 0.0f, 0.0f };
    mCylinder.direction = { 0.0f, 0.0f, 1.0f };
    mCylinder.radius = 1.0f;
    mCylinder.height = 2.0f;  // -1 <= z <= 1

    Test0A();
    Test0B();
    Test1A();
    Test1B();
    Test2A();
    Test2B();
    Test3A();
    Test3B();
    Test3C();
    Test3D();
    Test3E();
    Test4A();
    Test4B();
    Test4C();
    Test4D();
    Test4E();
    Test4F();
    Test5();
}

void UnitTestIntrTriangle3Cylinder3::Test0A()
{
    mTriangle.v[0] = { 0.0f, 0.0f, -1.1f };
    mTriangle.v[1] = { 1.0f, 0.0f, -1.2f };
    mTriangle.v[2] = { 0.0f, 1.0f, -1.3f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test0A failed.");
}

void UnitTestIntrTriangle3Cylinder3::Test0B()
{
    mTriangle.v[0] = { 0.0f, 0.0f, 1.1f };
    mTriangle.v[1] = { 1.0f, 0.0f, 1.2f };
    mTriangle.v[2] = { 0.0f, 1.0f, 1.3f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test0B failed.");
}

void UnitTestIntrTriangle3Cylinder3::Test1A()
{
    mTriangle.v[0] = { 0.0f, 0.0f, -1.1f };
    mTriangle.v[1] = { 1.0f, 0.0f, -1.2f };
    mTriangle.v[2] = { 0.0f, 1.0f, -1.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test1A failed.");
}

void UnitTestIntrTriangle3Cylinder3::Test1B()
{
    mTriangle.v[0] = { 0.0f, 0.0f, 1.1f };
    mTriangle.v[1] = { 1.0f, 0.0f, 1.2f };
    mTriangle.v[2] = { 0.0f, 1.0f, 1.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test1B failed.");
}

void UnitTestIntrTriangle3Cylinder3::Test2A()
{
    // DiskOverlapsSegment, |Q0| <= r.
    mTriangle.v[0] = { 0.0f, 0.0f, -1.0f };
    mTriangle.v[1] = { 2.0f, 0.0f, -1.2f };
    mTriangle.v[2] = { 0.0f, 2.0f, -1.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test2A failed, |Q0| <= r.");

    // DiskOverlapsSegment, |Q1| <= r.
    mTriangle.v[0] = { 1.0f, 1.0f, -1.0f };
    mTriangle.v[1] = { 0.0f, 1.0f, -1.2f };
    mTriangle.v[2] = { 0.0f, 0.0f, -1.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test2A failed, |Q1| <= r.");

    // DiskOverlapsSegment, Q0 and Q1 outside disk but segment overlaps
    mTriangle.v[0] = { 1.0f, 1.0f, -1.0f };
    mTriangle.v[1] = { 0.0f, 0.0f, -1.2f };
    mTriangle.v[2] = { -1.0f, -0.5f, -1.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test2A failed, Q0 and Q1 outside disk but segment overlaps.");

    // DiskOverlapsSegment, no overlap
    mTriangle.v[0] = { 1.0f, 1.0f, -1.0f };
    mTriangle.v[1] = { 0.0f, 0.0f, -1.2f };
    mTriangle.v[2] = { 0.0f, 2.0f, -1.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test2A failed, no overlap.");
}

void UnitTestIntrTriangle3Cylinder3::Test2B()
{
    // DiskOverlapsSegment, |Q0| <= r.
    mTriangle.v[0] = { 0.0f, 0.0f, 1.0f };
    mTriangle.v[1] = { 2.0f, 0.0f, 1.2f };
    mTriangle.v[2] = { 0.0f, 2.0f, 1.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test2B failed, |Q0| <= r.");

    // DiskOverlapsSegment, |Q1| <= r.
    mTriangle.v[0] = { 1.0f, 1.0f, 1.0f };
    mTriangle.v[1] = { 0.0f, 1.0f, 1.2f };
    mTriangle.v[2] = { 0.0f, 0.0f, 1.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test2B failed, |Q1| <= r.");

    // DiskOverlapsSegment, Q0 and Q1 outside disk but segment overlaps
    mTriangle.v[0] = { 1.0f, 1.0f, 1.0f };
    mTriangle.v[1] = { 0.0f, 0.0f, 1.2f };
    mTriangle.v[2] = { -1.0f, -0.5f, 1.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test2B failed, Q0 and Q1 outside disk but segment overlaps.");

    // DiskOverlapsSegment, no overlap
    mTriangle.v[0] = { 1.0f, 1.0f, 1.0f };
    mTriangle.v[1] = { 0.0f, 0.0f, 1.2f };
    mTriangle.v[2] = { 0.0f, 2.0f, 1.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test2B failed, no overlap.");
}

void UnitTestIntrTriangle3Cylinder3::Test3A()
{
    // DiskOverlapsPolygon, polygon contains (0,0)
    mTriangle.v[0] = { -2.0f, -2.0f, 0.25f };
    mTriangle.v[1] = { 2.0f, -2.0f, 0.5f };
    mTriangle.v[2] = { 0.0f, 1.125f, 0.75f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test3A failed, polygon contains (0,0).");

    // DiskOverlapsPolygon, polygon does not contain (0,0) but overlap
    mTriangle.v[0] = { -2.0f, -2.0f, 0.25f };
    mTriangle.v[1] = { 2.0f, -2.0f, 0.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, 0.75f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test3A failed, polygon does not contain (0,0) but overlap.");

    // DiskOverlapsPolygon, polygon does not contain (0,0), no overlap
    mTriangle.v[0] = { -2.0f, 2.0f, 0.25f };
    mTriangle.v[1] = { 2.0f, 2.0f, 0.5f };
    mTriangle.v[2] = { 0.0f, 4.0f, 0.75f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test3A failed, polygon does not contain (0,0), no overlap.");
}

void UnitTestIntrTriangle3Cylinder3::Test3B()
{
    // DiskOverlapsPolygon, polygon contains (0,0)
    mTriangle.v[0] = { -2.0f, -2.0f, -2.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, -1.0f };
    mTriangle.v[2] = { 0.0f, 1.125f, 0.5f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test3B failed, polygon contains (0,0).");

    // DiskOverlapsPolygon, polygon does not contain (0,0) but overlap
    mTriangle.v[0] = { -2.0f, 8.0f, -1.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, 0.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, -2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test3B failed, polygon does not contain (0,0) but overlap.");

    // DiskOverlapsPolygon, polygon does not contain (0,0), no overlap
    mTriangle.v[0] = { -2.0f, -2.0f, -1.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, 0.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, -2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test3B failed, polygon does not contain (0,0), no overlap.");
}

void UnitTestIntrTriangle3Cylinder3::Test3C()
{
    // DiskOverlapsPolygon, polygon contains (0,0)
    mTriangle.v[0] = { -2.0f, -2.0f, -2.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, -1.125f };
    mTriangle.v[2] = { 0.0f, 1.125f, 0.5f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test3C failed, polygon contains (0,0).");

    // DiskOverlapsPolygon, polygon does not contain (0,0) but overlap
    mTriangle.v[0] = { -2.0f, 8.0f, -1.125f };
    mTriangle.v[1] = { 2.0f, -2.0f, 0.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, -2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test3C failed, polygon does not contain (0,0) but overlap.");

    // DiskOverlapsPolygon, polygon does not contain (0,0), no overlap
    mTriangle.v[0] = { -2.0f, -2.0f, -1.125f };
    mTriangle.v[1] = { 2.0f, -2.0f, 0.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, -2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test3C failed, polygon does not contain (0,0), no overlap.");
}

void UnitTestIntrTriangle3Cylinder3::Test3D()
{
    // DiskOverlapsPolygon, polygon contains (0,0)
    mTriangle.v[0] = { -2.0f, -2.0f, 2.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, 1.0f };
    mTriangle.v[2] = { 0.0f, 1.125f, 0.5f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test3D failed, polygon contains (0,0).");

    // DiskOverlapsPolygon, polygon does not contain (0,0) but overlap
    mTriangle.v[0] = { -2.0f, 8.0f, 1.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, 0.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, 2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test3D failed, polygon does not contain (0,0) but overlap.");

    // DiskOverlapsPolygon, polygon does not contain (0,0), no overlap
    mTriangle.v[0] = { -2.0f, -2.0f, 1.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, 0.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, 2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test3D failed, polygon does not contain (0,0), no overlap.");
}

void UnitTestIntrTriangle3Cylinder3::Test3E()
{
    // DiskOverlapsPolygon, polygon contains (0,0)
    mTriangle.v[0] = { -2.0f, -2.0f, 2.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, 1.125f };
    mTriangle.v[2] = { 0.0f, 1.125f, 0.5f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test3E failed, polygon contains (0,0).");

    // DiskOverlapsPolygon, polygon does not contain (0,0) but overlap
    mTriangle.v[0] = { -2.0f, 8.0f, 1.125f };
    mTriangle.v[1] = { 2.0f, -2.0f, 0.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, 2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test3E failed, polygon does not contain (0,0) but overlap.");

    // DiskOverlapsPolygon, polygon does not contain (0,0), no overlap
    mTriangle.v[0] = { -2.0f, -2.0f, 1.125f };
    mTriangle.v[1] = { 2.0f, -2.0f, 0.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, 2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test3E failed, polygon does not contain (0,0), no overlap.");
}

void UnitTestIntrTriangle3Cylinder3::Test4A()
{
    // DiskOverlapsPolygon, polygon contains (0,0)
    mTriangle.v[0] = { -2.0f, -2.0f, 2.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, 1.125f };
    mTriangle.v[2] = { 0.0f, 1.125f, -1.5f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test4A failed, polygon contains (0,0).");

    // DiskOverlapsPolygon, polygon does not contain (0,0) but overlap
    mTriangle.v[0] = { -2.0f, 8.0f, 1.125f };
    mTriangle.v[1] = { 2.0f, -2.0f, -1.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, 2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test4A failed, polygon does not contain (0,0) but overlap.");

    // DiskOverlapsPolygon, polygon does not contain (0,0), no overlap
    mTriangle.v[0] = { -2.0f, -2.0f, 1.125f };
    mTriangle.v[1] = { 2.0f, -2.0f, -1.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, 2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test4A failed, polygon does not contain (0,0), no overlap.");
}

void UnitTestIntrTriangle3Cylinder3::Test4B()
{
    // DiskOverlapsPolygon, polygon contains (0,0)
    mTriangle.v[0] = { -2.0f, -2.0f, 2.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, 1.0f };
    mTriangle.v[2] = { 0.0f, 1.125f, -1.5f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test4B failed, polygon contains (0,0).");

    // DiskOverlapsPolygon, polygon does not contain (0,0) but overlap
    mTriangle.v[0] = { -2.0f, 8.0f, 1.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, -1.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, 2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test4B failed, polygon does not contain (0,0) but overlap.");

    // DiskOverlapsPolygon, polygon does not contain (0,0), no overlap
    mTriangle.v[0] = { -2.0f, -2.0f, 1.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, -1.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, 2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test4B failed, polygon does not contain (0,0), no overlap.");
}

void UnitTestIntrTriangle3Cylinder3::Test4C()
{
    // DiskOverlapsPolygon, polygon contains (0,0)
    mTriangle.v[0] = { -2.0f, -2.0f, -2.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, -1.125f };
    mTriangle.v[2] = { 0.0f, 1.125f, 1.5f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test4C failed, polygon contains (0,0).");

    // DiskOverlapsPolygon, polygon does not contain (0,0) but overlap
    mTriangle.v[0] = { -2.0f, 8.0f, -1.125f };
    mTriangle.v[1] = { 2.0f, -2.0f, 1.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, -2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test4C failed, polygon does not contain (0,0) but overlap.");

    // DiskOverlapsPolygon, polygon does not contain (0,0), no overlap
    mTriangle.v[0] = { -2.0f, -2.0f, -1.125f };
    mTriangle.v[1] = { 2.0f, -2.0f, 1.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, -2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test4C failed, polygon does not contain (0,0), no overlap.");
}

void UnitTestIntrTriangle3Cylinder3::Test4D()
{
    // DiskOverlapsPolygon, polygon contains (0,0)
    mTriangle.v[0] = { -2.0f, -2.0f, -2.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, -1.0f };
    mTriangle.v[2] = { 0.0f, 1.125f, 1.5f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test4D failed, polygon contains (0,0).");

    // DiskOverlapsPolygon, polygon does not contain (0,0) but overlap
    mTriangle.v[0] = { -2.0f, 8.0f, -1.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, 1.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, -2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test4D failed, polygon does not contain (0,0) but overlap.");

    // DiskOverlapsPolygon, polygon does not contain (0,0), no overlap
    mTriangle.v[0] = { -2.0f, -2.0f, -1.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, 1.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, -2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test4D failed, polygon does not contain (0,0), no overlap.");
}

void UnitTestIntrTriangle3Cylinder3::Test4E()
{
    // DiskOverlapsPolygon, polygon contains (0,0)
    mTriangle.v[0] = { -2.0f, -2.0f, 0.95f };
    mTriangle.v[1] = { 2.0f, -2.0f, 0.85f };
    mTriangle.v[2] = { 0.0f, 1.125f, -1.5f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test4E failed, polygon contains (0,0).");

    // DiskOverlapsPolygon, polygon does not contain (0,0) but overlap
    mTriangle.v[0] = { 0.5f, 2.0f, 0.85f };
    mTriangle.v[1] = { 2.0f, 0.0f, -1.5f };
    mTriangle.v[2] = { 0.5f, -2.0f, 0.95f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test4E failed, polygon does not contain (0,0) but overlap.");

    // DiskOverlapsPolygon, polygon does not contain (0,0), no overlap
    mTriangle.v[0] = { 1.5f, 2.0f, 0.85f };
    mTriangle.v[1] = { 2.0f, 0.0f, -1.5f };
    mTriangle.v[2] = { 1.5f, -2.0f, 0.95f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test4E failed, polygon does not contain (0,0), no overlap.");
}

void UnitTestIntrTriangle3Cylinder3::Test4F()
{
    // DiskOverlapsPolygon, polygon contains (0,0)
    mTriangle.v[0] = { -2.0f, -2.0f, -0.95f };
    mTriangle.v[1] = { 2.0f, -2.0f, -0.85f };
    mTriangle.v[2] = { 0.0f, 1.125f, 1.5f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test4F failed, polygon contains (0,0).");

    // DiskOverlapsPolygon, polygon does not contain (0,0) but overlap
    mTriangle.v[0] = { 0.5f, 2.0f, -0.85f };
    mTriangle.v[1] = { 2.0f, 0.0f, 1.5f };
    mTriangle.v[2] = { 0.5f, -2.0f, -0.95f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test4F failed, polygon does not contain (0,0) but overlap.");

    // DiskOverlapsPolygon, polygon does not contain (0,0), no overlap
    mTriangle.v[0] = { 1.5f, 2.0f, -0.85f };
    mTriangle.v[1] = { 2.0f, 0.0f, 1.5f };
    mTriangle.v[2] = { 1.5f, -2.0f, -0.95f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test4F failed, polygon does not contain (0,0), no overlap.");
}

void UnitTestIntrTriangle3Cylinder3::Test5()
{
    // DiskOverlapsPolygon, polygon contains (0,0)
    mTriangle.v[0] = { -2.0f, -2.0f, 2.0f };
    mTriangle.v[1] = { 2.0f, -2.0f, 0.5f };
    mTriangle.v[2] = { 0.0f, 1.125f, -1.5f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test5 failed, polygon contains (0,0).");

    // DiskOverlapsPolygon, polygon does not contain (0,0) but overlap
    mTriangle.v[0] = { -2.0f, 8.0f, 0.5f };
    mTriangle.v[1] = { 2.0f, -2.0f, -1.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, 2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == true,
        "Test5 failed, polygon does not contain (0,0) but overlap.");

    // DiskOverlapsPolygon, polygon does not contain (0,0), no overlap
    mTriangle.v[0] = { -2.0f, -2.0f, 0.5f };
    mTriangle.v[1] = { 2.0f, -2.0f, -1.5f };
    mTriangle.v[2] = { 0.0f, -0.5f, 2.0f };
    mIntersects = mQuery.ExecuteUseGeometry(mTriangle, mCylinder);
    UTAssert(
        mIntersects == false,
        "Test5 failed, polygon does not contain (0,0), no overlap.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrTriangle3Cylinder3.h>

namespace gtl
{
    template class TIQuery<float, Triangle3<float>, Cylinder3<float>>;
    template class TIQuery<double, Triangle3<double>, Cylinder3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Triangle3<Rational>, Cylinder3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrTriangle3Cylinder3)
