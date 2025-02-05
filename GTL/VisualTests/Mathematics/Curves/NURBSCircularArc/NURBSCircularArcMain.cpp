#include "NURBSCircularArcWindow2.h"
#include <iostream>

int main()
{
    try
    {
        Window::Parameters parameters(L"NURBSCircularArcWindow2", 0, 0, 512, 512);
        auto window = TheWindowSystem.Create<NURBSCircularArcWindow2>(parameters);
        TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
        TheWindowSystem.Destroy(window);
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
