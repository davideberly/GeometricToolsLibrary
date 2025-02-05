#include "NURBSCircularArcWindow2.h"

NURBSCircularArcWindow2::NURBSCircularArcWindow2(Parameters& parameters)
    :
    Window2(parameters)
{
    mDoFlip = true;
    OnDisplay();
}

void NURBSCircularArcWindow2::OnIdle()
{
    OnDisplay();
}

void NURBSCircularArcWindow2::OnDisplay()
{
    std::uint32_t constexpr white = 0xFFFFFFFF;
    std::uint32_t constexpr blue = 0xFFFF0000;
    std::uint32_t constexpr red = 0xFF0000FF;
    std::uint32_t constexpr green = 0xFF00FF00;
    
    ClearScreen(white);

    DrawCircle(mXSize / 2, mYSize / 2, 200, blue, false);

    float const angle0 = C_PI<float> / 6.0f;
    float const angle1 = 3.0f * C_PI<float> / 8.0f;
    float const angleSubtend = angle1 - angle0;
    Arc2<float> arc{};
    arc.center = { static_cast<float>(mXSize / 2), static_cast<float>(mYSize / 2) };
    arc.radius = 200.0f;
    arc.end[0] = { std::cos(angle0), std::sin(angle0) };
    arc.end[1] = { std::cos(angle1), std::sin(angle1) };
    arc.end[0] = arc.center + arc.radius * arc.end[0];
    arc.end[1] = arc.center + arc.radius * arc.end[1];

    NURBSCircularArcDegree2<float> curve(arc);
    float length = arc.radius * angleSubtend;
    std::size_t imax = static_cast<std::size_t>(length);
    std::array<Vector2<float>, 4> jet{};
    for (std::size_t i = 0; i <= imax; ++i)
    {
        float u = static_cast<float>(i) / static_cast<float>(imax);
        curve.Evaluate(u, 0, jet.data());
        std::int32_t x = static_cast<std::int32_t>(jet[0][0]);
        std::int32_t y = static_cast<std::int32_t>(jet[0][1]);
        SetPixel(x, y, red);
    }

    arc.center += { 50.0f, 25.0f };
    arc.radius = 175.0f;
    arc.end[0] = { std::cos(angle0), std::sin(angle0) };
    arc.end[1] = { std::cos(angle1), std::sin(angle1) };
    arc.end[0] = arc.center + arc.radius * arc.end[0];
    arc.end[1] = arc.center + arc.radius * arc.end[1];

    DrawCircle(
        static_cast<std::int32_t>(arc.center[0]),
        static_cast<std::int32_t>(arc.center[1]),
        static_cast<std::int32_t>(arc.radius),
        blue, false);

    NURBSCircularArcDegree2<float>curve2(arc);
    length = arc.radius * angleSubtend;
    imax = static_cast<std::size_t>(length);
    for (std::size_t i = 0; i <= imax; ++i)
    {
        float u = static_cast<float>(i) / static_cast<float>(imax);
        curve2.Evaluate(u, 0, jet.data());
        std::int32_t x = static_cast<std::int32_t>(jet[0][0]);
        std::int32_t y = static_cast<std::int32_t>(jet[0][1]);
        SetPixel(x, y, green);
    }

    mScreenTextureNeedsUpdate = true;
    Window2::OnDisplay();
}
