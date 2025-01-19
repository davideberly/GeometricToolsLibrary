// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Graphics/Shaders/ProgramFactory.h>
using namespace gtl;

ProgramFactory::ProgramFactory()
    :
    version(""),
    vsEntry(""),
    psEntry(""),
    gsEntry(""),
    csEntry(""),
    defines(),
    flags(0)
{
}

std::shared_ptr<VisualProgram> ProgramFactory::CreateFromFiles(
    std::string const& vsFile, std::string const& psFile,
    std::string const& gsFile)
{
    GTL_ARGUMENT_ASSERT(
        vsFile != "" && psFile != "",
        "A program must have a vertex shader and a pixel shader.");

    // GetStringFromFile will throw an exception if the file cannot be opened.
    std::string vsSource = GetStringFromFile(vsFile);
    std::string psSource = GetStringFromFile(psFile);
    std::string gsSource = "";
    if (gsFile != "")
    {
        gsSource = GetStringFromFile(gsFile);
    }

    return CreateFromNamedSources(vsFile, vsSource, psFile, psSource, gsFile, gsSource);
}

std::shared_ptr<VisualProgram> ProgramFactory::CreateFromSources(
    std::string const& vsSource, std::string const& psSource,
    std::string const& gsSource)
{
    return CreateFromNamedSources("vs", vsSource, "ps", psSource, "gs", gsSource);
}

std::shared_ptr<ComputeProgram> ProgramFactory::CreateFromFile(
    std::string const& csFile)
{
    GTL_ARGUMENT_ASSERT(csFile != "", "A program must have a compute shader.");

    // GetStringFromFile will throw an exception if the file cannot be opened.
    std::string csSource = GetStringFromFile(csFile);

    return CreateFromNamedSource(csFile, csSource);
}

std::shared_ptr<ComputeProgram> ProgramFactory::CreateFromSource(std::string const& csSource)
{
    return CreateFromNamedSource("cs", csSource);
}

std::string ProgramFactory::GetStringFromFile(std::string const& filename)
{
    std::ifstream input(filename);
    GTL_RUNTIME_ASSERT(
        input,
        "Cannot open file " + filename);

    std::string source = "";
    while (!input.eof())
    {
        std::string line;
        getline(input, line);
        source += line + "\n";
    }
    input.close();
    return source;
}

void ProgramFactory::PushDefines()
{
    mDefinesStack.push(defines);
    defines.Clear();
}

void ProgramFactory::PopDefines()
{
    if (mDefinesStack.size() > 0)
    {
        defines = mDefinesStack.top();
        mDefinesStack.pop();
    }
}

void ProgramFactory::PushFlags()
{
    mFlagsStack.push(flags);
    flags = 0;
}

void ProgramFactory::PopFlags()
{
    if (mFlagsStack.size() > 0)
    {
        flags = mFlagsStack.top();
        mFlagsStack.pop();
    }
}
