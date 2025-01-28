#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/ImageProcessing/Rasterize2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRasterize2
    {
    public:
        UnitTestRasterize2();
    };
}

UnitTestRasterize2::UnitTestRasterize2()
{
    UTInformation("Mathematics/ImageProcessing/Rasterize2 (tested by CollisionRecord");
}

#else

#include <GTL/Mathematics/ImageProcessing/Rasterize2.h>

namespace gtl
{
    template class Rasterize2<std::int32_t>;
    template class Rasterize2<std::int64_t>;

    using SInteger = std::int64_t;
    using PixelType = std::int32_t;
    template void Rasterize2<SInteger>::DrawFloodFill4(SInteger, SInteger, SInteger,
        SInteger, PixelType, PixelType,
        std::function<void(SInteger, SInteger, PixelType)> const&,
        std::function<PixelType(SInteger, SInteger)> const&);
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Rasterize2)
