#define POINT_PARALLELEPIPED_TEST
#include <Mathematics/DistPoint3Parallelepiped3.h>
#include <Mathematics/Constants.h>
#include <Mathematics/Timer.h>
#include <fstream> 
#include <iomanip>
#include <iostream>
using namespace gte;

int32_t main()
{
    Vector3<double> center = Vector3<double>::Zero();
    std::array<Vector3<double>, 3> axis{};
    axis[0] = { 1.0, 0.125, 0.0 };
    axis[1] = { -1.0, 0.5, -0.375 };
    axis[2] = { -0.5, -0.0125, 1.5 };
    double dot = DotCross(axis[0], axis[1], axis[2]);
    if (dot < 0.0)
    {
        std::swap(axis[1], axis[2]);
    }

    DCPQuery<double, Vector3<double>, Parallelepiped3<double>> query{};
    DCPQuery<double, Vector3<double>, Parallelepiped3<double>>::Result result{};

    double radius = 3.0;
    Parallelepiped3<double> ppd(center, axis);
    Vector3<double> point = { radius, 0.0, 0.0 };

#if defined(POINT_PARALLELEPIPED_TEST)
    query.mOutput.open("PointParallelepipedTest.txt");
    std::ofstream& output = query.mOutput;
#else
    std::ofstream output("PointParallelepipedTest.txt");
#endif

    Timer timer{};
    for (std::size_t i = 0; i < 1024; ++i)
    {
        double theta = GTE_C_TWO_PI * i / 1024;
        double csTheta = std::cos(theta);
        double snTheta = std::sin(theta);
        for (std::size_t j = 0; j < 1024; ++j)
        {
            double phi = GTE_C_TWO_PI * j / 1024;
            double csPhi = std::cos(phi);
            double snPhi = std::sin(phi);

            point[0] = radius * csPhi * csTheta;
            point[1] = radius * csPhi * snTheta;
            point[2] = radius * snPhi;

#if defined(POINT_PARALLELEPIPED_TEST)
            query.mI = i;
            query.mJ = j;
#endif
            result = query(point, ppd);
            auto const& K = result.closest[1];
            output
                << std::setprecision(17)
                << "(" << i << ", " << j << "): "
                << "P = (" << point[0] << ", " << point[1] << ", " << point[2] << "), "
                << "K = (" << K[0] << ", " << K[1] << ", " << K[2] << "), "
                << result.distance << std::endl;
        }
    }
    output.close();
    auto milliseconds = timer.GetMilliseconds();
    std::cout << "milliseconds = " << milliseconds << std::endl;

#if defined(POINT_PARALLELEPIPED_TEST)
    std::ofstream visitedFile("PointParallelepipedCases.txt");
    for (std::size_t i = 0; i < query.mVisited.size(); ++i)
    {
        if (query.mVisited[i] != 0)
        {
            visitedFile << "visited[" << i << "] = " << query.mVisited[i] << std::endl;
        }
    }
    visitedFile.close();
#endif
    return 0;
}
