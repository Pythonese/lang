#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include "../ErrorChecker/error_checker.h"
#ifndef TOKENIZER_H
#define TOKENIZER_H 1

namespace tokenizer
{
    const std::string opers = error_checker::opers;
    const std::string ignore = error_checker::ignore;
    const std::string parens = error_checker::parens + "<>";
    const std::string openParens = error_checker::openParens + "<";
    const std::string closeParens = error_checker::closeParens + ">";
    const std::string separators = error_checker::separators;
    size_t slen(char* s);
    struct Token1
    {
        enum Type
        {
            NONE, OPER,
            U, I, F, C, S, B,
            LPAR, RPAR,
            VAL_NAME, VAR_NAME,
            EOL, COMMA, ERROR,
        };
        Type type;
        std::string value;
        Token1(Type t, const std::string& s) : type(t), value(s) {}
        Token1(Type t, std::string&& s) : type(t), value(std::move(s)) {}
        Token1() : type(NONE), value("") {}
        std::string typeToString() const;
        std::string toString() const;
    };
    std::vector<tokenizer::Token1> split(const std::string&);
    size_t findCloseParen(size_t start, const std::vector<tokenizer::Token1>& tokens);
    size_t findOpenParen(size_t start, const std::vector<tokenizer::Token1>& tokens);
    std::vector<tokenizer::Token1> correctTokens(const std::vector<tokenizer::Token1>&);
    std::vector<tokenizer::Token1> tokenize(const std::string&);
    size_t checkTokens(const std::vector<tokenizer::Token1>&);
}

#endif