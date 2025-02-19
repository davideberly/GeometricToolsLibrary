#pragma once

#include <Applications/Window3.h>
#include <Mathematics/HelmertTransformation7.h>
using namespace gte;

class TestHelmertTransformationWindow3 : public Window3
{
public:
    TestHelmertTransformationWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    void CreateScene();
    void Align();

    static std::size_t constexpr numPoints = 32;
    static std::size_t constexpr numIterations = 1024;
    std::vector<Vector3<double>> mPPoints;
    std::vector<Vector3<double>> mQPoints;
    HelmertTransformation7<double> mHelmert;
    std::string mMessage;

    std::vector<std::shared_ptr<Visual>> mPPointSpheres;
    std::vector<std::shared_ptr<Visual>> mQPointSpheres;
};
