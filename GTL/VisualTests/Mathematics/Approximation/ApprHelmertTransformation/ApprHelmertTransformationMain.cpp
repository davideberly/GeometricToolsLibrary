#include "TestHelmertTransformationWindow3.h"
#include <iostream>

int main()
{
    try
    {
        Window::Parameters parameters(L"TestHelmertTransformationWindow3", 0, 0, 768, 768);
        auto window = TheWindowSystem.Create<TestHelmertTransformationWindow3>(parameters);
        TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
        TheWindowSystem.Destroy(window);
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
