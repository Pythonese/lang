#include <string>
#include <vector>
#include <algorithm>
#include <map>
#ifndef ERROR_CHECKER_H
#define ERROR_CHECKER_H 1
namespace error_checker
{
    const std::string opers = "+-:=.!\\,";
    const std::string ignore = " \t";
    const std::string parens = "()[]{}<>";
    const std::string openParens = "({[";
    const std::string closeParens = ")}]";
    const std::string separators = opers + ignore + parens + "\n";

    struct Verdict
    {
        bool good;
        std::string reason;
        std::string lineIndex;
        Verdict(bool g, std::string r, std::string l) : good(g), reason(r), lineIndex(l) {}
        std::string toString();
    };

    std::string removeComments(const std::string&);

    Verdict parensGood(const std::string&);

    bool isValName(const std::string&);
    bool isVarName(const std::string&);
    bool isUint(const std::string&);

    Verdict namesGood1(const std::string&);
    Verdict valsGood(const std::string&);

    std::string codeGood(const std::string&);
}

#endif