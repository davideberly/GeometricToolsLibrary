// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Utility/Lattice.h>
using namespace gtl;

namespace gtl
{
    class UnitTestLattice
    {
    public:
        UnitTestLattice();

    private:
        void TestCompileTime_2();
        void TestCompileTime_2_3();
        void TestCompileTime_2_3_5();
        void TestRunTime_1D();
        void TestRunTime_2D();
        void TestRunTime_3D();
    };
}

UnitTestLattice::UnitTestLattice()
{
    UTInformation("Mathematics/Utility/Lattice");

    TestCompileTime_2();
    TestCompileTime_2_3();
    TestCompileTime_2_3_5();
    TestRunTime_1D();
    TestRunTime_2D();
    TestRunTime_3D();
}

void UnitTestLattice::TestCompileTime_2()
{
    // Lattice<true>(), dimensions(), size(std::size_t), size()
    Lattice<true, 2> latticeT{};
    UTAssert(
        latticeT.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT.size() == 2,
        "Invalid number of elements.");

    // index(std::int32_t...)
    for (std::size_t x = 0; x < latticeT.size(0); ++x)
    {
        std::size_t index = latticeT.index((std::int32_t)x);
        UTAssert(
            index == x,
            "Invalid index.");
    }

    // index(std::array<std::size_t, 2> const&)
    for (std::size_t i = 0; i < latticeT.size(0); ++i)
    {
        std::array<std::size_t, 1> x{ i };
        std::size_t index = latticeT.index(x);
        UTAssert(
            index == x[0],
            "Invalid index.");
    }

    // coordinate(std::size_t)
    for (std::size_t index = 0; index < latticeT.size(); ++index)
    {
        std::array<std::size_t, 1> x = latticeT.coordinate(index);
        UTAssert(
            index == x[0],
            "Invalid index.");
    }

    // Lattice<false>(), dimensions(), size(std::size_t), size()
    Lattice<false, 2> latticeF{};
    UTAssert(
        latticeF.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF.size() == 2,
        "Invalid number of elements.");

    // index(std::int32_t)
    for (std::size_t x = 0; x < latticeF.size(0); ++x)
    {
        std::size_t index = latticeF.index((std::int32_t)x);
        UTAssert(
            index == x,
            "Invalid index.");
    }

    // index(std::array<std::size_t, 2> const&)
    for (std::size_t i = 0; i < latticeF.size(0); ++i)
    {
        std::array<std::size_t, 1> x{ i };
        std::size_t index = latticeF.index(x);
        UTAssert(
            index == x[0],
            "Invalid index.");
    }

    // coordinate(std::size_t)
    for (std::size_t index = 0; index < latticeF.size(); ++index)
    {
        std::array<std::size_t, 1> x = latticeF.coordinate(index);
        UTAssert(
            x[0] == index,
            "Invalid coordinate.");
    }
}

void UnitTestLattice::TestCompileTime_2_3()
{
    // Lattice<true>, dimensions(), size(std::size_t), size()
    Lattice<true, 2, 3> latticeT{};
    UTAssert(
        latticeT.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeT.size() == 6,
        "Invalid number of elements.");

    // index(std::int32_t, std::int32_t)
    std::size_t index = 0;
    for (std::int32_t i1 = 0; i1 < (std::int32_t)latticeT.size(1); ++i1)
    {
        for (std::int32_t i0 = 0; i0 < (std::int32_t)latticeT.size(0); ++i0, ++index)
        {
            std::size_t k = latticeT.index(i0, i1);
            UTAssert(
                k == index,
                "Invalid index.");
        }
    }

    // index(std::array<std::size_t, 2> const&)
    index = 0;
    for (std::size_t i1 = 0; i1 < latticeT.size(1); ++i1)
    {
        for (std::size_t i0 = 0; i0 < latticeT.size(0); ++i0, ++index)
        {
            std::array<std::size_t, 2> x = { i0, i1 };
            std::size_t k = latticeT.index(x);
            UTAssert(
                k == index,
                "Invalid index.");
        }
    }

    // coordinate(std::size_t)
    for (index = 0; index < latticeT.size(); ++index)
    {
        std::array<std::size_t, 2> x = latticeT.coordinate(index);
        UTAssert(
            x[0] == (index % 2) && x[1] == (index / 2),
            "Invalid coordinate.");
    }

    // Lattice<false>, dimensions(), size(std::size_t), size()
    Lattice<false, 2, 3> latticeF{};
    UTAssert(
        latticeF.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeF.size() == 6,
        "Invalid number of elements.");

    // index(std::int32_t, std::int32_t)
    index = 0;
    for (std::int32_t i0 = 0; i0 < (std::int32_t)latticeF.size(0); ++i0)
    {
        for (std::int32_t i1 = 0; i1 < (std::int32_t)latticeF.size(1); ++i1, ++index)
        {
            std::size_t k = latticeF.index(i0, i1);
            UTAssert(
                k == index,
                "Invalid index.");
        }
    }

    // index(std::array<std::size_t, 2> const&)
    index = 0;
    for (std::size_t i0 = 0; i0 < latticeF.size(0); ++i0)
    {
        for (std::size_t i1 = 0; i1 < latticeF.size(1); ++i1, ++index)
        {
            std::array<std::size_t, 2> x = { i0, i1 };
            std::size_t k = latticeF.index(x);
            UTAssert(
                k == index,
                "Invalid index.");
        }
    }

    // coordinate(std::size_t)
    for (index = 0; index < latticeF.size(); ++index)
    {
        std::array<std::size_t, 2> x = latticeF.coordinate(index);
        UTAssert(
            x[1] == (index % 3) && x[0] == (index / 3),
            "Invalid coordinate.");
    }
}

void UnitTestLattice::TestCompileTime_2_3_5()
{
    // Lattice<true>(), dimensions(), size(std::size_t), size()
    Lattice<true, 2, 3, 5> latticeT;
    UTAssert(
        latticeT.dimensions() == 3,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeT.size(2) == 5,
        "Invalid size for dimension 2.");
    UTAssert(
        latticeT.size() == 30,
        "Invalid number of elements.");

    // index(std::int32_t, std::int32_t, std::int32_t)
    std::size_t index = 0;
    for (std::int32_t i2 = 0; i2 < (std::int32_t)latticeT.size(2); ++i2)
    {
        for (std::int32_t i1 = 0; i1 < (std::int32_t)latticeT.size(1); ++i1)
        {
            for (std::int32_t i0 = 0; i0 < (std::int32_t)latticeT.size(0); ++i0, ++index)
            {
                std::size_t k = latticeT.index(i0, i1, i2);
                UTAssert(
                    k == index,
                    "Invalid index.");
            }
        }
    }

    // index(std::array<std::size_t, 3> const&)
    index = 0;
    for (std::size_t i2 = 0; i2 < latticeT.size(2); ++i2)
    {
        for (std::size_t i1 = 0; i1 < latticeT.size(1); ++i1)
        {
            for (std::size_t i0 = 0; i0 < latticeT.size(0); ++i0, ++index)
            {
                std::array<std::size_t, 3> x = { i0, i1, i2 };
                std::size_t k = latticeT.index(x);
                UTAssert(
                    k == index,
                    "Invalid index.");
            }
        }
    }

    // coordinate(std::size_t)
    for (index = 0; index < latticeT.size(); ++index)
    {
        std::array<std::size_t, 3> x = latticeT.coordinate(index);
        UTAssert(
            x[0] == (index % 2) && x[1] == ((index / 2) % 3) && x[2] == ((index / 2) / 3),
            "Invalid coordinate.");
    }

    // Lattice<false>(), dimensions(), size(std::size_t), size()
    Lattice<false, 2, 3, 5> latticeF;
    UTAssert(
        latticeF.dimensions() == 3,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeF.size(2) == 5,
        "Invalid size for dimension 2.");
    UTAssert(
        latticeF.size() == 30,
        "Invalid number of elements.");

    // index(std::int32_t, std::int32_t, std::int32_t)
    index = 0;
    for (std::int32_t i0 = 0; i0 < (std::int32_t)latticeF.size(0); ++i0)
    {
        for (std::int32_t i1 = 0; i1 < (std::int32_t)latticeF.size(1); ++i1)
        {
            for (std::int32_t i2 = 0; i2 < (std::int32_t)latticeF.size(2); ++i2, ++index)
            {
                std::size_t k = latticeF.index(i0, i1, i2);
                UTAssert(
                    k == index,
                    "Invalid index.");
            }
        }
    }

    // index(std::array<std::size_t, 3> const&)
    index = 0;
    for (std::size_t i0 = 0; i0 < latticeF.size(0); ++i0)
    {
        for (std::size_t i1 = 0; i1 < latticeF.size(1); ++i1)
        {
            for (std::size_t i2 = 0; i2 < latticeF.size(2); ++i2, ++index)
            {
                std::array<std::size_t, 3> x = { i0, i1, i2 };
                std::size_t k = latticeF.index(x);
                UTAssert(
                    k == index,
                    "Invalid index.");
            }
        }
    }

    // coordinate(std::size_t)
    for (index = 0; index < latticeF.size(); ++index)
    {
        std::array<std::size_t, 3> x = latticeF.coordinate(index);
        UTAssert(
            x[2] == (index % 5) && x[1] == ((index / 5) % 3) && x[0] == ((index / 5) / 3),
            "Invalid coordinate.");
    }
}

void UnitTestLattice::TestRunTime_1D()
{
    // Lattice<true>{}, dimension(), size(std::size_t), size()
    Lattice<true> latticeT0{};
    UTAssert(
        latticeT0.dimensions() == 0,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT0.size() == 0,
        "Invalid number of elements.");

    // Lattice<true>(std::vector<std::size_t> const&), dimension(), size(std::size_t), size()
    Lattice<true> latticeT1({ 2 });
    UTAssert(
        latticeT1.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT1.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT1.size() == 2,
        "Invalid number of elements.");

    // Lattice<true>(std::initializer_list<std::size_t> const&), dimension(), size(std::size_t), size()
    Lattice<true> latticeT2{ 3 };
    UTAssert(
        latticeT2.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT2.size(0) == 3,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT2.size() == 3,
        "Invalid number of elements.");
    Lattice<true> saveT{ 3 };

    // Lattice<true>(Lattice const&), dimension(), size(std::size_t), size()
    Lattice<true> latticeT3 = latticeT1;
    UTAssert(
        latticeT3.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT3.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT3.size() == 2,
        "Invalid number of elements.");
    UTAssert(
        latticeT1.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT1.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT1.size() == 2,
        "Invalid number of elements.");

    // Lattice<true>(Lattice&&), dimension(), size(std::size_t), size()
    Lattice<true> latticeT4 = std::move(saveT);
    UTAssert(
        latticeT4.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT4.size(0) == 3,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT4.size() == 3,
        "Invalid number of elements.");

    // index(std::int32_t...)
    for (std::size_t x = 0; x < latticeT3.size(0); ++x)
    {
        std::size_t index = latticeT3.index((std::int32_t)x);
        UTAssert(
            index == x,
            "Invalid index.");
    }

    // index(std::vector<std::size_t> const&)
    for (std::size_t i = 0; i < latticeT3.size(0); ++i)
    {
        std::vector<std::size_t> x{ i };
        std::size_t index = latticeT3.index(x);
        UTAssert(
            index == x[0],
            "Invalid index.");
    }

    // coordinate(std::size_t)
    for (std::size_t index = 0; index < latticeT3.size(); ++index)
    {
        std::vector<std::size_t> x = latticeT3.coordinate(index);
        UTAssert(
            index == x[0],
            "Invalid index.");
    }


    // Lattice<false>{}, dimension(), size(std::size_t), size()
    Lattice<false> latticeF0{};
    UTAssert(
        latticeF0.dimensions() == 0,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF0.size() == 0,
        "Invalid number of elements.");

    // Lattice<false>(std::vector<std::size_t> const&), dimension(), size(std::size_t), size()
    Lattice<false> latticeF1({ 2 });
    UTAssert(
        latticeF1.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF1.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF1.size() == 2,
        "Invalid number of elements.");

    // Lattice<false>(std::initializer_list<std::size_t> const&), dimension(), size(std::size_t), size()
    Lattice<false> latticeF2{ 3 };
    UTAssert(
        latticeF2.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF2.size(0) == 3,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF2.size() == 3,
        "Invalid number of elements.");
    Lattice<false> saveF{ 3 };

    // Lattice<false>(Lattice const&), dimension(), size(std::size_t), size()
    Lattice<false> latticeF3 = latticeF1;
    UTAssert(
        latticeF3.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF3.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF3.size() == 2,
        "Invalid number of elements.");
    UTAssert(
        latticeF1.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF1.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF1.size() == 2,
        "Invalid number of elements.");

    // Lattice<false>(Lattice&&), dimension(), size(std::size_t), size()
    Lattice<false> latticeF4 = std::move(saveF);
    UTAssert(
        latticeF4.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF4.size(0) == 3,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF4.size() == 3,
        "Invalid number of elements.");

    // index(std::int32_t...)
    for (std::size_t x = 0; x < latticeF3.size(0); ++x)
    {
        std::size_t index = latticeF3.index((std::int32_t)x);
        UTAssert(
            index == x,
            "Invalid index.");
    }

    // index(std::vector<std::size_t> const&)
    for (std::size_t i = 0; i < latticeF3.size(0); ++i)
    {
        std::vector<std::size_t> x{ i };
        std::size_t index = latticeF3.index(x);
        UTAssert(
            index == x[0],
            "Invalid index.");
    }

    // coordinate(std::size_t)
    for (std::size_t index = 0; index < latticeF3.size(); ++index)
    {
        std::vector<std::size_t> x = latticeF3.coordinate(index);
        UTAssert(
            index == x[0],
            "Invalid index.");
    }
}

void UnitTestLattice::TestRunTime_2D()
{
    // Lattice<true>{}, dimension(), size(std::size_t), size()
    Lattice<true> latticeT0{};
    UTAssert(
        latticeT0.dimensions() == 0,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT0.size() == 0,
        "Invalid number of elements.");

    // Lattice<true>(std::vector<std::size_t> const&), dimension(), size(std::size_t), size()
    Lattice<true> latticeT1({ 2, 3 });
    UTAssert(
        latticeT1.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT1.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT1.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeT1.size() == 6,
        "Invalid number of elements.");

    // Lattice<true>(std::initializer_list<std::size_t> const&), dimension(), size(std::size_t), size()
    Lattice<true> latticeT2{ 4, 5 };
    UTAssert(
        latticeT2.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT2.size(0) == 4,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT2.size(1) == 5,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeT2.size() == 20,
        "Invalid number of elements.");
    Lattice<true> saveT{ 4, 5 };

    // Lattice<true>(Lattice const&), dimension(), size(std::size_t), size()
    Lattice<true> latticeT3 = latticeT1;
    UTAssert(
        latticeT3.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT3.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT3.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeT3.size() == 6,
        "Invalid number of elements.");
    UTAssert(
        latticeT1.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT1.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT1.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeT1.size() == 6,
        "Invalid number of elements.");

    // Lattice<true>(Lattice&&), dimension(), size(std::size_t), size()
    Lattice<true> latticeT4 = std::move(saveT);
    UTAssert(
        latticeT4.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT4.size(0) == 4,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT4.size(1) == 5,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeT4.size() == 20,
        "Invalid number of elements.");

    // index(std::int32_t, std::int32_t)
    std::size_t index = 0;
    for (std::int32_t i1 = 0; i1 < (std::int32_t)latticeT3.size(1); ++i1)
    {
        for (std::int32_t i0 = 0; i0 < (std::int32_t)latticeT3.size(0); ++i0, ++index)
        {
            std::size_t k = latticeT3.index(i0, i1);
            UTAssert(
                k == index,
                "Invalid index.");
        }
    }

    // index(std::vector<std::size_t> const&)
    index = 0;
    for (std::size_t i1 = 0; i1 < latticeT3.size(1); ++i1)
    {
        for (std::size_t i0 = 0; i0 < latticeT3.size(0); ++i0, ++index)
        {
            std::vector<std::size_t> x = { i0, i1 };
            std::size_t k = latticeT3.index(x);
            UTAssert(
                k == index,
                "Invalid index.");
        }
    }

    // coordinate(std::size_t)
    for (index = 0; index < latticeT3.size(); ++index)
    {
        std::vector<std::size_t> x = latticeT3.coordinate(index);
        UTAssert(
            x[0] == (index % 2) && x[1] == (index / 2),
            "Invalid coordinate.");
    }


    // Lattice<false>{}, dimension(), size(std::size_t), size()
    Lattice<false> latticeF0{};
    UTAssert(
        latticeF0.dimensions() == 0,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF0.size() == 0,
        "Invalid number of elements.");

    // Lattice<false>(std::vector<std::size_t> const&), dimension(), size(std::size_t), size()
    Lattice<false> latticeF1({ 2, 3 });
    UTAssert(
        latticeF1.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF1.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF1.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeF1.size() == 6,
        "Invalid number of elements.");

    // Lattice<false>(std::initializer_list<std::size_t> const&), dimension(), size(std::size_t), size()
    Lattice<false> latticeF2{ 4, 5 };
    UTAssert(
        latticeF2.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF2.size(0) == 4,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF2.size(1) == 5,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeF2.size() == 20,
        "Invalid number of elements.");
    Lattice<false> saveF{ 4, 5 };

    // Lattice<false>(Lattice const&), dimension(), size(std::size_t), size()
    Lattice<false> latticeF3 = latticeF1;
    UTAssert(
        latticeF3.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF3.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF3.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeF3.size() == 6,
        "Invalid number of elements.");
    UTAssert(
        latticeF1.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF1.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF1.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeF1.size() == 6,
        "Invalid number of elements.");

    // Lattice<false>(Lattice&&), dimension(), size(std::size_t), size()
    Lattice<false> latticeF4 = std::move(saveF);
    UTAssert(
        latticeF4.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF4.size(0) == 4,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF4.size(1) == 5,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeF4.size() == 20,
        "Invalid number of elements.");

    // index(std::int32_t, std::int32_t)
    index = 0;
    for (std::int32_t i0 = 0; i0 < (std::int32_t)latticeF3.size(0); ++i0)
    {
        for (std::int32_t i1 = 0; i1 < (std::int32_t)latticeF3.size(1); ++i1, ++index)
        {
            std::size_t k = latticeF3.index(i0, i1);
            UTAssert(
                k == index,
                "Invalid index.");
        }
    }

    // index(std::vector<std::size_t> const&)
    index = 0;
    for (std::size_t i0 = 0; i0 < latticeF3.size(0); ++i0)
    {
        for (std::size_t i1 = 0; i1 < latticeF3.size(1); ++i1, ++index)
        {
            std::vector<std::size_t> x = { i0, i1 };
            std::size_t k = latticeF3.index(x);
            UTAssert(
                k == index,
                "Invalid index.");
        }
    }

    // coordinate(std::size_t)
    for (index = 0; index < latticeF3.size(); ++index)
    {
        std::vector<std::size_t> x = latticeF3.coordinate(index);
        UTAssert(
            x[1] == (index % 3) && x[0] == (index / 3),
            "Invalid coordinate.");
    }
}

void UnitTestLattice::TestRunTime_3D()
{
    // Lattice<true>{}, dimension(), size(std::size_t), size()
    Lattice<true> latticeT0{};
    UTAssert(
        latticeT0.dimensions() == 0,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT0.size() == 0,
        "Invalid number of elements.");

    // Lattice<true>(std::vector<std::size_t> const&), dimension(), size(std::size_t), size()
    Lattice<true> latticeT1({ 2, 3, 5 });
    UTAssert(
        latticeT1.dimensions() == 3,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT1.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT1.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeT1.size(2) == 5,
        "Invalid size for dimension 2.");
    UTAssert(
        latticeT1.size() == 30,
        "Invalid number of elements.");

    // Lattice<true>(std::initializer_list<std::size_t> const&), dimension(), size(std::size_t), size()
    Lattice<true> latticeT2{ 7, 11, 13 };
    UTAssert(
        latticeT2.dimensions() == 3,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT2.size(0) == 7,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT2.size(1) == 11,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeT2.size(2) == 13,
        "Invalid size for dimension 2.");
    UTAssert(
        latticeT2.size() == 1001,
        "Invalid number of elements.");
    Lattice<true> saveT{ 7, 11, 13 };

    // Lattice<true>(Lattice const&), dimension(), size(std::size_t), size()
    Lattice<true> latticeT3 = latticeT1;
    UTAssert(
        latticeT3.dimensions() == 3,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT3.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT3.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeT3.size(2) == 5,
        "Invalid size for dimension 2.");
    UTAssert(
        latticeT3.size() == 30,
        "Invalid number of elements.");
    UTAssert(
        latticeT1.dimensions() == 3,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT1.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT1.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeT1.size(2) == 5,
        "Invalid size for dimension 2.");
    UTAssert(
        latticeT1.size() == 30,
        "Invalid number of elements.");

    // Lattice<true>(Lattice&&), dimension(), size(std::size_t), size()
    Lattice<true> latticeT4 = std::move(saveT);
    UTAssert(
        latticeT4.dimensions() == 3,
        "Invalid number of dimensions.");
    UTAssert(
        latticeT4.size(0) == 7,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeT4.size(1) == 11,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeT4.size(2) == 13,
        "Invalid size for dimension 2.");
    UTAssert(
        latticeT4.size() == 1001,
        "Invalid number of elements.");

    // index(std::int32_t, std::int32_t, std::int32_t)
    std::size_t index = 0;
    for (std::int32_t i2 = 0; i2 < (std::int32_t)latticeT3.size(2); ++i2)
    {
        for (std::int32_t i1 = 0; i1 < (std::int32_t)latticeT3.size(1); ++i1)
        {
            for (std::int32_t i0 = 0; i0 < (std::int32_t)latticeT3.size(0); ++i0, ++index)
            {
                std::size_t k = latticeT3.index(i0, i1, i2);
                UTAssert(
                    k == index,
                    "Invalid index.");
            }
        }
    }

    // index(std::vector<std::size_t> const&)
    index = 0;
    for (std::size_t i2 = 0; i2 < latticeT3.size(2); ++i2)
    {
        for (std::size_t i1 = 0; i1 < latticeT3.size(1); ++i1)
        {
            for (std::size_t i0 = 0; i0 < latticeT3.size(0); ++i0, ++index)
            {
                std::vector<std::size_t> x = { i0, i1, i2 };
                std::size_t k = latticeT3.index(x);
                UTAssert(
                    k == index,
                    "Invalid index.");
            }
        }
    }

    // coordinate(std::size_t)
    for (index = 0; index < latticeT3.size(); ++index)
    {
        std::vector<std::size_t> x = latticeT3.coordinate(index);
        UTAssert(
            x[0] == (index % 2) && x[1] == ((index / 2) % 3) && x[2] == ((index / 2) / 3),
            "Invalid coordinate.");
    }


    // Lattice<false>{}, dimension(), size(std::size_t), size()
    Lattice<false> latticeF0{};
    UTAssert(
        latticeF0.dimensions() == 0,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF0.size() == 0,
        "Invalid number of elements.");

    // Lattice<false>(std::vector<std::size_t> const&), dimension(), size(std::size_t), size()
    Lattice<false> latticeF1({ 2, 3, 5 });
    UTAssert(
        latticeF1.dimensions() == 3,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF1.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF1.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeF1.size(2) == 5,
        "Invalid size for dimension 2.");
    UTAssert(
        latticeF1.size() == 30,
        "Invalid number of elements.");

    // Lattice<false>(std::initializer_list<std::size_t> const&), dimension(), size(std::size_t), size()
    Lattice<false> latticeF2{ 7, 11, 13 };
    UTAssert(
        latticeF2.dimensions() == 3,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF2.size(0) == 7,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF2.size(1) == 11,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeF2.size(2) == 13,
        "Invalid size for dimension 2.");
    UTAssert(
        latticeF2.size() == 1001,
        "Invalid number of elements.");
    Lattice<false> saveF{ 7, 11, 13 };

    // Lattice<false>(Lattice const&), dimension(), size(std::size_t), size()
    Lattice<false> latticeF3 = latticeF1;
    UTAssert(
        latticeF3.dimensions() == 3,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF3.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF3.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeF3.size(2) == 5,
        "Invalid size for dimension 2.");
    UTAssert(
        latticeF3.size() == 30,
        "Invalid number of elements.");
    UTAssert(
        latticeF1.dimensions() == 3,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF1.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF1.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeF1.size(2) == 5,
        "Invalid size for dimension 2.");
    UTAssert(
        latticeF1.size() == 30,
        "Invalid number of elements.");

    // Lattice<false>(Lattice&&), dimension(), size(std::size_t), size()
    Lattice<false> latticeF4 = std::move(saveF);
    UTAssert(
        latticeF4.dimensions() == 3,
        "Invalid number of dimensions.");
    UTAssert(
        latticeF4.size(0) == 7,
        "Invalid size for dimension 0.");
    UTAssert(
        latticeF4.size(1) == 11,
        "Invalid size for dimension 1.");
    UTAssert(
        latticeF4.size(2) == 13,
        "Invalid size for dimension 2.");
    UTAssert(
        latticeF4.size() == 1001,
        "Invalid number of elements.");

    // index(std::int32_t, std::int32_t, std::int32_t)
    index = 0;
    for (std::int32_t i0 = 0; i0 < (std::int32_t)latticeF3.size(0); ++i0)
    {
        for (std::int32_t i1 = 0; i1 < (std::int32_t)latticeF3.size(1); ++i1)
        {
            for (std::int32_t i2 = 0; i2 < (std::int32_t)latticeF3.size(2); ++i2, ++index)
            {
                std::size_t k = latticeF3.index(i0, i1, i2);
                UTAssert(
                    k == index,
                    "Invalid index.");
            }
        }
    }

    // index(std::vector<std::size_t> const&)
    index = 0;
    for (std::size_t i0 = 0; i0 < latticeF3.size(0); ++i0)
    {
        for (std::size_t i1 = 0; i1 < latticeF3.size(1); ++i1)
        {
            for (std::size_t i2 = 0; i2 < latticeF3.size(2); ++i2, ++index)
            {
                std::vector<std::size_t> x = { i0, i1, i2 };
                std::size_t k = latticeF3.index(x);
                UTAssert(
                    k == index,
                    "Invalid index.");
            }
        }
    }

    // coordinate(std::size_t)
    for (index = 0; index < latticeF3.size(); ++index)
    {
        std::vector<std::size_t> x = latticeF3.coordinate(index);
        UTAssert(
            x[2] == (index % 5) && x[1] == ((index / 5) % 3) && x[0] == ((index / 5) / 3),
            "Invalid coordinate.");
    }
}

#else

#include <GTL/Utility/Lattice.h>

namespace gtl
{
    template class Lattice<true, 2>;
    template class Lattice<true, 2, 3>;
    template class Lattice<true, 2, 3, 5>;

    template class Lattice<false, 2>;
    template class Lattice<false, 2, 3>;
    template class Lattice<false, 2, 3, 5>;

    template class Lattice<true>;
    template class Lattice<false>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Lattice)
