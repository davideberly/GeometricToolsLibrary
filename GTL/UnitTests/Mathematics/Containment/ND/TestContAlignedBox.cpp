#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Containment/ND/ContAlignedBox.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestContAlignedBox
    {
    public:
        UnitTestContAlignedBox();

    private:
        void Test();
    };
}

UnitTestContAlignedBox::UnitTestContAlignedBox()
{
    UTInformation("Mathematics/Containment/ND/ContAlignedBox");

    Test();
}

void UnitTestContAlignedBox::Test()
{
    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0, 1.0);
    std::vector<Vector3<double>> points(100);
    std::array<double, 3> rmin{}, rmax{};
    rmin.fill(std::numeric_limits<double>::max());
    rmax.fill(-std::numeric_limits<double>::max());
    for (auto& p : points)
    {
        for (std::size_t i = 0; i < 3; ++i)
        {
            double value = urd(dre);
            rmin[i] = std::min(rmin[i], value);
            rmax[i] = std::max(rmax[i], value);
            p[i] = value;
        }
    }

    AlignedBox3<double> box0{};
    GetContainer(points, box0);
    for (int i = 0; i < 3; ++i)
    {
        UTAssert(box0.min[i] == rmin[i] && box0.max[i] == rmax[i], "incorrect extremum");
    }

    Vector3<double> test{};  // zero vector
    UTAssert(InContainer(test, box0), "InContainer failed");
    test = { 1.0, 1.0, 1.0 };
    UTAssert(!InContainer(test, box0), "InContainer failed");

    AlignedBox3<double> box1{};
    box1.min = { 0.0, 0.0, 0.0 };
    box1.max = { 1.0, 1.0, 1.0 };
    box0.min = { 0.5, -1.0, 0.5 };
    box0.max = { 2.0, 2.0, 0.75 };
    AlignedBox3<double> merge{};
    MergeContainers(box0, box1, merge);
    bool equal0 = (merge.min == Vector3<double>{0.0, -1.0, 0.0});
    bool equal1 = (merge.max == Vector3<double>{2.0, 2.0, 1.0});
    UTAssert(equal0 && equal1, "MergeContainers failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Containment/ND/ContAlignedBox.h>

namespace gtl
{
    template void GetContainer(std::vector<Vector<float, 3>> const&, AlignedBox<float, 3>&);
    template bool InContainer(Vector<float, 3> const&, AlignedBox<float, 3> const&);
    template void MergeContainers(AlignedBox<float, 3> const&, AlignedBox<float, 3> const&, AlignedBox<float, 3>&);

    template void GetContainer(std::vector<Vector<double, 3>> const&, AlignedBox<double, 3>&);
    template bool InContainer(Vector<double, 3> const&, AlignedBox<double, 3> const&);
    template void MergeContainers(AlignedBox<double, 3> const&, AlignedBox<double, 3> const&, AlignedBox<double, 3>&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template void GetContainer(std::vector<Vector<Rational, 3>> const&, AlignedBox<Rational, 3>&);
    template bool InContainer(Vector<Rational, 3> const&, AlignedBox<Rational, 3> const&);
    template void MergeContainers(AlignedBox<Rational, 3> const&, AlignedBox<Rational, 3> const&, AlignedBox<Rational, 3>&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ContAlignedBox)
