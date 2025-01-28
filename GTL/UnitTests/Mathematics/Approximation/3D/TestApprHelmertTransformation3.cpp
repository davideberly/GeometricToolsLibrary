#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/3D/ApprHelmertTransformation3.h>
#include <GTL/Mathematics/Algebra/Rotation.h>
#include <iostream>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestApprHelmertTransformation3
    {
    public:
        UnitTestApprHelmertTransformation3();

    private:
        void Test();
    };
}

UnitTestApprHelmertTransformation3::UnitTestApprHelmertTransformation3()
{
    UTInformation("Mathematics/Approximation/3D/ApprHelmertTransformation3");

    Test();
}

void UnitTestApprHelmertTransformation3::Test()
{
    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0, 1.0);
    Quaternion<double> qq(urd(dre), urd(dre), urd(dre), urd(dre));
    Normalize(qq);
    Matrix3x3<double> rotate = Rotation<double>(qq);
    Vector3<double> translate{ urd(dre), urd(dre), urd(dre) };
    double scale = 1.01;

    std::vector<Vector3<double>> p(32), q(32);
    double epsilon = 1e-06;
    for (std::size_t i = 0; i < p.size(); ++i)
    {
        p[i][0] = urd(dre);
        p[i][1] = urd(dre);
        p[i][2] = urd(dre);
        q[i][0] = p[i][0] + epsilon * urd(dre);
        q[i][1] = p[i][1] + epsilon * urd(dre);
        q[i][2] = p[i][2] + epsilon * urd(dre);
        q[i] = scale * (rotate * q[i]) + translate;
    }

    ApprHelmertTransformation3<double> helmert{};
    Matrix3x3<double> outRotate{};
    Vector3<double> outTranslate{};
    double outScale{};
    double outFunction{};
    std::size_t numIterations = 1024;
    std::size_t iteration = helmert.Fit(p, q, numIterations,
        outRotate, outTranslate, outScale, outFunction);
    (void)iteration;
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/3D/ApprHelmertTransformation3.h>

namespace gtl
{
    template class ApprHelmertTransformation3<float>;
    template class ApprHelmertTransformation3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprHelmertTransformation3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprHelmertTransformation3)
