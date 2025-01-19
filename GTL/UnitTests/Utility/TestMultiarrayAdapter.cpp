// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Utility/MultiarrayAdapter.h>
using namespace gtl;

namespace gtl
{
    class UnitTestMultiarrayAdapter
    {
    public:
        UnitTestMultiarrayAdapter();

    private:
        void TestCompileTime_2();
        void TestCompileTime_2_3();
        void TestCompileTime_2_3_5();
        void TestCompileTime_2_3_5_7();
        void TestRunTime_2();
        void TestRunTime_2_3();
    };
}

UnitTestMultiarrayAdapter::UnitTestMultiarrayAdapter()
{
    UTInformation("Mathematics/Utility/MultiarrayAdapter");

    TestCompileTime_2();
    TestCompileTime_2_3();
    TestCompileTime_2_3_5();
    TestCompileTime_2_3_5_7();
    TestRunTime_2();
    TestRunTime_2_3();
}

void UnitTestMultiarrayAdapter::TestCompileTime_2()
{
    MultiarrayAdapter<std::int32_t, true, 2> maT0{};
    UTAssert(
        maT0.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        maT0.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        maT0.size() == 2,
        "Invalid number of elements.");
    UTAssert(
        maT0.data() == nullptr,
        "Even though dimension and sizes are not zero, data is null.");

    std::array<std::int32_t, 2> maT0Storage{};
    maT0.reset(maT0Storage.data());

    maT0.fill(17);
    std::int32_t const* datac = maT0.data();
    UTAssert(
        datac != nullptr && datac[0] == 17 && datac[1] == 17,
        "fill or data() const failed.");

    std::int32_t* data = maT0.data();
    UTAssert(
        data != nullptr && data[0] == 17 && data[1] == 17,
        "data() failed.");
    data[0] = 1;
    data[1] = 2;

    UTAssert(
        maT0[0] == 1 && maT0[1] == 2,
        "operator[] const failed.");
    maT0[0] = 3;
    maT0[1] = 4;
    UTAssert(
        maT0[0] == 3 && maT0[1] == 4,
        "operator[] failed.");

    UTAssert(
        maT0(0) == 3 && maT0(1) == 4,
        "operator(...) const failed.");
    maT0(0) = 1;
    maT0(1) = 2;
    UTAssert(
        maT0(0) == 1 && maT0(1) == 2,
        "operator(...) failed.");

    std::array<std::int32_t, 2> maT1Storage{};
    MultiarrayAdapter<std::int32_t, true, 2> maT1(maT1Storage.data());
    maT1[0] = 3;
    maT1[1] = 4;

    UTAssert(
        (maT0 == maT1) == false,
        "operator== failed");
    UTAssert(
        (maT0 != maT1) == true,
        "operator!= failed");
    UTAssert(
        (maT0 < maT1) == true,
        "operator< failed");
    UTAssert(
        (maT0 <= maT1) == true,
        "operator<= failed");
    UTAssert(
        (maT0 > maT1) == false,
        "operator> failed");
    UTAssert(
        (maT0 >= maT1) == false,
        "operator>= failed");
}

void UnitTestMultiarrayAdapter::TestCompileTime_2_3()
{
    MultiarrayAdapter<std::int32_t, true, 2, 3> maT0;
    UTAssert(
        maT0.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        maT0.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        maT0.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        maT0.size() == 6,
        "Invalid number of elements.");

    std::array<std::int32_t, 6> maT0Storage{};
    maT0.reset(maT0Storage.data());

    maT0.fill(17);
    std::int32_t const* datac = maT0.data();
    UTAssert(
        datac != nullptr,
        "data() const failed.");
    std::size_t index;
    for (index = 0; index < maT0.size(); ++index)
    {
        UTAssert(
            datac[index] == 17,
            "fill or data() const failed.");
    }

    std::int32_t* data = maT0.data();
    UTAssert(
        data != nullptr,
        "data() failed.");
    for (index = 0; index < maT0.size(); ++index)
    {
        UTAssert(
            data[index] == 17,
            "fill or data() const failed.");
    }

    for (index = 0; index < maT0.size(); ++index)
    {
        data[index] = (std::int32_t)(index + 1);
        UTAssert(
            data[index] == (std::int32_t)(index + 1),
            "operator[] const or operator[] failed.");
    }

    index = 0;
    for (std::size_t i1 = 0; i1 < maT0.size(1); ++i1)
    {
        for (std::size_t i0 = 0; i0 < maT0.size(0); ++i0, ++index)
        {
            UTAssert(
                maT0(i0, i1) == (std::int32_t)(index + 1),
                "operator(...) const failed.");
        }
    }

    index = 0;
    for (std::size_t i1 = 0; i1 < maT0.size(1); ++i1)
    {
        for (std::size_t i0 = 0; i0 < maT0.size(0); ++i0, ++index)
        {
            maT0(i0, i1) = (std::int32_t)(index + 2);
            UTAssert(
                maT0(i0, i1) == (std::int32_t)(index + 2),
                "operator(...) failed.");
        }
    }

    std::array<std::int32_t, 6> maT1Storage{};
    MultiarrayAdapter<std::int32_t, true, 2, 3> maT1{};
    maT1.reset(maT1Storage.data());
    for (index = 0; index < maT0.size(); ++index)
    {
        maT1[index] = (std::int32_t)(index + 3);
    }

    UTAssert(
        (maT0 == maT1) == false,
        "operator== failed");
    UTAssert(
        (maT0 != maT1) == true,
        "operator!= failed");
    UTAssert(
        (maT0 < maT1) == true,
        "operator< failed");
    UTAssert(
        (maT0 <= maT1) == true,
        "operator<= failed");
    UTAssert(
        (maT0 > maT1) == false,
        "operator> failed");
    UTAssert(
        (maT0 >= maT1) == false,
        "operator>= failed");
}

void UnitTestMultiarrayAdapter::TestCompileTime_2_3_5()
{
    MultiarrayAdapter<std::int32_t, true, 2, 3, 5> maT0{};
    UTAssert(
        maT0.dimensions() == 3,
        "Invalid number of dimensions.");
    UTAssert(
        maT0.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        maT0.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        maT0.size(2) == 5,
        "Invalid size for dimension 2.");
    UTAssert(
        maT0.size() == 30,
        "Invalid number of elements.");

    std::array<std::int32_t, 30> maT0Storage{};
    maT0.reset(maT0Storage.data());
    maT0.fill(17);
    std::int32_t const* datac = maT0.data();
    UTAssert(
        datac != nullptr,
        "data() const failed.");
    std::size_t index;
    for (index = 0; index < maT0.size(); ++index)
    {
        UTAssert(
            datac[index] == 17,
            "fill or data() const failed.");
    }

    std::int32_t* data = maT0.data();
    UTAssert(
        data != nullptr,
        "data() failed.");
    for (index = 0; index < maT0.size(); ++index)
    {
        UTAssert(
            data[index] == 17,
            "fill or data() const failed.");
    }

    for (index = 0; index < maT0.size(); ++index)
    {
        data[index] = (std::int32_t)(index + 1);
        UTAssert(
            data[index] == (std::int32_t)(index + 1),
            "operator[] const or operator[] failed.");
    }

    index = 0;
    for (std::size_t i2 = 0; i2 < maT0.size(2); ++i2)
    {
        for (std::size_t i1 = 0; i1 < maT0.size(1); ++i1)
        {
            for (std::size_t i0 = 0; i0 < maT0.size(0); ++i0, ++index)
            {
                UTAssert(
                    maT0(i0, i1, i2) == (std::int32_t)(index + 1),
                    "operator(...) const failed.");
            }
        }
    }

    index = 0;
    for (std::size_t i2 = 0; i2 < maT0.size(2); ++i2)
    {
        for (std::size_t i1 = 0; i1 < maT0.size(1); ++i1)
        {
            for (std::size_t i0 = 0; i0 < maT0.size(0); ++i0, ++index)
            {
                maT0(i0, i1, i2) = (std::int32_t)(index + 2);
                UTAssert(
                    maT0(i0, i1, i2) == (std::int32_t)(index + 2),
                    "operator(...) failed.");
            }
        }
    }

    std::array<std::int32_t, 30> maT1Storage{};
    MultiarrayAdapter<std::int32_t, true, 2, 3, 5> maT1;
    maT1.reset(maT1Storage.data());
    for (index = 0; index < maT0.size(); ++index)
    {
        maT1[index] = (std::int32_t)(index + 3);
    }

    UTAssert(
        (maT0 == maT1) == false,
        "operator== failed");
    UTAssert(
        (maT0 != maT1) == true,
        "operator!= failed");
    UTAssert(
        (maT0 < maT1) == true,
        "operator< failed");
    UTAssert(
        (maT0 <= maT1) == true,
        "operator<= failed");
    UTAssert(
        (maT0 > maT1) == false,
        "operator> failed");
    UTAssert(
        (maT0 >= maT1) == false,
        "operator>= failed");
}

void UnitTestMultiarrayAdapter::TestCompileTime_2_3_5_7()
{
    MultiarrayAdapter<std::int32_t, true, 2, 3, 5, 7> maT0;
    UTAssert(
        maT0.dimensions() == 4,
        "Invalid number of dimensions.");
    UTAssert(
        maT0.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        maT0.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        maT0.size(2) == 5,
        "Invalid size for dimension 2.");
    UTAssert(
        maT0.size(3) == 7,
        "Invalid size for dimension 3.");
    UTAssert(
        maT0.size() == 210,
        "Invalid number of elements.");

    std::array<std::int32_t, 210> maT0Storage{};
    maT0.reset(maT0Storage.data());
    maT0.fill(17);
    std::int32_t const* datac = maT0.data();
    UTAssert(
        datac != nullptr,
        "data() const failed.");
    std::size_t index;
    for (index = 0; index < maT0.size(); ++index)
    {
        UTAssert(
            datac[index] == 17,
            "fill or data() const failed.");
    }

    std::int32_t* data = maT0.data();
    UTAssert(
        data != nullptr,
        "data() failed.");
    for (index = 0; index < maT0.size(); ++index)
    {
        UTAssert(
            data[index] == 17,
            "fill or data() const failed.");
    }

    for (index = 0; index < maT0.size(); ++index)
    {
        data[index] = (std::int32_t)(index + 1);
        UTAssert(
            data[index] == (std::int32_t)(index + 1),
            "operator[] const or operator[] failed.");
    }

    index = 0;
    for (std::size_t i3 = 0; i3 < maT0.size(3); ++i3)
    {
        for (std::size_t i2 = 0; i2 < maT0.size(2); ++i2)
        {
            for (std::size_t i1 = 0; i1 < maT0.size(1); ++i1)
            {
                for (std::size_t i0 = 0; i0 < maT0.size(0); ++i0, ++index)
                {
                    std::size_t k = maT0.index(i0, i1, i2, i3);
                    UTAssert(
                        k == index,
                        "Invalid index.");
                    std::array<std::size_t, 4> sizes = { i0, i1, i2, i3 };
                    k = maT0.index(sizes);
                    UTAssert(
                        k == index,
                        "Invalid index.");
                }
            }
        }
    }

    MultiarrayAdapter<std::int32_t, false, 2, 3, 5, 7> maF0;
    index = 0;
    for (std::size_t i0 = 0; i0 < maF0.size(0); ++i0)
    {
        for (std::size_t i1 = 0; i1 < maF0.size(1); ++i1)
        {
            for (std::size_t i2 = 0; i2 < maT0.size(2); ++i2)
            {
                for (std::size_t i3 = 0; i3 < maT0.size(3); ++i3, ++index)
                {
                    std::size_t k = maF0.index(i0, i1, i2, i3);
                    UTAssert(
                        k == index,
                        "Invalid index.");
                    std::array<std::size_t, 4> sizes = { i0, i1, i2, i3 };
                    k = maF0.index(sizes);
                    UTAssert(
                        k == index,
                        "Invalid index.");
                }
            }
        }
    }

    index = 0;
    for (std::size_t i3 = 0; i3 < maT0.size(3); ++i3)
    {
        for (std::size_t i2 = 0; i2 < maT0.size(2); ++i2)
        {
            for (std::size_t i1 = 0; i1 < maT0.size(1); ++i1)
            {
                for (std::size_t i0 = 0; i0 < maT0.size(0); ++i0, ++index)
                {
                    UTAssert(
                        maT0(i0, i1, i2, i3) == (std::int32_t)(index + 1),
                        "operator(...) const failed.");
                }
            }
        }
    }

    index = 0;
    for (std::size_t i3 = 0; i3 < maT0.size(3); ++i3)
    {
        for (std::size_t i2 = 0; i2 < maT0.size(2); ++i2)
        {
            for (std::size_t i1 = 0; i1 < maT0.size(1); ++i1)
            {
                for (std::size_t i0 = 0; i0 < maT0.size(0); ++i0, ++index)
                {
                    maT0(i0, i1, i2, i3) = (std::int32_t)(index + 2);
                    UTAssert(
                        maT0(i0, i1, i2, i3) == (std::int32_t)(index + 2),
                        "operator(...) failed.");
                }
            }
        }
    }

    MultiarrayAdapter<std::int32_t, true, 2, 3, 5, 7> maT1;
    std::array<std::int32_t, 210> maT1Storage{};
    maT1.reset(maT1Storage.data());
    for (index = 0; index < maT0.size(); ++index)
    {
        maT1[index] = (std::int32_t)(index + 3);
    }

    UTAssert(
        (maT0 == maT1) == false,
        "operator== failed");
    UTAssert(
        (maT0 != maT1) == true,
        "operator!= failed");
    UTAssert(
        (maT0 < maT1) == true,
        "operator< failed");
    UTAssert(
        (maT0 <= maT1) == true,
        "operator<= failed");
    UTAssert(
        (maT0 > maT1) == false,
        "operator> failed");
    UTAssert(
        (maT0 >= maT1) == false,
        "operator>= failed");
}

void UnitTestMultiarrayAdapter::TestRunTime_2()
{
    std::vector<std::int32_t> maT0Storage(2);
    MultiarrayAdapter<std::int32_t, true> maT0({ 2 }, maT0Storage.data());
    UTAssert(
        maT0.dimensions() == 1,
        "Invalid number of dimensions.");
    UTAssert(
        maT0.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        maT0.size() == 2,
        "Invalid number of elements.");

    maT0.fill(17);
    std::int32_t const* datac = maT0.data();
    UTAssert(
        datac != nullptr && datac[0] == 17 && datac[1] == 17,
        "fill or data() const failed.");

    std::int32_t* data = maT0.data();
    UTAssert(
        data != nullptr && data[0] == 17 && data[1] == 17,
        "data() failed.");
    data[0] = 1;
    data[1] = 2;

    UTAssert(
        maT0[0] == 1 && maT0[1] == 2,
        "operator[] const failed.");
    maT0[0] = 3;
    maT0[1] = 4;
    UTAssert(
        maT0[0] == 3 && maT0[1] == 4,
        "operator[] failed.");

    std::size_t index;
    for (std::size_t i0 = 0; i0 < maT0.size(); ++i0)
    {
        index = maT0.index(i0);
        UTAssert(
            i0 == index,
            "Invalid index.");
        std::vector<std::size_t> coordinate(1);
        coordinate[0] = i0;
        index = maT0.index(coordinate);
        UTAssert(
            i0 == index,
            "Invalid index.");
    }

    std::vector<std::int32_t> maF0Storage(2);
    MultiarrayAdapter<std::int32_t, false> maF0({ 2 }, maF0Storage.data());
    for (std::size_t i0 = 0; i0 < maF0.size(); ++i0)
    {
        index = maF0.index(i0);
        UTAssert(
            i0 == index,
            "Invalid index.");
        std::vector<std::size_t> coordinate(1);
        coordinate[0] = i0;
        index = maF0.index(coordinate);
        UTAssert(
            i0 == index,
            "Invalid index.");
    }

    UTAssert(
        maT0(0) == 3 && maT0(1) == 4,
        "operator(...) const failed.");
    maT0(0) = 1;
    maT0(1) = 2;
    UTAssert(
        maT0(0) == 1 && maT0(1) == 2,
        "operator(...) failed.");

    std::vector<std::int32_t> maT1Storage(2);
    MultiarrayAdapter<std::int32_t, true> maT1({ 2 }, maT1Storage.data());
    maT1[0] = 3;
    maT1[1] = 4;

    UTAssert(
        (maT0 == maT1) == false,
        "operator== failed");
    UTAssert(
        (maT0 != maT1) == true,
        "operator!= failed");
    UTAssert(
        (maT0 < maT1) == true,
        "operator< failed");
    UTAssert(
        (maT0 <= maT1) == true,
        "operator<= failed");
    UTAssert(
        (maT0 > maT1) == false,
        "operator> failed");
    UTAssert(
        (maT0 >= maT1) == false,
        "operator>= failed");
}

void UnitTestMultiarrayAdapter::TestRunTime_2_3()
{
    std::vector<std::int32_t> maT0Storage(6);
    MultiarrayAdapter<std::int32_t, true> maT0({ 2, 3 }, maT0Storage.data());
    UTAssert(
        maT0.dimensions() == 2,
        "Invalid number of dimensions.");
    UTAssert(
        maT0.size(0) == 2,
        "Invalid size for dimension 0.");
    UTAssert(
        maT0.size(1) == 3,
        "Invalid size for dimension 1.");
    UTAssert(
        maT0.size() == 6,
        "Invalid number of elements.");

    maT0.fill(17);
    std::int32_t const* datac = maT0.data();
    UTAssert(
        datac != nullptr,
        "data() const failed.");
    std::size_t index;
    for (index = 0; index < maT0.size(); ++index)
    {
        UTAssert(
            datac[index] == 17,
            "fill or data() const failed.");
    }

    std::int32_t* data = maT0.data();
    UTAssert(
        data != nullptr,
        "data() failed.");
    for (index = 0; index < maT0.size(); ++index)
    {
        UTAssert(
            data[index] == 17,
            "fill or data() const failed.");
    }

    for (index = 0; index < maT0.size(); ++index)
    {
        data[index] = (std::int32_t)(index + 1);
        UTAssert(
            data[index] == (std::int32_t)(index + 1),
            "operator[] const or operator[] failed.");
    }

    index = 0;
    for (std::size_t i1 = 0; i1 < maT0.size(1); ++i1)
    {
        for (std::size_t i0 = 0; i0 < maT0.size(0); ++i0, ++index)
        {
            std::size_t k = maT0.index(i0, i1);
            UTAssert(
                k == index,
                "Invalid index.");
            std::vector<std::size_t> sizes = { i0, i1 };
            k = maT0.index(sizes);
            UTAssert(
                k == index,
                "Invalid index.");
        }
    }

    MultiarrayAdapter<std::int32_t, false> maF0{};
    std::vector<std::int32_t> maF0Storage(6);
    maF0.reset({ 2, 3 }, maF0Storage.data());
    index = 0;
    for (std::size_t i0 = 0; i0 < maF0.size(0); ++i0)
    {
        for (std::size_t i1 = 0; i1 < maF0.size(1); ++i1, ++index)
        {
            std::size_t k = maF0.index(i0, i1);
            UTAssert(
                k == index,
                "Invalid index.");
            std::vector<std::size_t> sizes = { i0, i1 };
            k = maF0.index(sizes);
            UTAssert(
                k == index,
                "Invalid index.");
        }
    }

    index = 0;
    for (std::size_t i1 = 0; i1 < maT0.size(1); ++i1)
    {
        for (std::size_t i0 = 0; i0 < maT0.size(0); ++i0, ++index)
        {
            UTAssert(
                maT0(i0, i1) == (std::int32_t)(index + 1),
                "operator(...) const failed.");
        }
    }

    index = 0;
    for (std::size_t i1 = 0; i1 < maT0.size(1); ++i1)
    {
        for (std::size_t i0 = 0; i0 < maT0.size(0); ++i0, ++index)
        {
            maT0(i0, i1) = (std::int32_t)(index + 2);
            UTAssert(
                maT0(i0, i1) == (std::int32_t)(index + 2),
                "operator(...) failed.");
        }
    }

    std::vector<std::int32_t> maT1Storage(6);
    MultiarrayAdapter<std::int32_t, true> maT1({ 2, 3 }, maT1Storage.data());
    for (index = 0; index < maT0.size(); ++index)
    {
        maT1[index] = (std::int32_t)(index + 3);
    }

    bool isEQ = (maT0 == maT1);
    UTAssert(
        isEQ == false,
        "operator== failed");

    bool isNE = (maT0 != maT1);
    UTAssert(
         isNE == true,
        "operator!= failed");

    bool isLT = (maT0 < maT1);
    UTAssert(
         isLT == true,
        "operator< failed");

    bool isLE = (maT0 <= maT1);
    UTAssert(
        isLE == true,
        "operator<= failed");

    bool isGT = (maT0 > maT1);
    UTAssert(
         isGT == false,
        "operator> failed");

    bool isGE = (maT0 >= maT1);
    UTAssert(
         isGE == false,
        "operator>= failed");
}

#else

#include <GTL/Utility/MultiarrayAdapter.h>

namespace gtl
{
    template class MultiarrayAdapter<float, true, 2>;
    template class MultiarrayAdapter<double, true, 2, 3>;
    template class MultiarrayAdapter<std::uint32_t, true, 2, 3, 5>;

    template class MultiarrayAdapter<std::int32_t, false, 2>;
    template class MultiarrayAdapter<std::int32_t, false, 2, 3>;
    template class MultiarrayAdapter<std::int32_t, false, 2, 3, 5>;

    template class MultiarrayAdapter<std::size_t, true>;
    template class MultiarrayAdapter<std::size_t, false>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(MultiarrayAdapter)
