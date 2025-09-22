#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Containment/3D/ContOrientedBox3.h>
#include <GTL/Mathematics/Algebra/Rotation.h>
#include <fstream>
#include <iomanip>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestContOrientedBox3
    {
    public:
        UnitTestContOrientedBox3();

    private:
        void Test();
        void TestGetContainer();
        void TestInContainer();
        void TestMergeContainers();
    };
}

UnitTestContOrientedBox3::UnitTestContOrientedBox3()
{
    UTInformation("Mathematics/Containment/3D/ContOrientedBox3");

    Test();
}

void UnitTestContOrientedBox3::Test()
{
    TestGetContainer();
    TestInContainer();
    TestMergeContainers();
}

void UnitTestContOrientedBox3::TestGetContainer()
{
    std::vector<Vector3<double>> points(128);

    //std::default_random_engine dre{};
    //std::uniform_real_distribution<double> urd(-1.0, 1.0);
    //for (auto& p : points)
    //{
    //    for (std::size_t i = 0; i < 3; ++i)
    //    {
    //        p[i] = urd(dre);
    //    }
    //}
    //
    //std::ofstream output("Mathematics/Containment/3D/Input/points3.binary", std::ios::binary);
    //output.write(reinterpret_cast<const char*>(points.data()), points.size() * sizeof(points[0]));
    //output.close();

    // For Mathematica.
    //std::ofstream output("Mathematics/Containment/3D/Mathematica/points3.txt");
    //for (auto const& p : points)
    //{
    //    output << std::setprecision(17) << p[0] << ", " << p[1] << ", " << p[2] << std::endl;
    //}
    //output.close();

    std::ifstream input("Mathematics/Containment/3D/Input/points3.binary", std::ios::binary);
    UTAssert(input, "Cannot open input file.");
    input.read(reinterpret_cast<char*>(points.data()), points.size() * sizeof(points[0]));
    input.close();

    OrientedBox3<double> box{};
    box.center = { 0.072465408805427900, 0.022150223692974959, -0.055530845512644020 };
    box.axis[0] = { -0.10329513045949264, -0.53261483961815670, -0.84003068315501517 };
    box.axis[1] = { 0.51214357304623348, 0.69551014447480841, -0.50395892642165663 };
    box.axis[2] = { 0.85266586457462568, -0.48227281859042881, 0.20093245591006340 };
    box.extent = { 1.3221047875843510, 1.3190622415322544, 1.2413169254676579 };

    // For Mathematica.
    //std::array<Vector3<double>, 8> vertices{};
    //box.GetVertices(vertices);
    //std::ofstream output("Mathematics/Containment/3D/Mathematica/vertices3.txt");
    //for (auto const& v : vertices)
    //{
    //    output << std::setprecision(17) << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
    //}
    //output.close();

    OrientedBox3<double> estimatedBox{};
    GetContainer(points, estimatedBox);

    double const maxError = 1e-12;
    double error{};
    error = Length(box.center - estimatedBox.center);
    UTAssert(error <= maxError, "Invalid estimated box center.");
    error = Length(box.axis[0] - estimatedBox.axis[0]);
    UTAssert(error <= maxError, "Invalid estimated box axis[0].");
    error = Length(box.axis[1] - estimatedBox.axis[1]);
    UTAssert(error <= maxError, "Invalid estimated box axis[1].");
    error = Length(box.axis[2] - estimatedBox.axis[2]);
    UTAssert(error <= maxError, "Invalid estimated box axis[2].");
    error = Length(box.extent - estimatedBox.extent);
    UTAssert(error <= maxError, "Invalid estimated box extent.");

    // GTL eigensolver
    // eval[0] = 0.29140133905045290
    // eval[1] = 0.31924804807336393
    // eval[2] = 0.37662662420931875
    // evec[0] = (-0.10329513045949264, -0.53261483961815670, -0.84003068315501517)
    // evec[1] = ( 0.51214357304623348,  0.69551014447480841, -0.50395892642165663)
    // evec[2] = ( 0.85266586457462568, -0.48227281859042881,  0.20093245591006340)
    //
    // Mathematica eigensolver
    // eval[0] = 0.293696
    // eval[1] = 0.321762
    // eval[2] = 0.379592
    // evec[0] = (-0.103295, -0.532615, -0.840031)
    // evec[1] = (0.512144, 0.69551, -0.503959)
    // evec[2] = (0.852666, -0.482273, 0.200932)
    // The file $(GTL_PATH)\GTL\UnitTests\Mathematics\Containment\3D\Mathematica\ContOrientedBox3.nb
    // shows that the box fit is accurate.
}

void UnitTestContOrientedBox3::TestInContainer()
{
    OrientedBox3<double> box{};
    box.center = { 0.0, 0.0, 0.0 };
    box.axis[0] = { 1.0, 0.0, 0.0 };
    box.axis[1] = { 0.0, 1.0, 0.0 };
    box.axis[2] = { 0.0, 0.0, 1.0 };
    box.extent = { 1.0, 2.0, 3.0 };

    Vector3<double> origin{ 4.0, 4.0, 4.0 };
    Vector3<double> direction{ -1.0, -1.0, -2.0 };

    Vector3<double> point0 = origin; // (4, 4, 4)
    bool inContainer = InContainer(point0, box);
    UTAssert(!inContainer, "InContainer failed for point0.");
    Vector3<double> point1 = origin + direction;  // (3, 3, 2)
    inContainer = InContainer(point1, box);
    UTAssert(!inContainer, "InContainer failed for point1.");
    Vector3<double> point2 = origin + 3.0 * direction;  // (1, 1, -2)
    inContainer = InContainer(point2, box);
    UTAssert(inContainer, "InContainer failed for point2.");
    Vector3<double> point3 = { 0.999999, 1.999999, 2.999999 };
    inContainer = InContainer(point3, box);
    UTAssert(inContainer, "InContainer failed for point3.");

    Quaternion<double> q{ 1.0, 2.0, 3.0, 4.0 };
    Normalize(q);
    Matrix3x3<double> rot = Rotation<double>(q);
    Vector3<double> trn = { 0.3, -0.2, 0.1 };
    OrientedBox3<double> rotbox{};
    rotbox.center = rot * box.center + trn;
    rotbox.axis[0] = rot.GetCol(0);
    rotbox.axis[1] = rot.GetCol(1);
    rotbox.axis[2] = rot.GetCol(2);
    rotbox.extent = box.extent;

    point0 = rot * point0 + trn;
    inContainer = InContainer(point0, rotbox);
    UTAssert(!inContainer, "InContainer failed for point0.");
    point1 = rot * point1 + trn;
    inContainer = InContainer(point1, rotbox);
    UTAssert(!inContainer, "InContainer failed for point1.");
    point2 = rot * point2 + trn;
    inContainer = InContainer(point2, rotbox);
    UTAssert(inContainer, "InContainer failed for point2.");
    point3 = rot * point3 + trn;
    inContainer = InContainer(point3, rotbox);
    UTAssert(inContainer, "InContainer failed for point3.");
}

void UnitTestContOrientedBox3::TestMergeContainers()
{
    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0, 1.0);

    OrientedBox3<double> box0{};
    box0.center = { 8.0 + urd(dre), 8.0 + urd(dre), 8.0 + urd(dre) };
    box0.extent = { 2.0 + urd(dre), 2.0 + urd(dre), 2.0 + urd(dre) };
    box0.axis[0] = { urd(dre), urd(dre), urd(dre) };
    ComputeOrthonormalBasis(1, box0.axis[0], box0.axis[1], box0.axis[2]);
    // box0.center = (8.9377355422484630, 8.6700171799891592, 7.2709540085935611)
    // box0.axis[0] = (0.64825684721228793, 0.64358221353584077, -0.40689678600683471)
    // box0.axis[1] = (0.53162923912927207, 0.0000000000000000, 0.84697718511352549)
    // box0.axis[2] = (0.54509945160971829, -0.76537698843139224, -0.34214712249919171)
    // box0.extent = (2.0944411927357036, 1.6163341010140064, 1.4420680859654098)

    // For Mathematica.
    //std::array<Vector3<double>, 8> vertices{};
    //box0.GetVertices(vertices);
    //std::ofstream output("Mathematics/Containment/3D/Mathematica/box0vertices3.txt");
    //for (auto const& v : vertices)
    //{
    //    output << std::setprecision(17) << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
    //}
    //output.close();

    OrientedBox3<double> box1{};
    box1.center = { urd(dre), urd(dre), urd(dre) };
    box1.extent = { 4.0 + urd(dre), 4.0 + urd(dre), 4.0 + urd(dre) };
    box1.axis[0] = { urd(dre), urd(dre), urd(dre) };
    ComputeOrthonormalBasis(1, box1.axis[0], box1.axis[1], box1.axis[2]);
    // box1.center = (0.96221938355387771, 0.45167792642377913, 0.93538987402100493)
    // box1.axis[0] = (0.21696606135482288, -0.60160294752706056, -0.76876499773788354)
    // box1.axis[1] = (0.0000000000000000, -0.78752450267295693, 0.61628334205113144)
    // box1.axis[2] = (-0.97617914760569202, -0.13371256940342108, -0.17086608956536714)
    // box1.extent = (3.5940588991159013, 4.5962117134990983, 3.2197235016884127)

    // For Mathematica.
    //box1.GetVertices(vertices);
    //output.open("Mathematics/Containment/3D/Mathematica/box1vertices3.txt");
    //for (auto const& v : vertices)
    //{
    //    output << std::setprecision(17) << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
    //}
    //output.close();

    OrientedBox3<double> merge{};
    merge.center = { 2.8780763585458917, 4.1089808903634886, 2.8191562835269419 };
    merge.axis[0] = { 0.78115508101329434, -0.091065565306781393, -0.61765994060038398 };
    merge.axis[1] = { 0.48638910891619908, -0.53149388627303429, 0.69349829385665551 };
    merge.axis[2] = { -0.39143629639421462, -0.84215278404203031, -0.37088585062716733 };
    merge.extent = { 6.0087501887857089, 6.2516895069169731, 10.863638237139167 };

    // For Mathematica.
    //merge.GetVertices(vertices);
    //output.open("Mathematics/Containment/3D/Mathematica/mergevertices3.txt");
    //for (auto const& v : vertices)
    //{
    //    output << std::setprecision(17) << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
    //}
    //output.close();

    OrientedBox3<double> estimatedMerge{};
    MergeContainers(box0, box1, estimatedMerge);

    double const maxError = 1e-12;
    double error{};
    error = Length(merge.center - estimatedMerge.center);
    UTAssert(error <= maxError, "Invalid estimated merge center.");
    error = Length(merge.axis[0] - estimatedMerge.axis[0]);
    UTAssert(error <= maxError, "estimatedMerge estimated merge axis[0].");
    error = Length(merge.axis[1] - estimatedMerge.axis[1]);
    UTAssert(error <= maxError, "estimatedMerge estimated merge axis[1].");
    error = Length(merge.axis[2] - estimatedMerge.axis[2]);
    UTAssert(error <= maxError, "Invalid estimated merge axis[2].");
    error = Length(merge.extent - estimatedMerge.extent);
    UTAssert(error <= maxError, "Invalid estimated merge extent.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Containment/3D/ContOrientedBox3.h>

namespace gtl
{
    template void GetContainer(std::vector<Vector3<float>> const&, OrientedBox3<float>&);
    template bool InContainer(Vector3<float> const&, OrientedBox3<float> const&);
    template void MergeContainers(OrientedBox3<float> const&, OrientedBox3<float> const&, OrientedBox3<float>&);

    template void GetContainer(std::vector<Vector3<double>> const&, OrientedBox3<double>&);
    template bool InContainer(Vector3<double> const&, OrientedBox3<double> const&);
    template void MergeContainers(OrientedBox3<double> const&, OrientedBox3<double> const&, OrientedBox3<double>&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template void GetContainer(std::vector<Vector3<Rational>> const&, OrientedBox3<Rational>&);
    template bool InContainer(Vector3<Rational> const&, OrientedBox3<Rational> const&);
    template void MergeContainers(OrientedBox3<Rational> const&, OrientedBox3<Rational> const&, OrientedBox3<Rational>&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ContOrientedBox3)
