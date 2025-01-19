// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Utility/StringUtility.h>
using namespace gtl;

class UnitTestStringUtility
{
public:
    UnitTestStringUtility();

private:
    void Test();
};

UnitTestStringUtility::UnitTestStringUtility()
{
    UTInformation("Mathematics/Utility/StringUtility");

    Test();
}

void UnitTestStringUtility::Test()
{
    std::string nstr = "abc DEF_123\t?\n";
    std::wstring wstr = ConvertNarrowToWide(nstr);
    UTAssert(wstr == L"abc DEF_123\t?\n", "Invalid ConvertNarrowToWide");

    nstr = ConvertWideToNarrow(wstr);
    UTAssert(nstr == "abc DEF_123\t?\n", "Invalid ConvertWideToNarrow");

    std::string tolower = ToLower(nstr);
    UTAssert(tolower == "abc def_123\t?\n", "Invalid ToLower case");

    std::string toupper = ToUpper(nstr);
    UTAssert(toupper == "ABC DEF_123\t?\n", "Invalid ToUpper case");

    std::vector<std::string> tokens;
    GetTokens(nstr, "aE?", tokens);
    UTAssert(tokens[0] == "bc D" && tokens[1] == "F_123\t" && tokens[2] == "\n",
        "Invalid GetTokens output");

    GetTextTokens(nstr, tokens);
    UTAssert(tokens[0] == "abc" && tokens[1] == "DEF_123" && tokens[2] == "?",
        "Invalid GetTextTokens output");

    nstr[4] = 'ø';  // ASCII 0xF8
    GetTextTokens(nstr, tokens);
    UTAssert(tokens[0] == "abc" && tokens[1] == "EF_123" && tokens[2] == "?",
        "Invalid GetTextTokens output");

    GetAdvancedTextTokens(nstr, tokens);
    UTAssert(tokens[0] == "abc" && tokens[1] == "øEF_123" && tokens[2] == "?",
        "Invalid GetAdvancedTextTokens output");
}

#else

#include <GTL/Utility/StringUtility.h>

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(StringUtility)
