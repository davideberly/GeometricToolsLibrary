#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrIntervals
    {
    public:
        UnitTestIntrIntervals();

    private:
        using TestQuery = TIQuery<double, std::array<double, 2>, std::array<double, 2>>;
        using TestOutput = TestQuery::Output;
        using FindQuery = FIQuery<double, std::array<double, 2>, std::array<double, 2>>;
        using FindOutput = FindQuery::Output;

        void TestStaticIntersection();
        void TestDynamicIntersection();
        void FindStaticIntersection();
        void FindDynamicIntersection();

        static std::string invalidIntersection;
        static std::string invalidNumIntersections;
        static std::string invalidType;
        static std::string invalidOverlap;
        static std::string invalidFirstTimeContact;
        static std::string invalidLastTimeContact;
    };

    std::string UnitTestIntrIntervals::invalidIntersection = "Invalid intersection.";
    std::string UnitTestIntrIntervals::invalidNumIntersections = "Invalid number of intersections.";
    std::string UnitTestIntrIntervals::invalidType = "Invalid type.";
    std::string UnitTestIntrIntervals::invalidOverlap = "Invalid overlap.";
    std::string UnitTestIntrIntervals::invalidFirstTimeContact = "Invalid first time of contact.";
    std::string UnitTestIntrIntervals::invalidLastTimeContact = "Invalid last time of contact.";
}

UnitTestIntrIntervals::UnitTestIntrIntervals()
{
    UTInformation("Mathematics/Intersection/1D/IntrIntervals");

    TestStaticIntersection();
    TestDynamicIntersection();
    FindStaticIntersection();
    FindDynamicIntersection();
}

void UnitTestIntrIntervals::TestStaticIntersection()
{
    TestQuery query{};
    TestOutput output{};
    std::array<double, 2> intr0{}, intr1{};
    double a{}, b{};

    // finite intervals
    intr0 = { 0.0, 1.0 };
    intr1 = { -2.0, -1.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == false, invalidIntersection);

    intr0 = { 0.0, 1.0 };
    intr1 = { -2.0, 0.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { 0.0, 1.0 };
    intr1 = { -2.0, 0.5 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { 0.0, 1.0 };
    intr1 = { -2.0, 1.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { 0.0, 1.0 };
    intr1 = { -2.0, 2.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { 0.0, 1.0 };
    intr1 = { 0.0, 0.5 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { 0.0, 1.0 };
    intr1 = { 0.0, 1.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { 0.0, 1.0 };
    intr1 = { 0.0, 2.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { 0.0, 1.0 };
    intr1 = { 0.5, 0.75 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { 0.0, 1.0 };
    intr1 = { 0.5, 1.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { 0.0, 1.0 };
    intr1 = { 0.5, 2.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { 0.0, 1.0 };
    intr1 = { 1.0, 2.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { 0.0, 1.0 };
    intr1 = { 2.0, 3.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == false, invalidIntersection);

    // finite and semi-infinite interval [0, +infinity)
    intr0 = { -2.0, -1.0 };
    a = 0.0;
    output = query(intr0, a, true);
    UTAssert(output.intersect == false, invalidIntersection);

    intr0 = { -2.0, 0.0 };
    a = 0.0;
    output = query(intr0, a, true);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { -2.0, 1.0 };
    a = 0.0;
    output = query(intr0, a, true);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { 0.0, 1.0 };
    a = 0.0;
    output = query(intr0, a, true);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { 1.0, 2.0 };
    a = 0.0;
    output = query(intr0, a, true);
    UTAssert(output.intersect == true, invalidIntersection);

    // finite and semi-infinite interval (-infinity, 0]
    intr0 = { 1.0, 2.0 };
    a = 0.0;
    output = query(intr0, a, false);
    UTAssert(output.intersect == false, invalidIntersection);

    intr0 = { 0.0, 2.0 };
    a = 0.0;
    output = query(intr0, a, false);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { -1.0, 2.0 };
    a = 0.0;
    output = query(intr0, a, false);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { -1.0, 0.0 };
    a = 0.0;
    output = query(intr0, a, false);
    UTAssert(output.intersect == true, invalidIntersection);

    intr0 = { -1.0, -0.5 };
    a = 0.0;
    output = query(intr0, a, false);
    UTAssert(output.intersect == true, invalidIntersection);

    // semi-infinite intervals [a,+infinity) and [b,+infinity)
    a = 0.0;
    b = 1.0;
    output = query(a, true, b, true);
    UTAssert(output.intersect == true, invalidIntersection);

    // semi-infinite intervals [a,+infinity) and (-infinity,b]
    a = 0.0;
    b = 1.0;
    output = query(a, true, b, false);
    UTAssert(output.intersect == true, invalidIntersection);

    a = 0.0;
    b = 0.0;
    output = query(a, true, b, false);
    UTAssert(output.intersect == true, invalidIntersection);

    a = 0.0;
    b = -1.0;
    output = query(a, true, b, false);
    UTAssert(output.intersect == false, invalidIntersection);

    // semi-infinite intervals (-infinity,a] and (-infinity,b]
    a = 0.0;
    b = 1.0;
    output = query(a, false, b, false);
    UTAssert(output.intersect == true, invalidIntersection);
}

void UnitTestIntrIntervals::TestDynamicIntersection()
{
    TestQuery query{};
    TestOutput output{};
    std::array<double, 2> intr0{}, intr1{};
    double speed0{}, speed1{};

    // intr0 to left of intr1
    intr0 = { 0.0, 1.0 };
    intr1 = { 2.0, 3.0 };
    speed0 = 2.0;
    speed1 = 1.0;
    output = query(intr0, speed0, intr1, speed1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.firstTime == 1.0, invalidFirstTimeContact);
    UTAssert(output.lastTime == 3.0, invalidLastTimeContact);

    intr0 = { 0.0, 1.0 };
    intr1 = { 2.0, 3.0 };
    speed0 = 1.0;
    speed1 = 2.0;
    output = query(intr0, speed0, intr1, speed1);
    UTAssert(output.intersect == false, invalidIntersection);
    UTAssert(output.firstTime == 0.0, invalidFirstTimeContact);
    UTAssert(output.lastTime == 0.0, invalidLastTimeContact);

    // intr0 to right of intr1
    intr0 = { 2.0, 3.0 };
    intr1 = { 0.0, 1.0 };
    speed0 = -2.0;
    speed1 = -1.0;
    output = query(intr0, speed0, intr1, speed1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.firstTime == 1.0, invalidFirstTimeContact);
    UTAssert(output.lastTime == 3.0, invalidLastTimeContact);

    intr0 = { 2.0, 3.0 };
    intr1 = { 0.0, 1.0 };
    speed0 = -1.0;
    speed1 = -2.0;
    output = query(intr0, speed0, intr1, speed1);
    UTAssert(output.intersect == false, invalidIntersection);
    UTAssert(output.firstTime == 0.0, invalidFirstTimeContact);
    UTAssert(output.lastTime == 0.0, invalidLastTimeContact);

    // intr0 and intr1 are initially intersecting
    intr0 = { 0.0, 3.0 };
    intr1 = { 1.0, 4.0 };
    speed0 = 1.0;
    speed1 = 2.0;
    output = query(intr0, speed0, intr1, speed1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.firstTime == 0.0, invalidFirstTimeContact);
    UTAssert(output.lastTime == 2.0, invalidLastTimeContact);

    intr0 = { 0.0, 3.0 };
    intr1 = { 1.0, 4.0 };
    speed0 = 1.0;
    speed1 = 1.0;
    output = query(intr0, speed0, intr1, speed1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.firstTime == 0.0, invalidFirstTimeContact);
    UTAssert(output.lastTime == -1.0, invalidLastTimeContact);
}

void UnitTestIntrIntervals::FindStaticIntersection()
{
    FindQuery query{};
    FindOutput output{};
    std::array<double, 2> intr0{}, intr1{};
    double a{}, b{};

    // finite intervals
    intr0 = { 0.0, 1.0 };
    intr1 = { -2.0, -1.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == false, invalidIntersection);
    UTAssert(output.numIntersections == 0, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isEmpty, invalidType);

    intr0 = { 0.0, 1.0 };
    intr1 = { -2.0, 0.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 1, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isPoint, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 0.0, invalidOverlap);

    intr0 = { 0.0, 1.0 };
    intr1 = { -2.0, 0.5 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 0.5, invalidOverlap);

    intr0 = { 0.0, 1.0 };
    intr1 = { -2.0, 1.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 1.0, invalidOverlap);

    intr0 = { 0.0, 1.0 };
    intr1 = { -2.0, 2.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 1.0, invalidOverlap);

    intr0 = { 0.0, 1.0 };
    intr1 = { 0.0, 0.5 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 0.5, invalidOverlap);

    intr0 = { 0.0, 1.0 };
    intr1 = { 0.0, 1.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 1.0, invalidOverlap);

    intr0 = { 0.0, 1.0 };
    intr1 = { 0.0, 2.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 1.0, invalidOverlap);

    intr0 = { 0.0, 1.0 };
    intr1 = { 0.5, 0.75 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == 0.5 && output.overlap[1] == 0.75, invalidOverlap);

    intr0 = { 0.0, 1.0 };
    intr1 = { 0.5, 1.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == 0.5 && output.overlap[1] == 1.0, invalidOverlap);

    intr0 = { 0.0, 1.0 };
    intr1 = { 0.5, 2.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == 0.5 && output.overlap[1] == 1.0, invalidOverlap);

    intr0 = { 0.0, 1.0 };
    intr1 = { 1.0, 2.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 1, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isPoint, invalidType);
    UTAssert(output.overlap[0] == 1.0 && output.overlap[1] == 1.0, invalidOverlap);

    intr0 = { 0.0, 1.0 };
    intr1 = { 2.0, 3.0 };
    output = query(intr0, intr1);
    UTAssert(output.intersect == false, invalidIntersection);
    UTAssert(output.numIntersections == 0, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isEmpty, invalidType);

    // finite and semi-infinite interval [0, +infinity)
    intr0 = { -2.0, -1.0 };
    a = 0.0;
    output = query(intr0, a, true);
    UTAssert(output.intersect == false, invalidIntersection);
    UTAssert(output.numIntersections == 0, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isEmpty, invalidType);

    intr0 = { -2.0, 0.0 };
    a = 0.0;
    output = query(intr0, a, true);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 1, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isPoint, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 0.0, invalidOverlap);

    intr0 = { -2.0, 1.0 };
    a = 0.0;
    output = query(intr0, a, true);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 1.0, invalidOverlap);

    intr0 = { 0.0, 1.0 };
    a = 0.0;
    output = query(intr0, a, true);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 1.0, invalidOverlap);

    intr0 = { 1.0, 2.0 };
    a = 0.0;
    output = query(intr0, a, true);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == 1.0 && output.overlap[1] == 2.0, invalidOverlap);

    // finite and semi-infinite interval (-infinity, 0]
    intr0 = { 1.0, 2.0 };
    a = 0.0;
    output = query(intr0, a, false);
    UTAssert(output.intersect == false, invalidIntersection);
    UTAssert(output.numIntersections == 0, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isEmpty, invalidType);

    intr0 = { 0.0, 2.0 };
    a = 0.0;
    output = query(intr0, a, false);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 1, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isPoint, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 0.0, invalidOverlap);

    intr0 = { -1.0, 2.0 };
    a = 0.0;
    output = query(intr0, a, false);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == -1.0 && output.overlap[1] == 0.0, invalidOverlap);

    intr0 = { -1.0, 0.0 };
    a = 0.0;
    output = query(intr0, a, false);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == -1.0 && output.overlap[1] == 0.0, invalidOverlap);

    intr0 = { -1.0, -0.5 };
    a = 0.0;
    output = query(intr0, a, false);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == -1.0 && output.overlap[1] == -0.5, invalidOverlap);

    // semi-infinite intervals [a,+infinity) and [b,+infinity)
    a = 0.0;
    b = 1.0;
    output = query(a, true, b, true);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 1, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isPositiveInfinite, invalidType);
    // overlap[1] = +1 indicates [overlap[0],+infinity)
    UTAssert(output.overlap[0] == 1.0 && output.overlap[1] == +1.0, invalidOverlap);

    // semi-infinite intervals [a,+infinity) and (-infinity,b]
    a = 0.0;
    b = 1.0;
    output = query(a, true, b, false);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 2, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isFinite, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 1.0, invalidOverlap);

    a = 0.0;
    b = 0.0;
    output = query(a, true, b, false);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 1, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isPoint, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 0.0, invalidOverlap);

    a = 0.0;
    b = -1.0;
    output = query(a, true, b, false);
    UTAssert(output.intersect == false, invalidIntersection);
    UTAssert(output.numIntersections == 0, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isEmpty, invalidType);

    // semi-infinite intervals (-infinity,a] and (-infinity,b]
    a = 0.0;
    b = 1.0;
    output = query(a, false, b, false);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 1, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isNegativeInfinite, invalidType);
    // overlap[0] = -1 indicates (-infinity,overlap[1]]
    UTAssert(output.overlap[0] == -1.0 && output.overlap[1] == 0.0, invalidOverlap);
}

void UnitTestIntrIntervals::FindDynamicIntersection()
{
    FindQuery query{};
    FindOutput output{};
    std::array<double, 2> intr0{}, intr1{};
    double speed0{}, speed1{};

    // intr0 to left of intr1
    intr0 = { 0.0, 1.0 };
    intr1 = { 2.0, 3.0 };
    speed0 = 2.0;
    speed1 = 1.0;
    output = query(intr0, speed0, intr1, speed1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 1, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isDynamicQuery, invalidType);
    UTAssert(output.overlap[0] == 2.0 && output.overlap[1] == 2.0, invalidOverlap);
    UTAssert(output.firstTime == 1.0, invalidFirstTimeContact);
    UTAssert(output.lastTime == 3.0, invalidLastTimeContact);

    intr0 = { 0.0, 1.0 };
    intr1 = { 2.0, 3.0 };
    speed0 = 1.0;
    speed1 = 2.0;
    output = query(intr0, speed0, intr1, speed1);
    UTAssert(output.intersect == false, invalidIntersection);
    UTAssert(output.numIntersections == 0, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isDynamicQuery, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 0.0, invalidOverlap);
    UTAssert(output.firstTime == 0.0, invalidFirstTimeContact);
    UTAssert(output.lastTime == 0.0, invalidLastTimeContact);

    // intr0 to right of intr1
    intr0 = { 2.0, 3.0 };
    intr1 = { 0.0, 1.0 };
    speed0 = -2.0;
    speed1 = -1.0;
    output = query(intr0, speed0, intr1, speed1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.numIntersections == 1, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isDynamicQuery, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 0.0, invalidOverlap);
    UTAssert(output.firstTime == 1.0, invalidFirstTimeContact);
    UTAssert(output.lastTime == 3.0, invalidLastTimeContact);

    intr0 = { 2.0, 3.0 };
    intr1 = { 0.0, 1.0 };
    speed0 = -1.0;
    speed1 = -2.0;
    output = query(intr0, speed0, intr1, speed1);
    UTAssert(output.intersect == false, invalidIntersection);
    UTAssert(output.numIntersections == 0, invalidNumIntersections);
    UTAssert(output.type == FindOutput::isDynamicQuery, invalidType);
    UTAssert(output.overlap[0] == 0.0 && output.overlap[1] == 0.0, invalidOverlap);
    UTAssert(output.firstTime == 0.0, invalidFirstTimeContact);
    UTAssert(output.lastTime == 0.0, invalidLastTimeContact);

    // intr0 and intr1 are initially intersecting
    intr0 = { 0.0, 3.0 };
    intr1 = { 1.0, 4.0 };
    speed0 = 1.0;
    speed1 = 2.0;
    output = query(intr0, speed0, intr1, speed1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.firstTime == 0.0, invalidFirstTimeContact);
    UTAssert(output.lastTime == 2.0, invalidLastTimeContact);

    intr0 = { 0.0, 3.0 };
    intr1 = { 1.0, 4.0 };
    speed0 = 1.0;
    speed1 = 1.0;
    output = query(intr0, speed0, intr1, speed1);
    UTAssert(output.intersect == true, invalidIntersection);
    UTAssert(output.firstTime == 0.0, invalidFirstTimeContact);
    UTAssert(output.lastTime == -1.0, invalidLastTimeContact);
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>

namespace gtl
{
    template class TIQuery<float, std::array<float, 2>, std::array<float, 2>>;
    template class FIQuery<float, std::array<float, 2>, std::array<float, 2>>;

    template class TIQuery<double, std::array<double, 2>, std::array<double, 2>>;
    template class FIQuery<double, std::array<double, 2>, std::array<double, 2>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, std::array<Rational, 2>, std::array<Rational, 2>>;
    template class FIQuery<Rational, std::array<Rational, 2>, std::array<Rational, 2>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrIntervals)
