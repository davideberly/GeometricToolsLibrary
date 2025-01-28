#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Miscellaneous/STLBinaryFile.h>
using namespace gtl;

namespace gtl
{
    class UnitTestSTLBinaryFile
    {
    public:
        UnitTestSTLBinaryFile();

    private:
    };
}

UnitTestSTLBinaryFile::UnitTestSTLBinaryFile()
{
    UTInformation("Mathematics/Miscellaneous/STLBinaryFile (worked in GTE, no changes porting to GTL");
}

#else

#include <GTL/Mathematics/Miscellaneous/STLBinaryFile.h>
#include <GTL/Mathematics/Algebra/Vector.h>

namespace gtl
{
    template struct STLBinaryFile<Vector3<float>>;
    template struct STLBinaryFile<Vector3<double>>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(STLBinaryFile)
