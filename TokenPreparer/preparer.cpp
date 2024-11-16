#include "preparer.h"

size_t commas(size_t start, const std::vector<tokenizer::Token1>& tokens)
{
    size_t i = start;
    do
    {
        if (tokens[i].type == tokenizer::Token1::VAL_NAME or tokens[i].type == tokenizer::Token1::LPAR)
        {
            size_t j = findCloseParen(i, tokens);
            if (tokens[j].type == tokenizer::Token1::COMMA)
            {
                i = j + 1;
                continue;
            }
            return j;
        }
        i++;
    } while (i < tokens.size());
    return i;
}

std::vector<token_preparer::Token1> token_preparer::twoOpersIntoParens(const std::vector<token_preparer::Token1> &v, size_t start, size_t end, const std::string &o1, const std::string &o2)
{
    if (start == end)
    {
        if (v[start].type > 100) throw std::runtime_error("Unexpected number of tokens");
        return {v[start]};
    }

    if (end - start == 1)
    {
        if (v[start].type == token_preparer::Token1::LPAR && v[end].type == token_preparer::Token1::RPAR) return {v[start], v[end]};
        throw std::runtime_error("Unexpected number of tokens");
    }

    if (end - start == 2 && v[start + 1].type == token_preparer::Token1::OPER && (v[start + 1].value == o1 || v[start + 1].value == o2))
    {
        return {v[start], v[start + 1], v[end]};
    }

    std::vector<token_preparer::Token1> tokens;

    if (tokenizer::findCloseParen(start, v) == end)
    {
        tokens.push_back(v[start]);
        std::vector<token_preparer::Token1> tokens1 = twoOpersIntoParens(v, start + 1, end - 1, o1, o2);
        tokens.insert(tokens.end(), tokens1.begin(), tokens1.end());
        tokens.push_back(v[end]);
        return tokens;
    }

    size_t offset = 0;
    for (size_t i = start; i <= end;)
    {
        size_t j = findCloseParen(i, v);
        if (v[j + 1].type == token_preparer::Token1::OPER && (v[j + 1].value == o1 || v[j + 1].value == o2))
        {
            size_t count = 0;
            while (j + 2 <= end && v[j + 1].type == token_preparer::Token1::OPER && (v[j + 1].value == o1 || v[j + 1].value == o2))
            {
                j = findCloseParen(j + 2, v);
                count++;
            }
            for (size_t k = 0; k < count; k++)
            {
                tokens.push_back(token_preparer::Token1(token_preparer::Token1::LPAR, "("));
            }
            size_t start1 = i;
            size_t end1 = findCloseParen(i, v);
            std::vector<token_preparer::Token1> tokens1 = twoOpersIntoParens(v, start1, end1, o1, o2);
            tokens.insert(tokens.end(), tokens1.begin(), tokens1.end());
            tokens.push_back(v[end1 + 1]);

            size_t start2 = end1 + 2;
            size_t end2 = findCloseParen(start2, v);
            std::vector<token_preparer::Token1> tokens2 = twoOpersIntoParens(v, start2, end2, o1, o2);
            tokens.insert(tokens.end(), tokens2.begin(), tokens2.end());
            i = end2 + 1;
            tokens.push_back(token_preparer::Token1(token_preparer::Token1::RPAR, ")"));
            count--;
            while (count)
            {
                tokens.push_back(v[i]);
                i++;
                size_t start1 = i;
                size_t end1 = findCloseParen(i, v);
                std::vector<token_preparer::Token1> tokens1 = twoOpersIntoParens(v, start1, end1, o1, o2);
                tokens.insert(tokens.end(), tokens1.begin(), tokens1.end());
                tokens.push_back(token_preparer::Token1(token_preparer::Token1::RPAR, ")"));
                i = end1 + 1;
                count--;
            }
            continue;
        }
        tokens.push_back(v[i]);
        i++;
    }
    return tokens;
}

std::vector<token_preparer::Token1> token_preparer::dotsAndBackSlashesIntoParens(const std::vector<token_preparer::Token1>& v)
{
    return twoOpersIntoParens(v, 0, v.size() - 1, ".", "\\");
}

std::vector<token_preparer::Token1> token_preparer::colonsIntoParens(const std::vector<token_preparer::Token1>& v)
{
    std::vector<token_preparer::Token1> tokens;
    for (size_t i = 0; i < v.size();)
    {
        if (v[i + 1].type == token_preparer::Token1::OPER and v[i + 1].value == ":")
        {
            tokens.push_back(token_preparer::Token1(token_preparer::Token1::LPAR, "("));
            tokens.push_back(v[i]);
            tokens.push_back(v[i + 1]);
            tokens.push_back(v[i + 2]);
            tokens.push_back(token_preparer::Token1(token_preparer::Token1::RPAR, ")"));
            i += 3;
            continue;
        }
        tokens.push_back(v[i]);
        i++;
    }
    return tokens;
}

std::vector<token_preparer::Token1> token_preparer::putCallOpers(const std::vector<token_preparer::Token1> &v)
{
    std::vector<token_preparer::Token1> tokens;
    for (size_t i = 0; i < v.size();)
    {
        if (v[i].type == tokenizer::Token1::VAL_NAME || v[i].type == tokenizer::Token1::VAR_NAME || v[i].type == tokenizer::Token1::RPAR)
        {
            i++;
            if ((v[i].type >= tokenizer::Token1::U && v[i].type <= tokenizer::Token1::B) || v[i].type == tokenizer::Token1::VAL_NAME || v[i].type == tokenizer::Token1::VAR_NAME || v[i].type == tokenizer::Token1::LPAR)
            {
                tokens.push_back(v[i - 1]);
                tokens.push_back(token_preparer::Token1(token_preparer::Token1::OPER, "$"));
                continue;
            }
            i--;
        }
        tokens.push_back(v[i]);
        i++;
        continue;
    }
    return tokens;
}

std::vector<token_preparer::Token1> token_preparer::callsIntoParens(const std::vector<token_preparer::Token1>& v)
{
    return twoOpersIntoParens(v, 0, v.size() - 1, "$", "$");
}

std::vector<token_preparer::Token1> token_preparer::assignsIntoParens(const std::vector<token_preparer::Token1>& v)
{
    return twoOpersIntoParens(v, 0, v.size() - 1, "=", ":=");
}

std::vector<token_preparer::Token1> token_preparer::removeBadParens(const std::vector<token_preparer::Token1>& v)
{
    std::vector<token_preparer::Token1> tokens;
    for (size_t i = 0; i < v.size();)
    {
        if (v[i].type == token_preparer::Token1::EOL)
        {
            if (v[i + 1].type == token_preparer::Token1::LPAR && v[i + 1].value == "(")
            {
                size_t j = findCloseParen(i + 1, v);
                if (v[j + 1].type == token_preparer::Token1::EOL)
                {
                    tokens.push_back(v[i]);
                    for (size_t k = i + 2; k < j; k++) tokens.push_back(v[k]);
                    tokens.push_back(v[j + 1]);
                    i = j + 2;
                    continue;
                }
            }
        }
        tokens.push_back(v[i]);
        i++;
    }
    return tokens;
}

std::vector<token_preparer::Token1> token_preparer::prepare(const std::vector<token_preparer::Token1>& v)
{
    return callsIntoParens(colonsIntoParens(dotsAndBackSlashesIntoParens(v)));
}
