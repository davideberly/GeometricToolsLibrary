#pragma once

#include <Applications/Console.h>
using namespace gte;

class GenericConsole : public Console
{
public:
    GenericConsole(Parameters& parameters);

    virtual void Execute() override;

private:
};
