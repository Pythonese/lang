#include "error_checker.h"

std::string error_checker::Verdict::toString()
{
    return good ? "" : "error: " + reason + " at line " + lineIndex;
}

std::string error_checker::removeComments(const std::string& s)
{
    std::string s1 = "";
    for (size_t i = 0; i < s.size();)
    {
        if (s[i] == ';')
        {
            i = s.find('\n', i + 1);
            if (i == std::string::npos)
            {
                break;
            }
            do i++; while (s[i] == ' ' or s[i] == '\t');
            continue;
        }
        s1 += s[i];
        i++;
    }
    return s1;
}

error_checker::Verdict error_checker::parensGood(const std::string& s)
{
    std::vector<char> stack;
    size_t line = 0;
    for (char c : s)
    {
        if (c == '\n')
        {
            line++;
        }
        if (openParens.find(c) != std::string::npos)
        {
            stack.push_back(c);
        }
        else if (closeParens.find(c) != std::string::npos)
        {
            if (stack.empty())
            {
                return Verdict(false, "parentheses not open", std::to_string(line));
            }
            if (openParens.find(stack.back()) == closeParens.find(c))
            {
                stack.pop_back();
            }
            else
            {
                return Verdict(false, "parentheses error", std::to_string(line));
            }
        }
    }
    if (!stack.empty())
    {
        return Verdict(false, "parentheses not closed", "end of file");
    }
    return Verdict(true, "", "");
}

bool error_checker::isValName(const std::string& s)
{
    if (s.size() == 0)
    {
        return false;
    }
    if (s[0] >= '0' && s[0] <= '9')
    {
        return false;
    }
    for (char c : s)
    {
        if (!((c == '_') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
        {
            return false;
        }
    }
    return true;
}

bool error_checker::isVarName(const std::string& s)
{
    if (s.size() == 0)
    {
        return false;
    }
    if (s[0] >= '0' && s[0] <= '9')
    {
        return false;
    }
    for (char c : s)
    {
        if (!((c == '_') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')))
        {
            return false;
        }
    }
    return true;
}

bool error_checker::isUint(const std::string& s)
{
    if (s.size() == 0)
    {
        return false;
    }
    if (s[0] == '_' || s[s.size() - 1] == '_')
    {
        return false;
    }
    for (char c : s)
    {
        if (!((c >= '0' && c <= '9') || c == '_'))
        {
            return false;
        }
    }
    return true;
}

error_checker::Verdict error_checker::namesGood1(const std::string& s)
{
    size_t line = 0;
    for (size_t i = std::find_if(s.begin(), s.end(), [](char c) { return separators.find(c) == std::string::npos; }) - s.begin(); i < s.size();)
    {
        if (s[i] == '\n')
        {
            line++;
        }
        if (s[i] == '"')
        {
            size_t j = s.find('"', i + 1);
            if (j == std::string::npos)
            {
                return Verdict(false, "string literal error", std::to_string(line));
            }
            i = j + 1;
            continue;
        }
        if (s[i] == '\'')
        {
            if (s[i + 1] == '\'' or s[i + 2] != '\'')
            {
                return Verdict(false, "char literal error", std::to_string(line));
            }
            i += 3;
            continue;
        }
        if (separators.find(s[i]) != std::string::npos)
        {
            i++;
            continue;
        }
        std::string t = s.substr(i, std::find_first_of(s.begin() + i + 1, s.end(), separators.begin(), separators.end()) - (s.begin() + i));
        i += t.size();
        if (!(isValName(t) or isVarName(t) or isUint(t)))
        {
            return Verdict(false, std::string("idk what error: " + t), std::to_string(line));
        }
    }
    return Verdict(true, "", "");
}

error_checker::Verdict error_checker::valsGood(const std::string& s)
{
    std::string s1 = "";
    size_t line = 0;
    for (size_t i = 0; i < s.size(); i++)
    {
        if (s[i] == '\n')
        {
            line++;
            s1 = "";
        }
        else if (s[i] >= 'A' && s[i] <= 'Z')
        {
            s1 += s[i];
        }
        else
        {
            if (s1.size() > 0)
            {
                while (s[i] == ' ' || s[i] == '\t') i++;
                if (s[i] == ':' && s[i + 1] == '=')
                {
                    i += 2;
                }
                else if (s[i] == '=')
                {
                    return Verdict(false, "'=' error", std::to_string(line));
                }
            }
        }
    }
    return Verdict(true, "", "");
}

std::string error_checker::codeGood(const std::string& s)
{
    std::string s1 = removeComments(s);
    return parensGood(s1).toString() + namesGood1(s1).toString() + valsGood(s1).toString();
}
