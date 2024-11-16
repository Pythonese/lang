#include "tokenize.h"

size_t tokenizer::slen(char *s)
{
    size_t i = 0;
    while (s[i] != '\0') i++;
    return i;
}

std::string tokenizer::Token1::typeToString() const
{
    switch (type)
    {
    case NONE:
        return "NONE";
        break;
    case OPER:
        return "OPER";
        break;
    case U:
        return "U";
        break;
    case I:
        return "I";
        break;
    case F:
        return "F";
        break;
    case C:
        return "C";
        break;
    case S:
        return "S";
        break;
    case B:
        return "B";
        break;
    case LPAR:
        return "LPAR";
        break;
    case RPAR:
        return "RPAR";
        break;
    case VAL_NAME:
        return "VAL_NAME";
        break;
    case VAR_NAME:
        return "VAR_NAME";
        break;
    case EOL:
        return "EOL";
        break;
    case COMMA:
        return "COMMA";
        break;
    case ERROR:
        return "ERROR";
        break;
    default:
        return "UNKNOWN " + std::to_string(type);
        break;
    }
}

std::string tokenizer::Token1::toString() const
{
    return typeToString() + " " + value;
}

void f(std::string& token, std::vector<tokenizer::Token1>& tokens)
{
    if (token.size() == 0)
    {
        return;
    }
    if (error_checker::isValName(token))
    {
        tokens.push_back(tokenizer::Token1(tokenizer::Token1::VAL_NAME, std::move(token)));
    }
    else if (error_checker::isVarName(token))
    {
        tokens.push_back(tokenizer::Token1(tokenizer::Token1::VAR_NAME, std::move(token)));
    }
    else if (error_checker::isUint(token))
    {
        tokens.push_back(tokenizer::Token1(tokenizer::Token1::U, std::move(token)));
    }
}

std::vector<tokenizer::Token1> tokenizer::split(const std::string& s)
{
    std::vector<Token1> tokens;
    std::string token;
    size_t start, end;
    for (size_t i = 0; i < s.size();)
    {
        if (s[i] == '"')
        {
            start = i + 1;
            end = s.find('"', i + 1);
            f(token, tokens);
            tokens.push_back(Token1(Token1::S, s.substr(start, end - start)));
            i = end + 1;
            continue;
        }
        if (s[i] == '\'')
        {
            f(token, tokens);
            tokens.push_back(Token1(Token1::C, std::string(1, s[i + 1])));
            i += 3;
            continue;
        }
        if (opers.find(s[i]) != std::string::npos)
        {
            f(token, tokens);
            tokens.push_back(Token1(Token1::OPER, std::string(1, s[i])));
            i++;
            continue;
        }
        if (ignore.find(s[i]) != std::string::npos)
        {
            f(token, tokens);
            i++;
            continue;
        }
        if (openParens.find(s[i]) != std::string::npos)
        {
            f(token, tokens);
            tokens.push_back(Token1(Token1::LPAR, std::string(1, s[i])));
            i++;
            continue;
        }
        if (closeParens.find(s[i]) != std::string::npos)
        {
            f(token, tokens);
            tokens.push_back(Token1(Token1::RPAR, std::string(1, s[i])));
            i++;
            continue;
        }
        if (s[i] == '\n')
        {
            f(token, tokens);
            tokens.push_back(Token1(Token1::EOL, ""));
            i++;
            continue;
        }
        if (s[i] == ',')
        {
            f(token, tokens);
            tokens.push_back(Token1(Token1::COMMA, ""));
            i++;
            continue;
        }
        token += s[i];
        i++;
    }
    f(token, tokens);
    return tokens;
}

size_t tokenizer::findCloseParen(size_t start, const std::vector<tokenizer::Token1>& tokens)
{
    if (tokens[start].type != tokenizer::Token1::LPAR)
    {
        if (!(tokens[start].type == tokenizer::Token1::OPER and tokens[start].value == "<")) return start;
    }
    size_t i = start;
    size_t k = 0;
    do
    {
        if (tokens[i].type == tokenizer::Token1::LPAR or tokens[i].type == tokenizer::Token1::OPER and tokens[i].value == "<")
        {
            k++;
        }
        else if (tokens[i].type == tokenizer::Token1::RPAR or tokens[i].type == tokenizer::Token1::OPER and tokens[i].value == ">")
        {
            k--;
        }
        i++;
    } while (k);
    return i - 1;
}

size_t tokenizer::findOpenParen(size_t end, const std::vector<tokenizer::Token1> &tokens)
{
    if (tokens[end].type != tokenizer::Token1::RPAR)
    {
        if (!(tokens[end].type == tokenizer::Token1::OPER and tokens[end].value == ">")) return end;
    }
    size_t i = end;
    size_t k = 0;
    do
    {
        if (tokens[i].type == tokenizer::Token1::RPAR or tokens[i].type == tokenizer::Token1::OPER and tokens[i].value == ">")
        {
            k++;
        }
        else if (tokens[i].type == tokenizer::Token1::LPAR or tokens[i].type == tokenizer::Token1::OPER and tokens[i].value == "<")
        {
            k--;
        }
        i--;
    } while (k);
    return i + 1;
}

std::vector<tokenizer::Token1> tokenizer::correctTokens(const std::vector<tokenizer::Token1>& v)
{
    std::vector<tokenizer::Token1> tokens;
    for (size_t i = 0; i < v.size();)
    {
        if (v[i].type == tokenizer::Token1::OPER and v[i].value == ":")
        {
            if (v[i + 1].type == tokenizer::Token1::OPER and v[i + 1].value == "=")
            {
                tokens.push_back(tokenizer::Token1(tokenizer::Token1::OPER, ":="));
                i += 2;
                continue;
            }
        }
        if (v[i].type == tokenizer::Token1::OPER and v[i].value == "=")
        {
            if (v[i + 1].type == tokenizer::Token1::OPER and v[i + 1].value == "=")
            {
                tokens.push_back(tokenizer::Token1(tokenizer::Token1::OPER, "=="));
                i += 2;
                continue;
            }
        }
        if (v[i].type == tokenizer::Token1::OPER and v[i].value == "!")
        {
            if (v[i + 1].type == tokenizer::Token1::OPER and v[i + 1].value == "=")
            {
                tokens.push_back(tokenizer::Token1(tokenizer::Token1::OPER, "!="));
                i += 2;
                continue;
            }
        }
        if (v[i].type == tokenizer::Token1::OPER and v[i].value == "+")
        {
            if (v[i + 1].type == tokenizer::Token1::OPER and v[i + 1].value == "+")
            {
                tokens.push_back(tokenizer::Token1(tokenizer::Token1::OPER, "++"));
                i += 2;
                continue;
            }
        }
        if (v[i].type == tokenizer::Token1::OPER and v[i].value == "-")
        {
            if (v[i + 1].type == tokenizer::Token1::OPER and v[i + 1].value == "-")
            {
                tokens.push_back(tokenizer::Token1(tokenizer::Token1::OPER, "--"));
                i += 2;
                continue;
            }
            if (v[i + 1].type == tokenizer::Token1::U)
            {
                if (v[i + 2].type == tokenizer::Token1::OPER and v[i + 2].value == ".")
                {
                    if (v[i + 3].type == tokenizer::Token1::U)
                    {
                        tokens.push_back(tokenizer::Token1(tokenizer::Token1::F, "-" + v[i + 1].value + "." + v[i + 3].value));
                        i += 4;
                        continue;
                    }
                }
                tokens.push_back(tokenizer::Token1(tokenizer::Token1::I, "-" + v[i + 1].value));
                i += 2;
                continue;
            }
        }
        if (v[i].type == tokenizer::Token1::U)
        {
            if (v[i + 1].type == tokenizer::Token1::OPER and v[i + 1].value == ".")
            {
                if (v[i + 2].type == tokenizer::Token1::U)
                {
                    tokens.push_back(tokenizer::Token1(tokenizer::Token1::F, v[i].value + "." + v[i + 2].value));
                    i += 3;
                    continue;
                }
            }
        }
        if (v[i].type == tokenizer::Token1::VAL_NAME and v[i].value == "TRUE" or v[i].value == "FALSE")
        {
            tokens.push_back(tokenizer::Token1(tokenizer::Token1::B, v[i].value));
            i++;
            continue;
        }
        if (v[i].type == tokenizer::Token1::COMMA)
        {
            tokens.push_back(tokenizer::Token1(tokenizer::Token1::EOL, ""));
            i++;
            continue;
        }
        tokens.push_back(v[i]);
        i++;
    }
    return tokens;
}

std::vector<tokenizer::Token1> tokenizer::tokenize(const std::string& s)
{
    return correctTokens(split(s));
}

size_t tokenizer::checkTokens(const std::vector<tokenizer::Token1>& v)
{
    size_t line = 0;
    if (v[0].type == tokenizer::Token1::OPER or (v[v.size() - 1].type == tokenizer::Token1::OPER and v[v.size() - 1].value == "."))
    {
        return 1;
    }
    for (size_t i = 0; i < v.size() - 1; i++)
    {
        if (v[i].type == tokenizer::Token1::OPER and v[i].value == ":")
        {
            if (!((v[i - 1].type == tokenizer::Token1::VAL_NAME or v[i - 1].type == tokenizer::Token1::VAR_NAME or (v[i - 1].type == tokenizer::Token1::RPAR and v[i - 1].value == ">")) and v[i + 1].type == tokenizer::Token1::VAL_NAME))
            {
                return 2;
            }
        }
        if (v[i].type == tokenizer::Token1::OPER and v[i + 1].type == tokenizer::Token1::OPER)
        {
            return 3;
        }
        if (v[i].type == tokenizer::Token1::OPER and (v[i].value == "." or v[i].value == "\\"))
        {
            if (!(v[i + 1].type == tokenizer::Token1::VAL_NAME or v[i + 1].type == tokenizer::Token1::VAR_NAME))
            {
                return 4;
            }
        }
        if (v[i].type == tokenizer::Token1::VAL_NAME or v[i].type == tokenizer::Token1::VAR_NAME or v[i].type == tokenizer::Token1::RPAR)
        {
            if (v[i + 1].type != tokenizer::Token1::OPER and v[i + 1].type != tokenizer::Token1::RPAR and v[i + 1].type != tokenizer::Token1::COMMA and v[i + 1].type != tokenizer::Token1::EOL)
            {
                if (!((v[i + 1].type >= tokenizer::Token1::U and v[i + 1].type <= tokenizer::Token1::B) or v[i + 1].type == tokenizer::Token1::LPAR or v[i + 1].type == tokenizer::Token1::VAL_NAME or v[i + 1].type == tokenizer::Token1::VAR_NAME))
                {
                    return 5;
                }
            }
        }
    }
    return 0;
}
