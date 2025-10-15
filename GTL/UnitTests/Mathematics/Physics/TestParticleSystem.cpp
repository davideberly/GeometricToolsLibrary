#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Physics/ParticleSystem.h>
using namespace gtl;

namespace gtl
{
    class UnitTestParticleSystem
    {
    public:
        UnitTestParticleSystem();

    private:
    };
}

UnitTestParticleSystem::UnitTestParticleSystem()
{
    UTInformation("Mathematics/Physics/ParticleSystem [tested by GTL/VisualTests/Mathematics/Physics/Rope]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Physics/ParticleSystem.h>

namespace gtl
{
    template class ParticleSystem<float, 2>;
    template class ParticleSystem<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ParticleSystem<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ParticleSystem)
