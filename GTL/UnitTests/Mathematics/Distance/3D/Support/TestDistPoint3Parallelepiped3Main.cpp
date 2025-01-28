#include "TestDistPoint3Parallelepiped3Window3.h"
#include <iostream>

int main()
{
    try
    {
        Window::Parameters parameters(L"TestDistPoint3Parallelepiped3Window3", 0, 0, 768, 768);
        auto window = TheWindowSystem.Create<TestDistPoint3Parallelepiped3Window3>(parameters);
        TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
        TheWindowSystem.Destroy(window);
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
